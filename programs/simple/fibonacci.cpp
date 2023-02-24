#include "iostream"

int main(){
    int max = 1024;
    int a = 1;
    int b = 1;

    while(a < max){
        std::cout << a << ", ";
        int c = a + b;
        a = b;
        b = c;
    }

    return 0;
}