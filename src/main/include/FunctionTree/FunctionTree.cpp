//
// Created by maximiliank on 15.02.23.
//

#include <llvm/IR/Instructions.h>
#include "FunctionTree.h"

FunctionTree::FunctionTree(llvm::Function *func) {
    this->func = func;
    this->name = func->getName().str();
}

FunctionTree* FunctionTree::construct(llvm::Function *func) {
    auto *FT = new FunctionTree(func);
    auto calls = FT->getCalledFunctions();

    if(!calls.empty()){
        for (auto &F : calls) {
            if(F != FT->func){
                auto sFT = construct(F);
                FT->subtrees.push_back(sFT);
            }
        }
    }

    return FT;
}

std::vector<llvm::Function *> FunctionTree::getCalledFunctions() {
    std::vector<llvm::Function *> funcs;

    for (auto &BB : *this->func) {
        for( auto &I : BB){
            if(InstructionCategory::isCallInstruction(I)){
                auto calleeInst = llvm::cast<llvm::CallInst>(&I);
                auto *cf = calleeInst->getCalledFunction();
                funcs.push_back(cf);
            }
        }
    }

    return funcs;
}

void FunctionTree::printPreorder() {
    if(this->subtrees.empty()){
        llvm::outs() << "------------Leaf-----------\n";
        llvm::outs() << "Node " << this->func->getName() << "\n";
    }else{
        llvm::outs() << "============================================\n";
        for (auto sft : this->subtrees) {
            sft->printPreorder();
        }
        llvm::outs() << "------------Node-----------\n";
        llvm::outs() << "Node " << this->func->getName() << "\n";
        llvm::outs() << "============================================\n\n\n\n";
    }

}

std::vector<llvm::Function *> FunctionTree::getPreOrderVector() {
    std::vector<llvm::Function *> list;

    if(this->subtrees.empty()){
        list.push_back(this->func);
    }else{
        for (auto sft : this->subtrees) {
            auto sublist = sft->getPreOrderVector();
            for(auto F : sublist){
                if(std::find(list.begin(), list.end(), F) == list.end()){
                    list.push_back(F);
                }
            }
        }
        list.push_back(this->func);
    }

    return list;
}


