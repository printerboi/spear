//
// Created by max on 25.11.22.
//

#ifndef RAPL_READER_REGISTERREADER_H
#define RAPL_READER_REGISTERREADER_H

#include "string"

/**
 * Class to read out the Intel RAPL Registers
 */
class RegisterReader {
    /**
     * The address of the register containing the energycounter
     */
    int energyReg;
    /**
     * The address of the register containing the unit register
     */
    int unitReg;
    /**
     * Char-Array to safe the file containing all the processor register
     */
    char* regFile = new char[15];

    public:
        /**
         * Constructor setting the core to read the rapl registers from
         * @param core The core to read
         */
        explicit RegisterReader(int core);
        /**
         * Placeholder method to profile c code directly. DO NOT USE!!!
         */
        void benchmarkCode();
        /**
         * Method to read the energy from the respective register
         * @return The current energy-counter
         */
        double getEnergy();
        /**
         * Method to read the energy-unit from the respective register
         * @return The current multiplier used for the energy-counter
         */
        double getMultiplier();
    private:
        /**
         * Reads a register in the register-file of the class
         * @param reg Offset of the register in the file
         * @param bytesToRead Bytes to read from the register
         * @return Value in the register as 64-bit unsigned integer
         */
        long long read(int reg);
};


#endif //RAPL_READER_REGISTERREADER_H
