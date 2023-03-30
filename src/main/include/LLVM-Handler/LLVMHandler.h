#ifndef BA_LLVMHANDLER_H
#define BA_LLVMHANDLER_H

#include <filesystem>
#include "iostream"
#include "llvm/IR/Module.h"
#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Analysis/LoopInfo.h>
#include "json/json.h"
#include "InstructionCategory.h"
#include "llvm/Analysis/CostModel.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Constants.h"

/**
 * LLVMHandler -  Class for combining LLVM IR with energy values to reason about the energy consumption of a program
 */
class LLVMHandler {
    public:

        std::map<std::string, EnergyFunction*> funcmap;

        /**
         * Constructor receiving a string to file. Loads the IR from the file to the class
         * @param energyModel JSON Object containing the energy model
         * @param file String containing a valid file path
         */
        explicit LLVMHandler( Json::Value energyModel, int valueIfIndeterminable );

        /**
         * Method to calculate the energy-consumption of block with respect to the given energy model
         * @param basicBlock Basic block to analyze
         * @return Double value of the approximated energy
         */
        double getBasicBlockSum(llvm::BasicBlock &basicBlock);

         int efficient;
         int inefficient;

/**
 * Fallback value of the loop bound calculation representing an upper bound
 */
int valueIfIndeterminable;
private:

    /**
     * JSON-Value containin the energy model used for the calculations
     */
    Json::Value energyValues;

};


#endif //BA_LLVMHANDLER_H
