

#include <llvm/IR/Instructions.h>
#include "InstructionCategory.h"


bool InstructionCategory::isCallInstruction(llvm::Instruction &Instruction) {
    //check if the given instruciton is either a call, callbr or an invoke instruction
    return llvm::isa<llvm::CallInst>( Instruction ) ||
        llvm::isa<llvm::CallBrInst>( Instruction ) ||
        llvm::isa<llvm::InvokeInst>( Instruction );
}

bool InstructionCategory::isMemoryInstruction(llvm::Instruction &Instruction) {
    //check if the given instruction interacts with the memory
    return llvm::isa<llvm::LoadInst>( Instruction ) ||
            llvm::isa<llvm::StoreInst>( Instruction ) ||
            llvm::isa<llvm::AllocaInst>( Instruction ) ||
            llvm::isa<llvm::AtomicCmpXchgInst>( Instruction ) ||
            llvm::isa<llvm::AtomicRMWInst>( Instruction ) ||
            llvm::isa<llvm::GetElementPtrInst>( Instruction );
}

bool InstructionCategory::isProgramFlowInstruction( llvm::Instruction &Instruction ){
    //Check if the given instruction interacts with the flow of the program
    return llvm::isa<llvm::BranchInst>(Instruction) ||
            llvm::isa<llvm::IndirectBrInst>(Instruction) ||
            llvm::isa<llvm::SwitchInst>(Instruction) ||
            llvm::isa<llvm::CatchSwitchInst>(Instruction) ||
            llvm::isa<llvm::CatchReturnInst>(Instruction) ||
            llvm::isa<llvm::ReturnInst>(Instruction);
}

bool InstructionCategory::isDivisionInstruction( llvm::Instruction &Instruction ){
    //check if the given instruction is a binaryinstruction and check for the opcode of all division instructions in llvm ir
    return llvm::isa<llvm::BinaryOperator>(Instruction) && (
            std::strcmp(Instruction.getOpcodeName(), "sdiv") == 0 ||
            std::strcmp(Instruction.getOpcodeName(), "udiv") == 0 ||
            std::strcmp(Instruction.getOpcodeName(), "fidv") == 0 ||
            std::strcmp(Instruction.getOpcodeName(), "urem") == 0 ||
            std::strcmp(Instruction.getOpcodeName(), "srem") == 0 ||
            std::strcmp(Instruction.getOpcodeName(), "frem") == 0
    );
}

InstructionCategory::Category InstructionCategory::getCategory( llvm::Instruction &Instruction ){
    //check the given instruction with the instruction-methods and the return the corresponding InstructionsCategory

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
    //Start with an undefined value for the name of the given category
    std::string categoryString = "undefined";

    //Check the elements of the category and return a string representing the category
    switch (category) {
        case Category::MEMORY:
            categoryString = "Memory";
            break;
        case Category::PROGRAMFLOW:
            categoryString = "Programflow";
            break;
        case Category::DIVISION:
            categoryString = "Division";
            break;
        case Category::CALL:
            categoryString = "Call";
            break;
        case Category::OTHER:
            categoryString = "Other";
            break;
    }

    return categoryString;
}


double InstructionCategory::getCalledFunctionEnergy(llvm::Instruction &Instruction, std::map<std::string, EnergyFunction*>& poolOfFunctions) {
    //Init the energy as 0.00 J
    double energy = 0.00;

    //Check if the given instruction is a call instruction
    if(llvm::isa<llvm::CallInst>( Instruction ) ){
        //Get the call-instruction object of the given instruction
        auto call_instruction = llvm::cast<llvm::CallInst>(&Instruction);
        //If the cast of the given instruction worked as intended
        if(call_instruction != nullptr){
            //Get the called instruction from the object
            auto called_function = call_instruction->getCalledFunction();

            //Get the Energyfunction object from the functionmap
            auto energyFunction = poolOfFunctions[called_function->getName().str()];
            energy = energyFunction->energy;
        }
    }

    return energy;
}
