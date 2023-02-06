//
// Created by maximiliank on 05.02.23.
//

#include "ProgramTree.h"

LoopNode::LoopNode(LoopTree *LT, ProgramTree *parent) : Node(parent) {
    this->loopTree = LT;
}

std::string LoopNode::toString() {
    std::string output;

    const void* addr = static_cast<const void*>(this);
    std::stringstream ss;
    ss << addr;

    output.append("LOOPNODE ");
    output.append(ss.str());

    return output;
}

bool LoopNode::isLeafNode() {
    return this->loopTree->subTrees.empty();
}

LoopNode* LoopNode::construct(LoopTree *lptr, ProgramTree *parent) {
    LoopNode *LN = new LoopNode(lptr, parent);

    if(LN->isLeafNode()){
        ProgramTree *PT = ProgramTree::construct(LN->loopTree->mainloop->getBlocksVector());
        LN->subtrees.push_back(PT);
    }else{
        ProgramTree *PT = ProgramTree::construct(LN->loopTree->mainloop->getBlocksVector());
        LN->subtrees.push_back(PT);

        for(auto *subTree : LN->loopTree->subTrees){
            LoopNode *SLN = LoopNode::construct(subTree, PT);

            //ProgramTree *SPT = ProgramTree::construct(subTree->mainloop->getBlocksVector());
            //SLN->subtrees.push_back(SPT);

            std::vector<std::string> allblocks;
            for(auto nb : subTree->mainloop->getBlocksVector()){
                allblocks.push_back(nb->getName().str());
            }

            //LoopNode *subLN = LoopNode::construct(&subTree);
            PT->replaceNodesWithLoopNode(subTree->mainloop->getBlocksVector(), SLN);
        }
    }

    LN->removeLoopEdgesFromSubtrees();



    return LN;
}

double LoopNode::getEnergy(LLVMHandler *handler) {
    double sum = 0.0;
    auto adjsNodes = this->getAdjacentNodes();

    for(auto subTrees : this->subtrees){
        sum += subTrees->getEnergy(handler);
    }

    sum = (double) this->loopTree->iterations *sum;

    if(!adjsNodes.empty()){
        double min = DBL_MAX;
        for(auto N : adjsNodes){
            double locsum = N->getEnergy(handler);
            if (locsum < min){
                min = locsum;

            }
        }

        sum += min;
    }

    return sum;
}

void LoopNode::removeLoopEdgesFromSubtrees(){
    for(auto sT : this->subtrees){
        if(!sT->containsLoopNodes()){
            auto *latchblock = this->loopTree->mainloop->getLoopLatch();
            auto *latchnode = sT->findBlock(latchblock);
            std::string lnm = this->loopTree->mainloop->getLoopLatch()->getName().str();
            std::vector<Edge *> tempedges;

            for(auto e : sT->edges){
                if(e->start != latchnode){
                    tempedges.push_back(e);
                }
            }

            sT->edges = tempedges;
        }else{
            auto *latchblock = this->loopTree->mainloop->getLoopLatch();
            auto *latchnode = sT->findBlock(latchblock);
            std::string lnm = this->loopTree->mainloop->getLoopLatch()->getName().str();
            std::vector<Edge *> tempedges;

            for(auto e : sT->edges){
                if(e->start != latchnode){
                    tempedges.push_back(e);
                }
            }

            sT->edges = tempedges;


            auto subLoopNodes = sT->getLoopNodes();
            for(auto sln : subLoopNodes){
                sln->removeLoopEdgesFromSubtrees();
            }
        }
    }
}

