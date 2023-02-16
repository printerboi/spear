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

        std::vector<EnergyFunction *> funcqueue;

        /**
         * Constructor receiving a string to file. Loads the IR from the file to the class
         * @param energyModel JSON Object containing the energy model
         * @param file String containing a valid file path
         */
        explicit LLVMHandler( Json::Value energyModel, long valueIfInteterminate );

        /**
         * Method to calculate the energy-consumption of block with respect to the given energy model
         * @param BB Basic block to analyze
         * @return Double value of the approximated energy
         */
        double getBasicBlockSum(llvm::BasicBlock &BB, llvm::Function *func );

        /**
         * Calculates the upper bound of iterations to a given loop. If the upper bound is unknown or can't be
         * calculated the parameter valueIfIndeterminate will be used as upper bound
         * @param L The loop to analyze
         * @return The long value representing the iterations the given loop will approximately run
         */
         long getLoopUpperBound(llvm::Loop *L);


private:

    /**
     * JSON-Value containin the energy model used for the calculations
     */
    Json::Value energyValues;
    /**
     * Fallback value of the loop bound calculation representing an upper bound
     */
    long valueIfIndeterminate;

};


#endif //BA_LLVMHANDLER_H
