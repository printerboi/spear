#include <iostream>
#include "../helper/randomFiller.cpp"

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

void print(int *array, int length){
    std::cout << "[";


    for(int index = 0; index < length; index++){
        std::cout << array[index] << ",";
    }

    std::cout << "]\n";
}

int main(){
    int length = 9000;
    int *sortarr = new int[length];
    fillArrayRandom(sortarr, length, length*2);
    sort(sortarr, length);

    return 0;
}