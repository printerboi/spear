#include <iostream>
#include "../helper/randomFiller.cpp"

int add(int a, int b){
    return a + b;
}

int main(){
    int a = getRandomInt(9000);
    int b = getRandomInt(9000);

    std::cout << add(a, b) << "\n";
}