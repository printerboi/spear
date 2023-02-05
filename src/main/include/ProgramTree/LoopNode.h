//
// Created by maximiliank on 05.02.23.
//

#ifndef BA_LOOPNODE_H
#define BA_LOOPNODE_H


#include "Node.h"
#include "../LoopTree/LoopTree.h"

class LoopNode : public Node {
    public:
        LoopNode(LoopTree *LT);
        LoopTree *loopTree;

        std::string toString();
};


#endif //BA_LOOPNODE_H
