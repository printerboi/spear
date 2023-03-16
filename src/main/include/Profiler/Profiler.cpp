//
// Created by max on 26.12.22.
//

#include <mutex>
#include "Profiler.h"
#include <thread>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include "iostream"
#include "../JSON-Handler/JSONHandler.h"


#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMSZ     27


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

void *executeFile(void *ptr){
    pthread_t thread = pthread_self();
    char *file;
    file = (char *) ptr;

    cpu_set_t set;

    CPU_ZERO(&set);        // clear cpu mask
    CPU_SET(0, &set);      // set cpu 3
    printf("Executing %s\n", file);
    pthread_setaffinity_np(thread, sizeof(cpu_set_t), &set);  // 0 is the calling process

    //execl(file, file);
    system(file);
    return nullptr;
}

double Profiler::benchmarkFile(std::string file, double *ptr) {
    auto powReader = new RegisterReader(0);
    std::string relPath = this->programspath;

    char *command = new char[1024];
    char *path = new char[1024];
    sprintf(command, "%s/%s", relPath.c_str(), file.c_str());
    sprintf(path, "%s/%s", relPath.c_str(), "src/compiled");

    double engAverage = 0;
    char* const args[] = {  };

    auto *preeng  = (double *) mmap(nullptr, sizeof (int) , PROT_READ | PROT_WRITE,
                         MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    auto *start  = (struct timespec *) mmap(nullptr, sizeof (int) , PROT_READ | PROT_WRITE,
                                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    auto *end  = (struct timespec *) mmap(nullptr, sizeof (int) , PROT_READ | PROT_WRITE,
                                            MAP_SHARED | MAP_ANONYMOUS, -1, 0);


    //std::cout << "Starting the profile of " << command << "\n";

    double cummulated_eng = 0.0;
    cpu_set_t set;

    for (int i = 0; i < this->repetitions; i++){
        pid_t c_pid = fork();

        if(c_pid == 0){
            //std::cout << "\t\tChild with pid " << c_pid << " executing the file. \n" << std::flush;
            CPU_ZERO(&set);        // clear cpu mask
            CPU_SET(0, &set);      // set cpu 3
            sched_setaffinity(c_pid, sizeof(cpu_set_t), &set);  // 0 is the calling process

            *preeng = powReader->getEnergy();

            execl(command, command);
            exit(1);

        }else{
            CPU_ZERO(&set);        // clear cpu mask
            CPU_SET(3, &set);      // set cpu 3
            sched_setaffinity(c_pid, sizeof(cpu_set_t), &set);  // 0 is the calling process


            wait(nullptr);

            double posteng = powReader->getEnergy();

            //If the register overflows...
            if(*preeng > posteng){
                pid_t ic_pid = fork();

                if(ic_pid == 0){
                    //std::cout << "\t\tChild with pid " << c_pid << " executing the file. \n" << std::flush;
                    CPU_ZERO(&set);        // clear cpu mask
                    CPU_SET(0, &set);      // set cpu 3
                    sched_setaffinity(ic_pid, sizeof(cpu_set_t), &set);  // 0 is the calling process

                    *preeng = powReader->getEnergy();

                    execl(command, command);
                    exit(1);

                }else {
                    CPU_ZERO(&set);        // clear cpu mask
                    CPU_SET(3, &set);      // set cpu 3
                    sched_setaffinity(ic_pid, sizeof(cpu_set_t), &set);  // 0 is the calling process

                    wait(nullptr);

                    posteng = powReader->getEnergy();
                }

            }else{
                cummulated_eng += posteng - *preeng;
            }

        }

        //std::cout << "\tI ran after the exec " << "\n" << std::flush;
    }

    *ptr = cummulated_eng/(double) this->repetitions;

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

long Profiler::getIterations() {
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

