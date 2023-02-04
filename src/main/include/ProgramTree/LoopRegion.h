//
// Created by maximiliank on 04.02.23.
//

#ifndef BA_LOOPREGION_H
#define BA_LOOPREGION_H


#include "ProgramTree.h"

class LoopRegion : ProgramTree {
    public:
        LoopRegion(llvm::Region region);
        long iterations;
};


#endif //BA_LOOPREGION_H
