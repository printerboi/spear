//
// Created by maximiliank on 05.02.23.
//

#include "ProgramTree.h"


std::string Node::toString() {
    std::string output;

    for (auto BB : this->blocks) {
        output.append(BB->getName());
    }

    return output;
}

Node::Node(ProgramTree *parent) {
    this->parent = parent;
}

double Node::getEnergy(LLVMHandler *handler) {
    double sum = 0.0;
    auto adjsNodes = this->getAdjacentNodes();

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

    for(auto &block : this->blocks){
        sum = sum + handler->getBasicBlockSum(*block);
    }

    return sum;
}

std::vector<Node *> Node::getAdjacentNodes() {
    std::vector<Node *> adjacent;
    for(auto E : this->parent->findEdgesStartingAtNode(this)){
        adjacent.push_back(E->end);
    }

    return adjacent;
}
