#include <iostream>
#include <array>
#include <algorithm>
#include "../helper/randomFiller.cpp"

int search(int *array, int length, int element){
    for(int i=0; i < length; i++){
        if(array[i] == element){
            return i;
        }
    }

    return -1;
}

int main(){
    int length = 9000;
    int *searchroom = new int[length];
    fillArray(searchroom, length);
    int toSearch = getRandomInt(length);

    int index = search(searchroom, length, toSearch);
    std::cout << "Element to search found at Index: " << index << "\n";

    return 0;
}