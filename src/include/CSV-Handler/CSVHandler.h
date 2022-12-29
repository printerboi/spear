//
// Created by max on 29.12.22.
//

#ifndef BA_CSVHANDLER_H
#define BA_CSVHANDLER_H

#include "string"
#include "vector"

/**
 * Class to handle CSV Files.
 */
class CSVHandler {
    public:
        /**
         * Static function to write a .csv file.
         * @param filename Name of the .csv file to be written
         * @param seperator Char to separate entries
         * @param data Data to fill the file with
         */
        static void writeCSV(std::string filename,
                             char seperator,
                             std::vector<std::pair<std::string, std::vector<double>>> data);
};


#endif //BA_CSVHANDLER_H
