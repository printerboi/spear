//
// Created by maximiliank on 05.02.23.
//

#ifndef BA_NODE_H
#define BA_NODE_H


#include "ProgramTree.h"
#include "../LLVM-Handler/LLVMHandler.h"

class Node {
    public:
        Node(llvm::BasicBlock *block);
        LLVMHandler handler;
        std::vector<llvm::BasicBlock *> blocks;
        std::vector<Node *> subnodes;
        Node *predecessor;
        Node *successor;
        bool IsStart();
        bool IsEnd();
        double calcEnergy();
};


#endif //BA_NODE_H
