//
// Created by max on 26.11.22.
//

#ifndef GRAPHBUILDER_PROGRAMDEPENDENCEGRAPH_H
#define GRAPHBUILDER_PROGRAMDEPENDENCEGRAPH_H

#include "list"
#include "tuple"
#include "set"
#include "iterator"
#include "Node.h"
#include "string"

class ProgramDependenceGraph {
    public:
        std::list<Node> nodes;
        std::set<std::tuple<Node, Node>> edges;
        ProgramDependenceGraph();
        std::iterator<std::output_iterator_tag, Node> getIterator();
};


#endif //GRAPHBUILDER_PROGRAMDEPENDENCEGRAPH_H
