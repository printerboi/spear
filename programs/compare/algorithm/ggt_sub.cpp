#include <iostream>
#include "../../helper/randomFiller.cpp"

int main(){
    int a_init = getRandomInt(3333);
    int b_init = getRandomInt(9000);

    int a = a_init;
    int b = b_init;

    while(b != 0){
        int t = b;
        b = a % b;
        a = t;
    }

    std::cout << "ggt of " << a_init << " and " << b_init << " is " << a << "\n";
}
