//
// Created by max on 13.01.23.
//

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
#include "../../../dist/json/json.h"
#include "InstructionCategory.h"
#include "llvm/Analysis/CostModel.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Constants.h"


/**
 * [LLVMHandler] Class to handle .ll files
 */
class LLVMHandler {
    public:
        /**
         * [LLVMHandler] Constructor receiving a string to file. Loads the IR from the file to the class
         * @param file String containing a valid file path
         */
        explicit LLVMHandler( Json::Value energy, long valueIfInteterminate );

        double getBasicBlockSum(llvm::BasicBlock &BB );

        double getLoopSum(llvm::Loop *L);

        /**
         * [print] Method returning a simple dump of the loaded file
         */
        void print();
    private:
        /**
         * Module of the loaded file
         */
        //llvm::Module *module;
        /**
         * LLVM Context of the loaded Module
         */
        //llvm::LLVMContext context;
        /**
         * LLVM Error Object of the loaded Module
         */
        //llvm::SMDiagnostic error;

        Json::Value energyValues;

        long valueIfIndeterminate;

        static long getLoopUpperBound(llvm::Loop *L, long valueIfIndeterminate);

        std::vector<llvm::BasicBlock *> getLoopBlocks(llvm::Loop *L);
};


#endif //BA_LLVMHANDLER_H
