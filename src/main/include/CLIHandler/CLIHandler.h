//
// Created by maximiliank on 03.04.24.
//

#ifndef SPEAR_CLIHANDLER_H
#define SPEAR_CLIHANDLER_H


#include "CLIOptions.h"
#include <vector>
#include <filesystem>
#include "iostream"
#include <exception>


class CLIHandler {
public:
    CLIHandler(const CLIHandler&)            = delete;
    CLIHandler& operator=(const CLIHandler&) = delete;

    static CLIOptions parseCLI(int argc, char *argv[]);

private:
    static bool hasOption(
            const std::vector<std::string_view>& arguments,
            const std::string_view& option_name
            );

    static std::string_view get_option(
            const std::vector<std::string_view>& arguments,
            const std::string_view& option_name
            );
};


#endif //SPEAR_CLIHANDLER_H
