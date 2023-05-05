#include <iostream>
#include "../helper/randomFiller.cpp"

int add(int a, int b){
    return a + b;
}

int main(){
    int a = 42;
    int b = 1111;

    int result = add(a, b);

    std::cout << result << "\n";
}