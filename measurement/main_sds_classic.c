#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#include "../sds.h"

#define ITER_NUM 100
#define CHAIN_LEN 100

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

    double time_data_keygen[ITER_NUM];
    double time_data_sig[ITER_NUM*CHAIN_LEN];
    double time_data_ver[ITER_NUM*CHAIN_LEN];

    SST sst;
    uint256 m[ITER_NUM*CHAIN_LEN];
    VSTclassic vst;
    uint256 sigma[HALF_KEY_NUM];


    srand(time(NULL));
    allocateVSTclassic(&vst,CHAIN_LEN);

    for(size_t i=0;i<ITER_NUM*CHAIN_LEN; i++) randomString(m[i].as_uchar);


    for (size_t i=0; i<ITER_NUM; i++){
        start = clock();
        sds_classic_sot_keygen(&sst,&vst, CHAIN_LEN);
        end = clock();
        time_data_keygen[i] = ((double) (end - start));

        for (size_t j=0; j<CHAIN_LEN; j++){
          start = end = 0;
          start = clock();
          sds_classic_sot_sig (sigma, &sst, m[i*ITER_NUM+j]);
          end = clock();
          time_data_sig[i*ITER_NUM+j] = ((double) (end - start));

          start = end = 0;
          start = clock();
          sds_classic_sot_ver(&vst, m[i*ITER_NUM+j], sigma);
          end = clock();
          time_data_ver[i*ITER_NUM+j] = ((double) (end - start));
          usleep(USLEEP);
        }
    }

    freeVSTclassic(&vst);


    printf("--------------------SDS for CLASSIC SOT------------------------\n");
    printf("Keygen-100\n");
    calculateStandardDeviation(ITER_NUM, time_data_keygen);
    printf("Sign\n");
    calculateStandardDeviation(ITER_NUM*CHAIN_LEN, time_data_sig);
    printf("Verify\n");
    calculateStandardDeviation(ITER_NUM*CHAIN_LEN, time_data_ver);
    printf("--------------------------------------------\n");

    return 0;
}
