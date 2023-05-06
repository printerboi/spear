#include <iostream>
#include "algorithm"
#include "vector"

void sort(int *array, int length){
    for(int index = length; index > 1; index--){
        for(int j=0; j < index-1; j++){
            if(array[j] > array[j+1]){
                int c = array[j];
                array[j] = array[j+1];
                array[j+1] = c;
            }
        }
    }
}

int main(){
    int length = 10000;
    std::vector<int> sortarr(length);

    int n = length-1;
    std::generate(sortarr.begin(), sortarr.end(), [&n]{ return n--; });

    for(int index = length; index > 1; index--){
        for(int j=0; j < index-1; j++){
            if(sortarr[j] > sortarr[j+1]){
                int c = sortarr[j];
                sortarr[j] = sortarr[j+1];
                sortarr[j+1] = c;
            }
        }
    }

    std::cout << "sorted " << sortarr[0] << "\n";

    return 0;
}
