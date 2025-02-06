#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

#include "../sds_daisyChain.h"
#include "../sds_daisyChain_extract.h"

#define ITER_NUM 10000
#define CHAIN_LEN 1
#define USLEEP 20000

void calculateStandardDeviation(int N, double* data) {
    double sum = 0;
    double squared_diffs = 0;
    double mean=0;
    double std_dev=0;
    int cnt = 0;
    for (int i = 0; i < N; i++){
        if (data[i] != 0) {
            cnt += 1;
            sum += data[i];
        }
    }
    mean = sum / cnt;

    for (int i = 0; i < N; i++){
        if (data[i] != 0) {
            squared_diffs += pow((data[i] - mean), 2);
        }
    }
    std_dev = sqrt(squared_diffs / (cnt-1));
    printf("Time in microseconds per call: [mean %lf]\t [standard deviation %lf]\n", mean, std_dev);
    printf("Total number of calls: %d\n", cnt);

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

        start = clock();
        sds_daisyChain_classic_sot_extract(&out_sst, vst, m0, sigma0, m1, sigma1);
        end = clock();
        time_data[i] = ((double) (end- start));

        assert(eq256(out_sst.sk, sst.sk));

        // Move to the next state
        randomString(m.as_uchar);
        sds_daisyChain_classic_sot_sig(&sigma, &sst, m);
        sds_daisyChain_classic_sot_ver(&vst, m, sigma);
        usleep(USLEEP);
    }

    printf("--------------------EXTRACT SDS DAISYCHAIN for CLASSIC SOT------------------------\n");
    calculateStandardDeviation(ITER_NUM, time_data);
    printf("--------------------------------------------\n");

    return 0;
}



