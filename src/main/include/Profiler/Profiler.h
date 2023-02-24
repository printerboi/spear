//
// Created by max on 26.12.22.
//

#ifndef BA_PROFILER_H
#define BA_PROFILER_H

#include "../Rapl-reader/RegisterReader.h"
#include "../Rapl-reader/PowercapReader.h"
#include "string"
#include "vector"

/**
 * Simple class to profile the llvm-code and output the data in an appropriate format
 */
class Profiler {
    public:
        /**
         * Times the energy values will be executed. Needed for the average
         */
        int iterations;
        /**
         * Times a single program will be executed
         */
        int repetitions;

        std::string programspath;
        /**
         * Creates a Profiler object and sets the iterations and repetitions property according to the parameters
         * @constructor
         * @param it Iterations for the average
         * @param rep Times a program will be executed repeatedly
         */
        explicit Profiler(int it, int rep, std::string path);
        /**
         * Runs the profile and returns the values for the benchmarked files
         */
        std::vector<double> profile();
    private:
        /**
         * Benchmarks a single file, calculates the used energy and returns the calculated value
         * @param file String to the file that needs to be benchmarked
         * @return The used energy
         */
        double benchmarkFile(std::string file);
};


#endif //BA_PROFILER_H
