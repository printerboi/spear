#include "iostream"

long fac(int a){
    long facvalue = 1;
    for(int i = a; i > 0; i--){
        facvalue = facvalue * i;
    }

    return facvalue;
}

int main(){
    int number = 15;

    std::cout << fac(number) << "\n";
}