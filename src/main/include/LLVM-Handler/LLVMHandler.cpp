
#include "LLVMHandler.h"
#include "InstructionCategory.h"
#include <cmath>

#include <utility>

LLVMHandler::LLVMHandler( Json::Value energy, int valueIfIndeterminable) {
    this->energyValues = std::move(energy);
    this->valueIfIndeterminable = valueIfIndeterminable;
    this->inefficient = 0;
    this->efficient = 0;
}

double LLVMHandler::getBasicBlockSum(llvm::BasicBlock &basicBlock ){
    //Init the sum of this block
    double blocksum = 0;

    //Iterate over the instructions in this block
    for ( auto &instruction : basicBlock ) {
        //Categorize the current instruction
        InstructionCategory::Category category = InstructionCategory::getCategory(instruction);

        //Get the energy from the JSON energy values by referencing the category
        double instructionValue = 0.00;
        if(category == InstructionCategory::Category::CALL){
            double calledValue = InstructionCategory::getCalledFunctionEnergy(instruction, this->funcmap);
            instructionValue = this->energyValues[InstructionCategory::toString(category)].asDouble();

            instructionValue += calledValue;
        }else{
            instructionValue = this->energyValues[InstructionCategory::toString(category)].asDouble();
        }

        //Add the value to the sum
        blocksum += instructionValue;
    }

    return blocksum;
}
