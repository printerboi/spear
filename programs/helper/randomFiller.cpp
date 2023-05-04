//
// Created by maximiliank on 27.04.23.
//

#include <time.h>
#include <stdlib.h>
#include "vector"

void fillArrayRandom(int *array, int length, int maxval){
    srand(time(NULL));

    for(int i = 0; i < length; i++){
        array[i] = rand()%maxval;
    }
}

void fillVectorRandom(std::vector<int> *vec, int length, int maxval){
    srand(time(NULL));

    for(int i = 0; i < length; i++){
        vec->push_back(rand() % maxval);
    }
}

void fillArray(int *array, int length){
    srand(time(NULL));

    for(int i = 0; i < length; i++){
        array[i] = i;
    }
}

int getRandomInt(int maxval){
    srand(time(NULL));

    return rand()%maxval;
}
