#include <iostream>
#include "vector"

int main()
{
    //allocate 2G of char data...
    int length = 2000 * 1024 * 1024;
    std::vector<char> elements(length);

    //std::fill(elements.begin(), elements.end(), 'c');

    std::cout << "First Element at address " << elements[0] << "\n";
    std::cout << "Last Element at address " << elements[length-1] << "\n";
}