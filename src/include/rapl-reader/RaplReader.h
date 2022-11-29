//
// Created by max on 23.11.22.
//

#ifndef RAPL_READER_RAPLREADER_H
#define RAPL_READER_RAPLREADER_H

#include "string"

/**
 * Simple Class to read the rapl-api
 * @author Maximilian Krebs
 * @date 23.11.2022
 * @property raplpath Path of the rapl api to read out
 */
class RaplReader {
    public:
        std::string raplpath;
        /**
         * Costructor creating a RaplReader Object with provided path to the rapl interface
         * @param basepath String containing the system path to the rapl interface to read out
         */
        explicit RaplReader(std::string basepath);

        /**
         * Method to read the current energycounter from the provided raplpath
         * @return The current energycounter as long value
         */
        long readEnergy() const;
        /**
         * Method to benchmark the function on the provided address.
         * Prints out a summary of the time the code ran, and the used energy
         * @param codeToRun Address of the function to benchmark
         */
        void benchmarkCode(void (*codeToRun)()) const;
};


#endif //RAPL_READER_RAPLREADER_H