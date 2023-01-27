//
// Created by maximiliank on 26.01.23.
//

#include "LoopTree.h"


LoopTree::LoopTree(llvm::Loop *main, std::vector<llvm::Loop *> subloops, LoopTree *parent){
    this->mainloop = main;
    this->parent = parent;

    for (auto sl : subloops) {
        LoopTree Slt = LoopTree(sl, sl->getSubLoops(), this);

        this->subTrees.push_back(Slt);
    }

    this->blocks = calcBlocks();
}


std::vector<llvm::BasicBlock *> LoopTree::calcBlocks(){
    std::vector<llvm::BasicBlock *> initBlocks = this->mainloop->getBlocksVector();
    std::vector<llvm::BasicBlock *> unized;
    std::vector<llvm::BasicBlock *> diff;

    //todo fix
    if( !this->isLeaf() ){
        llvm::StringRef n = this->mainloop->getName();

        //llvm::outs() << "| " << n << " |\n";

        for(auto subloop : this->subTrees){

            unized.insert(unized.end(), subloop.blocks.begin(), subloop.blocks.end());
        }


        for (auto &BB : initBlocks) {
            if(std::find(unized.begin(), unized.end(), BB) == unized.end()){
                diff.insert(diff.end(), BB);
            }
        }



        //std::set_difference(initBlocks.begin(), initBlocks.end(), unized.begin(), unized.end(), diff.begin());

        /*if(this->blocks.empty()){
            //diff.insert(diff.end(), unized.begin(), unized.end());
            std::set_difference(initBlocks.begin(), initBlocks.end(), unized.begin(), unized.end(), diff.begin());
        }else{
            std::set_difference(this->blocks.begin(), this->blocks.end(), unized.begin(), unized.end(), diff.begin());
        }*/

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
        llvm::outs() << this->mainloop->getName() << " (LEAF) " << "\n";

        for (auto &BB : this->blocks) {
            llvm::outs() << "\t";
            BB->print(llvm::outs());
            llvm::outs() << "\n";
        }
        llvm::outs() << "-------------------------------------------\n";
    }else{
        for (auto slt: this->subTrees) {
            slt.printPreOrder();
        }
        llvm::outs() << "-------------------------------------------\n";
        llvm::outs() << this->mainloop->getName() << " (NODE) " << "\n";
        for (auto &BB : this->blocks) {
            llvm::outs() << "\t";
            BB->print(llvm::outs());
            llvm::outs() << "\n";
        }
        llvm::outs() << "-------------------------------------------\n";

    }
}
