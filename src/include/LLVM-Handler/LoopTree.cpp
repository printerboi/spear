//
// Created by maximiliank on 26.01.23.
//

#include "LoopTree.h"


LoopTree::LoopTree(llvm::Loop *main, std::vector<llvm::Loop *> subloops, LoopTree *parent, LLVMHandler *handler){
    this->mainloop = main;
    this->parent = parent;
    this->handler = handler;

    for (auto sl : subloops) {
        LoopTree Slt = LoopTree(sl, sl->getSubLoops(), this, this->handler);

        this->subTrees.push_back(Slt);
    }

    std::vector<llvm::BasicBlock *> calcedBlocks = calcBlocks();
    this->blocks.insert(this->blocks.end(), calcedBlocks.begin(), calcedBlocks.end());

    this->iterations = LLVMHandler::getLoopUpperBound(this->mainloop, 1000);
}


std::vector<llvm::BasicBlock *> LoopTree::calcBlocks(){
    std::vector<llvm::BasicBlock *> initBlocks = this->mainloop->getBlocksVector();
    std::vector<llvm::BasicBlock *> unized;
    std::vector<llvm::BasicBlock *> diff;

    if( !this->isLeaf() ){

        for(auto subloop : this->subTrees){
            unized.insert(unized.end(), subloop.mainloop->getBlocksVector().begin(), subloop.mainloop->getBlocksVector().end());
        }


        for (auto &BB : initBlocks) {
            if(std::find(unized.begin(), unized.end(), BB) == unized.end()){
                //llvm::outs() << "Found " << BB->getName() << "\n";
                diff.insert(diff.end(), BB);
            }
        }

        return diff;
    }else{
        return initBlocks;
    }

    return initBlocks;
}

void LoopTree::addSubloop(llvm::Loop *L) {

}

bool LoopTree::isLeaf() {
    return this->subTrees.empty();
}

void LoopTree::printPreOrder() {

    if (this->isLeaf()) {
        llvm::outs() << "-------------------------------------------\n";
        llvm::outs() << this->mainloop->getName() << " (LEAF) " << "i=" << this->iterations << " e=" << (long) this->calcEnergy() << " n= " << this->mainloop->getBlocksVector().size() << "\n";
        llvm::outs() << "-------------------------------------------\n";
    }else{
        for (auto slt: this->subTrees) {
            slt.printPreOrder();
        }
        llvm::outs() << "-------------------------------------------\n";
        llvm::outs() << this->mainloop->getName() << " (NODE) "<< "i=" << this->iterations << " e=" << (long) this->calcEnergy() << " n= " << this->mainloop->getBlocksVector().size() << "\n";
        llvm::outs() << "-------------------------------------------\n";

    }
}

double LoopTree::calcEnergy() {
    double sum = 0;
    double result = 0;

    if(this->isLeaf()){
        for (auto &BB : this->blocks) {
            sum = sum + this->handler->getBasicBlockSum(*BB);
        }

        result = sum * (double) this->iterations;

        return result;
    }else{
        for (auto &BB : this->blocks) {
            sum = sum + this->handler->getBasicBlockSum(*BB);
        }

        for (auto subTree : this->subTrees) {
            sum = sum + subTree.calcEnergy();
        }

        result = sum * (double) this->iterations;

        return result;
    }
}
