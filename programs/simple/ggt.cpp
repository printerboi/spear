#include "iostream"

int main(){
    int a = 24;
    int b = 36;

    int rest = a % b;
    a = b;
    b = rest;

    while(rest != 0){
        rest = a % b;
        a = b;
        b = rest;
    }

    std::cout << a << "\n";

    return 0;
}