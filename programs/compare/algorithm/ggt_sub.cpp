#include <iostream>
#include "../../helper/randomFiller.cpp"

int main(){
    int a_init = getRandomInt(3333);
    int b_init = getRandomInt(9000);

    int a = a_init;
    int b = b_init;

    while(a != b){
        if(a > b){
            a = a - b;
        }else{
            b = b - a;
        }
    }

    std::cout << "ggt of " << a_init << " and " << b_init << " is " << a << "\n";
}
