//
// Created by maximiliank on 03.02.23.
//

#include "ProgramTree.h"

ProgramTree::ProgramTree(Node *start) {
    this->startNode = start;
}

ProgramTree ProgramTree::constructPhaseITree(llvm::BasicBlock * block, Node *parent) {
    if (block->getTerminator()->getNumSuccessors() == 0){
        return nullptr;
    }else{
        Node *N = new Node(block);




        ProgramTree *P  = new ProgramTree(N);
    }


    return ProgramTree(nullptr);
}


