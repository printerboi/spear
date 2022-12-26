#include "src/include/Bencher/Bencher.h"

int main(){
    Bencher B = Bencher(10, 100);
    B.benchmark();

    return 0;
}