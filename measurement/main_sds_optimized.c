#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#include "../sds.h"

#define ITER_NUM 100
#define CHAIN_LEN 100
#define USLEEP 20000

void randomString(unsigned char message[]){
  int i, r;
  for (i=0;i<8;i++){
    r = rand();
    memcpy(message+i*sizeof(int),&r,sizeof(int));
  }
}

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

int main (void){
    clock_t start, end;
    double time_data_keygen[ITER_NUM];
    double time_data_sig[ITER_NUM*CHAIN_LEN];
    double time_data_ver[ITER_NUM*CHAIN_LEN];


    SST sst;
    uint256 m[ITER_NUM*CHAIN_LEN];
    VSToptimized vst;
    uint256 sigma[2][HALF_KEY_NUM];


    srand(time(NULL));
    allocateVSToptimized(&vst,CHAIN_LEN);

    for(size_t i=0;i<ITER_NUM*CHAIN_LEN; i++) randomString(m[i].as_uchar);


    for (size_t i=0; i<ITER_NUM; i++){
        start = clock();
        sds_optimized_sot_keygen(&sst,&vst, CHAIN_LEN);
        end = clock();
        time_data_keygen[i] = ((double) (end - start));

        for (size_t j=0; j<CHAIN_LEN; j++){
          start = end = 0;
          start = clock();
          sds_optimized_sot_sig (sigma, &sst, m[i*ITER_NUM+j]);
          end = clock();
          time_data_sig[i*ITER_NUM+j] = ((double) (end - start));

          start = end = 0;
          start = clock();
          sds_optimized_sot_ver(&vst, m[i*ITER_NUM+j], (const uint256 (*) [HALF_KEY_NUM]) sigma);
          end = clock();
          time_data_ver[i*ITER_NUM+j] = ((double) (end - start));
          usleep(USLEEP);
        }
    }

    freeVSToptimized(&vst);

    printf("--------------------SDS for OPTIMIZED SOT------------------------\n");
    printf("Keygen-100");
    calculateStandardDeviation(ITER_NUM, time_data_keygen);
    printf("Sign\n");
    calculateStandardDeviation(ITER_NUM*CHAIN_LEN, time_data_sig);
    printf("Verify\n");
    calculateStandardDeviation(ITER_NUM*CHAIN_LEN, time_data_ver);
    printf("--------------------------------------------\n");

    return 0;
}
