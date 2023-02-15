void E(int a){
    a++;
    E(a);
}

void C(int a){
    a=a*a;
    E(a);
}

void B(){
    int a;
    a = 100;
    C(a);
}

void D(int a){
    a--;
}


void A(){
    int a;
    a++;
    D(a);
    E(a);
}

int main(){

    A();
    B();

    return 0;
}