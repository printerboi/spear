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

int main(){
    int length = 9000;
    int *sortarr = new int[length];
    fillArrayRandom(sortarr, length, length*2);

    for(int index = length; index > 1; index--){
        for(int j=0; j < index-1; j++){
            if(sortarr[j] > sortarr[j+1]){
                int c = sortarr[j];
                sortarr[j] = sortarr[j+1];
                sortarr[j+1] = c;
            }
        }
    }

    return 0;
}
