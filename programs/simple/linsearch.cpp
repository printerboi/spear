#include <iostream>
#include <algorithm>
#include "vector"

int search(std::vector<int> array, int length, int element){
    for(int i=0; i < length; i++){
        if(array[i] == element){
            return i;
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

    int index = search(searchroom, length, toSearch);
    std::cout << "Element to search found at Index: " << index << "\n";

    return 0;
}