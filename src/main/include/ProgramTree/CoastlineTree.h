//
// Created by maximiliank on 05.02.23.
//

#ifndef BA_COASTLINETREE_H
#define BA_COASTLINETREE_H


#include <vector>
#include <llvm/IR/BasicBlock.h>

class CoastlineTree {
    CoastlineTree();
    std::vector<CoastlineTree> predecessors;
    std::vector<CoastlineTree> successors;
    CoastlineTree construct(llvm::BasicBlock *bb);
};


#endif //BA_COASTLINETREE_H
