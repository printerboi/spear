


#include "EnergyFunction.h"
#include "../ProgramGraph/ProgramGraph.h"


//Constructor of the EnergyFunction class
EnergyFunction::EnergyFunction() {
    this->programGraph = new ProgramGraph();
    this->energy = 0.00;
}