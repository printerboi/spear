#include <cstring>
#include "src/main/include/Profiler/Profiler.h"
#include "src/main/include/LLVM-Handler/LLVMHandler.h"
#include "src/main/include/LLVM-Handler/InstructionCategory.h"
#include "src/main/include/JSON-Handler/JSONHandler.h"
#include "src/main/passes/energy/energy.cpp"

#include "iostream"
#include "filesystem"
#include <chrono>
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Analysis/RegionPass.h"
#include "llvm/Transforms/Utils/InstructionNamer.h"
#include "llvm/Transforms/Utils/Mem2Reg.h"
#include "llvm/Transforms/Scalar/LoopRotation.h"

int main(int argc, const char **argv){
    std::string helpString = "Usage: spear option <arguments>\n==============================\nOptions:"
                             "\n\t-p\t Profile the system and generate the estimated energy usage of the device. Used for any further analysis"
                             "\n\t-a\t Analyzes a given program. Further parameters are needed:"
                             "\n\t\t\t --model Path to a .json containing the profile"
                             "\n\t\t\t --mode Type of analysis (program/function)"
                             "\n\t\t\t --format Format of the result to print (plain/json)"
                             "\n\t\t\t --strategy Type of analysis-strategy (worst/best/average)"
                             "\n\t\t\t --loopbound Value with with which loops get approximed if their upper bound can't be calculated (0 - INT_MAX)";

    if( argc >= 2 ){
        if( std::strcmp( argv[1], "-b" ) == 0 && argc == 4 ){
            try {
                //Get the parameters from the arguments
                int rep = std::stoi( argv[2] );
                int ite = std::stoi( argv[3] );

                //Create a Profiler-object
                Profiler B = Profiler(ite, rep);

                std::cout << "Starting the profile..." << std::endl;

                //Start the time measurement
                auto start = std::chrono::system_clock::now();
                //Launch the benchmarking
                std::vector<double> result = B.profile();
                //Stop the time measurement
                auto end = std::chrono::system_clock::now();
                //Calculate the elapsed time by substracting the two timestamps
                std::chrono::duration<double> timerun = end - start;

                //Group the vector format of the results
                std::vector<std::pair<std::string, double>> data = {
                        {InstructionCategory::toString(InstructionCategory::Category::MEMSTORE), result[0]},
                        {InstructionCategory::toString(InstructionCategory::Category::MEMLOAD), result[0]},
                        {InstructionCategory::toString(InstructionCategory::Category::PROGRAMFLOW), result[1]},
                        {InstructionCategory::toString(InstructionCategory::Category::DIVISION), result[2]},
                        {InstructionCategory::toString(InstructionCategory::Category::OTHER), result[3]},
                        {"Base", result[4]},
                };
                //Pass the grouped values to the csv handler, so it can be written to a file
                //CSVHandler::writeCSV("benchmarkresult.csv", ',' , data);
                JSONHandler::write("benchmarkresult.json", data);

                std::cout << "Benchmark finished!" << std::endl;
                std::cout << "Elapsed Time: " << timerun.count() << "s" << std::endl;
            }catch( std::invalid_argument &iv ){
                std::cerr << "The given arguments are not useable as ints";
                return 1;
            }
        }else if(std::strcmp( argv[1], "-a" ) == 0 && argc == 12) {
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


            if (std::filesystem::exists(argv[2]) && std::filesystem::exists(argv[11])) {
                auto module_up = llvm::parseIRFile(argv[11], error, context).release();

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


                if (std::strcmp(argv[3], "--mode") == 0) {
                    mode = argv[4];
                }

                if (std::strcmp(argv[5], "--format") == 0) {
                    format = argv[6];
                }

                if (std::strcmp(argv[7], "--strategy") == 0) {
                    strategy = argv[8];
                }

                if (std::strcmp(argv[9], "--loopbound") == 0) {
                    loopbound = argv[10];
                }

                modulePassManager.addPass(Energy(argv[2], mode, format, strategy, loopbound));
                modulePassManager.run(*module_up, moduleAnalysisManager);

            } else {
                std::cerr << helpString;
                return 1;
            }
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
    }

    return 0;
}
