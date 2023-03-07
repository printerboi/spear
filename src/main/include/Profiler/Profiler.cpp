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

#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMSZ     27


Profiler::Profiler(int it, int rep, std::string path){
    this->iterations = it;
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

    benchmarkFile("src/compiled/cast", &casptr);
    benchmarkFile("src/compiled/call", &callptr);
    benchmarkFile("src/compiled/memoryread", &memreadptr);
    benchmarkFile("src/compiled/memorywrite", &memwritptr);
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
        casptr,
        callptr,
        memreadptr,
        memwritptr,
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

    auto *preeng  = (uint64_t *) mmap(nullptr, sizeof (int) , PROT_READ | PROT_WRITE,
                         MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    auto *start  = (struct timespec *) mmap(nullptr, sizeof (int) , PROT_READ | PROT_WRITE,
                                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    auto *end  = (struct timespec *) mmap(nullptr, sizeof (int) , PROT_READ | PROT_WRITE,
                                            MAP_SHARED | MAP_ANONYMOUS, -1, 0);


    std::cout << "Starting the profile of " << command << "\n";

    long cummulated_eng = 0.0;
    cpu_set_t set;

    for (int i = 0; i < this->iterations; i++){
        pid_t c_pid = fork();

        if(c_pid == 0){
            //std::cout << "\t\tChild with pid " << c_pid << " executing the file. \n" << std::flush;
            CPU_ZERO(&set);        // clear cpu mask
            CPU_SET(0, &set);      // set cpu 3
            sched_setaffinity(c_pid, sizeof(cpu_set_t), &set);  // 0 is the calling process


            uint64_t cmp = powReader->getEnergy();
            *preeng = powReader->getEnergy();
            /*while (*preeng == cmp){
                *preeng = powReader->getEnergy();
            }*/

            execl(command, command);
            exit(1);

        }else{
            CPU_ZERO(&set);        // clear cpu mask
            CPU_SET(3, &set);      // set cpu 3
            sched_setaffinity(c_pid, sizeof(cpu_set_t), &set);  // 0 is the calling process

            //std::cout << "\tParent with pid " << c_pid << " forking. \n"<< std::flush;

            wait(nullptr);

            //uint64_t cmp = powReader->getEnergy();
            uint64_t posteng = powReader->getEnergy();
            /*while (posteng == cmp){
                posteng = powReader->getEnergy();
            }*/

            //std::cout << "time run: " << timerun << "\n";

            //std::cout << "\tChild finished execution with "<< *preeng << "\n" << std::flush;
            //std::cout << "\tParent finished execution with "<< posteng << "\n" << std::flush;
            //std::cout << "\t\t\t=> "<< posteng - *preeng << "\n" << std::flush;

            cummulated_eng += posteng - *preeng;
        }

        //std::cout << "\tI ran after the exec " << "\n" << std::flush;
    }

    *ptr = (double) cummulated_eng/(double) this->iterations;

    return 0;
}