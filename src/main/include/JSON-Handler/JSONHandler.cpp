//
// Created by maximiliank on 18.01.23.
//

#include <fstream>
#include "iostream"
#include "JSONHandler.h"

JSONHandler::JSONHandler() = default;

//Read the data from the provided file
Json::Value JSONHandler::read(const std::string& filename) {
    //init reader and data
    Json::Reader reader;
    Json::Value data;

    //Create a filestream to the provided file
    std::ifstream fileStream;
    fileStream.open(filename);

    //Move the data from the stream to our initialized JSON::Value object
    fileStream >> data;

    return data;
}

//Write the provided data to a file to save the generated profile
void JSONHandler::write(
        const std::string& filename,
        const std::map<std::string, std::string>& cpu,
        const std::string& timeStartString,
        const std::string& timeEndString,
        const std::string& iterationsString,
        const std::map<std::string, double>& profile,
        const std::string& unit
    ) {

    //Init the writer, data and some help-objects
    Json::StyledWriter styledWriter;
    Json::Value data;
    Json::Value cpuJson;
    Json::Value profileJson;

    //Open an output-filestream to the provided filename
    std::ofstream fileStream;
    fileStream.open(filename);

    //If the opening of the filestream worked as expected...
    if(fileStream.is_open()){
        //create the cpu json-object
        data["cpu"] = cpuJson;
        for ( const auto& cpuObj : cpu ) {
            data["cpu"][cpuObj.first] = cpuObj.second;
        }

        //create object for the executiontime, the used iterations and the energy unit
        data["startOfExecution"] = timeStartString;
        data["endOfExecution"] = timeEndString;
        data["iterationsString"] = iterationsString;
        data["unit"] = unit;

        //Create the profile object
        data["profile"] = profileJson;
        for ( const auto& profileObj : profile ) {
            data["profile"][profileObj.first] = profileObj.second;
        }

        //write the data to the filestream
        fileStream << styledWriter.write(data);
        fileStream.close();
    }else{
        std::cout << "ERROR opening the file" << "\n";
    }
}


