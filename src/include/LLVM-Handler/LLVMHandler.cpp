//
// Created by max on 13.01.23.
//

#include "LLVMHandler.h"
#include "InstructionCategory.h"
#include "llvm/Analysis/CostModel.h"

LLVMHandler::LLVMHandler(std::string file, Json::Value energy) {
    //this->module = std::move(llvm::parseIRFile(file, this->error, this->context));
    this->module = llvm::parseIRFile(file, this->error, this->context).release();
    this->energyValues = energy;
}

double LLVMHandler::getBasicBlockSum( llvm::BasicBlock &BB ){
    double blocksum = 0;

    for ( auto &I : BB ) {
        InstructionCategory::Category cat = InstructionCategory::getCategory(I );
        double iValue = this->energyValues[InstructionCategory::toString(cat)].asDouble();
        blocksum += iValue;
    }

    return blocksum;
}

void LLVMHandler::sliceBlock(llvm::BasicBlock &BB, std::string v){

    for ( auto &I : BB ) {
        llvm::StringRef target = I.getName();
        llvm::outs() << target << " writes to " << "\n";
        for (int i = 0; i < I.getNumOperands(); ++i) {
            auto *op = I.getOperand(i);
            llvm::StringRef operatorName = op->getName();
            llvm::outs() << "\t" << i << " -> " << operatorName << "\n";
        }
    }
}

std::set<llvm::StringRef> LLVMHandler::kill(llvm::BasicBlock &BB) {
    std::set<llvm::StringRef> killedVars;

    for ( auto &I : BB ) {
        llvm::StringRef sr = I.getName();

        if( I.getName().compare("") != 0 ){

            if( InstructionCategory::isCallInstruction(I) || InstructionCategory::isMemloadInstruction(I) || !I.mayHaveSideEffects() ) {
                killedVars.insert(sr);
            }
        }
    }

    return killedVars;
}

void LLVMHandler::print(  ) {
    for ( auto &F : *this->module ) {
        if( F.getName() == "main" ){
            llvm::outs() << "Function " << F.getName() << "\n";
            for ( auto &BB : F ) {
                /*for ( auto &I : BB ) {
                    llvm::outs() <<  I.getOpcodeName();
                    llvm::outs() << " -> " << InstructionCategory::toString( InstructionCategory::getCategory( I ) ) << "\n";
                }
                 */
                llvm::outs() << "\n=======================================\n";
                llvm::outs() << " Summe des Blocks: "  << round(this->getBasicBlockSum( BB )) << "\n";
                std::set<llvm::StringRef> v = kill(BB);
                llvm::outs() << "{";
                for( auto s : v){
                    llvm::outs() << s << ",";
                }
                llvm::outs() << "}" << '\n';
                llvm::outs() << "\n=======================================\n";

            }
        }
    }
}