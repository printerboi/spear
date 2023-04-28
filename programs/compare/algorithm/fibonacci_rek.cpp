#include "iostream"
#include "../../helper/randomFiller.cpp"

int fib(int n){
    if(n == 0){
        return 0;
    }else if(n == 1){
        return 1;
    }else{
        return fib(n-1) + fib(n-2);
    }
}

int main(){
    //int randomNumber = getRandomInt(40);
    int randomNumber = 35;
    std::cout << "The " << randomNumber << "-th number is: " << fib(randomNumber) << "\n";

}