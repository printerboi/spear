#include <iostream>

int main(){

    int a = 42;
    int b = 133;
    int c = 999;

    double mean = (a+b+c)/3;

    double err_a = (a-mean);
    double err_b = (b-mean);
    double err_c = (c-mean);

    double variance = (err_a*err_a + err_b*err_b + err_c*err_c)/(2);

    std::cout << mean << "\n";
    std::cout << "Die Varianz von " << a << ", " << b << ", " << c  << " ist " << variance << "\n";

    return 0;
}