//
// Created by maximiliank on 15.02.23.
//

#ifndef BA_FUNCTIONTREE_H
#define BA_FUNCTIONTREE_H


#include <llvm/IR/Function.h>
#include "../LLVM-Handler/InstructionCategory.h"

class FunctionTree {
public:
    std::vector<FunctionTree *> subtrees;
    llvm::Function *func;
    std::string name;

    FunctionTree(llvm::Function * func);

    static FunctionTree* construct(llvm::Function * func);

    void printPreorder();

    std::vector<llvm::Function *> getPreOrderVector();

private:
    std::vector<llvm::Function *> getCalledFunctions();


};


#endif //BA_FUNCTIONTREE_H
