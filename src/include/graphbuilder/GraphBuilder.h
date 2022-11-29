//
// Created by max on 26.11.22.
//

#ifndef GRAPHBUILDER_GRAPHBUILDER_H
#define GRAPHBUILDER_GRAPHBUILDER_H

#include <memory>
#include "llvm/IR/Module.h"
#include "string"
#include "ProgramDependenceGraph.h"

class GraphBuilder {
    public:
        std::unique_ptr<llvm::Module> module;
        explicit GraphBuilder(std::string);
        ProgramDependenceGraph construct();
};


#endif //GRAPHBUILDER_GRAPHBUILDER_H
