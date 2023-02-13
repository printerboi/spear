
#include "LoopTree.h"


LoopTree::LoopTree(llvm::Loop *main, std::vector<llvm::Loop *> subloops, LLVMHandler *handler){
    this->mainloop = main;
    this->handler = handler;

    //Iterate over the given Subloops
    for (auto sl : subloops) {
        //For each subloop create a new LoopTree with parameters regarding this subloop
        LoopTree *Slt = new LoopTree(sl, sl->getSubLoops(), this->handler);

        //Add the subtree to the vector of subtrees
        this->subTrees.push_back(Slt);
    }

    //Calculate the basic blocks only contained in this loop, excluding all blocks present in the subtrees
    std::vector<llvm::BasicBlock *> calcedBlocks = calcBlocks();
    this->blocks.insert(this->blocks.end(), calcedBlocks.begin(), calcedBlocks.end());

    //Calculate the iterations of this loop
    this->iterations = handler->getLoopUpperBound(this->mainloop);
}


std::vector<llvm::BasicBlock *> LoopTree::calcBlocks(){
    //All the blocks present in the loop
    std::vector<llvm::BasicBlock *> initBlocks = this->mainloop->getBlocksVector();
    //Vector for storing the combined blocks of the subloops
    std::vector<llvm::BasicBlock *> unized;
    //Vector for string the calculated difference of this loop and its subloops
    std::vector<llvm::BasicBlock *> diff;

    //Test the leafness of the current LoopTree
    if( !this->isLeaf() ){
        //If we are not in a leaf, we have to calculate the blocks of this tree by building the set-difference
        //of all subloops and the initblocks present in this subloop
        for(auto subloop : this->subTrees){
            //Calculate the union of all subloops
            unized.insert(unized.end(), subloop->mainloop->getBlocksVector().begin(), subloop->mainloop->getBlocksVector().end());
        }

        //Iterate over the blocks in this loop. Find the blocks that are not present in the union but in this loop
        for (auto &BB : initBlocks) {
            if(std::find(unized.begin(), unized.end(), BB) == unized.end()){
                diff.insert(diff.end(), BB);
            }
        }

        return diff;
    }else{
        //If we are in a leaf, we can simply return the init blocks, as there are no subloops
        return initBlocks;
    }
}

bool LoopTree::isLeaf() {
    return this->subTrees.empty();
}

void LoopTree::printPreOrder() {

    if (this->isLeaf()) {
        llvm::outs() << "-------------------------------------------\n";
        llvm::outs() << this->mainloop->getName() << " (LEAF) " << "i=" << this->iterations << "\n";
        llvm::outs() << "-------------------------------------------\n";
        for (auto bb : this->blocks) {
            bb->print(llvm::outs());
        }
    }else{
        for (auto slt: this->subTrees) {
            slt->printPreOrder();
        }
        llvm::outs() << "-------------------------------------------\n";
        llvm::outs() << this->mainloop->getName() << " (NODE) "<< "i=" << this->iterations  << "\n";
        llvm::outs() << "-------------------------------------------\n";
        for (auto bb : this->blocks) {
            bb->print(llvm::outs());
        }

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
            sum = sum + subTree->calcEnergy();
        }

        result = sum * (double) this->iterations;

        return result;
    }
}

std::vector<llvm::BasicBlock *> LoopTree::getLatches() {
    if(this->isLeaf()){
        std::vector<llvm::BasicBlock *> latches;
        latches.push_back(this->mainloop->getLoopLatch());
        return latches;
    }else{
        std::vector<llvm::BasicBlock *> latches;

        for (auto sT : this->subTrees) {
            std::vector<llvm::BasicBlock *> calced = sT->getLatches();
            calced.push_back(this->mainloop->getLoopLatch());
            for(auto &lb : calced){
                if(std::find(latches.begin(), latches.end(), lb) == latches.end()){
                    latches.push_back(lb);
                }
            }
        }

        return latches;
    }
}