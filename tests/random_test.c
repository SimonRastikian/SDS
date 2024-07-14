#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "../random.h"

int main(void){
    unsigned char str[25][32] = {0};
    printf("Running Randomness tests...\n");

    for (size_t i = 0; i<25; i++) random_32_bytes(str[i]);
    for (size_t i = 0; i<25; i++) 
        for (size_t j = i+1; j<25; j++)
            assert(memcmp(str[i],str[j],32)!=0);
    printf("\tPassed!\n");
    return 0;
}