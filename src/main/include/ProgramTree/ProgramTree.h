//
// Created by maximiliank on 03.02.23.
//

#ifndef BA_PROGRAMTREE_H
#define BA_PROGRAMTREE_H


#include <vector>
#include <llvm/Analysis/RegionInfo.h>
#include "llvm/IR/BasicBlock.h"
#include "Node.h"
#include "Edge.h"
#include "LoopNode.h"

class ProgramTree {
    public:
        std::vector<Node *> nodes;
        std::vector<Edge *> edges;

        static ProgramTree* construct(llvm::Function &F);
        void printNodes();
        void printEdges();
        Node *findBlock(llvm::BasicBlock *BB);
        void removeNode(Node *N);
        void removeOrphanedEdges();
        bool replaceNodesWithLoopNode(std::vector<llvm::BasicBlock *> blocks, LoopNode *LPN);
};


#endif //BA_PROGRAMTREE_H
