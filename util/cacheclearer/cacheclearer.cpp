#include <string.h>
#include <stdlib.h>
#include <time.h>

int main(){

    // Global variables.
    const size_t bigger_than_cachesize = 20 * 1024 * 1024;
    long *p = new long[bigger_than_cachesize];

    for(int i = 0; i < bigger_than_cachesize; i++)
    {
        p[i] = rand();
    }

    return 0;
}