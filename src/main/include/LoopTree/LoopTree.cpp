
#include "LoopTree.h"


LoopTree::LoopTree(llvm::Loop *main, const std::vector<llvm::Loop *>& subloops, LLVMHandler *handler, llvm::ScalarEvolution *scalarEvolution){
    this->mainloop = main;
    this->handler = handler;

    //Iterate over the given Subloops
    for (auto subLoop : subloops) {
        //For each subloop create a new LoopTree with parameters regarding this subloop
        auto *subLoopTree = new LoopTree(subLoop, subLoop->getSubLoops(), this->handler, scalarEvolution);

        //Add the subtree to the vector of subgraphs
        this->subTrees.push_back(subLoopTree);
    }

    //Calculate the basic blocks only contained in this loop, excluding all blocks present in the subgraphs
    std::vector<llvm::BasicBlock *> calcedBlocks = calcBlocks();
    this->blocks.insert(this->blocks.end(), calcedBlocks.begin(), calcedBlocks.end());

    this->iterations=0;
    //Calculate the iterations of this loop
    this->iterations = handler->getLoopUpperBound(this->mainloop, scalarEvolution);
}


std::vector<llvm::BasicBlock *> LoopTree::calcBlocks(){
    //All the blocks present in the loop
    std::vector<llvm::BasicBlock *> initBlocks = this->mainloop->getBlocksVector();
    //Vector for storing the combined blocks of the subloops
    std::vector<llvm::BasicBlock *> combined;
    //Vector for string the calculated difference of this loop and its subloops
    std::vector<llvm::BasicBlock *> difference;

    //Test the leafness of the current LoopTree
    if( !this->isLeaf() ){
        //If we are not in a leaf, we have to calculate the blocks of this tree by building the set-difference
        //of all subloops and the initblocks present in this subloop
        for(auto subloop : this->subTrees){
            //Calculate the union of all subloops
            combined.insert(combined.end(), subloop->mainloop->getBlocksVector().begin(), subloop->mainloop->getBlocksVector().end());
        }

        //Iterate over the blocks in this loop. Find the blocks that are not present in the union but in this loop
        for (auto &basicBlock : initBlocks) {
            if(std::find(combined.begin(), combined.end(), basicBlock) == combined.end()){
                difference.insert(difference.end(), basicBlock);
            }
        }

        return difference;
    }else{
        //If we are in a leaf, we can simply return the init blocks, as there are no subloops
        return initBlocks;
    }
}

bool LoopTree::isLeaf() const {
    return this->subTrees.empty();
}

void LoopTree::printPreOrder() {

    if (this->isLeaf()) {
        llvm::outs() << "-------------------------------------------\n";
        llvm::outs() << this->mainloop->getName() << " (LEAF) " << "i=" << this->iterations << "\n";
        llvm::outs() << "-------------------------------------------\n";
        for (auto basicBlock : this->blocks) {
            basicBlock->print(llvm::outs());
        }
    }else{
        for (auto subLoopTree: this->subTrees) {
            subLoopTree->printPreOrder();
        }
        llvm::outs() << "-------------------------------------------\n";
        llvm::outs() << this->mainloop->getName() << " (NODE) "<< "i=" << this->iterations  << "\n";
        llvm::outs() << "-------------------------------------------\n";
        for (auto basicBlock : this->blocks) {
            basicBlock->print(llvm::outs());
        }

    }
}

/*
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
}*/

std::vector<llvm::BasicBlock *> LoopTree::getLatches() {
    if(this->isLeaf()){
        std::vector<llvm::BasicBlock *> latches;
        latches.push_back(this->mainloop->getLoopLatch());
        return latches;
    }else{
        std::vector<llvm::BasicBlock *> latches;

        for (auto subTree : this->subTrees) {
            std::vector<llvm::BasicBlock *> subTreeLatches = subTree->getLatches();
            subTreeLatches.push_back(this->mainloop->getLoopLatch());
            for(auto &latch : subTreeLatches){
                if(std::find(latches.begin(), latches.end(), latch) == latches.end()){
                    latches.push_back(latch);
                }
            }
        }

        return latches;
    }
}

LoopTree::~LoopTree(){
    for(auto loopTree : this->subTrees){
        delete loopTree;
    }


}