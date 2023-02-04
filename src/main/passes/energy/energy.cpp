#include <llvm/Passes/PassBuilder.h>
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
#include "../../include/ProgramTree/LoopTree.h"
#include "../../include/ProgramTree/IfTree.h"


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
            auto *KLoop = new llvm::LoopInfoBase<llvm::BasicBlock, llvm::Loop>();
            KLoop->releaseMemory();
            KLoop->analyze(*DT);

            llvm::outs() << F.getName() << " " << KLoop->empty() << "\n";


            if(!KLoop->empty()){
                //Inputprogramm contains for loops
                std::vector<LoopTree *> trees;
                std::vector<llvm::BasicBlock *> latches;
                for (auto liiter = KLoop->begin(); liiter < KLoop->end(); ++liiter) {
                    auto topLoop= *liiter;

                    LoopTree LT = LoopTree(topLoop, topLoop->getSubLoops(), &handler);
                    //llvm::outs() << "==========================================================\n";
                    //LT.printPreOrder();
                    //llvm::outs() << "==========================================================\n";
                    trees.push_back(&LT);
                    for (auto &bb : LT.getLatches()) {
                        latches.push_back(bb);
                    }
                }


                std::vector<llvm::BasicBlock *> ifblocks;
                for (auto &bb : F) {
                    if(bb.getTerminator()->getNumSuccessors() == 2){
                        if(std::find(latches.begin(), latches.end(), &bb) == latches.end()){
                            ifblocks.push_back(&bb);
                        }
                    }
                }

                for (auto &fb : ifblocks) {
                    fb->print(llvm::outs());
                }

            }else{
                //Programm is loop-free
                std::vector<llvm::BasicBlock *> blockList;
                for (auto &bb : F) {
                    blockList.push_back(&bb);
                }

                for (auto &bb : blockList) {
                    if(bb->getTerminator()->getNumSuccessors() == 2){
                        llvm::outs() << "Found possible header " << bb->getName() << "\n";
                    }
                }

                //std::vector<IfTree> ifs = IfTree::constructCondition(blockList);



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