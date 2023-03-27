


#include "EnergyFunction.h"

EnergyFunction::EnergyFunction(llvm::Function *function) {
    this->func = function;
    this->energy = 0.00;
}