#include <iostream>
#include "algorithm"
#include "vector"

int main(){
    int datapoints = 1000;
    std::vector<int> elements(datapoints);

    int maxval = 1000;
    int n = 0;
    std::generate(elements.begin(), elements.end(), [&n, &maxval]{ return (n++ % maxval); });

    int additionalwork = 3333;
    int half = additionalwork % 2;
    int quart = additionalwork % 4;
    int eight = additionalwork % 8;

    for(int i=0; i < datapoints; i++){
        elements[i] = elements[i] + elements[i];
    }

    elements[datapoints-1] = half+quart+eight;

    std::cout << elements[datapoints-1] << std::endl;

    return 0;
}