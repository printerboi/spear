//
// Created by max on 26.12.22.
//

#include "Bencher.h"
#include "iostream"

Bencher::Bencher(int it, int rep){
    this->iterations = it;
    this->repetitions = rep;
}

void Bencher::benchmark() {
    uint64_t engUsed = 0;

    std::cout << "Benchmarking group M...\n" << std::endl;
    engUsed = benchmarkFile("/home/max/Documents/workbench/bachelorarbeit/llvm-code-under-bench/groupM.ll");
    printf("Group M: %lu\n", engUsed);

    std::cout << "Benchmarking group B...\n" << std::endl;
    engUsed = benchmarkFile("/home/max/Documents/workbench/bachelorarbeit/llvm-code-under-bench/groupB.ll");
    printf("Group B: %lu\n", engUsed);

    std::cout << "Benchmarking group D...\n" << std::endl;
    engUsed = benchmarkFile("/home/max/Documents/workbench/bachelorarbeit/llvm-code-under-bench/groupD.ll");
    printf("Group D: %lu\n", engUsed);

    std::cout << "Benchmarking group G...\n" << std::endl;
    engUsed = benchmarkFile("/home/max/Documents/workbench/bachelorarbeit/llvm-code-under-bench/groupG.ll");
    printf("Group G: %lu\n", engUsed);
}

uint64_t Bencher::benchmarkFile(std::string file) {
    RegisterReader regReader = RegisterReader(0);

    char *command = new char[255];
    sprintf(command, "lli %s", file.c_str());
    uint64_t engAverage = 0;

    for (int i = 0; i < this->iterations; ++i) {
        uint64_t preEng = regReader.getEnergy() * regReader.getMultiplier();
        for (int j = 0; j < this->repetitions; ++j) {
            system(command);
        }
        uint64_t postEng = regReader.getEnergy() * regReader.getMultiplier();
        uint64_t absEng = postEng - preEng;
        absEng = absEng / this->repetitions;

        engAverage = engAverage + absEng;
    }

    return engAverage/this->iterations;
}