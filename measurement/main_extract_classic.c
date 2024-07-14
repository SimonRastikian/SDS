#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "../sds.h"
#include "../sds_extract.h"

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

    SST sst, sstcpy0, sstcpy1;
    uint256 m0[ITER_NUM*CHAIN_LEN], m1[ITER_NUM*CHAIN_LEN];
    VSTclassic vst;
    uint256 sigma0[HALF_KEY_NUM],sigma1[HALF_KEY_NUM];


    srand(time(NULL));
    allocateVSTclassic(&vst,CHAIN_LEN);
    for(size_t i=0;i<ITER_NUM*CHAIN_LEN; i++) {
        randomString(m0[i].as_uchar);
        randomString(m1[i].as_uchar);
    }


    for (size_t i=0;i<ITER_NUM;i++){
        // Fork state
        sstcpy0.cnt = sstcpy1.cnt = vst.cnt = 0;
        sstcpy0.max_epoch = sstcpy1.max_epoch = CHAIN_LEN;
        sds_classic_sot_keygen(&sst,&vst,CHAIN_LEN);

        assign256(&(sstcpy0.sk), sst.sk);
        assign256(&(sstcpy1.sk), sst.sk);

        sds_classic_sot_sig (sigma0, &sst, m0[i]);
        sds_classic_sot_sig (sigma1, &sstcpy1, m1[i]);

        start = end = 0;
        start = clock();
        sds_classic_sot_extract(&sstcpy1, vst, m0[i], sigma0, m1[i], sigma1);
        end = clock();
        cpu_time_extract += ((double) (end*1000000 - start*1000000));

        assert(eq256(sstcpy1.sk, sstcpy0.sk));
    }
    freeVSTclassic(&vst);


    printf("--------------------EXTRACT SDS for CLASSIC SOT------------------------\n");
    printf("EXTRACT Classic SDS time in microseconds per %d calls: %lf\n", ITER_NUM, cpu_time_extract/CLOCKS_PER_SEC);
    cpu_time_extract = cpu_time_extract/ITER_NUM;
    printf("EXTRACT Classic SDS time in microseconds per call: %lf\n", cpu_time_extract/CLOCKS_PER_SEC);
    printf("--------------------------------------------\n");

    return 0;
}



