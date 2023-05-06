#include <iostream>
#include "../../helper/randomFiller.cpp"

int main(){
    uint64_t a_init = 2147483647;
    uint64_t b_init = 2;

    uint64_t a = a_init;
    uint64_t b = b_init;

    while(a != b){
        if(a > b){
            a = a - b;
        }else{
            b = b - a;
        }
    }

    std::cout << "ggt of " << a_init << " and " << b_init << " is " << a << "\n";
}
