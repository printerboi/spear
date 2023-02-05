//
// Created by maximiliank on 05.02.23.
//

#include "IfNode.h"
#include <cfloat>

double IfNode::calcEnergy() {
    double sum = 0;
    auto min = DBL_MAX;

    for (auto sn : this->subnodes) {
        double calcedForSubtree = sn->calcEnergy();
        if (calcedForSubtree < min){
            min = calcedForSubtree;
        }
    }

    for (auto &bb : this->blocks) {
        sum = sum + this->handler.getBasicBlockSum(*bb);
    }

    return sum;
}
