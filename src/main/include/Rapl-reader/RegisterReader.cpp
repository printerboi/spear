//
// Created by max on 25.11.22.
//

#include "RegisterReader.h"

#include <utility>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include "iostream"
#include "cstdio"
#include "cmath"
#include <cstring>


RegisterReader::RegisterReader(int core) {
    //Package -> 0x611
    //Cores -> 0x639
    this->energyReg = 0x639;
    this->unitReg = 0x606;
    sprintf(this->regFile, "/dev/cpu/%d/msr", core);
}

long long RegisterReader::read(int reg) {
    int regfd = 0;
    uint64_t regValBuffer;

    //Open the file containing our registers in readonly mode
    regfd = open(this->regFile, O_RDONLY);
    //read 8 bytes from the registerfile at the offset energyReg and store the contents at the address of
    //our previously defined 64-integer regValBuffer
    pread(regfd, &regValBuffer, 8, reg);

    close(regfd);

    return (long long) regValBuffer;
}

double RegisterReader::getEnergy() {
    char regValBuffer[8] = {};
    u_int64_t result = read(this->energyReg);
    //std::memcpy(&result, regValBuffer, 8);

    auto mutlitplier = this->getMultiplier();
    //result = (double) result * mutlitplier;

    return (double) result * mutlitplier;
}

double RegisterReader::getMultiplier() {
    double unit;
    long long result = read(this->unitReg);
    unit = (char) ((result >> 8) & 0x1F);
    //std::memcpy(&result, &buffer, 1);
    double multiplier = pow(0.5, (double) unit);
    return (double) multiplier;
}

void RegisterReader::benchmarkCode() {
    timespec timeFirst{};
    timespec timeSecond{};
    int iterations = 1000000000;
    uint32_t a = 1;
    uint32_t b = 3;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timeFirst);
    uint64_t energyBefore = this->getEnergy();
    energyBefore = energyBefore * this->getMultiplier();


    //============================================
    //              Code to benchmark
    //============================================
    for (long i = 0; i < iterations; ++i) {
        uint32_t c = a + b;
    }
    //============================================

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timeFirst);
    uint64_t energyAfter = this->getEnergy();
    energyAfter = energyAfter * this->getMultiplier();

    uint64_t absEnergy = energyAfter - energyBefore;
    double middled = (double) absEnergy/iterations;

    double absTime = (double) timeFirst.tv_nsec - (double) timeSecond.tv_nsec;

    printf("Energy-Information for the benchmarked Code:");
    printf("\n\n");
    printf("Time running: %f ns ", absTime);
    printf("Energy consumed: %lu µJ ", absEnergy);
    printf("Energy consumed per Iteration: %f µJ", middled);
    printf("\n");
}
