#include <cstring>
#include "src/main/include/Profiler/Profiler.h"
#include "src/main/include/LLVM-Handler/LLVMHandler.h"
#include "src/main/include/LLVM-Handler/InstructionCategory.h"
#include "src/main/include/JSON-Handler/JSONHandler.h"
#include "src/main/passes/energy/energy.cpp"

#include "iostream"
#include "filesystem"
#include <stdexcept>
#include <chrono>
#include <getopt.h>
#include <csignal>

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Analysis/RegionPass.h"
#include "llvm/Transforms/Utils/InstructionNamer.h"
#include "llvm/Transforms/Utils/Mem2Reg.h"
#include "llvm/Transforms/Scalar/LoopRotation.h"

#include "src/main/include/CLIHandler/CLIHandler.h"

static struct option spearOptions[] = {
        {"profile", optional_argument, nullptr, 'p'},
        {"analyze", optional_argument, nullptr, 'a'},
        {nullptr}
};

void runProfileRoutine(CLIOptions opts){
    //Get the parameters from the arguments
    int rep = opts.repeatAmount;

    std::string compiledPath = opts.codePath;

    std::map<std::string, std::string> profileCode;
    profileCode["call"] = compiledPath + "/" + "src/compiled/call";
    profileCode["memory"] = compiledPath + "/" + "src/compiled/memoryread";
    profileCode["programflow"] = compiledPath + "/" + "src/compiled/programflow";
    profileCode["division"] = compiledPath + "/" + "src/compiled/division";
    profileCode["others"] = compiledPath + "/" + "src/compiled/stdbinary";

    std::cout << profileCode["call"] << " -> " << std::filesystem::exists(profileCode["call"])  << std::endl;
    std::cout << profileCode["memory"] << " -> " << std::filesystem::exists(profileCode["memory"]) << std::endl;
    std::cout << profileCode["programflow"] << " -> " << std::filesystem::exists(profileCode["programflow"]) << std::endl;
    std::cout << profileCode["division"] << " -> " << std::filesystem::exists(profileCode["division"]) << std::endl;
    std::cout << profileCode["others"] << " -> " << std::filesystem::exists(profileCode["others"]) << std::endl;

    if(std::filesystem::exists(profileCode["call"]) && std::filesystem::exists(profileCode["memory"]) && std::filesystem::exists(profileCode["programflow"]) && std::filesystem::exists(profileCode["division"]) && std::filesystem::exists(profileCode["others"])){

        std::cout << "Starting the profile..." << std::endl;

        Profiler profiler = Profiler(rep, &profileCode);

        //Start the time measurement
        auto start = std::chrono::system_clock::now();
        //Launch the benchmarking
        try{
            std::map<std::string, double> result = profiler.profile();
            //Stop the time measurement
            auto end = std::chrono::system_clock::now();
            //Calculate the elapsed time by substracting the two timestamps
            std::chrono::duration<double> timerun = end - start;

            std::stringstream starttimestream;
            std::stringstream endtimestream;
            starttimestream << start.time_since_epoch().count();
            endtimestream << end.time_since_epoch().count();

            std::map<std::string, std::string> cpu = {
                    {"name", Profiler::getCPUName()},
                    {"architecture", Profiler::getArchitecture()},
                    {"cores", Profiler::getNumberOfCores()}
            };

            //Group the vector format of the results
            std::map<std::string, double> data = {
                    {InstructionCategory::toString(InstructionCategory::Category::CALL),        result.at("call")},
                    {InstructionCategory::toString(InstructionCategory::Category::MEMORY),     result.at("memory")},
                    {InstructionCategory::toString(InstructionCategory::Category::PROGRAMFLOW), result.at("programflow")},
                    {InstructionCategory::toString(InstructionCategory::Category::DIVISION),    result.at("division")},
                    {InstructionCategory::toString(InstructionCategory::Category::OTHER),       result.at("others")},
            };
            //Pass the grouped values to the csv handler, so it can be written to a file
            //CSVHandler::writeCSV("benchmarkresult.csv", ',' , data);
            char *outputpath = new char[255];
            sprintf(outputpath, "%s/profile.json", opts.saveLocation.c_str());
            std::cout << "Writing " << outputpath << "\n";
            JSONHandler::write(outputpath, cpu, starttimestream.str(), endtimestream.str(), std::to_string(profiler.repetitions),  data, Profiler::getUnit());

            std::cout << "Profiling finished!" << std::endl;
            std::cout << "Elapsed Time: " << timerun.count() << "s" << std::endl;
        }catch(std::invalid_argument &ia){
            std::cerr << "Execution of profile code failed..." << "\n";
        }
    }else{
        std::cerr << "The given path to the profile does not contain a /src/compiled folder!" << "\n";
    }


}

