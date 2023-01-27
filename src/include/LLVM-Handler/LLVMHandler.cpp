//
// Created by max on 13.01.23.
//

#include "LLVMHandler.h"

LLVMHandler::LLVMHandler( Json::Value energy, long valueIfIntederminate) {
    //this->module = std::move(llvm::parseIRFile(file, this->error, this->context));
    //this->module = llvm::parseIRFile(file, this->error, this->context).release();
    this->energyValues = energy;
    this->valueIfIndeterminate = valueIfIntederminate;
}

double LLVMHandler::getBasicBlockSum( llvm::BasicBlock &BB ){
    double blocksum = 0;

    for ( auto &I : BB ) {
        InstructionCategory::Category cat = InstructionCategory::getCategory(I );
        double iValue = this->energyValues[InstructionCategory::toString(cat)].asDouble();
        blocksum += iValue;
    }

    return blocksum;
}

double LLVMHandler::getLoopSum(llvm::Loop *L){
    long bound = LLVMHandler::getLoopUpperBound(L, this->valueIfIndeterminate);
    auto blockList = L->getBlocksVector();
    double sum = 0.0;

    for (auto BB : blockList) {
        sum += LLVMHandler::getBasicBlockSum(*BB);
    }

    llvm::outs() << "\t Bound: " << bound << "\n";
    llvm::outs() << "\t Raw Sum: " << sum << "\n";

    return (double) bound * sum;
}

long LLVMHandler::getLoopUpperBound(llvm::Loop *L, long valueIfIndeterminate){
    //Get the Latch instruction responsible for containing the compare instruction
    auto li = L->getLatchCmpInst();
    //Init the bound with a default value if we are not comparing with a natural number
    long bound = valueIfIndeterminate;
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

std::vector<llvm::BasicBlock *> LLVMHandler::getLoopBlocks(llvm::Loop *L){
    return L->getBlocks();
}


/*
void LLVMHandler::print(  ) {
    for ( auto &F : *this->module ) {
        if( F.getName() == "main" ){
            llvm::outs() << "Function " << F.getName() << "\n";
            for ( auto &BB : F ) {
                llvm::outs() << round(getBasicBlockSum( BB )) << '\n';
                std::set<llvm::StringRef> v = LLVMHandler::paramsin(BB);
                std::set<llvm::StringRef> g = LLVMHandler::paramsout(BB);

                llvm::outs() << "{";
                for( auto s : v){
                    llvm::outs() << s << ", ";
                }
                llvm::outs() << "}|";

                llvm::outs() << "{";
                for( auto s : g){
                    llvm::outs() << s << ", ";
                }
                llvm::outs() << "}\n";
            }
        }
    }
}*/
