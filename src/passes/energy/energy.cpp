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

using namespace llvm;

namespace {
    cl::opt<std::string> energyModelPath("m", cl::desc("Energymodel as JSON"), cl::value_desc("filepath to .json file"));

    struct Energy : public FunctionPass {
        static char ID;
        Json::Value energyJson;

        Energy() : FunctionPass(ID) {
            std::string filename = energyModelPath;
            if( llvm::sys::fs::exists( energyModelPath ) && !llvm::sys::fs::is_directory( energyModelPath ) ){
                //Create a LLVMHandler object
                this->energyJson = JSONHandler::read( filename );
            }
        }

        double getBasicBlockSum( llvm::BasicBlock &BB ){
            double blocksum = 0;

            for ( auto &I : BB ) {
                InstructionCategory::Category cat = InstructionCategory::getCategory(I );
                double iValue = this->energyJson[InstructionCategory::toString(cat)].asDouble();
                blocksum += iValue;
            }

            return blocksum;
        }

        bool runOnFunction(Function &F) override {
            errs().write_escaped(F.getName()) << " ==> ";


            for(auto &BB : F){
                llvm::outs() << round(getBasicBlockSum( BB )) << '\n';
                std::set<llvm::StringRef> v = LLVMHandler::kill(BB);
                llvm::outs() << "{";
                for( auto s : v){
                    llvm::outs() << s << ", ";
                }
                llvm::outs() << "}\n";
            }
            return false;
        }
    }; // end of struct Hello
}  // end of anonymous namespace

char Energy::ID = 0;
static RegisterPass<Energy> X("energy", "Hello World Pass",
                             false /* Only looks at CFG */,
                             true /* Analysis Pass */);