void runAnalysisRoutine(CLIOptions opts){
    llvm::LLVMContext context;
    llvm::SMDiagnostic error;
    llvm::PassBuilder passBuilder;
    llvm::LoopAnalysisManager loopAnalysisManager;
    llvm::FunctionAnalysisManager functionAnalysisManager;
    llvm::CGSCCAnalysisManager cGSCCAnalysisManager;
    llvm::ModuleAnalysisManager moduleAnalysisManager;
    llvm::ModulePassManager modulePassManager;
    llvm::FunctionPassManager functionPassManager;
    llvm::CGSCCPassManager callgraphPassManager;


    auto module_up = llvm::parseIRFile(opts.programPath, error, context).release();

    passBuilder.registerModuleAnalyses(moduleAnalysisManager);
    passBuilder.registerCGSCCAnalyses(cGSCCAnalysisManager);
    passBuilder.registerFunctionAnalyses(functionAnalysisManager);
    passBuilder.registerLoopAnalyses(loopAnalysisManager);
    passBuilder.crossRegisterProxies(
            loopAnalysisManager, functionAnalysisManager, cGSCCAnalysisManager,
            moduleAnalysisManager);

    //instname
    functionPassManager.addPass(llvm::InstructionNamerPass());
    //mem2reg
    functionPassManager.addPass(llvm::PromotePass());

    //loop-simplify
    functionPassManager.addPass(llvm::LoopSimplifyPass());

    //loop-rotate
    functionPassManager.addPass(llvm::createFunctionToLoopPassAdaptor(llvm::LoopRotatePass()));
    modulePassManager.addPass(llvm::createModuleToFunctionPassAdaptor(std::move(functionPassManager)));



    modulePassManager.addPass(Energy(opts.profilePath, opts.mode, opts.format, opts.strategy, opts.loopBound));
    modulePassManager.run(*module_up, moduleAnalysisManager);
}

