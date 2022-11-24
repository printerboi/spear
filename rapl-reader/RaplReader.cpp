//
// Created by max on 23.11.22.
//

#include "RaplReader.h"
#include <fstream>
#include <utility>
#include "iostream"
#include "cmath"
#include "ctime"
using namespace std;

RaplReader::RaplReader(string path) {
    this->raplpath = std::move(path);
}

long RaplReader::readEnergy() const {
    ifstream energyFile;
    string energyStr;
    string buffer;

    string path = this->raplpath;

    try{
        energyFile.open(path.append("/energy_uj"));
    }catch(ios_base::failure& e){
        cerr << e.what() << "\n";
    }

    if(energyFile.is_open()){
        while(!energyFile.eof()){
            getline(energyFile, buffer);
            energyStr.append(buffer);
        }
    }
    energyFile.close();

    long energy = stol(energyStr);

    return energy;
}


void RaplReader::benchmarkCode(void (*codeToRun)()) const {
    timespec timeFirst{};
    timespec timeSecond{};

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timeFirst);
    long energyBefore = readEnergy();

    //============================================
    //              Code to benchmark
    //============================================
    codeToRun();
    //============================================

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timeFirst);
    long energyAfter = readEnergy();

    double absEnergy = (double) energyAfter - (double) energyBefore;
    double absTime = (double) timeFirst.tv_nsec - (double) timeSecond.tv_nsec;
    double watts = (absEnergy * pow(10, -6))/absTime * pow(10, -9);

    printf("Energy-Information for the benchmarked Code:");
    printf("\n\n");
    printf("Time running: %f ns ", absTime);
    printf("Energy consumed: %f µJ ", absEnergy);
    printf("Watts used: %.10e W\n", watts);
}

static void forLoop() {
    for (int i = 0; i < 1000000; ++i) {}
}

static void add() {
    int x = 42;
    int y = 43;
    int z = x + y;
}

int main(){
    RaplReader RP = RaplReader("/sys/class/powercap/intel-rapl:0");

    RP.benchmarkCode(&add);

    return 0;
}
