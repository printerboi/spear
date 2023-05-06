#include <iostream>
#include "vector"

int main()
{
    //allocate 1kB of char data...
    int length = 1024;
    std::vector<char> elements(length);

    //std::fill(elements.begin(), elements.end(), 1);

    std::cout << "First Element at address " << elements[0] << "\n";
    std::cout << "Last Element at address " << elements[length-1] << "\n";
}