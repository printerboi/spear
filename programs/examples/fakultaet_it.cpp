#include "iostream"
#include "../../helper/randomFiller.cpp"

uint64_t fac(int a){
    uint64_t facvalue = 1;
    for(int i = a; i > 0; i--){
        facvalue = facvalue * i;
    }

    return facvalue;
}

int main(){
    int number = getRandomInt(20);

    std::cout << number << "! = " << fac(number) << "\n";
}