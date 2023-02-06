
int main(){
    int a = 10;
    if(a < 50){
        for (int i = 0; i < 500; ++i) {
            if(a+i < 40){
                for (int j = 0; j < 400; ++j) {
                    a = a /2;
                }
            }else{
                for (int j = 0; j < 400; ++j) {
                    a = a * 2;
                }
            }
        }
    }else{
        for (int i = 0; i < 500; ++i) {
            a--;
        }
    }

    return 0;
}