//
// Created by max on 13.01.23.
//

#include "LLVMHandler.h"

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
                if( llvm::isa<llvm::StoreInst>(I) ){
                    llvm::outs() << " -> Is Store Instruction!" << "\n";
                }else if(llvm::isa<llvm::CallInst>(I) ){
                    llvm::outs() << " -> Is Call Instruction!" << "\n";
                }else if( llvm::isa<llvm::ReturnInst>(I) ){
                    llvm::outs() << " -> Is Return Instruction!" << "\n";
                }else{
                    llvm::outs() << " -> Is Generic Instruction!" << "\n";
                }
            }
            llvm::outs() << "\n=======================================\n";
        }
    }
}