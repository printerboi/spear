
#include <mutex>
#include "Profiler.h"
#include <thread>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include "../JSON-Handler/JSONHandler.h"


Profiler::Profiler(int rep, std::string path){
    this->repetitions = rep;
    this->programspath = path;
}

std::vector<double> Profiler::profile() {

    double casptr = 0.0;
    double callptr = 0.0;
    double memreadptr = 0.0;
    double memwritptr = 0.0;
    double flowptr = 0.0;
    double divpointer = 0.0;
    double stdbinptr = 0.0;

    //benchmarkFile("src/compiled/cast", &casptr);
    benchmarkFile("src/compiled/call", &callptr);
    benchmarkFile("src/compiled/memoryread", &memreadptr);
    //benchmarkFile("src/compiled/memorywrite", &memwritptr);
    benchmarkFile("src/compiled/programflow", &flowptr);
    benchmarkFile("src/compiled/division", &divpointer);
    benchmarkFile("src/compiled/stdbinary", &stdbinptr);

    /*double cast = benchmarkFile("src/compiled/cast", casptr);
    double memoryread = benchmarkFile("src/compiled/memoryread", memreadptr);
    double memorywrite = benchmarkFile("src/compiled/memoryread", memreadptr);
    double programflow = benchmarkFile("src/compiled/programflow", flowptr);
    double division = benchmarkFile("src/compiled/division", divpointer);
    double stdbinary = benchmarkFile("src/compiled/stdbinary", stdbinptr);*/


    /*return {
            cast,
            memoryread,
            memorywrite,
            programflow,
            division,
            stdbinary
    };*/

    return {
        callptr,
        memreadptr,
        flowptr,
        divpointer,
        stdbinptr
    };
}

double Profiler::benchmarkFile(const std::string& file, double *energyPointer) const {
    auto powReader = new RegisterReader(0);
    std::string programPath = this->programspath;

    char *command = new char[1024];
    char *path = new char[1024];
    sprintf(command, "%s/%s", programPath.c_str(), file.c_str());
    sprintf(path, "%s/%s", programPath.c_str(), "src/compiled");

    double energyAverage = 0;
    char* const args[] = {  };

    auto *sharedEnergyBefore  = (double *) mmap(nullptr, sizeof (int) , PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    auto *sharedStart  = (struct timespec *) mmap(nullptr, sizeof (int) , PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    auto *sharedEnd  = (struct timespec *) mmap(nullptr, sizeof (int) , PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    double accumulatedEnergy = 0.0;
    cpu_set_t cpuMask;

    for (int i = 0; i < this->repetitions; i++){
        pid_t childProcessId = fork();

        if(childProcessId == 0){
            //std::cout << "\t\tChild with pid " << childProcessId << " executing the file. \n" << std::flush;
            CPU_ZERO(&cpuMask);        // clear cpu mask
            CPU_SET(0, &cpuMask);      // cpuMask cpu 3
            sched_setaffinity(childProcessId, sizeof(cpu_set_t), &cpuMask);  // 0 is the calling process

            *sharedEnergyBefore = powReader->getEnergy();

            execl(command, command);
            exit(1);

        }else{
            CPU_ZERO(&cpuMask);        // clear cpu mask
            CPU_SET(3, &cpuMask);      // cpuMask cpu 3
            sched_setaffinity(childProcessId, sizeof(cpu_set_t), &cpuMask);  // 0 is the calling process

            wait(nullptr);

            double energyAfter = powReader->getEnergy();

            //If the register overflows...
            if(*sharedEnergyBefore > energyAfter){
                pid_t ic_pid = fork();

                if(ic_pid == 0){
                    CPU_ZERO(&cpuMask);        // clear cpu mask
                    CPU_SET(0, &cpuMask);      // cpuMask cpu 3
                    sched_setaffinity(ic_pid, sizeof(cpu_set_t), &cpuMask);  // 0 is the calling process

                    *sharedEnergyBefore = powReader->getEnergy();

                    execl(command, command);
                    exit(1);

                }else {
                    CPU_ZERO(&cpuMask);        // clear cpu mask
                    CPU_SET(3, &cpuMask);      // cpuMask cpu 3
                    sched_setaffinity(ic_pid, sizeof(cpu_set_t), &cpuMask);  // 0 is the calling process

                    wait(nullptr);

                    energyAfter = powReader->getEnergy();
                }

            }else{
                accumulatedEnergy += energyAfter - *sharedEnergyBefore;
            }

        }
    }

    *energyPointer = accumulatedEnergy / (double) this->repetitions;
    return 0;
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

long Profiler::getIterations() const {
    return this->repetitions;
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

