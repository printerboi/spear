#include "vector"
#include "algorithm"
#include <iostream>

int main(){
    int datapoints = 1000;
    std::vector<int> elements(datapoints);

    int maxval = 1000;
    int n = 0;
    std::generate(elements.begin(), elements.end(), [&n, &maxval]{ return (n++ % maxval); });

    int sum = 0;
    int count = 0;
    for(int i = 0; i < datapoints; i++){
        sum = sum + elements[i];
        count++;
    }

    int mean = sum / count;

    std::cout << mean << std::endl;

    return 0;
}