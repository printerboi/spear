#include "src/include/Bencher/Bencher.h"
#include "src/include/CSV-Handler/CSVHandler.h"

int main(){
    Bencher B = Bencher(10, 100);
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

    return 0;
}