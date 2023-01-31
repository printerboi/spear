//
// Created by maximiliank on 18.01.23.
//

#ifndef BA_JSONHANDLER_H
#define BA_JSONHANDLER_H

#include "json/json.h"

/**
 * Class for handling the reading and writing of JSON files to save and load the energy data measured in the profile
 */
class JSONHandler {
    public:
        /**
         * Default constructor
         */
        explicit JSONHandler();

        /**
         * Method for writing the energy-data to a provided file
         * @param filename String containing the path to the file the method should write to
         * @param data Datastructure containing the energy values with respect to the specific group
         */
        static void write(std::string filename, std::vector<std::pair<std::string, double>> data);

        /**
         * Method for reading the Json-Values from a provided file
         * @param filename Path to the file that should be read
         * @return Return the JSON::Value which was read from the provided file
         */
        static Json::Value read(std::string filename);
};


#endif //BA_JSONHANDLER_H
