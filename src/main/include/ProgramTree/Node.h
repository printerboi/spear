//
// Created by maximiliank on 05.02.23.
//

#ifndef BA_NODE_H
#define BA_NODE_H

#include "vector"
#include "llvm/IR/BasicBlock.h"

class Node {
    public:
        std::vector<llvm::BasicBlock *> predecessors;
        std::vector<llvm::BasicBlock *> successors;
        std::vector<llvm::BasicBlock *> blocks;

    virtual std::string toString();
};


#endif //BA_NODE_H
