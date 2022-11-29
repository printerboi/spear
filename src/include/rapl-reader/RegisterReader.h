//
// Created by max on 25.11.22.
//

#ifndef RAPL_READER_REGISTERREADER_H
#define RAPL_READER_REGISTERREADER_H

#include "string"

class RegisterReader {
    int energyReg;
    char* regFile = new char[15];

    public:
        explicit RegisterReader(int core);
        uint64_t read();
        void benchmarkCode();
};


#endif //RAPL_READER_REGISTERREADER_H
