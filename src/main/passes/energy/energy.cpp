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
#include "../../include/LoopTree/LoopTree.h"


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

    void visitor(const llvm::Region &region){
        llvm::outs() << region.getNameStr() << "\n";
        for (auto &it : region) {
            auto subregion = it.get();
            visitor(*subregion);
        }
    }



    /**
     * Main runner of the energy pass. The pass will apply function-wise.
     * @param F Reference to a function
     * @param FAM Reference to a FunctionAnalysisManager
     */
    llvm::PreservedAnalyses run(llvm::Function &F, llvm::FunctionAnalysisManager &FAM) {
        if(this->energyJson){

            llvm::errs().write_escaped(F.getName()) << "\n\n";
            LLVMHandler handler = LLVMHandler( this->energyJson, MAXITERATIONS );

            auto* DT = new llvm::DominatorTree();
            DT->recalculate(F);
            //generate the LoopInfoBase for the current function
            auto &KLoop = FAM.getResult<llvm::LoopAnalysis>(F);
            auto &regres = FAM.getResult<llvm::RegionInfoAnalysis>(F);
            llvm::Region &entry = *regres.getTopLevelRegion();
            auto loops = KLoop.getTopLevelLoops();

            //visitor(entry);
            //ProgramTree PT = ProgramTree::construct(&entry);
            //PT.printPreOrder();

            //KLoop.releaseMemory();
            //KLoop->analyze(*DT);
            std::vector<llvm::BasicBlock *> functionBlocks;

            for(auto &blocks : F){
                functionBlocks.push_back(&blocks);
            }

            ProgramTree *PT = ProgramTree::construct(functionBlocks);


            if(!loops.empty()){
                //Inputprogramm contains for loops
                std::vector<LoopTree *> trees;
                std::vector<llvm::BasicBlock *> latches;
                for (auto liiter = loops.begin(); liiter < loops.end(); ++liiter) {
                    auto topLoop= *liiter;

                    LoopTree LT = LoopTree(topLoop, topLoop->getSubLoops(), &handler);
                    /*llvm::outs() << "==========================================================\n";
                    LT.printPreOrder();
                    llvm::outs() << "==========================================================\n";*/
                    trees.push_back(&LT);
                    for (auto &bb : LT.getLatches()) {
                        latches.push_back(bb);
                    }

                    LoopNode *LN = LoopNode::construct(&LT);
                    auto succeeded = PT->replaceNodesWithLoopNode(topLoop->getBlocksVector(), LN);

                }

                PT->printNodes();
                PT->printEdges();

            }else{


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