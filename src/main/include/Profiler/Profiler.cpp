
#include <mutex>
#include "Profiler.h"
#include <thread>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <cassert>
#include "../JSON-Handler/JSONHandler.h"


Profiler::Profiler(int repetitions, std::map<std::string, std::string> *profileCode){
    this->repetitions = repetitions;
    this->profileCode = profileCode;
}

std::map<std::string, double> Profiler::profile() {

    auto codemap = *this->profileCode;
    std::map<std::string, double> results;

    results["call"] = measureFile(codemap.at("call"));
    results["memory"] = measureFile(codemap.at("memory"));
    results["programflow"] = measureFile(codemap.at("programflow"));
    results["division"] = measureFile(codemap.at("division"));
    results["others"] = measureFile(codemap.at("others"));

    return results;
}

double Profiler::measureFile(const std::string& file) const {
    double energy = 0.0;
    auto powReader = new RegisterReader(0);
    auto *sharedEnergyBefore  = (double *) mmap(nullptr, sizeof (int) , PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    double accumulatedEnergy = 0.0;
    cpu_set_t cpuMask;

    for (int i = 0; i < this->repetitions; i++){

        pid_t childProcessId = fork();

        if(childProcessId == 0){

            *sharedEnergyBefore = powReader->getEnergy();

            if(execv(file.c_str(), new char*) == -1){
                throw std::invalid_argument("Profilecode not found!!!");
                assert(false);
                exit(1);
                break;
            }

        }else{

            //waitpid(childProcessId, nullptr, 0);
            wait(nullptr);

            double energyAfter = powReader->getEnergy();

            //If the register overflows...
            if(*sharedEnergyBefore > energyAfter){
                pid_t ic_pid = fork();

                if(ic_pid == 0){

                    *sharedEnergyBefore = powReader->getEnergy();

                    if(execv(file.c_str(), new char*) == -1){
                        throw std::invalid_argument("Profilecode not found!!!");
                        assert(false);
                        exit(1);
                        break;
                    }

                }else {

                    wait(nullptr);

                    energyAfter = powReader->getEnergy();
                }

                accumulatedEnergy += energyAfter - *sharedEnergyBefore;
            }else{
                accumulatedEnergy += energyAfter - *sharedEnergyBefore;
            }

        }
    }

    if(this->repetitions > 0){
        energy = accumulatedEnergy / (double) this->repetitions;
    }else{
        energy = accumulatedEnergy;
    }

    return energy;
}

std::string Profiler::getCPUName() {
    char buffer[128];
    std::string result;
    std::string command = "cat /proc/cpuinfo | grep 'model name' | uniq";

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "failed to execute the command";
    }

    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr)
            result += buffer;
    }

    pclose(pipe);

    std::stringstream resstream;
    std::string segment;
    resstream << result;

    std::vector<std::string> seglist;
    while(std::getline(resstream, segment, ':'))
    {
        seglist.push_back(segment);
    }

    if(segment.length() >= 2 ){
        auto lastchar = segment[segment.length()-1];
        auto firstchar = segment[0];

        if(lastchar == '\n'){
            segment.erase(segment.length()-1);
        }

        if(firstchar == ' '){
            segment.erase(0, 1);
        }

    }

    return  segment;
}

std::string Profiler::getArchitecture() {
    char buffer[128];
    std::string result;
    std::string command = "cat /proc/cpuinfo | grep 'cpu family' | uniq";

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "failed to execute the command";
    }

    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr)
            result += buffer;
    }

    pclose(pipe);

    std::stringstream resstream;
    std::string segment;
    resstream << result;

    std::vector<std::string> seglist;
    while(std::getline(resstream, segment, ':'))
    {
        seglist.push_back(segment);
    }

    if(segment.length() >=2 ){
        auto lastchar = segment[segment.length()-1];
        auto firstchar = segment[0];

        if(lastchar == '\n'){
            segment.erase(segment.length()-1);
        }

        if(firstchar == ' '){
            segment.erase(0, 1);
        }

    }


    return segment;
}

std::string Profiler::getNumberOfCores() {
    char buffer[128];
    std::string result;
    std::string command = " cat /proc/cpuinfo | grep 'siblings' | uniq";

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "failed to execute the command";
    }

    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr)
            result += buffer;
    }

    pclose(pipe);

    std::stringstream resstream;
    std::string segment;
    resstream << result;

    std::vector<std::string> seglist;
    while(std::getline(resstream, segment, ':'))
    {
        seglist.push_back(segment);
    }

    if(segment.length() >=2 ){
        auto lastchar = segment[segment.length()-1];
        auto firstchar = segment[0];

        if(lastchar == '\n'){
            segment.erase(segment.length()-1);
        }

        if(firstchar == ' '){
            segment.erase(0, 1);
        }

    }

    return segment;
}

std::string Profiler::getUnit() {
    auto powReader = new RegisterReader(0);
    auto unit = powReader->readMultiplier();

    return std::to_string(unit);
}

