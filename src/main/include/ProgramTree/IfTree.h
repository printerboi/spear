//
// Created by maximiliank on 31.01.23.
//

#ifndef BA_IFTREE_H
#define BA_IFTREE_H


#include <llvm/IR/BasicBlock.h>
#include "../LLVM-Handler/LLVMHandler.h"
#include "ProgramTree.h"

class IfTree : public ProgramTree{
    public:
        llvm::BasicBlock *header;

        IfTree(llvm::BasicBlock * head);
    private:
        void calcBlocksForHeader();
};


#endif //BA_IFTREE_H
