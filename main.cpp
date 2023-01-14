#include <cstring>
#include "src/include/Bencher/Bencher.h"
#include "src/include/CSV-Handler/CSVHandler.h"
#include "src/include/LLVM-Handler/LLVMHandler.h"

#include "iostream"
#include "filesystem"

int main(int argc, const char **argv){
    if( argc >= 2 ){
        if( std::strcmp( argv[1], "-b" ) == 0 && argc == 4 ){
            try {
                int rep = std::stoi( argv[2] );
                int ite = std::stoi( argv[3] );

                Bencher B = Bencher(ite, rep);
                std::vector<double> result = B.benchmark();
                std::vector<double> groupM(1, result[0]);
                std::vector<double> groupB(1, result[1]);
                std::vector<double> groupD(1, result[2]);
                std::vector<double> groupG(1, result[3]);

                std::vector<std::pair<std::string, std::vector<double>>> data = {
                        {"Group M", groupM},
                        {"Group B", groupB},
                        {"Group D", groupD},
                        {"Group G", groupG},
                };
                CSVHandler::writeCSV("benchmarkresult.csv", ',' ,data);

            }catch( std::invalid_argument &iv ){
                std::cerr << "The given arguments are not useable as ints";
                return 1;
            }
        }else if( std::strcmp( argv[1], "-m" ) == 0 && argc == 3 ){
            if( std::filesystem::exists( argv[2] ) && !std::filesystem::is_directory( argv[1] ) ){
                LLVMHandler llh = LLVMHandler( argv[2] );
                llh.print();
            }else{
                std::cerr << "Error: Please provide a valid file";
                return 1;
            }
        }else{
            std::cerr << "Usage: ...";
            return 1;
        }
    }else{
        std::cerr << "Usage: ...";
        return 1;
    }

    return 0;
}
