#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "../sds_daisyChain.h"
#include "../sds_daisyChain_extract.h"

#define ITER_NUM 10000
#define CHAIN_LEN 1

void randomString(unsigned char message[]){
    int i, r;
    for (i=0;i<8;i++){
        r = rand();
        memcpy(message+i*sizeof(int),&r,sizeof(int));
    }
}


int main (void){
    clock_t start, end;
    double cpu_time_extract=0;


    SST sst, sstcpy0, sstcpy1, out_sst;
    uint256 m, m0,m1;
    VSTclassic vst;
    Sigmaclassic sigma, sigma0,sigma1;


    srand(time(NULL));
    sds_daisyChain_classic_sot_keygen(&sst,&vst);

    for(size_t i=0;i<ITER_NUM;i++){

        // Fork state
        assign256(&(sstcpy0.sk), sst.sk);
        assign256(&(sstcpy1.sk), sst.sk);

        randomString(m0.as_uchar);
        randomString(m1.as_uchar);

        sds_daisyChain_classic_sot_sig(&sigma0, &sstcpy0, m0);
        sds_daisyChain_classic_sot_sig(&sigma1, &sstcpy1, m1);

        start = end = 0;
        start = clock();
        sds_daisyChain_classic_sot_extract(&out_sst, vst, m0, sigma0, m1, sigma1);
        end = clock();
        cpu_time_extract += ((double) (end*1000000 - start*1000000));

        assert(eq256(out_sst.sk, sst.sk));

        // Move to the next state
        randomString(m.as_uchar);
        sds_daisyChain_classic_sot_sig(&sigma, &sst, m);
        sds_daisyChain_classic_sot_ver(&vst, m, sigma);

    }

    printf("--------------------EXTRACT SDS DAISYCHAIN for CLASSIC SOT------------------------\n");
    printf("EXTRACT Classic SDS DaisyChain time in microseconds per %d calls: %lf\n", ITER_NUM, cpu_time_extract/CLOCKS_PER_SEC);
    cpu_time_extract = cpu_time_extract/ITER_NUM;
    printf("EXTRACT Classic SDS DaisyChain time in microseconds per call: %lf\n", cpu_time_extract/CLOCKS_PER_SEC);
    printf("--------------------------------------------\n");

    return 0;
}



