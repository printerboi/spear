//
// Created by max on 23.11.22.
//

#include "RaplReader.h"
#include <fstream>
#include "iostream"
#include "cmath"
#include "ctime"
using namespace std;

RaplReader::RaplReader(std::string path) {
    this->raplpath = path;
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


void RaplReader::benchmarkCode() {
    timespec timeFirst{};
    timespec timeSecond{};

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timeFirst);
    long energyBefore = readEnergy();

    //============================================
    //              Code to benchmark
    //============================================
    for (long i = 0; i < 100000000000; ++i) {}
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

/*int main(){
    RaplReader RP = RaplReader();
    RP.benchmarkCode();

    return 0;
}*/
