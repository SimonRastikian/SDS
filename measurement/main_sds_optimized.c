#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "../sds.h"

#define ITER_NUM 100
#define CHAIN_LEN 100

void randomString(unsigned char message[]){
  int i, r;
  for (i=0;i<8;i++){
    r = rand();
    memcpy(message+i*sizeof(int),&r,sizeof(int));
  }
}

int main (void){
    clock_t start, end;

    double cpu_time_keygen=0;
    double cpu_time_sig=0;
    double cpu_time_ver=0;

    SST sst;
    uint256 m[ITER_NUM*CHAIN_LEN];
    VSToptimized vst;
    uint256 sigma[2][HALF_KEY_NUM];


    srand(time(NULL));
    allocateVSToptimized(&vst,CHAIN_LEN);

    for(size_t i=0;i<ITER_NUM*CHAIN_LEN; i++) randomString(m[i].as_uchar);


    for (size_t i=0; i<ITER_NUM; i++){
        start = end = 0;
        start = clock();
        sds_optimized_sot_keygen(&sst,&vst, CHAIN_LEN);
        end = clock();
        cpu_time_keygen += ((double) (end*1000000 - start*1000000));

        for (size_t j=0; j<CHAIN_LEN; j++){
          start = end = 0;
          start = clock();
          sds_optimized_sot_sig (sigma, &sst, m[i*ITER_NUM+j]);
          end = clock();
          cpu_time_sig += ((double) (end*1000000 - start*1000000));

          start = end = 0;
          start = clock();
          sds_optimized_sot_ver(&vst, m[i*ITER_NUM+j], (const uint256 (*) [HALF_KEY_NUM]) sigma);
          end = clock();
          cpu_time_ver += ((double) (end*1000000 - start*1000000));
        }
    }

    freeVSToptimized(&vst);

    printf("--------------------SDS for OPTIMIZED SOT------------------------\n");

    printf("Optimized SDS keygen time in microseconds per %d calls for %d messages each: %lf\n", ITER_NUM,CHAIN_LEN,cpu_time_keygen/CLOCKS_PER_SEC);
    printf("Optimized SDS sig time in microseconds per %d calls: %lf\n", ITER_NUM*CHAIN_LEN,cpu_time_sig/CLOCKS_PER_SEC);
    printf("Optimized SDS ver time in microseconds per %d calls: %lf\n", ITER_NUM*CHAIN_LEN,cpu_time_ver/CLOCKS_PER_SEC);

    cpu_time_keygen = cpu_time_keygen/ITER_NUM;
    cpu_time_sig = cpu_time_sig/(ITER_NUM*CHAIN_LEN);
    cpu_time_ver = cpu_time_ver/(ITER_NUM*CHAIN_LEN);

    printf("Optimized SDS keygen time in microseconds per call: %lf\n", cpu_time_keygen/CLOCKS_PER_SEC);
    printf("Optimized SDS sig time in microseconds per call: %lf\n", cpu_time_sig/CLOCKS_PER_SEC);
    printf("Optimized SDS ver time in microseconds per call: %lf\n", cpu_time_ver/CLOCKS_PER_SEC);

    printf("--------------------------------------------\n");

    return 0;
}
