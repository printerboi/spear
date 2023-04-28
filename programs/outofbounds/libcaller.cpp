#include <iostream>
#include <ctime>
#include<numeric>
#include <vector>
#include "../helper/randomFiller.cpp"

int main(){

    int datapoints = 10000000;
    int maximalValue = 100;
    std::vector<int> a_vector {};
    std::vector<int> b_vector {};

    fillVectorRandom(&a_vector, datapoints, maximalValue);
    fillVectorRandom(&b_vector, datapoints, maximalValue);

    uint64_t dot_product = std::inner_product(a_vector.begin(), a_vector.end(), b_vector.begin(), 0);

    std::cout << "Operation took " << std::clock() << " Clock cycles" << "\n";
    std::cout << "The dot product of the two random vectors is " << dot_product << "\n";

}