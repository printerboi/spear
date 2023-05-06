#include <iostream>
#include <algorithm>
#include "vector"

int search(std::vector<int> array, int left, int right, int element){
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
    int length = 10000000;
    std::vector<int> searchroom(length);

    int n = 0;
    std::generate(searchroom.begin(), searchroom.end(), [&n]{ return n++; });
    int toSearch = length-1;

    int index = search(searchroom, 0, length-1, toSearch);

    std::cout << "Element to search found at Index: " << index << "\n";

    return 0;
}