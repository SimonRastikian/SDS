#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "../sds.h"

#define SEED_TEST 100
#define CHAIN_LEN 10

void randomString(unsigned char message[]){
  int i, r;
  for (i=0;i<8;i++){
    r = rand();
    memcpy(message+i*sizeof(int),&r,sizeof(int));
  }
}

void test_classic_sds(){
  int i;
  SST sst;
  uint256 m;
  VSTclassic vst;
  uint256 sigma[HALF_KEY_NUM];
  allocateVSTclassic(&vst,CHAIN_LEN);

  sds_classic_sot_keygen(&sst,&vst, CHAIN_LEN);
  for (i=0;i<CHAIN_LEN;i++){
    randomString(m.as_uchar);
    sds_classic_sot_sig (sigma, &sst, m);
    assert(sds_classic_sot_ver(&vst, m, sigma));
  }
  freeVSTclassic(&vst);
}

void test_optimized_sds(){
  int i;
  SST sst;
  VSToptimized vst;
  uint256 sigma[2][HALF_KEY_NUM];
  uint256 m;
  allocateVSToptimized(&vst,CHAIN_LEN);

  sds_optimized_sot_keygen(&sst, &vst, CHAIN_LEN);
  for (i=0;i<CHAIN_LEN;i++){
    randomString(m.as_uchar);
    sds_optimized_sot_sig(sigma, &sst, m);
    assert(sds_optimized_sot_ver(&vst, m, (const uint256 (*) [HALF_KEY_NUM]) sigma));
  }
  freeVSToptimized(&vst);
}

int main (void){
  int i;

  printf("Running SDS tests...\n");
  srand(time(NULL));

  for(i=0;i<SEED_TEST;i++){
    test_classic_sds();
    test_optimized_sds();
  }

  printf("\tPassed!\n");
  return 0;
}
