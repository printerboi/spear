//
// Created by max on 26.12.22.
//

#include <mutex>
#include "Profiler.h"
#include <thread>
#include <unistd.h>
#include <sys/wait.h>
#include "iostream"


Profiler::Profiler(int it, int rep, std::string path){
    this->iterations = it;
    this->repetitions = rep;
    this->programspath = path;
}

std::vector<double> Profiler::profile() {

    double cast = benchmarkFile("src/compiled/cast");
    double memoryread = benchmarkFile("src/compiled/memoryread");
    double memorywrite = benchmarkFile("src/compiled/memorywrite");
    double programflow = benchmarkFile("src/compiled/programflow");
    double division = benchmarkFile("src/compiled/division");
    double stdbinary = benchmarkFile("src/compiled/stdbinary");


    return {
            cast,
            memoryread,
            memorywrite,
            programflow,
            division,
            stdbinary
    };
}

double Profiler::benchmarkFile(std::string file) {
    auto powReader = new RegisterReader(0);
    std::string relPath = this->programspath;

    char *command = new char[1024];
    char *path = new char[1024];
    sprintf(command, "%s/%s", relPath.c_str(), file.c_str());
    sprintf(path, "%s/%s", relPath.c_str(), "src/compiled");

    double engAverage = 0;
    char* const args[] = {  };

    for (int i = 0; i < this->iterations; ++i) {

        int pid, status;
        pid = fork();

        if(pid==0){
            for (int j = 0; j < this->repetitions; ++j) {

                //system(command);
                execlp(command, command);
            }

        }

        auto preEng = (double) powReader->getEnergy();
        //auto start = std::chrono::system_clock::now();
        struct timespec start, end;
        clock_gettime(CLOCK_REALTIME, &start);

        if (waitpid(pid, &status, 0) > 0) {
            auto postEng = (double) powReader->getEnergy();
            //auto end = std::chrono::system_clock::now();
            clock_gettime(CLOCK_REALTIME, &end);

            auto timerun = end.tv_nsec - start.tv_nsec;

            //std::cout << "time run: " << timerun << "\n";

            auto beforeSleep = (double) powReader->getEnergy();
            std::this_thread::sleep_for(std::chrono::nanoseconds(timerun));
            auto afterSleep = (double) powReader->getEnergy();


            //auto noise = afterSleep - beforeSleep;
            double noise = 0.0;
            auto absEng = (postEng - preEng) - noise;

            absEng = absEng / this->repetitions;

            //std::cout << postEng << "\n";
            //std::cout << preEng << "\n";

            engAverage = engAverage + absEng;
        }




    }

    //int status;

    //if (waitpid(pid, &status, 0) > 0) {
        return engAverage/this->iterations;
    //}

}