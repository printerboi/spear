
#include "LLVMHandler.h"
#include "InstructionCategory.h"
#include <math.h>

LLVMHandler::LLVMHandler( Json::Value energy, int valueIfIntederminate) {
    this->energyValues = energy;
    this->valueIfIndeterminate = valueIfIntederminate;
}

double LLVMHandler::getBasicBlockSum( llvm::BasicBlock &BB, llvm::Function *parent ){
    //Init the sum of this block
    double blocksum = 0;

    //Iterate over the instructions in this block
    for ( auto &I : BB ) {
        //Categorize the current instruction
        InstructionCategory::Category cat = InstructionCategory::getCategory(I);

        //Get the energy from the JSON energy values by referencing the category
        double iValue = 0.00;
        if(InstructionCategory::isCallInstruction(I)){
            double calledValue = InstructionCategory::getCalledFunctionEnergy(I, this->funcqueue);
            iValue = this->energyValues[InstructionCategory::toString(cat)].asDouble();

            iValue += calledValue;
        }else{
            iValue = this->energyValues[InstructionCategory::toString(cat)].asDouble();
        }

        //Add the value to the sum
        blocksum += iValue;
    }

    return blocksum;
}

long LLVMHandler::getLoopUpperBound(llvm::Loop *L, llvm::ScalarEvolution *se){
    //Get the Latch instruction responsible for containing the compare instruction
    auto li = L->getLatchCmpInst();
    //Init the bound with a default value if we are not comparing with a natural number
    long bound = this->valueIfIndeterminate;
    auto balt = L->getBounds(*se);
    //Assume the number to compare with is the second argument of the instruction

    if(balt.hasValue()){
        auto &end = balt->getFinalIVValue();
        auto &start = balt->getInitialIVValue();
        auto step = balt->getStepValue();
        auto direction = balt->getDirection();

        long constIntEnd;
        long constIntStart;
        long constIntStep;

        auto* CIE = llvm::dyn_cast<llvm::ConstantInt>(&end);
        auto* CIS = llvm::dyn_cast<llvm::ConstantInt>(&start);
        auto* CII = llvm::dyn_cast<llvm::ConstantInt>(step);

        if ( CIE && CIS && CII ) {
            if (CIE->getBitWidth() <= 32 && CIS->getBitWidth() <= 32 && CII->getBitWidth() <= 32) {
                constIntEnd = CIE->getSExtValue();
                constIntStart = CIS->getSExtValue();
                constIntStep = CII->getSExtValue();

                if(direction == llvm::Loop::LoopBounds::Direction::Decreasing){
                    double num_rep = ceil((double)constIntStart/(double) abs(constIntStep) - (double)constIntEnd);
                    bound = (long) num_rep;
                }else if(direction == llvm::Loop::LoopBounds::Direction::Increasing){
                    double num_rep = ceil((double)constIntEnd/(double) abs(constIntStep) - (double)constIntStart);
                    bound = (long) num_rep;
                }
            }
        }
    }

    return bound;
}
