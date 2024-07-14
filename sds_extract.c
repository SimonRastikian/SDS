#include <stdio.h>
#include "sot_extract.h"
#include "sds_extract.h"

/********* SDS extract for CLASSICAL SOT *********/
void sds_classic_sot_extract
    (SST* sst, VSTclassic vst,\
    const uint256 m0, const uint256 sigma0[HALF_KEY_NUM],\
    const uint256 m1, const uint256 sigma1[HALF_KEY_NUM]){
  if (eq256(m0,m1)){
    printf("Refuse to extract: Input messages are similar\n");
    return;
  }
  if (!sds_classic_sot_ver(&vst, m0, sigma0)) {
    printf("Refuse to extract: First pair of message-signature is not valid\n");
    return;
  }
  vst.cnt--;
  if (!sds_classic_sot_ver(&vst, m1, sigma1)) {
    printf("Refuse to extract: Second pair of message-signature is not valid\n");
    return;
  }
  vst.cnt--;
  classic_sot_extract (&(sst->sk), m0, sigma0, m1, sigma1);
  sst->cnt = vst.cnt;
}

/********* SDS extract for OPTIMIZED SOT *********/
void sds_optimized_sot_extract
    (SST* sst, VSToptimized vst,\
    const uint256 m0, const uint256 sigma0[2][HALF_KEY_NUM],\
    const uint256 m1, const uint256 sigma1[2][HALF_KEY_NUM]){

  if (eq256(m0,m1)){
    printf("Refuse to extract: input messages are similar\n");
    return;
  }
  if (!sds_optimized_sot_ver(&vst, m0, sigma0)) {
    printf("Refuse to extract: First pair of message-signature is not valid\n");
    return;
  }
  vst.cnt--;
  if (!sds_optimized_sot_ver(&vst, m1, sigma1)) {
    printf("Refuse to extract: Second pair of message-signature is not valid\n");
    return;
  }
  vst.cnt--;
  optimized_sot_extract (&(sst->sk), m0, sigma0, m1, sigma1);
  sst->cnt = vst.cnt;
}
