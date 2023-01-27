//
// Created by maximiliank on 26.01.23.
//

#ifndef BA_ENERGY_H
#define BA_ENERGY_H

#include "llvm/IR/PassManager.h"


namespace llvm {
    class EnergyPass : public PassInfoMixin<EnergyPass> {
        public:
            PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
    };
}



#endif //BA_ENERGY_H
