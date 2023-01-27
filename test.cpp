//
// Created by max on 13.01.23.
//

int main(){
    int a = 0;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 100; ++j) {
            for (int o = 0; o < 200; ++o) {
                a++;
            }
        }
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 33; ++j) {
            for (int o = 0; o < 333; ++o) {
                a++;
            }
        }
    }
    int b = a;


    return b;
}