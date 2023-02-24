#include <vector>
#include "math.h"
#include "string"
#include "iostream"
#include <sstream>


int ggt(int a, int b){
    int rest = a % b;
    a = b;
    b = rest;

    while(rest != 0){
        rest = a % b;
        a = b;
        b = rest;
    }

    return a;
}

double findPubKey(double phi){
    double ggtCarrier = 0.0;
    double e = 7;

    while(e < phi){
        ggtCarrier = ggt(e, phi);
        if(ggtCarrier == 1){
            break;
        }else{
            e++;
        }
    }

    return e;
}

int main(){
    double p = 11;
    double q = 13;

    double n = p * q;
    double phi = (p-1) * (q-1);

    double pubkey = findPubKey(phi);

    double privkeypred = 1/pubkey;
    double privkey = fmod(privkeypred, phi);

    std::string message = "Hello world";

    double ciphertext = pow((long) message.c_str(), pubkey);
    double cleartext = pow(ciphertext, privkey);

    std::cout << message.c_str() << "\n";
    std::cout << (long) message.c_str() << "\n";
    std::cout << ciphertext << "\n";
    char * const oss = reinterpret_cast<char * const>((long) round(cleartext));
    std::cout << oss << "\n";

    return 0;
}