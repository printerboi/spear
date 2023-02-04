//
// Created by maximiliank on 03.02.23.
//

#include "ProgramTree.h"

bool ProgramTree::isLeaf() {
    return this->subtrees.empty();
}

ProgramTree ProgramTree::construct(llvm::Region *region) {
    if(!regionHasSubregion(region)){
        ProgramTree P = ProgramTree(region);
        for (auto *bb : region->blocks()) {
            P.blocks.push_back(bb);
        }

        return P;
    }else{
        ProgramTree P = ProgramTree(region);
        P.region = region;

        for (auto &sr : *region) {
            auto subregion = sr.get();
            ProgramTree srPT = construct(subregion);
            P.subtrees.push_back(srPT);
        }

        P.calcBlocks();

        return P;
    }
}

bool ProgramTree::regionHasSubregion(llvm::Region *reg) {
    int subRegCounter = 0;
    for (auto &sr : *reg) {
        subRegCounter++;
    }

    return subRegCounter != 0;
}

ProgramTree::ProgramTree(llvm::Region *region) {
    this->region = region;
}

void ProgramTree::printPreOrder() {
    if(this->isLeaf()){
        llvm::outs() << "=================================\n";
        llvm::outs() << this->region->getNameStr() << " (LEAF) \n";
        for (auto *bb : this->blocks) {
            bb->print(llvm::outs());
        }
        llvm::outs() << "=================================\n";
    }else{
        for (auto spt : this->subtrees) {
            spt.printPreOrder();
        }

        llvm::outs() << "=================================\n";
        llvm::outs() << this->region->getNameStr() << " (NODE) \n";
        for (auto *bb : this->calcBlocks()) {
            bb->print(llvm::outs());
        }
        llvm::outs() << "=================================\n";
    }
}

std::vector<llvm::BasicBlock *> ProgramTree::calcBlocks() {
    if(this->isLeaf()){
        std::vector<llvm::BasicBlock *> bbset;
        for (auto *bb : region->blocks()) {
            bbset.push_back(bb);
        }

        bbset.push_back(this->region->getExit());

        this->blocks = bbset;

        return bbset;
    }else{
        std::vector<llvm::BasicBlock *> bbSubset;
        std::vector<llvm::BasicBlock *> diff;
        std::vector<llvm::BasicBlock *> all;
        for (auto spt : this->subtrees) {
            std::vector<llvm::BasicBlock *> singleBBSubs = spt.calcBlocks();

            for (auto bb : spt.region->blocks()) {
                bbSubset.push_back(bb);
            }
        }

        for (auto *bb : region->blocks()) {
            all.push_back(bb);
        }

        for (auto *bb : all) {
            if (std::find(bbSubset.begin(), bbSubset.end(), bb) == bbSubset.end()){
                diff.push_back(bb);
            }
        }



        return diff;
    }
}
