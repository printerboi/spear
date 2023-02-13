
int main(){
    int a = 10;
    if(a < 50){
        for (int i = 0; i < 500; ++i) {
            if(i%2 == 0){
                a++;
            }
        }
    }else{
        a--;
    }

    return 0;
}