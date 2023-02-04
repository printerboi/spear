//
// Created by maximiliank on 05.02.23.
//

#include <llvm/IR/CFG.h>
#include "CoastlineTree.h"

CoastlineTree CoastlineTree::construct(llvm::BasicBlock *bb) {
    if(bb->hasNPredecessors(0)){

    }else if(bb->getTerminator()->getNumSuccessors() == 0){

    }else{
        for (auto it = llvm::pred_begin(bb), et = llvm::pred_end(bb); it != et; ++it)
        {
            auto pred = *it;

        }
    }


    return CoastlineTree();
}
