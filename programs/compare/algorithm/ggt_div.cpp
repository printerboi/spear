#include <iostream>
#include "../../helper/randomFiller.cpp"

int main(){
    uint64_t a_init = 2147483647;
    uint64_t b_init = 2;

    uint64_t a = a_init;
    uint64_t b = b_init;

    while(b != 0){
        uint64_t t = b;
        b = a % b;
        a = t;
    }

    std::cout << "ggt of " << a_init << " and " << b_init << " is " << a << "\n";
}
