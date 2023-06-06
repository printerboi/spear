


#include "EnergyFunction.h"

//Constructor of the EnergyFunction class
EnergyFunction::EnergyFunction(llvm::Function *function) {
    this->func = function;
    this->energy = 0.00;
}