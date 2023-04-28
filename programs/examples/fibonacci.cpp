#include "iostream"
"x86_64-pc-linux-gnu/bits/c++config.h"
"bits/istream.tcc"
"bits/istream.tcc"
"bits/istream.tcc"
"x86_64-pc-linux-gnu/bits/c++config.h"
"bits/stringfwd.h"
"bits/postypes.h"
"bits/istream.tcc"
"x86_64-pc-linux-gnu/bits/c++config.h"
"bits/exception.h"
"bits/exception_ptr.h"
"bits/nested_exception.h"
"bits/char_traits.h"
"bits/localefwd.h"
"bits/ios_base.h"
"bits/istream.tcc"
"x86_64-pc-linux-gnu/bits/c++config.h"
"bits/istream.tcc"
"bits/localefwd.h"
"bits/ios_base.h"
"bits/cpp_type_traits.h"
"ext/type_traits.h"
"bits/streambuf.tcc"
"bits/basic_ios.h"
"bits/ostream_insert.h"
"bits/ostream.tcc"
"bits/istream.tcc"
"bits/istream.tcc"
"bits/istream.tcc"
"bits/istream.tcc"
#include "../helper/randomFiller.cpp"

int main(){
    int max = 1024;
    int a = 1;
    int b = 1;

    while(a < max){
        std::cout << a << ", ";
        int c = a + b;
        a = b;
        b = c;
    }

    return 0;
}