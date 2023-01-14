//
// Created by max on 13.01.23.
//

#include "LLVMHandler.h"
#include "InstructionCategory.h"

LLVMHandler::LLVMHandler(std::string file) {
    //this->module = std::move(llvm::parseIRFile(file, this->error, this->context));
    this->module = llvm::parseIRFile(file, this->error, this->context).release();
}

void LLVMHandler::print(  ) {
    for ( auto &F : *this->module ) {
        llvm::outs() << "Function " << F.getName() << "\n";
        for ( auto &BB : F ) {
            for ( auto &I : BB ) {
                llvm::outs() <<  I.getOpcodeName();
                llvm::outs() << " -> " << InstructionCategory::toString( InstructionCategory::getCategory( I ) ) << "\n";
            }
            llvm::outs() << "\n=======================================\n";
        }
    }
}