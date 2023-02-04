//
// Created by maximiliank on 31.01.23.
//

#include <llvm/IR/Function.h>
#include "IfTree.h"

IfTree::IfTree(llvm::BasicBlock *head) {
    this->header = head;
}

void IfTree::calcBlocksForHeader() {
    std::vector<llvm::BasicBlock *> currBlocks;

    if( this->header->getTerminator()->getNumSuccessors() == 2 ){
        for(int i = 0; i < this->header->getTerminator()->getNumSuccessors(); i++){
            llvm::BasicBlock *bb = this->header->getTerminator()->getSuccessor(i);

        }
    }
}
