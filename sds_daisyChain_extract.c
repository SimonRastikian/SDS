#include <stdio.h>
#include <string.h>
#include "hash.h"
#include "sot_extract.h"
#include "sds_daisyChain_extract.h"

/********* SDS extract for CLASSICAL SOT *********/
void sds_daisyChain_classic_sot_extract
    (SST* sst, VSTclassic vst,\
    const uint256 m0, const Sigmaclassic sigma0,\
    const uint256 m1, const Sigmaclassic sigma1){

  uint256 hash0, hash1;
  VSTclassic tmp_vst;
  memcpy(&tmp_vst, &vst, sizeof(VSTclassic));

  hash_uint256_uint256doublelist(&hash0, m0, (const uint256 (*)[HALF_KEY_NUM]) sigma0.vk);
  hash_uint256_uint256doublelist(&hash1, m1, (const uint256 (*)[HALF_KEY_NUM]) sigma1.vk);

  if (eq256(hash0,hash1)){
    printf("Refuse to extract: Input messages are similar\n");
    return;
  }
  if (!sds_daisyChain_classic_sot_ver(&vst, m0, sigma0)) {
    printf("Refuse to extract: First pair of message-signature is not valid\n");
    return;
  }
  if (!sds_daisyChain_classic_sot_ver(&tmp_vst, m1, sigma1)) {
    printf("Refuse to extract: Second pair of message-signature is not valid\n");
    return;
  }
  classic_sot_extract(&(sst->sk), hash0, sigma0.sigma_prime, hash1, sigma1.sigma_prime);
}

/********* SDS extract for OPTIMIZED SOT *********/
void sds_daisyChain_optimized_sot_extract
    (SST* sst, VSToptimized vst,\
    const uint256 m0, const SigmaOptimized sigma0,\
    const uint256 m1, const SigmaOptimized sigma1){

  uint256 hash0, hash1;
  VSToptimized tmp_vst;

  memcpy(&tmp_vst, &vst, sizeof(VSToptimized));

  hash_2uint256(&hash0, m0, sigma0.vk);
  hash_2uint256(&hash1, m1, sigma1.vk);

  if (eq256(hash0,hash1)){
    printf("Refuse to extract: input messages are similar\n");
    return;
  }
  if (!sds_daisyChain_optimized_sot_ver(&vst, m0, sigma0)) {
    printf("Refuse to extract: First pair of message-signature is not valid\n");
    return;
  }
  if (!sds_daisyChain_optimized_sot_ver(&tmp_vst, m1, sigma1)) {
    printf("Refuse to extract: Second pair of message-signature is not valid\n");
    return;
  }
  optimized_sot_extract(&(sst->sk), hash0, sigma0.sigma_prime, hash1, sigma1.sigma_prime);
}
