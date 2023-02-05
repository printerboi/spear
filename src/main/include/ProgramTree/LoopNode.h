//
// Created by maximiliank on 05.02.23.
//

#ifndef BA_LOOPNODE_H
#define BA_LOOPNODE_H


#include "Node.h"

class LoopNode : Node {
    public:
        LoopNode(long iters);
        long iterations;
        double calcEnergy();
};


#endif //BA_LOOPNODE_H
