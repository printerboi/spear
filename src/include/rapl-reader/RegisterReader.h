//
// Created by max on 25.11.22.
//

#ifndef RAPL_READER_REGISTERREADER_H
#define RAPL_READER_REGISTERREADER_H

#include "string"

class RegisterReader {
    int energyReg;
    int unitReg;
    char* regFile = new char[15];

    public:
        explicit RegisterReader(int core);
        void benchmarkCode();
        uint64_t getEnergy();
        uint64_t getMultiplier();
    private:
        uint64_t read(int reg, int bytesToRead);
};


#endif //RAPL_READER_REGISTERREADER_H
