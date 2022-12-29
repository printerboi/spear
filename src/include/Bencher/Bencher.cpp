//
// Created by max on 26.12.22.
//

#include "Bencher.h"

Bencher::Bencher(int it, int rep){
    this->iterations = it;
    this->repetitions = rep;
}

std::vector<double> Bencher::benchmark() {
    return {
            benchmarkFile("llvm-code-under-bench/groupM.ll"),
            benchmarkFile("llvm-code-under-bench/groupB.ll"),
            benchmarkFile("llvm-code-under-bench/groupD.ll"),
            benchmarkFile("llvm-code-under-bench/groupG.ll")
    };
}

double Bencher::benchmarkFile(std::string file) {
    PowercapReader powReader = PowercapReader();
    char* relPath = getenv("BA_REL_PATH");

    char *command = new char[255];
    sprintf(command, "lli -O0 %s/%s", relPath ,file.c_str());
    double engAverage = 0;

    for (int i = 0; i < this->iterations; ++i) {
        auto preEng = (double) powReader.getEnergy();

        for (int j = 0; j < this->repetitions; ++j) {
            system(command);
        }
        auto postEng = (double) powReader.getEnergy();

        auto absEng = postEng - preEng;

        absEng = absEng / this->repetitions;

        engAverage = engAverage + absEng;
    }

    return engAverage/this->iterations;
}