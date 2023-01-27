#include <llvm/Passes/PassBuilder.h>
#include "energy.h"
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
#include "../../include/LLVM-Handler/LoopTree.h"


llvm::cl::opt<std::string> energyModelPath("m", llvm::cl::desc("Energymodel as JSON"), llvm::cl::value_desc("filepath to .json file"));

struct Energy : llvm::PassInfoMixin<Energy> {
// Main entry point, takes IR unit to run the
// pass on (&F) and the corresponding pass
// manager (to be queried/modified if need be)
    Json::Value energyJson;
    int MAXITERATIONS = 1000;


    explicit Energy(std::string filename){
        if( llvm::sys::fs::exists( filename ) && !llvm::sys::fs::is_directory( filename ) ){
            //Create a LLVMHandler object
            this->energyJson = JSONHandler::read( filename );
        }
    }

    Energy(){
        if( llvm::sys::fs::exists( energyModelPath ) && !llvm::sys::fs::is_directory( energyModelPath ) ){
            //Create a LLVMHandler object
            this->energyJson = JSONHandler::read( energyModelPath );
        }
    }

    llvm::PreservedAnalyses run(llvm::Function &F, llvm::FunctionAnalysisManager &) {
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

            for (auto liiter = KLoop->begin(); liiter < KLoop->end(); ++liiter) {
                auto topLoop= *liiter;

                LoopTree LT = LoopTree(topLoop, topLoop->getSubLoops(), nullptr, &handler);
                llvm::outs() << "==========================================================\n";
                LT.printPreOrder();
                llvm::outs() << "==========================================================\n";
            }
        }else{
            llvm::errs() << "Please provide an energyfile with -m <path to the energy.json>" << "\n";
        }

        return llvm::PreservedAnalyses::all();
    }

    static bool isRequired() { return true; }
};


llvm::PassPluginLibraryInfo getHelloWorldPluginInfo() {

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

// This is the core interface for pass plugins. It guarantees that 'opt' will
// be able to recognize HelloWorld when added to the pass pipeline on the
// command line, i.e. via '-passes=hello-world'
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return getHelloWorldPluginInfo();
}