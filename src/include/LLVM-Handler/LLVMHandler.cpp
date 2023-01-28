
#include "LLVMHandler.h"

LLVMHandler::LLVMHandler( Json::Value energy, long valueIfIntederminate) {
    this->energyValues = energy;
    this->valueIfIndeterminate = valueIfIntederminate;
}

double LLVMHandler::getBasicBlockSum( llvm::BasicBlock &BB ){
    //Init the sum of this block
    double blocksum = 0;

    //Iterate over the instructions in this block
    for ( auto &I : BB ) {
        //Categorize the current instruction
        InstructionCategory::Category cat = InstructionCategory::getCategory(I);

        //Get the energy from the JSON energy values by referencing the category
        double iValue = this->energyValues[InstructionCategory::toString(cat)].asDouble();
        //Add the value to the sum
        blocksum += iValue;
    }

    return blocksum;
}

long LLVMHandler::getLoopUpperBound(llvm::Loop *L){
    //Get the Latch instruction responsible for containing the compare instruction
    auto li = L->getLatchCmpInst();
    //Init the bound with a default value if we are not comparing with a natural number
    long bound = this->valueIfIndeterminate;
    //Assume the number to compare with is the second argument of the instruction

    for(int i=0; i < li->getNumOperands(); i++){
        //Try to cast the value to a constantint pointer
        if (auto *CI = llvm::dyn_cast<llvm::ConstantInt>(li->getOperand(i))) {
            //Gets a 64-bit signed int value from the cast constant
            bound = CI->getSExtValue();
        }
    }

    return bound;
}