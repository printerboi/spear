//
// Created by maximiliank on 05.02.23.
//

#ifndef BA_EDGE_H
#define BA_EDGE_H

#include "Node.h"

class Edge {
    public:
        Node * start;
        Node * end;

        Edge(Node *start, Node *end);
        std::string toString();
};


#endif //BA_EDGE_H
