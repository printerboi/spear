//
// Created by max on 25.11.22.
//

#include "RegisterReader.h"

#include <utility>
#include <fcntl.h>
#include <unistd.h>
#include "iostream"
#include "cstdio"
#include "cmath"


RegisterReader::RegisterReader(int core) {
    this->energyReg = 0x611;
    this->unitReg = 0x606;
    sprintf(this->regFile, "/dev/cpu/%d/msr", core);
}

uint64_t RegisterReader::read(int reg, int bytesToRead) {
    int regfd = 0;

    //Specify the buffer to be a 64bit unsigned integer. The read bytes will therefore be converted automatically
    uint64_t  regValBuffer = 0;

    //Open the file containing our registers in readonly mode
    regfd = open(this->regFile, O_RDONLY);
    //read 8 bytes from the registerfile at the offset energyReg and store the contents at the address of
    //our previously defined 64-integer regValBuffer
    pread(regfd, &regValBuffer, bytesToRead, reg);

    return regValBuffer;
}

uint64_t RegisterReader::getEnergy() {
    return read(this->energyReg, 8);
}

uint64_t RegisterReader::getMultiplier() {
    uint64_t val = read(this->unitReg+1, 4);
    return (uint64_t) ( 1/pow(2, (double) val) );
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
