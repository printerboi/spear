#include <iostream>

int main()
{
    int length = 1000000;
    int *elements = (int *) malloc(length * sizeof (int));

    elements[0] = 42;
    elements[length-1] = 42;

    std::cout << "First Element at address " << elements << "\n";
    std::cout << "Last Element at address " << (elements + length-1) << "\n";
}