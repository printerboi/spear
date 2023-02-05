
int main(){
    int a = 10;
    if(a < 50){
        for (int i = 0; i < 500; ++i) {
            for (int j = 0; j < 400; ++j) {
                a++;
            }
        }
    }else{
        for (int i = 0; i < 500; ++i) {
            a--;
        }
    }

    return 0;
}