//
// Created by maximiliank on 03.02.23.
//

#ifndef BA_PROGRAMTREE_H
#define BA_PROGRAMTREE_H


#include <vector>
#include "llvm/IR/BasicBlock.h"

class ProgramTree {
    public:
        std::vector<ProgramTree *> subtrees;
        std::vector<llvm::BasicBlock *> blocks;

        virtual bool isLeaf();
        virtual void printPreOrder() = 0;
        virtual void calcEnergy() = 0;
};


#endif //BA_PROGRAMTREE_H
