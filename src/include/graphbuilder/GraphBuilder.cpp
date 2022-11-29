//
// Created by max on 26.11.22.
//

#include "GraphBuilder.h"
#include "ProgramDependenceGraph.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Analysis/CFG.h"
#include "llvm/Analysis/DDG.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Analysis/DependenceAnalysis.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/Analysis/TargetTransformInfoImpl.h"
#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/IR/Dominators.h"


GraphBuilder::GraphBuilder(std::string irFilePath) {
    std::unique_ptr<llvm::LLVMContext> Context(new llvm::LLVMContext);
    llvm::SMDiagnostic diagnostics;
    std::unique_ptr<llvm::Module> mod = llvm::parseIRFile(irFilePath, diagnostics, *Context);

    bool broken_debug_info = false;
    if (mod == nullptr ||
        llvm::verifyModule(*mod, &llvm::errs(), &broken_debug_info)) {
        llvm::errs() << "error: module not valid\n";
    }else{
        if (broken_debug_info) {
            llvm::errs() << "caution: debug info is broken\n";
        }else{
            llvm::errs() << "Parsing successfull. Generating the DDG\n";
        }

        auto mainFunction = mod->getFunction("main");

        llvm::TargetLibraryInfoImpl Impl = llvm::TargetLibraryInfoImpl();
        llvm::TargetLibraryInfo TLI = llvm::TargetLibraryInfo(Impl, mainFunction);
        llvm::AAResults AA = llvm::AAResults(TLI);

        llvm::AssumptionCache assCache = llvm::AssumptionCache(*mainFunction);
        llvm::DominatorTree dominatorTree = llvm::DominatorTree(*mainFunction);
        llvm::LoopInfo loopInf = llvm::LoopInfo(dominatorTree);
        llvm::ScalarEvolution scarev = llvm::ScalarEvolution(*mainFunction, TLI, assCache, dominatorTree, loopInf);

        llvm::DependenceInfo DI = llvm::DependenceInfo(mainFunction, &AA, &scarev, &loopInf);
        llvm::DataDependenceGraph DDG = llvm::DataDependenceGraph(*mainFunction, DI);
    }

}

ProgramDependenceGraph GraphBuilder::construct() {

    return {};
}


int main(){
    GraphBuilder G = GraphBuilder("../lltest.ll");

    llvm::llvm_shutdown();
    return 0;
}
