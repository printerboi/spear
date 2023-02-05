//
// Created by maximiliank on 05.02.23.
//

#include "Node.h"

bool Node::IsStart() {
    return this->predecessor == nullptr;
}

bool Node::IsEnd() {
    return this->successor == nullptr;
}

Node::Node(llvm::BasicBlock *block) {
    this->blocks.push_back(block);
}

double Node::calcEnergy() {
    double sum = 0;
    for (auto &bb : this->blocks) {
        sum = sum + this->handler.getBasicBlockSum(*bb);
    }

    return sum;
}
