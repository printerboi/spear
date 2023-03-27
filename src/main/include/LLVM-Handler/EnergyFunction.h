//
// Created by maximiliank on 24.03.23.
//

#ifndef SPEAR_ENERGYFUNCTION_H
#define SPEAR_ENERGYFUNCTION_H


#include <llvm/IR/Function.h>

class EnergyFunction {
public:
    llvm::Function * func;
    double energy;
    explicit EnergyFunction(llvm::Function *function);
};


#endif //SPEAR_ENERGYFUNCTION_H
