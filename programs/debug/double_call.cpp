//
// Created by maximiliank on 08.03.23.
//

int add(int n, int val){
    int r = n + val;
    return r;
}

int subone(int n){
    int r = add(n, -1);
    return r;
}



int main(){
    int a = 5;
    add(a, 5);
    subone(10);

    return 0;
}