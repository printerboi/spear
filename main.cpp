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
    std::string helpString = "Usage: ba option <arguments>\n==============================\nOptions:"
                             "\n\t-b\t Benchmarks the system and generates the energy values used for any further analysis"
                             "\n\t-a\t ..."
                             "\n\t-d\t Debug. Please provide a energy JSON file followed by the IR code .ll ";

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
        }else if( std::strcmp( argv[1], "-a" ) == 0 && argc == 4 ) {
            std::cout << "Not yet implemented" << std::endl;
        }else if(std::strcmp( argv[1], "-d" ) == 0 && argc >= 4){
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


            if( std::filesystem::exists(argv[2]) && std::filesystem::exists(argv[7] )){
                auto module_up = llvm::parseIRFile(argv[7], error, context).release();

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

                //instcombine
                //functionPassManager.addPass(llvm::InstCombinePass());
                //loop-simplify
                functionPassManager.addPass(llvm::LoopSimplifyPass());

                //loop-rotate
                functionPassManager.addPass( llvm::createFunctionToLoopPassAdaptor(llvm::LoopRotatePass()) );

                modulePassManager.addPass(llvm::createModuleToFunctionPassAdaptor(std::move(functionPassManager)));

                std::string mode = "program";
                std::string format = "plain";

                llvm::outs() << argv[4] << "\n";
                llvm::outs() << argv[5] << "\n";
                llvm::outs() << argv[6] << "\n";
                llvm::outs() << argv[7] << "\n";

                if(std::strcmp(argv[3], "-mode") == 0){
                    mode = argv[4];
                }

                if(std::strcmp(argv[5], "-format") == 0){
                    format = argv[6];
                }


                modulePassManager.addPass(Energy(argv[2], mode, format));
                modulePassManager.run(*module_up, moduleAnalysisManager);
                /*
                for(auto & F : module_up->getFunctionList()){
                    if( F.getName() == "main" ){

                        //instname
                        functionPassManager.addPass(llvm::InstructionNamerPass());
                        //mem2reg
                        functionPassManager.addPass(llvm::PromotePass());

                        //instcombine
                        //functionPassManager.addPass(llvm::InstCombinePass());
                        //loop-simplify
                        functionPassManager.addPass(llvm::LoopSimplifyPass());

                        //loop-rotate
                        functionPassManager.addPass( llvm::createFunctionToLoopPassAdaptor(llvm::LoopRotatePass()) );

                        functionPassManager.addPass(Energy(argv[2]));
                        functionPassManager.run(F, functionAnalysisManager);
                    }
                }*/
            }else{
                std::cerr << helpString;
                return 1;
            }
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
