#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

#include "../sds.h"
#include "../sds_extract.h"

#define ITER_NUM 10000
#define CHAIN_LEN 1

// if the data is extremely sparse or many calculated execution time is zero
// then you can increase the usleep time or potentially use clock_gettime
// instead of clock
#define USLEEP 0

void calculateStandardDeviation(int N, double* data) {
    double sum = 0;
    double squared_diffs = 0;
    double mean=0;
    double std_dev=0;
    for (int i = 0; i < N; i++)
            sum += data[i];
    mean = sum / N;

    for (int i = 0; i < N; i++){
        if (data[i] != 0) {
            squared_diffs += pow((data[i] - mean), 2);
        }
    }
    std_dev = sqrt(squared_diffs / (N-1));
    printf("Time in microseconds per call: [mean %lf]\t [standard deviation %lf]\n", mean, std_dev);
}

void randomString(unsigned char message[]){
    int i, r;
    for (i=0;i<8;i++){
        r = rand();
        memcpy(message+i*sizeof(int),&r,sizeof(int));
    }
}


int main (void){
    clock_t start, end;
    double time_data[ITER_NUM];

    SST sst, sstcpy0, sstcpy1;
    uint256 m0[ITER_NUM*CHAIN_LEN], m1[ITER_NUM*CHAIN_LEN];
    VSToptimized vst;
    uint256 sigma0[2][HALF_KEY_NUM],sigma1[2][HALF_KEY_NUM];


    srand(time(NULL));
    allocateVSToptimized(&vst,CHAIN_LEN);
    for(size_t i=0;i<ITER_NUM*CHAIN_LEN; i++) {
        randomString(m0[i].as_uchar);
        randomString(m1[i].as_uchar);
    }


    for (size_t i=0;i<ITER_NUM;i++){
        // Fork state
        sstcpy0.cnt = sstcpy1.cnt = vst.cnt = 0;
        sstcpy0.max_epoch = sstcpy1.max_epoch = CHAIN_LEN;
        sds_optimized_sot_keygen(&sst,&vst,CHAIN_LEN);

        assign256(&(sstcpy0.sk), sst.sk);
        assign256(&(sstcpy1.sk), sst.sk);

        sds_optimized_sot_sig (sigma0, &sst, m0[i]);
        sds_optimized_sot_sig (sigma1, &sstcpy1, m1[i]);

        start = end = 0;
        start = clock();
        sds_optimized_sot_extract(&sstcpy1, vst, m0[i], (const uint256 (*)[HALF_KEY_NUM]) sigma0, m1[i], (const uint256 (*)[HALF_KEY_NUM]) sigma1);
        end = clock();
        time_data[i] = ((double) (end - start));

        assert(eq256(sstcpy1.sk, sstcpy0.sk));
        usleep(USLEEP);

    }
    freeVSToptimized(&vst);

    printf("--------------------EXTRACT SDS for OPTIMIZED SOT------------------------\n");
    calculateStandardDeviation(ITER_NUM, time_data);
    printf("--------------------------------------------\n");

    return 0;
}