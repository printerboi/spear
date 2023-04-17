
#ifndef BA_PROFILER_H
#define BA_PROFILER_H

#include "../Rapl-reader/RegisterReader.h"
#include "../Rapl-reader/PowercapReader.h"
#include "string"
#include "vector"
#include "json/json.h"

/**
 * Simple class to profile the llvm-code and output the data in an appropriate format
 */
class Profiler {
    public:
        /**
         * Times a single program will be executed
         */
        long repetitions;

        std::map<std::string, std::string> *profileCode;
        /**
         * Creates a Profiler object and sets the iterations and repetitions property according to the parameters
         * @constructor
         * @param it Iterations for the average
         * @param rep Times a program will be executed repeatedly
         */
        explicit Profiler(int rep, std::map<std::string, std::string> *profileCode);
        /**
         * Runs the profile and returns the values for the benchmarked files
         */
        std::map<std::string, double> profile();

        static std::string getCPUName();

        static std::string getArchitecture();

        static std::string getNumberOfCores();

        static std::string getUnit();
    private:
        /**
         * Benchmarks a single file, calculates the used energy and returns the calculated value
         * @param file String to the file that needs to be benchmarked
         * @return The used energy
         */
        double measureFile(const std::string& file) const;
};


#endif //BA_PROFILER_H
