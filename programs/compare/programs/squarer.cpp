#include <iostream>
#include "algorithm"
#include "vector"

int main(){
    int datapoints = 1000;
    std::vector<int> elements(datapoints);

    int maxval = 100;
    int n = 0;
    std::generate(elements.begin(), elements.end(), [&n, &maxval]{ return (n++ % maxval); });

    for(int i=0; i < datapoints; i++){
        elements[i] = elements[i] * elements[i];
    }

    std::cout << elements[datapoints-1] << std::endl;

    return 0;
}