//
// Created by maximiliank on 24.03.23.
//

#ifndef SPEAR_ENERGYFUNCTION_H
#define SPEAR_ENERGYFUNCTION_H


#include <llvm/IR/Function.h>
class ProgramGraph;

class EnergyFunction {
public:
    //Reference to the corresponding function
    llvm::Function * func;
    //The energy used by the function
    double energy;

    ProgramGraph* programGraph;

    //Simple constructor
    explicit EnergyFunction();
};


#endif //SPEAR_ENERGYFUNCTION_H
