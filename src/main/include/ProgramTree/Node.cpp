//
// Created by maximiliank on 05.02.23.
//

#include "Node.h"

std::string Node::toString() {
    std::string output;

    for (auto BB : this->blocks) {
        output.append(BB->getName());
    }

    return output;
}
