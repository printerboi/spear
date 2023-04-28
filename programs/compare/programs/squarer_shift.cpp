#include <iostream>
#include "../../helper/randomFiller.cpp"


int main(){
    int n = 1000;
    int elements[n];

    fillArrayRandom(elements, n, n*4);

    int before = elements[0];

    for(int i=0; i < n; i++){
        elements[i] = elements[i] + elements[i];
    }

    return 0;
}