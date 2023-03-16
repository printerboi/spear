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

void JSONHandler::write(std::string filename, std::vector<std::pair<std::string, std::string>> cpu, std::string timestart, std::string timeend, std::string iterations, std::vector<std::pair<std::string, double>> profile) {
    Json::StyledWriter swriter;
    Json::Value data;
    Json::Value cpuJson;
    Json::Value profileJson;
    std::ofstream fileStream;
    fileStream.open(filename);

    if(fileStream.is_open()){
        data["cpu"] = cpuJson;
        for ( auto c : cpu ) {
            data["cpu"][c.first] = c.second;
        }

        data["startOfExecution"] = timestart;
        data["endOfExecution"] = timeend;
        data["iterations"] = iterations;

        data["profile"] = profileJson;
        for ( auto p : profile ) {
            data["profile"][p.first] = p.second;
        }

        fileStream << swriter.write(data);
        fileStream.close();
    }else{
        std::cout << "ERROR ERROR ERROR FILE NOT OPEN" << "\n";
    }
}

Json::Value JSONHandler::parse(std::string json) {
    Json::Value data;
    std::stringstream jsonstream;
    jsonstream << json;
    jsonstream >> data;

    return data;
}


