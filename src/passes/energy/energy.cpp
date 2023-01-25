#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "../../include/LLVM-Handler/LLVMHandler.h"
#include "llvm/Support/CommandLine.h"
#include "../../include/JSON-Handler/JSONHandler.h"
#include "llvm/Support/FileSystem.h"
#include "../../include/LLVM-Handler/InstructionCategory.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/ScalarEvolutionExpressions.h"

using namespace llvm;

namespace {
    cl::opt<std::string> energyModelPath("m", cl::desc("Energymodel as JSON"), cl::value_desc("filepath to .json file"));

    struct Energy : public FunctionPass {
        static char ID;
        Json::Value energyJson;
        int MAXITERATIONS = 1000;

        Energy() : FunctionPass(ID) {
            std::string filename = energyModelPath;
            if( llvm::sys::fs::exists( energyModelPath ) && !llvm::sys::fs::is_directory( energyModelPath ) ){
                //Create a LLVMHandler object
                this->energyJson = JSONHandler::read( filename );
            }
        }

        bool runOnFunction(Function &F) override {
            errs().write_escaped(F.getName()) << "\n\n";
            LLVMHandler handler = LLVMHandler( this->energyJson, MAXITERATIONS );

            auto* DT = new llvm::DominatorTree();
            DT->recalculate(F);
            //generate the LoopInfoBase for the current function
            auto *KLoop = new llvm::LoopInfoBase<llvm::BasicBlock, llvm::Loop>();


            KLoop->releaseMemory();
            KLoop->analyze(*DT);

            for (auto liiter = KLoop->begin(); liiter < KLoop->end(); ++liiter) {
                auto topLoop= *liiter;
                //auto subloopVector = topLoops->getSubLoops();

                /*for(auto subloop : subloopVector){
                    llvm::outs() << subloop->getName() << "\n";
                    double loopvalue = handler.getLoopSum(subloop);


                    llvm::outs() << "The loop will use " << loopvalue << "µJ of Energy" << "\n";
                    llvm::outs() << "==========================================================";
                }*/
                energyForLoop( topLoop, handler );
            }

            /*for (auto i : KLoop->getLoopsInPreorder()) {
                auto l = i;

                llvm::outs() << l->getName() << "\n";
                double loopvalue = handler.getLoopSum(l);


                llvm::outs() << "The loop will use " << loopvalue << "µJ of Energy" << "\n";
                llvm::outs() << "==========================================================";
            }*/

            /*for(auto &BB : F){
                llvm::outs() << "===================================================" << '\n';
                llvm::outs() << BB.getName() << " " << round(getBasicBlockSum( BB )) << '\n';
                llvm::outs() << "===================================================" << '\n';
            }*/
            return false;
        }

        void energyForLoop(Loop *L, LLVMHandler handler){
            double loopvalue = handler.getLoopSum(L);
            llvm::outs() << L->getName() << " " << "Energy used: " << loopvalue << "\n";

            auto subloopVector = L->getSubLoops();
            llvm::outs() << "==================================================" << "\n";
            for(auto subloop : subloopVector){
                energyForLoop(subloop, handler);
            }
        }
    }; // end of struct Hello
}  // end of anonymous namespace

char Energy::ID = 0;
static RegisterPass<Energy> X("energy", "Energy Approximation pass",
                             false /* Only looks at CFG */,
                             true /* Analysis Pass */);