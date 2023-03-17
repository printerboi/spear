
int adder(int *number){
    number = number + 10;
    return *number;
}

int main(){
    int a = 10;
    for(int i=0; i < 100; i+=50){
        a = adder(&a);
    }

    return 0;
}