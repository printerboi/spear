#include <cstring>
#include "src/include/Bencher/Bencher.h"
#include "src/include/LLVM-Handler/LLVMHandler.h"
#include "src/include/LLVM-Handler/InstructionCategory.h"
#include "src/include/JSON-Handler/JSONHandler.h"
#include "src/passes/energy/energy.cpp"

#include "iostream"
#include "filesystem"
#include <chrono>
#include <llvm/IR/LegacyPassManager.h>
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Transforms/Utils/InstructionNamer.h"
#include "llvm/Transforms/Utils/Mem2Reg.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar/LoopRotation.h"

int main(int argc, const char **argv){
    std::string helpString = "Usage: ba option <arguments>\n==============================\nOptions:\n\t-b\t Benchmarks the system and generates the energy values used for any further analysis\n\t-a\t ...";

    if( argc >= 2 ){
        if( std::strcmp( argv[1], "-b" ) == 0 && argc == 4 ){
            try {
                //Get the parameters from the arguments
                int rep = std::stoi( argv[2] );
                int ite = std::stoi( argv[3] );

                //Create a Bencher-object
                Bencher B = Bencher(ite, rep);

                std::cout << "Starting the benchmark..." << std::endl;

                //Start the time measurement
                auto start = std::chrono::system_clock::now();
                //Launch the benchmarking
                std::vector<double> result = B.benchmark();
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
            if (std::filesystem::exists(argv[2]) && std::filesystem::exists(argv[3]) &&
                !std::filesystem::is_directory(argv[2]) && !std::filesystem::is_directory(argv[3])) {
                //Create a LLVMHandler object
                Json::Value energyJson = JSONHandler::read(argv[3]);
                LLVMHandler llh = LLVMHandler(energyJson, 5000);

                //Debug printing
            } else {
                std::cerr << "Error: Please provide a valid file";
                return 1;
            }
        }else if(std::strcmp( argv[1], "-d" ) == 0 && argc == 2){
            llvm::LLVMContext context;
            llvm::SMDiagnostic error;
            llvm::PassBuilder passBuilder;
            llvm::LoopAnalysisManager loopAnalysisManager;
            llvm::FunctionAnalysisManager functionAnalysisManager;
            llvm::CGSCCAnalysisManager cGSCCAnalysisManager;
            llvm::ModuleAnalysisManager moduleAnalysisManager;
            llvm::ModulePassManager modulePassManager;
            llvm::FunctionPassManager functionPassManager;

            auto module_up = std::move(llvm::parseIRFile("/home/maximiliank/Dokumente/workbench/Bachelor/bachelorarbeit/test.ll", error, context));

            passBuilder.registerModuleAnalyses(moduleAnalysisManager);
            passBuilder.registerCGSCCAnalyses(cGSCCAnalysisManager);
            passBuilder.registerFunctionAnalyses(functionAnalysisManager);
            passBuilder.registerLoopAnalyses(loopAnalysisManager);
            passBuilder.crossRegisterProxies(
                    loopAnalysisManager, functionAnalysisManager, cGSCCAnalysisManager,
                    moduleAnalysisManager);

            //functionPassManager = passBuilder.buildFunctionSimplificationPipeline(llvm::OptimizationLevel::O2, llvm::ThinOrFullLTOPhase::ThinLTOPostLink);
            modulePassManager = passBuilder.buildPerModuleDefaultPipeline( llvm::OptimizationLevel::O2 );

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

                    functionPassManager.addPass(Energy("/home/maximiliank/Dokumente/workbench/Bachelor/bachelorarbeit/cmake-build-debug/benchmarkresult.json"));
                    functionPassManager.run(F, functionAnalysisManager);
                }

            }

 /*           //instname
            functionPassManager.addPass(llvm::InstructionNamerPass());
            //mem2reg
            functionPassManager.addPass(llvm::PromotePass());

            //instcombine
            functionPassManager.addPass(llvm::InstCombinePass());

            //loop-simplify
            functionPassManager.addPass(llvm::LoopSimplifyPass());

            functionPassManager.addPass(Energy("/home/maximiliank/Dokumente/workbench/Bachelor/bachelorarbeit/cmake-build-debug/benchmarkresult.json"));

            modulePassManager.run(*std::move(module_up), moduleAnalysisManager);*/

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
