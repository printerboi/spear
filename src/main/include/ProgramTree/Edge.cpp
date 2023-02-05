//
// Created by maximiliank on 05.02.23.
//

#include "ProgramTree.h"

Edge::Edge(Node *start, Node *end) {
    this->start = start;
    this->end = end;
}

std::string Edge::toString() {
    std::string output = "(";

    output.append(this->start->toString());

    output.append(" | ");

    output.append(this->end->toString());

    output.append(")");

    return output;
}
