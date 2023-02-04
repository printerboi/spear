//
// Created by maximiliank on 03.02.23.
//

#ifndef BA_PROGRAMTREE_H
#define BA_PROGRAMTREE_H


#include <vector>
#include <llvm/Analysis/RegionInfo.h>
#include "llvm/IR/BasicBlock.h"

class ProgramTree {
    public:
        ProgramTree(llvm::Region *region);

        std::vector<ProgramTree> subtrees;
        llvm::Region *region;
        std::vector<llvm::BasicBlock *> blocks;

        static ProgramTree construct(llvm::Region *reg);
        bool isLeaf();
        void printPreOrder() ;
        void calcEnergy();
    private:
        static bool regionHasSubregion(llvm::Region *reg);
        std::vector<llvm::BasicBlock *> calcBlocks();
        boo
};


#endif //BA_PROGRAMTREE_H
