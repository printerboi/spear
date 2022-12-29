//
// Created by max on 29.12.22.
//

#include "CSVHandler.h"
#include <fstream>

void CSVHandler::writeCSV(std::string filename,
                          char seperator,
                          std::vector<std::pair<std::string, std::vector<double>>> data){
    //Open as ofstream to the file
    std::ofstream fileToWrite(filename);

    //Insert the column-names into the file
    for (int i = 0; i < data.size(); ++i) {
        fileToWrite << data.at(i).first;

        //if we are at the last entry, don't print the separator
        if(i != data.size() - 1){
            fileToWrite << seperator;
        }
    }
    //Add a linebreak after the columns
    fileToWrite << "\n";

    //Insert the data into the file
    for (int j = 0; j < data.size(); ++j) {

        for (int i = 0; i < data.at(j).second.size(); ++i) {
            fileToWrite << data.at(j).second.at(i);
            if(j != data.size() - 1){
                fileToWrite << seperator;
            }
        }

        //if we reached the last entry of the data, insert a linebreak
        if(j == data.size() - 1){
            fileToWrite << "\n";
        }
    }

    //close the file to mitigate side effects
    fileToWrite.close();
}