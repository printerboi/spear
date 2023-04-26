#include <iostream>

int add(int a, int b){
    return a + b;
}

int main(){
    int a = 1025;
    int b = 3000;

    std::cout << add(a, b) << "\n";
}