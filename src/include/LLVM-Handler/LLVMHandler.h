//
// Created by max on 13.01.23.
//

#ifndef BA_LLVMHANDLER_H
#define BA_LLVMHANDLER_H

#include <filesystem>
#include "iostream"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/IR/Instructions.h"


class LLVMHandler {
    public:
        LLVMHandler( std::string file );
        void print(  );
    private:
        //std::unique_ptr<llvm::Module> module;
        llvm::Module *module;
        llvm::LLVMContext context;
        llvm::SMDiagnostic error;
};


#endif //BA_LLVMHANDLER_H
