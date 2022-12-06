//
// Created by max on 25.11.22.
//

#include "RegisterReader.h"

#include <utility>
#include <fcntl.h>
#include <unistd.h>
#include "iostream"
#include "cstdio"


RegisterReader::RegisterReader(int core) {
    this->energyReg = 0x611;
    sprintf(this->regFile, "/dev/cpu/%d/msr", core);
}

uint64_t RegisterReader::read() {
    int REGSIZE = 8;
    int regfd;

    //Specify the buffer to be a 64bit unsigned integer. The read bytes will therefore be converted automatically
    uint64_t  regValBuffer;

    //Open the file containing our registers in readonly mode
    regfd = open(this->regFile, O_RDONLY);
    //read 8 bytes from the registerfile at the offset energyReg and store the contents at the address of
    //our previously defined 64-integer regValBuffer
    pread(regfd, &regValBuffer, 8, this->energyReg);

    return regValBuffer;
}

void RegisterReader::benchmarkCode() {
    timespec timeFirst{};
    timespec timeSecond{};
    int iterations = 1000000000;
    uint32_t a = 1;
    uint32_t b = 3;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timeFirst);
    uint64_t energyBefore = read();


    //============================================
    //              Code to benchmark
    //============================================
    for (long i = 0; i < iterations; ++i) {
        uint32_t c = a / b;
    }
    //============================================

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timeFirst);
    uint64_t energyAfter = read();
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

int main(){
    RegisterReader Rr = RegisterReader(0);
    //uint64_t content = Rr.read();
    Rr.benchmarkCode();

    return 0;
}
