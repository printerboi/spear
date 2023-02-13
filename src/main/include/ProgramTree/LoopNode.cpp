#include "ProgramTree.h"

//Creates a LoopNode with the given LoopTree and ProgramTree
LoopNode::LoopNode(LoopTree *LT, ProgramTree *parent) : Node(parent) {
    this->loopTree = LT;
}

//Get the string representation of this LoopNode
std::string LoopNode::toString() {
    //Init the output string
    std::string output;

    //Get the Address of this LoopNode
    const void* addr = static_cast<const void*>(this);
    std::stringstream ss;
    ss << addr;

    //Add the address to the output
    output.append("LOOPNODE ");
    output.append(ss.str());

    //Return output string
    return output;
}

//Test if this LoopNde has subtrees
bool LoopNode::isLeafNode() const {
    return this->loopTree->subTrees.empty();
}

//Construct a LoopTree by recursively calling this method until we reach a leaf
LoopNode* LoopNode::construct(LoopTree *lptr, ProgramTree *parent) {
    //Create a Toplevel LoopNode
    LoopNode *LN = new LoopNode(lptr, parent);

    //End-condition
    if(LN->isLeafNode()){
        //Create a ProgramTree from this LoopTrees mainloop
        ProgramTree *PT = ProgramTree::construct(LN->loopTree->mainloop->getBlocksVector());

        //Add the ProgramTree to the list of subtrees
        LN->subtrees.push_back(PT);
    }else{
        //Further recursion

        //Create a ProgramTree from this LoopTrees mainloop
        ProgramTree *PT = ProgramTree::construct(LN->loopTree->mainloop->getBlocksVector());
        //Add the ProgramTree to the list of subtrees
        LN->subtrees.push_back(PT);

        //Iterate over the subloops of this LoopNodes LoopTree
        for(auto *subTree : LN->loopTree->subTrees){
            //Construct a LoopNode for the current Sub-LoopTree
            LoopNode *SLN = LoopNode::construct(subTree, PT);

            //ProgramTree *SPT = ProgramTree::construct(subTree->mainloop->getBlocksVector());
            //SLN->subtrees.push_back(SPT);

            //Init the list of blocks contained in the subloop
            std::vector<std::string> allblocks;

            //Add the subloops blocks to the list of contained blocks
            for(auto nb : subTree->mainloop->getBlocksVector()){
                allblocks.push_back(nb->getName().str());
            }

            //LoopNode *subLN = LoopNode::construct(&subTree);

            //Replace the nodes in the sub-ProgramTree
            PT->replaceNodesWithLoopNode(subTree->mainloop->getBlocksVector(), SLN);
        }
    }

    //Remove the loop-edges in this LoopNode, so we won't create infinite recursions
    LN->removeLoopEdgesFromSubtrees();

    //Return the LoopNode
    return LN;
}

//Get the LoopNodes energy
double LoopNode::getEnergy(LLVMHandler *handler) {
    //Init the calculation result
    double sum = 0.0;
    //Calculate the adjacent nodes
    auto adjsNodes = this->getAdjacentNodes();

    //Get the energy from all contained subtrees
    for(auto subTrees : this->subtrees){
        sum += subTrees->getEnergy(handler);
    }

    //Multiply the calculated energy from the subtrees by the iterations of this LoopNode's loop
    sum = (double) this->loopTree->iterations * sum;

    //Handle if-conditions contained in this LoopNode, if we're dealing with a leaf-Node
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

    //Return the calculation result
    return sum;
}


//Remove the loop-edges from the LoopNode
void LoopNode::removeLoopEdgesFromSubtrees(){
    //Iterate over the ProgramTrees contained in this LoopNode
    for(auto sT : this->subtrees){
        //Get the BasicBlock used as latch in this ProgramTrees LoopTree
        auto *latchblock = this->loopTree->mainloop->getLoopLatch();
        //Get the Node the latchblock is contained in
        auto *latchnode = sT->findBlock(latchblock);
        //Init the list of edges we want to keep
        std::vector<Edge *> tempedges;

        //Iterate over the edges contained in the Sub-ProgramTree
        for(auto e : sT->edges){
            //If the start node is not the latch of the loop, add it to the list of edges we want to keep
            if(e->start != latchnode){
                tempedges.push_back(e);
            }
        }

        //Set the edges-list of the Sub-ProgramTree to the calculated list
        sT->edges = tempedges;

        //If we have further LoopNodes contained in this LoopNode, remove their loopedges too
        if(sT->containsLoopNodes()){
            auto subLoopNodes = sT->getLoopNodes();
            for(auto sln : subLoopNodes){
                sln->removeLoopEdgesFromSubtrees();
            }
        }
    }
}
