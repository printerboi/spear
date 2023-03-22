//
// Created by maximiliank on 18.01.23.
//

#include <fstream>
#include "iostream"
#include "JSONHandler.h"

JSONHandler::JSONHandler() = default;

Json::Value JSONHandler::read(const std::string& filename) {
    Json::Reader reader;
    Json::Value data;
    std::ifstream fileStream;
    fileStream.open(filename);
    fileStream >> data;

    return data;
}

void JSONHandler::write(
        const std::string& filename,
        const std::vector<std::pair<std::string, std::string>>& cpu,
        const std::string& timeStartString,
        const std::string& timeEndString,
        const std::string& iterationsString,
        const std::vector<std::pair<std::string,
        double>>& profile
    ) {

    Json::StyledWriter styledWriter;
    Json::Value data;
    Json::Value cpuJson;
    Json::Value profileJson;
    std::ofstream fileStream;
    fileStream.open(filename);

    if(fileStream.is_open()){
        data["cpu"] = cpuJson;
        for ( const auto& cpuObj : cpu ) {
            data["cpu"][cpuObj.first] = cpuObj.second;
        }

        data["startOfExecution"] = timeStartString;
        data["endOfExecution"] = timeEndString;
        data["iterationsString"] = iterationsString;

        data["profile"] = profileJson;
        for ( const auto& profileObj : profile ) {
            data["profile"][profileObj.first] = profileObj.second;
        }

        fileStream << styledWriter.write(data);
        fileStream.close();
    }else{
        std::cout << "ERROR ERROR ERROR FILE NOT OPEN" << "\n";
    }
}

Json::Value JSONHandler::parse(const std::string& json) {
    Json::Value data;
    std::stringstream jsonstream;
    jsonstream << json;
    jsonstream >> data;

    return data;
}


