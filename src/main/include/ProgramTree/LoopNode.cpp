//
// Created by maximiliank on 05.02.23.
//

#include "LoopNode.h"
#include "ProgramTree.h"

LoopNode::LoopNode(LoopTree *LT) {
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

LoopNode* LoopNode::construct(LoopTree *lptr) {
    LoopNode *LN = new LoopNode(lptr);

    ProgramTree *PT = ProgramTree::construct(LN->loopTree->mainloop->getBlocksVector());
    LN->subtrees.push_back(PT);

    return LN;
}
