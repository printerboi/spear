#include "iostream"
#include "../../helper/randomFiller.cpp"

int fib(int max){
    long numbers[max + 2];

    numbers[0] = 0;
    numbers[1] = 1;

    for(int i = 2; i <= max; i++){
        numbers[i] = numbers[i-1] + numbers[i-2];
    }

    return numbers[max];
}

int main(){
    //int randomNumber = getRandomInt(40);
    int randomNumber = 35;
    std::cout << "The " << randomNumber << "-th number is: " << fib(randomNumber) << "\n";
}