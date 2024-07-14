#include <assert.h>

#include "sot_extract.h"


static void core_extract
    (uint256 *out_key,\
    const uint256 m0, const uint256 sigma0[HALF_KEY_NUM],\
    const uint256 m1, const uint256 sigma1[HALF_KEY_NUM]){
  int i;
  uint8 b, cnt;
  cnt = b = 0;

  for (i=0;i<32 && !b;i++) {
    b = m0.as_uint8[i]^m1.as_uint8[i];
    if (b) while(!((b>>cnt)&1)) cnt ++;
  }
  if (i==32) assert("Could not extract SOT key" && 0);
  xor256(out_key,sigma0[(i-1)*8+cnt],sigma1[(i-1)*8+cnt]);
}

/********* CLASSICAL SOT *********/
// We deviate from the original paper syntax and drop vk which has no reason to
// exist between inputs
// The theoretical paper defines extract with five inputs and one output
void classic_sot_extract
    (uint256 *out_key,\
    const uint256 m0, const uint256 sigma0[HALF_KEY_NUM],\
    const uint256 m1, const uint256 sigma1[HALF_KEY_NUM]){

  core_extract(out_key, m0, sigma0, m1, sigma1);
}

/********* SPACE OPTIMIZING SOT *********/
// We deviate from the original paper syntax and drop vk which has no reason to
// exist between inputs
// The theoretical paper defines extract with five inputs and one output
void optimized_sot_extract
    (uint256 *out_key,\
    const uint256 m0, const uint256 sigma0[2][HALF_KEY_NUM],\
    const uint256 m1, const uint256 sigma1[2][HALF_KEY_NUM]){

  core_extract(out_key, m0, sigma0[0], m1, sigma1[0]);
}