int main(int argc, char *argv[]){
    std::string helpString = "Usage: spear option <arguments>\n==============================\nOptions:"
                             "\n\tprogram\t Profile the system and generate the estimated energy usage of the device. Used for any further analysis"
                             "\n\tanalyze\t Analyzes a given program. Further parameters are needed:"
                             "\n\t\t\t --mode Type of analysis (program/function)"
                             "\n\t\t\t --format Format of the result to print (plain/json)"
                             "\n\t\t\t --strategy Type of analysis-strategy (worst/best/average)"
                             "\n\t\t\t --loopbound Value with with which loops get approximed if their upper bound can't be calculated (0 - INT_MAX)"
                             "\n\n";

    CLIOptions opts = CLIHandler::parseCLI(argc, argv);

    if(opts.operation == Operation::PROFILE){
        // Check if the parser returned valid options
        if(!opts.codePath.empty() && opts.repeatAmount != -1 && !opts.saveLocation.empty()){
            //std::cout << "Options valid" << std::endl;
            runProfileRoutine(opts);
            return 0;
        }else{
            std::string profileHelpMsg =  "Usage: spear profile <arguments>\n==============================\nArguments:"
                                           "\n\t Profile the system and generate the estimated energy usage of the device. Used for any further analysis"
                                           "\n\t\t --iterations Amount of measurement repetitions (int)"
                                           "\n\t\t --model Path to the compiled profile programs"
                                           "\n\t\t --savelocation Path the calculated profile will be saved to"
                                           "\n\n";
            std::cerr << profileHelpMsg << std::endl;
            return 1;
        }
    }else if(opts.operation == Operation::ANALYZE){
        // Check if the parser returned valid options
        if(!opts.profilePath.empty() && opts.mode != Mode::UNDEFINED && opts.format != Format::UNDEFINED && opts.strategy != Strategy::UNDEFINED && opts.loopBound != -1 && !opts.programPath.empty()){
            //std::cout << "Options valid" << std::endl;
            runAnalysisRoutine(opts);
            return 0;
        }else{
            std::string profileHelpMsg = "Usage: spear analyze <arguments>\n==============================\nArguments:"
                                         "\n\tAnalyzes a given program. Further parameters are needed:"
                                         "\n\t\t --mode Type of analysis (program/function)"
                                         "\n\t\t --format Format of the result to print (plain/json)"
                                         "\n\t\t --strategy Type of analysis-strategy (worst/best/average)"
                                         "\n\t\t --loopbound Value with with which loops get approximed if their upper bound can't be calculated (0 - INT_MAX)"
                                         "\n\n";
            std::cerr << profileHelpMsg << std::endl;
            return 1;
        }
    }else{
        return 1;
    }

    /*if( argc >= 2 ){
        if( std::strcmp( argv[1], "-p" ) == 0 && argc == 5 && std::filesystem::exists(argv[4]) ){
            try {
                //Get the parameters from the arguments
                int rep = std::stoi( argv[2] );



                if(std::filesystem::exists(argv[3])){

                    std::string compiledPath(argv[3]);

                    std::map<std::string, std::string> profileCode;
                    profileCode["call"] = compiledPath + "/" + "src/compiled/call";
                    profileCode["memory"] = compiledPath + "/" + "src/compiled/memoryread";
                    profileCode["programflow"] = compiledPath + "/" + "src/compiled/programflow";
                    profileCode["division"] = compiledPath + "/" + "src/compiled/division";
                    profileCode["others"] = compiledPath + "/" + "src/compiled/stdbinary";

                    std::cout << profileCode["call"] << " -> " << std::filesystem::exists(profileCode["call"])  << std::endl;
                    std::cout << profileCode["memory"] << " -> " << std::filesystem::exists(profileCode["memory"]) << std::endl;
                    std::cout << profileCode["programflow"] << " -> " << std::filesystem::exists(profileCode["programflow"]) << std::endl;
                    std::cout << profileCode["division"] << " -> " << std::filesystem::exists(profileCode["division"]) << std::endl;
                    std::cout << profileCode["others"] << " -> " << std::filesystem::exists(profileCode["others"]) << std::endl;

                    if(std::filesystem::exists(profileCode["call"]) && std::filesystem::exists(profileCode["memory"]) && std::filesystem::exists(profileCode["programflow"]) && std::filesystem::exists(profileCode["division"]) && std::filesystem::exists(profileCode["others"])){

                        std::cout << "Starting the profile..." << std::endl;

                        Profiler profiler = Profiler(rep, &profileCode);

                        //Start the time measurement
                        auto start = std::chrono::system_clock::now();
                        //Launch the benchmarking
                        try{
                            std::map<std::string, double> result = profiler.profile();
                            //Stop the time measurement
                            auto end = std::chrono::system_clock::now();
                            //Calculate the elapsed time by substracting the two timestamps
                            std::chrono::duration<double> timerun = end - start;

                            std::stringstream starttimestream;
                            std::stringstream endtimestream;
                            starttimestream << start.time_since_epoch().count();
                            endtimestream << end.time_since_epoch().count();

                            std::map<std::string, std::string> cpu = {
                                    {"name", Profiler::getCPUName()},
                                    {"architecture", Profiler::getArchitecture()},
                                    {"cores", Profiler::getNumberOfCores()}
                            };

                            //Group the vector format of the results
                            std::map<std::string, double> data = {
                                    {InstructionCategory::toString(InstructionCategory::Category::CALL),        result.at("call")},
                                    {InstructionCategory::toString(InstructionCategory::Category::MEMORY),     result.at("memory")},
                                    {InstructionCategory::toString(InstructionCategory::Category::PROGRAMFLOW), result.at("programflow")},
                                    {InstructionCategory::toString(InstructionCategory::Category::DIVISION),    result.at("division")},
                                    {InstructionCategory::toString(InstructionCategory::Category::OTHER),       result.at("others")},
                            };
                            //Pass the grouped values to the csv handler, so it can be written to a file
                            //CSVHandler::writeCSV("benchmarkresult.csv", ',' , data);
                            char *outputpath = new char[255];
                            sprintf(outputpath, "%s/profile.json", argv[4]);
                            std::cout << "Writing " << outputpath << "\n";
                            JSONHandler::write(outputpath, cpu, starttimestream.str(), endtimestream.str(), std::to_string(profiler.repetitions),  data, Profiler::getUnit());

                            std::cout << "Profiling finished!" << std::endl;
                            std::cout << "Elapsed Time: " << timerun.count() << "s" << std::endl;
                        }catch(std::invalid_argument &ia){
                            std::cerr << "Execution of profile code failed..." << "\n";
                            return 1;
                        }
                    }else{
                        std::cerr << "The given path to the profile does not contain a /src/compiled folder!" << "\n";
                        return 1;
                    }


                }else{
                    std::cerr << "The given path to the profile does not exist!!!";
                    return 1;
                }


            }catch( std::invalid_argument &iv ){
                std::cerr << "The given arguments are not useable as ints";
                return 1;
            }
        }else if(std::strcmp( argv[1], "-a" ) == 0 && argc == 13) {
            llvm::LLVMContext context;
            llvm::SMDiagnostic error;
            llvm::PassBuilder passBuilder;
            llvm::LoopAnalysisManager loopAnalysisManager;
            llvm::FunctionAnalysisManager functionAnalysisManager;
            llvm::CGSCCAnalysisManager cGSCCAnalysisManager;
            llvm::ModuleAnalysisManager moduleAnalysisManager;
            llvm::ModulePassManager modulePassManager;
            llvm::FunctionPassManager functionPassManager;
            llvm::CGSCCPassManager callgraphPassManager;


            std::cout << std::filesystem::exists(argv[3]) << "\n";
            std::cout << std::filesystem::exists(argv[12]) << "\n";
            if (std::filesystem::exists(argv[3]) && std::filesystem::exists(argv[12])) {
                auto module_up = llvm::parseIRFile(argv[12], error, context).release();

                std::vector<std::string> arguments(argv + 1, argv + argc);

                passBuilder.registerModuleAnalyses(moduleAnalysisManager);
                passBuilder.registerCGSCCAnalyses(cGSCCAnalysisManager);
                passBuilder.registerFunctionAnalyses(functionAnalysisManager);
                passBuilder.registerLoopAnalyses(loopAnalysisManager);
                passBuilder.crossRegisterProxies(
                        loopAnalysisManager, functionAnalysisManager, cGSCCAnalysisManager,
                        moduleAnalysisManager);

                //instname
                functionPassManager.addPass(llvm::InstructionNamerPass());
                //mem2reg
                functionPassManager.addPass(llvm::PromotePass());

                //loop-simplify
                functionPassManager.addPass(llvm::LoopSimplifyPass());

                //loop-rotate
                functionPassManager.addPass(llvm::createFunctionToLoopPassAdaptor(llvm::LoopRotatePass()));
                modulePassManager.addPass(llvm::createModuleToFunctionPassAdaptor(std::move(functionPassManager)));

                std::string mode;
                std::string format;
                std::string strategy;
                std::string loopbound;


                if (std::strcmp(argv[4], "--mode") == 0) {
                    mode = argv[5];
                }

                if (std::strcmp(argv[6], "--format") == 0) {
                    format = argv[7];
                }

                if (std::strcmp(argv[8], "--strategy") == 0) {
                    strategy = argv[9];
                }

                if (std::strcmp(argv[10], "--loopbound") == 0) {
                    loopbound = argv[11];
                }

                modulePassManager.addPass(Energy(argv[3], mode, format, strategy, loopbound));
                modulePassManager.run(*module_up, moduleAnalysisManager);

            } else {
                std::cerr << helpString;
                return 1;
            }
        }else if(std::strcmp( argv[1], "-a" ) == 0 && argc == 4){
            double eng = 0.00;
            std::string cmd = argv[3];
            if(access( cmd.c_str(), F_OK ) != -1){
                eng = Profiler::measureProgram(cmd, std::stoi(argv[2]));
            }

            std::cout << eng << "\n";

        }else if(std::strcmp( argv[1], "-t" ) == 0 && argc == 4){
            double time = 0.00;
            std::string cmd = argv[3];
            if(access( cmd.c_str(), F_OK ) != -1){
                time = Profiler::timeProgram(cmd, std::stoi(argv[2]));
            }

            std::cout << time << "\n";

        }else if(std::strcmp( argv[1], "-s" ) == 0 && argc == 4){
            double time = 0.00;
            std::string cmd = argv[3];
            if(access( cmd.c_str(), F_OK ) != -1){
                time = Profiler::timeProgram(cmd, std::stoi(argv[2]));
            }

            std::cout << time << "\n";

        }else if(std::strcmp( argv[1], "-h" ) == 0){
            std::cerr << helpString;
            return 1;
        }else{
            std::cerr << helpString;
            return 1;
        }
    }else{
        std::cerr << helpString;
        return 1;
    }*/

    return 0;
}