

#include <llvm/IR/Instructions.h>
#include "InstructionCategory.h"

EnergyFunction::EnergyFunction(llvm::Function *func) {
    this->func = func;
    this->energy = 0.00;
}


bool InstructionCategory::isMemloadInstruction(llvm::Instruction &Instruction) {
    return Instruction.mayReadFromMemory() || llvm::isa<llvm::GetElementPtrInst>(Instruction);
}

bool InstructionCategory::isCallInstruction(llvm::Instruction &Instruction) {
    return llvm::isa<llvm::CallInst>( Instruction );
}

bool InstructionCategory::isMemstoreInstruction(llvm::Instruction &Instruction) {
    return Instruction.mayWriteToMemory();
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

bool InstructionCategory::isCastInstruction(llvm::Instruction &Instruction) {
    return llvm::isa<llvm::CastInst>(Instruction);
}

InstructionCategory::Category InstructionCategory::getCategory( llvm::Instruction &Instruction ){
    if( isMemloadInstruction( Instruction ) ){
        return InstructionCategory::Category::MEMLOAD;
    }else if( isMemstoreInstruction( Instruction ) ){
        return InstructionCategory::Category::MEMSTORE;
    }else if( isProgramFlowInstruction( Instruction ) || isCallInstruction( Instruction ) ){
        return InstructionCategory::Category::PROGRAMFLOW;
    }else if( isDivisionInstruction( Instruction ) ){
        return InstructionCategory::Category::DIVISION;
    }else if( isCastInstruction( Instruction ) ){
        return InstructionCategory::Category::CAST;
    }else{
        return InstructionCategory::Category::OTHER;
    }
}

std::string InstructionCategory::toString(InstructionCategory::Category category) {
    std::string catString = "undefined";

    switch (category) {
        case Category::MEMLOAD:
            catString = "Memload";
            break;
        case Category::MEMSTORE:
            catString = "Memstore";
            break;
        case Category::PROGRAMFLOW:
            catString = "Programflow";
            break;
        case Category::DIVISION:
            catString = "Division";
            break;
        case Category::CAST:
            catString = "Cast";
            break;
        case Category::OTHER:
            catString = "Other";
            break;
    }

    return catString;
}

double InstructionCategory::getCalledFunctionEnergy(llvm::Instruction &Instruction, std::vector<EnergyFunction *> function_pool) {
    double energy = 0.00;

    if(isCallInstruction(Instruction) ){
        auto call_instruction = llvm::cast<llvm::CallInst>(&Instruction);
        if(call_instruction != nullptr){
            auto called_function = call_instruction->getCalledFunction();

            for(auto energy_function : function_pool){
                if(energy_function->func == called_function){
                    energy = energy_function->energy;
                    break;
                }
            }
        }
    }

    return energy;
}
