//
// Created by max on 26.12.22.
//

#include "Profiler.h"

Profiler::Profiler(int it, int rep){
    this->iterations = it;
    this->repetitions = rep;
}

std::vector<double> Profiler::benchmark() {
    double baseValue = benchmarkFile("profile/compiled/base");
    double groupM = benchmarkFile("profile/compiled/groupM");
    double groupB = benchmarkFile("profile/compiled/groupB");
    double groupD = benchmarkFile("profile/compiled/groupD");
    double groupG = benchmarkFile("profile/compiled/groupG");

    return {
            groupM,
            groupB,
            groupD,
            groupG,
            baseValue
    };
}

double Profiler::benchmarkFile(std::string file) {
    PowercapReader powReader = PowercapReader();
    char* relPath = getenv("BA_REL_PATH");

    char *command = new char[255];
    sprintf(command, "%s/%s", relPath, file.c_str());
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