#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#include "../sds_daisyChain.h"

#define ITER_NUM 10000
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
    double time_data_sig[ITER_NUM];
    double time_data_ver[ITER_NUM];

    SST sst;
    uint256 m;
    VSTclassic vst;
    Sigmaclassic sigma;

    srand(time(NULL));

    for (size_t i=0; i<ITER_NUM; i++){
      randomString(&m.as_uchar);
      start = end = 0;
      start = clock();
      sds_daisyChain_classic_sot_keygen(&sst,&vst);
      end = clock();
      time_data_keygen[i] = ((double) (end - start));

      start = end = 0;
      start = clock();
      sds_daisyChain_classic_sot_sig(&sigma, &sst, m);
      end = clock();
      time_data_sig[i] = ((double) (end - start));

      start = end = 0;
      start = clock();
      sds_daisyChain_classic_sot_ver(&vst, m, sigma);
      end = clock();
      time_data_ver[i] = ((double) (end - start));
      usleep(USLEEP);
    }


    printf("--------------------SDS DaisyChain for CLASSIC SOT------------------------\n");
    printf("Keygen\n");
    calculateStandardDeviation(ITER_NUM, time_data_keygen);
    printf("Sign\n");
    calculateStandardDeviation(ITER_NUM, time_data_sig);
    printf("Verify\n");
    calculateStandardDeviation(ITER_NUM, time_data_ver);
    printf("--------------------------------------------\n");

    return 0;
}
