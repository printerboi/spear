//
// Created by maximiliank on 05.02.23.
//

#include "LoopNode.h"

double LoopNode::calcEnergy() {
    double sum = 0;

    for (auto sn : this->subnodes) {
        sum = sum + sn->calcEnergy();
    }

    for (auto &bb : this->blocks) {
        sum = sum + this->handler.getBasicBlockSum(*bb);
    }

    return (double) this->iterations * sum;
}
