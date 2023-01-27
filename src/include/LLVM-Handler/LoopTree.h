//
// Created by maximiliank on 26.01.23.
//

#ifndef BA_LOOPTREE_H
#define BA_LOOPTREE_H


#include <vector>
#include <llvm/Analysis/LoopInfo.h>

class LoopTree {
    public:
        llvm::Loop * mainloop;
        LoopTree * parent;
        std::vector<LoopTree> subTrees;
        std::vector<llvm::BasicBlock *> blocks;
        long iterations;

        LoopTree();
        LoopTree(llvm::Loop *main, std::vector<llvm::Loop *> subloops, LoopTree *parent);

        double calcEnergy();
        void addSubloop( llvm::Loop * L );
        void printPreOrder();
    private:
        std::vector<llvm::BasicBlock *> calcBlocks();
        bool isLeaf();
};


#endif //BA_LOOPTREE_H
