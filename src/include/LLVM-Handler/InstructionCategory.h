//
// Created by max on 14.01.23.
//

#ifndef BA_INSTRUCTIONCATEGORY_H
#define BA_INSTRUCTIONCATEGORY_H


#include <llvm/IR/Instruction.h>

/**
 * [InstructionCategory] - Class handling/containing a enum to categorize instructions
 */
class InstructionCategory {
    public:
        /**
         * [Category] - Enum containing the possible categories
         */
        enum Category { MEMORY, PROGRAMFLOW, DIVISION, OTHER };
        /**
         * [getCategory] - Calculates the category enum type of the provided Instruction
         * @param Instruction LLVM Instruction to categorize
         * @return Returns the Enum Category the instruction belongs to
         */
        static Category getCategory( llvm::Instruction &Instruction );
        /**
         * [toString] - Returns the string the representation of the enum category enum
         * @param category The enum category which should be converted to a string
         * @return Returns a string representing the category
         */
        static std::string toString( Category category );

        static double getCategoryValue( Category category );
    private:
        /**
         *
         * @param I
         * @return
         */
        static bool isMemoryInstruction( llvm::Instruction &Instruction );
        static bool isProgramFlowInstruction( llvm::Instruction &Instruction );
        static bool isDivisionInstruction( llvm::Instruction &Instruction );
};

#endif //BA_INSTRUCTIONCATEGORY_H
