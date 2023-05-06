#include <iostream>
#include <ctime>
#include<numeric>
#include <vector>
#include "algorithm"
#include "../helper/randomFiller.cpp"

int main(){

    int datapoints = 10000000;
    std::vector<int> a_vector(datapoints);
    std::vector<int> b_vector(datapoints);

    int maxval = 15;
    int n = 0;
    std::generate(a_vector.begin(), a_vector.end(), [&n, &maxval]{ return (n++ % maxval); });
    std::generate(b_vector.begin(), b_vector.end(), [&n, &maxval]{ return (n++ % maxval); });


    uint64_t dot_product = std::inner_product(a_vector.begin(), a_vector.end(), b_vector.begin(), 0);

    std::cout << "Operation took " << std::clock() << " Clock cycles" << "\n";
    std::cout << "The dot product of the two random vectors is " << dot_product << "\n";

}