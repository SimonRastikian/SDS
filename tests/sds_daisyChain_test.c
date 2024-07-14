#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "../sds_daisyChain.h"

#define SEED_TEST 1000

void randomString(unsigned char message[]){
  int i, r;
  for (i=0;i<8;i++){
    r = rand();
    memcpy(message+i*sizeof(int),&r,sizeof(int));
  }
}

void test_classic_sds_daisyChain(){
  SST sst;
  uint256 m;
  VSTclassic vst;
  Sigmaclassic sigma;

  sds_daisyChain_classic_sot_keygen(&sst,&vst);
  randomString(m.as_uchar);
  sds_daisyChain_classic_sot_sig(&sigma, &sst, m);
  assert(sds_daisyChain_classic_sot_ver(&vst, m, sigma));
}

void test_optimized_sds_daisyChain(){
  SST sst;
  uint256 m;
  VSToptimized vst;
  SigmaOptimized sigma;

  sds_daisyChain_optimized_sot_keygen(&sst,&vst);
  randomString(m.as_uchar);
  sds_daisyChain_optimized_sot_sig(&sigma, &sst, m);
  assert(sds_daisyChain_optimized_sot_ver(&vst, m, sigma));
}

int main (void){
  int i;

  printf("Running SDS Daisy Chain tests...\n");
  srand(time(NULL));

  for(i=0;i<SEED_TEST;i++){
    test_classic_sds_daisyChain();
    test_optimized_sds_daisyChain();
  }

  printf("\tPassed!\n");
  return 0;
}
