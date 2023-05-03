#include "iostream"
#include "../../helper/randomFiller.cpp"

uint64_t fac(int a){
    if(a == 0){
        return 1;
    }else{
        return a * fac(a-1);
    }
}

int main(){
    uint64_t number = getRandomInt(20);

    std::cout << number << "! = " << fac(number) << "\n";
}