

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
    return llvm::isa<llvm::CallInst>( Instruction ) ||
        llvm::isa<llvm::CallBrInst>( Instruction ) ||
        llvm::isa<llvm::InvokeInst>( Instruction );
}

bool InstructionCategory::isMemoryInstruction(llvm::Instruction &Instruction) {
    return llvm::isa<llvm::LoadInst>( Instruction ) ||
            llvm::isa<llvm::StoreInst>( Instruction ) ||
            llvm::isa<llvm::AllocaInst>( Instruction ) ||
            llvm::isa<llvm::AtomicCmpXchgInst>( Instruction ) ||
            llvm::isa<llvm::AtomicRMWInst>( Instruction ) ||
            llvm::isa<llvm::GetElementPtrInst>( Instruction );
}

bool InstructionCategory::isMemstoreInstruction(llvm::Instruction &Instruction) {
    return Instruction.mayWriteToMemory();
}

bool InstructionCategory::isProgramFlowInstruction( llvm::Instruction &Instruction ){
    return llvm::isa<llvm::BranchInst>(Instruction) ||
            llvm::isa<llvm::IndirectBrInst>(Instruction) ||
            llvm::isa<llvm::SwitchInst>(Instruction) ||
            llvm::isa<llvm::CatchSwitchInst>(Instruction) ||
            llvm::isa<llvm::CatchReturnInst>(Instruction) ||
            llvm::isa<llvm::ReturnInst>(Instruction);
}

bool InstructionCategory::isDivisionInstruction( llvm::Instruction &Instruction ){
    return llvm::isa<llvm::BinaryOperator>(Instruction) && (
            std::strcmp(Instruction.getOpcodeName(), "sdiv") == 0 ||
            std::strcmp(Instruction.getOpcodeName(), "udiv") == 0 ||
            std::strcmp(Instruction.getOpcodeName(), "fidv") == 0 ||
            std::strcmp(Instruction.getOpcodeName(), "urem") == 0 ||
            std::strcmp(Instruction.getOpcodeName(), "srem") == 0 ||
            std::strcmp(Instruction.getOpcodeName(), "frem") == 0
    );
}

bool InstructionCategory::isCastInstruction(llvm::Instruction &Instruction) {
    return llvm::isa<llvm::CastInst>(Instruction);
}

InstructionCategory::Category InstructionCategory::getCategory( llvm::Instruction &Instruction ){
    if( isMemoryInstruction( Instruction ) ){
        return InstructionCategory::Category::MEMORY;
    }else if( isProgramFlowInstruction( Instruction ) ){
        return InstructionCategory::Category::PROGRAMFLOW;
    }else if( isDivisionInstruction( Instruction ) ){
        return InstructionCategory::Category::DIVISION;
    }else if( isCallInstruction( Instruction ) ){
        return InstructionCategory::Category::CALL;
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
        case Category::CALL:
            catString = "Call";
            break;
        case Category::OTHER:
            catString = "Other";
            break;
    }

    return catString;
}

double InstructionCategory::getCalledFunctionEnergy(llvm::Instruction &Instruction, std::vector<EnergyFunction *> function_pool) {
    double energy = 0.00;

    if(llvm::isa<llvm::CallInst>( Instruction ) ){
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
