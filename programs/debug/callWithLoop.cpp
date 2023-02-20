
int adder(int *number){
    number = number + 10;
    return *number;
}

int main(){
    int a = 10;
    for(int i=0; i < 10; i++){
        a = adder(&a);
    }

    return 0;
}