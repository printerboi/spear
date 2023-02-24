//
// Created by max on 26.12.22.
//

#include <mutex>
#include "Profiler.h"
#include <thread>
#include <unistd.h>


Profiler::Profiler(int it, int rep, std::string path){
    this->iterations = it;
    this->repetitions = rep;
    this->programspath = path;
}

std::vector<double> Profiler::profile() {
    double cast = benchmarkFile("src/compiled/cast");
    double memoryread = benchmarkFile("src/compiled/memoryread");
    double memorywrite = benchmarkFile("src/compiled/memorywrite");
    double programflow = benchmarkFile("src/compiled/programflow");
    double division = benchmarkFile("src/compiled/division");
    double stdbinary = benchmarkFile("src/compiled/stdbinary");


    return {
            cast,
            memoryread,
            memorywrite,
            programflow,
            division,
            stdbinary
    };
}

double Profiler::benchmarkFile(std::string file) {
    auto powReader = RegisterReader(0);
    std::string relPath = this->programspath;
    cpu_set_t set;

    CPU_ZERO(&set);        // clear cpu mask
    CPU_SET(3, &set);      // set cpu 3
    sched_setaffinity(0, sizeof(cpu_set_t), &set);  // 0 is the calling process

    char *command = new char[255];
    sprintf(command, "%s/%s", relPath.c_str(), file.c_str());
    double engAverage = 0;

    for (int i = 0; i < this->iterations; ++i) {
        auto preEng = (double) powReader.getEnergy();
        auto start = std::chrono::system_clock::now();

        for (int j = 0; j < this->repetitions; ++j) {

            CPU_ZERO(&set);        // clear cpu mask
            CPU_SET(0, &set);      // set cpu 0
            sched_setaffinity(0, sizeof(cpu_set_t), &set);  // 0 is the calling process

            //system(command);
            execl(command, nullptr, nullptr);

            CPU_ZERO(&set);        // clear cpu mask
            CPU_SET(3, &set);      // set cpu 3
            sched_setaffinity(0, sizeof(cpu_set_t), &set);  // 0 is the calling process
        }
        auto postEng = (double) powReader.getEnergy();
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> timerun = end - start;

        auto beforeSleep = (double) powReader.getEnergy();
        std::this_thread::sleep_for(timerun);
        auto afterSleep = (double) powReader.getEnergy();



        auto noise = afterSleep - beforeSleep;
        auto absEng = (postEng - preEng) - noise;

        absEng = absEng / this->repetitions;

        engAverage = engAverage + absEng;
    }

    return engAverage/this->iterations;
}