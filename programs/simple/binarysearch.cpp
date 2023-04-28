#include <iostream>
#include <array>
#include <algorithm>
#include "../helper/randomFiller.cpp"

int search(int *array, int left, int right, int element){
    if(right >= left){
        int mid = left + (right - left) / 2;

        if(array[mid] == element){
            return mid;
        }else if( array[mid] > element ){
            return search(array, left, mid-1, element);
        }else{
            return search(array, mid+1, right, element);
        }
    }

    return -1;
}

int main(){
    int length = 9000;
    int *searchroom = new int[length];
    fillArray(searchroom, length);
    int toSearch = getRandomInt(length);

    int index = search(searchroom, 0, length-1, toSearch);

    std::cout << "Element to search found at Index: " << index << "\n";

    return 0;
}