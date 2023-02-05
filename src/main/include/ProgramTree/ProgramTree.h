//
// Created by maximiliank on 03.02.23.
//

#ifndef BA_PROGRAMTREE_H
#define BA_PROGRAMTREE_H


#include <vector>
#include "llvm/IR/BasicBlock.h"
#include "Node.h"

class ProgramTree {
    public:
        ProgramTree(Node *start);
        ProgramTree constructPhaseITree(llvm::BasicBlock * blocks);
        Node *startNode;
};


#endif //BA_PROGRAMTREE_H
