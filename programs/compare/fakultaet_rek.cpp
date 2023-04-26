#include "iostream"


long fac(int a){
    if(a == 0){
        return 1;
    }else{
        return a * fac(a-1);
    }
}

int main(){
    int number = 15;

    std::cout << fac(number) << "\n";
}