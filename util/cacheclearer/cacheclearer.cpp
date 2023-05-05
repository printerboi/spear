#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

int main(){

    // Global variables.
    const size_t bigger_than_cachesize = 16 * 1024 * 1024;

    std::vector<char> clearCacheArray(bigger_than_cachesize);
    for (size_t i = 0; i < bigger_than_cachesize; ++i){
        clearCacheArray[i] = rand();
    }
    
    //for(int i = 0; i < bigger_than_cachesize; i++)
    //{
    //    p[i] = rand();
    //}

    return 0;
}
