#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "../sds.h"
#include "../sds_extract.h"


#define SEED_TEST 100
#define CHAIN_LEN 10

void randomString(unsigned char message[]);

void test_classic_Xsds(){
  int r=0;
  SST sst, sstcpy0, sstcpy1, out_sst;
  uint256 m0,m1;
  VSTclassic vst;
  uint256 sigma0[HALF_KEY_NUM],sigma1[HALF_KEY_NUM];
  allocateVSTclassic(&vst,CHAIN_LEN);


  while(!r) r=rand()%CHAIN_LEN;
  sds_classic_sot_keygen(&sst,&vst,CHAIN_LEN);


  for (size_t i=0;i<r;i++){
    // Fork state
    if(i==r-1){
      sstcpy0.cnt = sst.cnt;
      sstcpy0.max_epoch = sst.max_epoch;
      sstcpy1.cnt = sst.cnt;
      sstcpy1.max_epoch = sst.max_epoch;
      assign256(&(sstcpy0.sk), sst.sk);
      assign256(&(sstcpy1.sk), sst.sk);
      vst.cnt = sst.cnt;
    }
    randomString(m0.as_uchar);

    sds_classic_sot_sig (sigma0, &sst, m0);
  }

  randomString(m1.as_uchar);
  sds_classic_sot_sig (sigma1, &sstcpy1, m1);

  sds_classic_sot_extract(&out_sst, vst, m0, sigma0, m1, sigma1);
  assert(out_sst.cnt==sstcpy0.cnt);
  assert(eq256(out_sst.sk, sstcpy0.sk));
  freeVSTclassic(&vst);
}

void test_optimized_Xsds(){
  int r=0;
  SST sst, sstcpy0, sstcpy1, out_sst;
  uint256 m0, m1;
  VSToptimized vst;
  uint256 sigma0[2][HALF_KEY_NUM], sigma1[2][HALF_KEY_NUM];
  allocateVSToptimized(&vst,CHAIN_LEN);

  while(!r) r=rand()%CHAIN_LEN;
  sds_optimized_sot_keygen(&sst, &vst, CHAIN_LEN);

  for (size_t i=0;i<r;i++){
    // Fork state
    if(i==r-1){
      sstcpy0.cnt = sst.cnt;
      sstcpy0.max_epoch = sst.max_epoch;
      sstcpy1.cnt = sst.cnt;
      sstcpy1.max_epoch = sst.max_epoch;
      assign256(&(sstcpy0.sk), sst.sk);
      assign256(&(sstcpy1.sk), sst.sk);
      vst.cnt = sst.cnt;
    }
    randomString(m0.as_uchar);
    sds_optimized_sot_sig(sigma0, &sst, m0);
  }
  randomString(m1.as_uchar);
  sds_optimized_sot_sig(sigma1, &sstcpy1, m1);

  sds_optimized_sot_extract(&out_sst, vst, m0, (const uint256 (*)[HALF_KEY_NUM]) sigma0, m1, (const uint256 (*)[HALF_KEY_NUM]) sigma1);
  assert(out_sst.cnt==sstcpy0.cnt);
  assert(eq256(out_sst.sk, sstcpy0.sk));
  freeVSToptimized(&vst);
}


int main (void){
  int i;
  printf("Running SDS Extract tests...\n");

  srand(time(NULL));
  for(i=0;i<SEED_TEST;i++){
    test_classic_Xsds();
    test_optimized_Xsds();
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

