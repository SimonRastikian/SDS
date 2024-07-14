#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "../sds_daisyChain.h"
#include "../sds_daisyChain_extract.h"


#define SEED_TEST 100
#define CHAIN_LEN 10

void randomString(unsigned char message[]);

void test_classic_Xsds_daisyChain(){
  SST sst, sstcpy0, sstcpy1, out_sst;
  uint256 m, m0,m1;
  VSTclassic vst;
  Sigmaclassic sigma, sigma0,sigma1;

  sds_daisyChain_classic_sot_keygen(&sst,&vst);

  for (size_t i=0;i<CHAIN_LEN;i++){
    if (i<CHAIN_LEN -1){
        randomString(m.as_uchar);
        // update secret state
        sds_daisyChain_classic_sot_sig(&sigma, &sst, m);
        // update corresponding verification state
        sds_daisyChain_classic_sot_ver(&vst, m, sigma);
    }
    else {
        // Prepare fork state
        assign256(&(sstcpy0.sk), sst.sk);
        assign256(&(sstcpy1.sk), sst.sk);
    }
  }
  // Prepare Fork state
  randomString(m0.as_uchar);
  sds_daisyChain_classic_sot_sig(&sigma0, &sstcpy0, m0);
  randomString(m1.as_uchar);
  sds_daisyChain_classic_sot_sig(&sigma1, &sstcpy1, m1);

  sds_daisyChain_classic_sot_extract(&out_sst, vst, m0, sigma0, m1, sigma1);
  assert(eq256(out_sst.sk, sst.sk));
}

void test_optimized_Xsds_daisyChain(){
  SST sst, sstcpy0, sstcpy1, out_sst;
  uint256 m, m0, m1;
  VSToptimized vst;
  SigmaOptimized sigma, sigma0, sigma1;

  sds_daisyChain_optimized_sot_keygen(&sst, &vst);

  for (size_t i=0;i<CHAIN_LEN;i++){
    if (i<CHAIN_LEN -1){
        randomString(m.as_uchar);
        // update secret state
        sds_daisyChain_optimized_sot_sig(&sigma, &sst, m);
        // update corresponding verification state
        sds_daisyChain_optimized_sot_ver(&vst, m, sigma);
    }
    else {
        // Prepare fork state
        assign256(&(sstcpy0.sk), sst.sk);
        assign256(&(sstcpy1.sk), sst.sk);
    }
  }
  // Prepare Fork state
  randomString(m0.as_uchar);
  sds_daisyChain_optimized_sot_sig(&sigma0, &sstcpy0, m0);
  randomString(m1.as_uchar);
  sds_daisyChain_optimized_sot_sig(&sigma1, &sstcpy1, m1);

  sds_daisyChain_optimized_sot_extract(&out_sst, vst, m0, sigma0, m1, sigma1);

  assert(eq256(out_sst.sk, sst.sk));
}


int main (void){
  int i;
  printf("Running SDS DaisyChain Extract tests...\n");

  srand(time(NULL));
  for(i=0;i<SEED_TEST;i++){
    test_classic_Xsds_daisyChain();
   test_optimized_Xsds_daisyChain();
  }

  printf("\tPassed!\n");
  return 0;
}

void randomString(unsigned char message[]){
  int i, r;
  for (i=0;i<8;i++){
    r = rand();
    memcpy(message+i*sizeof(int),&r,sizeof(int));
  }
}

