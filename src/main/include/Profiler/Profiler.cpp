//
// Created by max on 26.12.22.
//

#include <mutex>
#include "Profiler.h"

Profiler::Profiler(int it, int rep){
    this->iterations = it;
    this->repetitions = rep;
}

std::vector<double> Profiler::profile() {
    double baseValue = benchmarkFile("profile/src/compiled/base");
    double groupM = benchmarkFile("profile/src/compiled/groupM");
    double groupB = benchmarkFile("profile/src/compiled/groupB");
    double groupD = benchmarkFile("profile/src/compiled/groupD");
    double groupG = benchmarkFile("profile/src/compiled/groupG");

    return {
            groupM,
            groupB,
            groupD,
            groupG,
            baseValue
    };
}

double Profiler::benchmarkFile(std::string file) {
    auto powReader = RegisterReader(0);
    char* relPath = getenv("BA_REL_PATH");

    char *command = new char[255];
    sprintf(command, "%s/%s", relPath, file.c_str());
    double engAverage = 0;

    for (int i = 0; i < this->iterations; ++i) {
        auto preEng = (double) powReader.getEnergy();

        for (int j = 0; j < this->repetitions; ++j) {
            cpu_set_t set;
            CPU_ZERO(&set);        // clear cpu mask
            CPU_SET(0, &set);      // set cpu 0
            sched_setaffinity(0, sizeof(cpu_set_t), &set);  // 0 is the calling process

            system(command);
        }
        auto postEng = (double) powReader.getEnergy();
        auto absEng = postEng - preEng;

        absEng = absEng / this->repetitions;

        engAverage = engAverage + absEng;
    }

    return engAverage/this->iterations;
}