#include <cstring>
#include "src/include/Bencher/Bencher.h"
#include "src/include/CSV-Handler/CSVHandler.h"
#include "src/include/LLVM-Handler/LLVMHandler.h"

#include "iostream"
#include "filesystem"
#include <chrono>

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

                //Format the results of the benchmark to fit in the corresponding vector-layout
                std::vector<double> groupM(1, result[0]);
                std::vector<double> groupB(1, result[1]);
                std::vector<double> groupD(1, result[2]);
                std::vector<double> groupG(1, result[3]);
                std::vector<double> base(1, result[4]);

                //Group the vector format of the results
                std::vector<std::pair<std::string, std::vector<double>>> data = {
                        {"Group M", groupM},
                        {"Group B", groupB},
                        {"Group D", groupD},
                        {"Group G", groupG},
                        {"Base", base},
                };
                //Pass the grouped values to the csv handler so it can be written to a file
                CSVHandler::writeCSV("benchmarkresult.csv", ',' ,data);

                std::cout << "Benchmark finished!" << std::endl;
                std::cout << "Elapsed Time: " << timerun.count() << "s" << std::endl;
            }catch( std::invalid_argument &iv ){
                std::cerr << "The given arguments are not useable as ints";
                return 1;
            }
        }else if( std::strcmp( argv[1], "-a" ) == 0 && argc == 3 ){
            if( std::filesystem::exists( argv[2] ) && !std::filesystem::is_directory( argv[1] ) ){
                //Create a LLVMHandler object
                LLVMHandler llh = LLVMHandler( argv[2] );

                //Debug printing
                llh.print();
            }else{
                std::cerr << "Error: Please provide a valid file";
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
