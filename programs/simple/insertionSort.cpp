#include <iostream>
#include <algorithm>
#include "vector"

void sort(int *array, int length){

    for(int index = 0; index < length; index++){
        int toSort = array[index];
        int iter = index - 1;

        while(iter >= 0 && array[iter] > toSort){
            array[iter + 1] = array[iter];
            iter = iter - 1;
        }
        array[iter + 1] = toSort;
    }
}


int main(){
    int length = 10000;
    std::vector<int> sortarr(length);

    int n = length-1;
    std::generate(sortarr.begin(), sortarr.end(), [&n]{ return n--; });

    for(int index = 0; index < length; index++){
        int toSort = sortarr[index];
        int iter = index - 1;

        while(iter >= 0 && sortarr[iter] > toSort){
            sortarr[iter + 1] = sortarr[iter];
            iter = iter - 1;
        }
        sortarr[iter + 1] = toSort;
    }

    std::cout << "sorted " << sortarr[0] << "\n";

    return 0;
}