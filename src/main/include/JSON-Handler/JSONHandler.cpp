//
// Created by maximiliank on 18.01.23.
//

#include <fstream>
#include "iostream"
#include "JSONHandler.h"

JSONHandler::JSONHandler() = default;

Json::Value JSONHandler::read(std::string filename) {
    Json::Reader reader;
    Json::Value data;
    std::ifstream fileStream;
    fileStream.open(filename);
    fileStream >> data;

    return data;
}

void JSONHandler::write(std::string filename, std::vector<std::pair<std::string, double>> data) {
    Json::StyledWriter swriter;
    Json::Value dataJson;
    std::ofstream fileStream;
    fileStream.open(filename);

    if(fileStream.is_open()){
        for ( auto p : data ) {
            dataJson[p.first] = p.second;
        }

        fileStream << swriter.write("");
        fileStream << swriter.write(dataJson);
        fileStream.close();
    }else{
        std::cout << "ERROR ERROR ERROR FILE NOT OPEN" << "\n";
    }
}


