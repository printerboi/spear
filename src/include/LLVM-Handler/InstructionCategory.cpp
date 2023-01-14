

#include <llvm/IR/Instructions.h>
#include "InstructionCategory.h"


bool InstructionCategory::isMemoryInstruction( llvm::Instruction &Instruction ){
    return llvm::isa<llvm::StoreInst>( Instruction ) ||
            llvm::isa<llvm::LoadInst>( Instruction ) ||
            llvm::isa<llvm::AllocaInst>( Instruction );
}

bool InstructionCategory::isProgramFlowInstruction( llvm::Instruction &Instruction ){
    return llvm::isa<llvm::BranchInst>(Instruction) ||
            llvm::isa<llvm::CallInst>(Instruction) ||
            llvm::isa<llvm::ReturnInst>(Instruction);
}

bool InstructionCategory::isDivisionInstruction( llvm::Instruction &Instruction ){
    return llvm::isa<llvm::BinaryOperator>(Instruction) && (
            std::strcmp(Instruction.getOpcodeName(), "sdiv") == 0 ||
            std::strcmp(Instruction.getOpcodeName(), "udiv") == 0
    );
}

InstructionCategory::Category InstructionCategory::getCategory( llvm::Instruction &Instruction ){
    if( isMemoryInstruction( Instruction ) ){
        return InstructionCategory::Category::MEMORY;
    }else if( isProgramFlowInstruction( Instruction ) ){
        return InstructionCategory::Category::PROGRAMFLOW;
    }else if( isDivisionInstruction( Instruction ) ){
        return InstructionCategory::Category::DIVISION;
    }else{
        return InstructionCategory::Category::OTHER;
    }
}

std::string InstructionCategory::toString(InstructionCategory::Category category) {
    std::string catString = "undefined";

    switch (category) {
        case Category::MEMORY:
            catString = "Memory";
            break;
        case Category::PROGRAMFLOW:
            catString = "Programflow";
            break;
        case Category::DIVISION:
            catString = "Division";
            break;
        case Category::OTHER:
            catString = "Other";
            break;
    }

    return catString;
}

