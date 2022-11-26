//
// Created by max on 26.11.22.
//

#include "GraphBuilder.h"
#include "llvm-12/llvm/IR/Instructions.h"
#include "llvm-12/llvm/IR/LLVMContext.h"
#include "llvm-12/llvm/IRReader/IRReader.h"
#include "llvm-12/llvm/Support/SourceMgr.h"

GraphBuilder::GraphBuilder(std::string irFilePath) {
    std::unique_ptr<llvm::LLVMContext> Context(new llvm::LLVMContext);
    llvm::SMDiagnostic diagnostics;
    this->module = llvm::parseIRFile(irFilePath, diagnostics, *Context);
}

ProgramDependenceGraph GraphBuilder::construct() {



    return {};
}
