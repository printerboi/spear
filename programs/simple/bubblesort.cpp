#include <iostream>
#include "../helper/randomFiller.cpp"

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
