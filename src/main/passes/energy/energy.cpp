#include <llvm/Passes/PassBuilder.h>
#include <llvm/Analysis/RegionInfo.h>
#include "llvm/Passes/PassPlugin.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "../../include/LLVM-Handler/LLVMHandler.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Analysis/PostDominators.h"
#include "../../include/JSON-Handler/JSONHandler.h"
#include "../../include/ProgramTree/ProgramTree.h"


llvm::cl::opt<std::string> energyModelPath("m", llvm::cl::desc("Energymodel as JSON"), llvm::cl::value_desc("filepath to .json file"));

struct Energy : llvm::PassInfoMixin<Energy> {
    Json::Value energyJson;
    int MAXITERATIONS = 1000;


    explicit Energy(std::string filename){
        if( llvm::sys::fs::exists( filename ) && !llvm::sys::fs::is_directory( filename ) ){
            //Create a JSONHandler object and read in the energypath
            this->energyJson = JSONHandler::read( filename );
        }
    }

    Energy(){
        if( llvm::sys::fs::exists( energyModelPath ) && !llvm::sys::fs::is_directory( energyModelPath ) ){
            //Create a JSONHandler object and read in the energypath
            this->energyJson = JSONHandler::read( energyModelPath );
        }
    }

    static void outputMetrics(llvm::Function *F, ProgramTree *PT, LLVMHandler *handler){
        double energyUsed = PT->getEnergy(handler);

        llvm::outs() << "\n";
        llvm::outs() << "Function " << F->getName() << "\n";
        llvm::outs() << "======================================================================" << "\n";
        llvm::outs() << "Estimated energy consumption: " << energyUsed << " µJ\n";
        llvm::outs() << "Number of basic blocks: " << F->getBasicBlockList().size() << "\n";
        llvm::outs() << "Number of instruction: " << F->getInstructionCount() << "\n";
        llvm::outs() << "Ø energy per block: " << energyUsed / (double) F->getBasicBlockList().size() << " µJ\n";
        llvm::outs() << "Ø energy per instruction: " << energyUsed / F->getInstructionCount() << " µJ\n";
        llvm::outs() << "======================================================================" << "\n";
        llvm::outs() << "\n";
    }



    /**
     * Main runner of the energy pass. The pass will apply function-wise.
     * @param F Reference to a function
     * @param FAM Reference to a FunctionAnalysisManager
     */
    llvm::PreservedAnalyses run(llvm::Function &F, llvm::FunctionAnalysisManager &FAM) {
        if(this->energyJson){
            //Init the Handler so we hand it to the ProgramTree later on
            LLVMHandler handler = LLVMHandler( this->energyJson, MAXITERATIONS );

            //Run the Loop-Analysis
            auto* DT = new llvm::DominatorTree();
            DT->recalculate(F);
            auto &KLoop = FAM.getResult<llvm::LoopAnalysis>(F);

            //Get the vector of Top-Level loops present in the program
            auto loops = KLoop.getTopLevelLoops();
            //Init a vector of references to BasicBlocks for all BBs in the function
            std::vector<llvm::BasicBlock *> functionBlocks;
            for(auto &blocks : F){
                functionBlocks.push_back(&blocks);
            }

            //Create the ProgramTree for the BBs present in the current function
            ProgramTree *PT = ProgramTree::construct(functionBlocks);

            //We need to distinguish if the function contains loops
            if(!loops.empty()){
                //If the function contains loops
                //Init a vector for the LoopTrees we will create for the loops
                std::vector<LoopTree *> trees;
                //Init a vector for all the latches of the functions.
                std::vector<llvm::BasicBlock *> latches;

                //Iterate over the top-level loops
                for (auto liiter = loops.begin(); liiter < loops.end(); ++liiter) {
                    //Get the loop, the iterator points to
                    auto topLoop= *liiter;

                    //Construct the LoopTree from the Information of the current top-level loop
                    LoopTree LT = LoopTree(topLoop, topLoop->getSubLoops(), &handler);

                    //Add the constructed tree to the List of LoopTrees
                    trees.push_back(&LT);

                    //Find all latches in the current loop
                    for (auto &bb : LT.getLatches()) {
                        latches.push_back(bb);
                    }

                    //Construct a LoopNode for the current loop
                    LoopNode *LN = LoopNode::construct(&LT, PT);
                    //Replace the blocks used by loop in the previous created ProgramTree
                    PT->replaceNodesWithLoopNode(topLoop->getBlocksVector(), LN);
                }

                //PT->printNodes(&handler);
                //llvm::outs() << "\n\n\n\n";
                //PT->printEdges();

                //Get the Energy from the ProgramTree and print it
                //llvm::outs() << "Energy used: " << PT->getEnergy(&handler) << " µJ\n";
                outputMetrics(&F, PT, &handler);

            }else{
                //If we don't have any loops, the ProgramTree needs no further handling, and we can calculate the energy
                //directly

                //Get the Energy from the ProgramTree and print it
                //llvm::outs() << "Energy used: " << PT->getEnergy(&handler) << " µJ\n";
                outputMetrics(&F, PT, &handler);
            }

        }else{
            llvm::errs() << "Please provide an energyfile with -m <path to the energy.json>" << "\n";
        }

        return llvm::PreservedAnalyses::all();
    }

    static bool isRequired() { return true; }
};



/**
 * Method for providing some basic information about the pass
 */
llvm::PassPluginLibraryInfo getEnergyPluginInfo() {
    return {LLVM_PLUGIN_API_VERSION, "Energy", LLVM_VERSION_STRING,
            [](llvm::PassBuilder &PB) {
                PB.registerPipelineParsingCallback(
                        [](llvm::StringRef Name, llvm::FunctionPassManager &FPM,
                           llvm::ArrayRef<llvm::PassBuilder::PipelineElement>) {
                            if (Name == "energy") {
                                FPM.addPass(Energy());
                                return true;
                            }
                            return false;
                        });
            }};
}

//Register the pass in llvm, so it is useable with opt
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return getEnergyPluginInfo();
}