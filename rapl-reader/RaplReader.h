//
// Created by max on 23.11.22.
//

#ifndef RAPL_READER_RAPLREADER_H
#define RAPL_READER_RAPLREADER_H

#include "string"

class RaplReader {
    public:
        std::string raplpath;
        RaplReader();
        long readEnergy();
        void benchmarkCode();
};


#endif //RAPL_READER_RAPLREADER_H