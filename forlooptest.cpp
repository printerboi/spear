//
// Created by max on 13.01.23.
//

int main(){
    int a = 0;
    for (int i = 0; i < 1; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int o = 0; o < 3; ++o) {
                if ( i%2 == 0 ){
                    int b = a;
                }else{
                    a--;
                }

                if(i%2 == 1){
                    a = a/2;
                }
            }
        }
    }

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 5; ++j) {
            for (int o = 0; o < 6; ++o) {
                a++;
                int c = 500;
                a = a + c;
            }
        }
    }
    int b = a;


    return b;
}