
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
            double calledValue = InstructionCategory::getCalledFunctionEnergy(instruction, this->funcqueue);
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

long LLVMHandler::getLoopUpperBound(llvm::Loop *loop, llvm::ScalarEvolution *scalarEvolution) const{
    //Get the Latch instruction responsible for containing the compare instruction
    auto li = loop->getLatchCmpInst();
    //Init the boundValue with a default value if we are not comparing with a natural number
    long boundValue = this->valueIfIndeterminable;
    auto loopBound = loop->getBounds(*scalarEvolution);
    //Assume the number to compare with is the second argument of the instruction

    if(loopBound.hasValue()){
        auto &endValueObj = loopBound->getFinalIVValue();
        auto &startValueObj = loopBound->getInitialIVValue();
        auto stepValueObj = loopBound->getStepValue();
        auto direction = loopBound->getDirection();

        long endValue;
        long startValue;
        long stepValue;

        auto* constantIntEnd = llvm::dyn_cast<llvm::ConstantInt>(&endValueObj);
        auto* constantIntStart = llvm::dyn_cast<llvm::ConstantInt>(&startValueObj);
        auto* constantIntStep = llvm::dyn_cast<llvm::ConstantInt>(stepValueObj);

        if (constantIntEnd && constantIntStart && constantIntStep ) {
            if (constantIntEnd->getBitWidth() <= 32 && constantIntStart->getBitWidth() <= 32 && constantIntStep->getBitWidth() <= 32) {
                endValue = constantIntEnd->getSExtValue();
                startValue = constantIntStart->getSExtValue();
                stepValue = constantIntStep->getSExtValue();

                if(direction == llvm::Loop::LoopBounds::Direction::Decreasing){
                    double numberOfRepetitions = ceil((double)startValue / (double) std::abs(stepValue) - (double)endValue);
                    boundValue = (long) numberOfRepetitions;
                }else if(direction == llvm::Loop::LoopBounds::Direction::Increasing){
                    double numberOfRepetitions = ceil((double)endValue / (double) std::abs(stepValue) - (double)startValue);
                    boundValue = (long) numberOfRepetitions;
                }
            }
        }
    }

    return boundValue;
}
