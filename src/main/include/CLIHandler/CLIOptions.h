//
// Created by maximiliank on 03.04.24.
//

#ifndef SPEAR_CLIOPTIONS_H
#define SPEAR_CLIOPTIONS_H

#include <string>
#include <utility>

/**
 * Enum to handle the operations available in the application
 */
enum class Operation {
    UNDEFINED,
    ANALYZE,
    PROFILE
};

/**
 * Enum to distinguish the analysis target in the application
 */
enum class Mode {
    UNDEFINED,
    PROGRAM,
    BLOCK,
    FUNCTION,
    INSTRUCTION,
    GRAPH
};

/**
 * Enum to distinguish the analysis target in the application
 */
enum class DeepCalls {
    UNDEFINED,
    ENABLED,
};

/**
 * Enum used to specify the output format
 */
enum class Format {
    UNDEFINED,
    PLAIN,
    JSON
};

/**
 * Enum describing the analysis strategy
 */
enum class Strategy {
    UNDEFINED,
    WORST,
    AVERAGE,
    BEST
};

class CLIOptions {
public:
    /**
     * Parsed amount of measurement repeats
     */
    int repeatAmount;

    /**
     * Path where profile should be saved
     */
    std::string saveLocation;

    /**
     * Path where the profile should be read from
     */
    std::string profilePath;

    /**
     * Parsed operation
     */
    Operation operation;

    /**
     * Parsed mode
     */
    Mode mode;

    /**
     * Parsed format
     */
    Format format;

    /**
     * Parsed strategy
     */
    Strategy strategy;

    /**
     * Parsed check if the user requests the calculation with calls or not
     */
    DeepCalls deepCalls;

    /**
     * Parsed loopbound;
     */
    int loopBound;

    /**
     * Path where the program should be read from
     */
    std::string programPath;

    /**
     * Parameter specifying if the user limits their execution to a function with the given name
     */
    std::string forFunction;

    /**
     * Path where the profile should be read from
     */
    std::string codePath;

    CLIOptions();

    static Mode strToMode(const std::string& str);

    static Format strToFormat(const std::string& str);

    static Strategy strToStrategy(const std::string& str);
};

class ProfileOptions : public CLIOptions{
public:

    ProfileOptions(std::string codePath,  int repeatAmount, std::string saveLocation);
};

class AnalysisOptions : public CLIOptions{
public:

    AnalysisOptions(std::string profilePath, Mode mode, Format format, Strategy strategy, int loopBound,
                    std::string programPath, DeepCalls deepCalls, std::string forFunction);
};


#endif //SPEAR_CLIOPTIONS_H
