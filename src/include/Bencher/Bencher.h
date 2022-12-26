//
// Created by max on 26.12.22.
//

#ifndef BA_BENCHER_H
#define BA_BENCHER_H

#include "string"

class Bencher {
    public:
        int iterations;
        int repetitions;
        explicit Bencher(int it, int rep);
        void benchmark();
    private:
        uint64_t benchmarkFile(std::string file);
};


#endif //BA_BENCHER_H
