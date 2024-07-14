#include <string.h>
#include <stdlib.h>

#include "sds_daisyChain.h"
#include "sot.h"
#include "hash.h"
#include "random.h"

// This is the function that will initialize the seed
static void set_initial_seed(uint256* init_seed){
  random_32_bytes(init_seed->as_uchar);
}


/********* Daisy Chain SDS using classic SOT *********/
/*****************************************************/
/* KEY GENERATION */
void sds_daisyChain_classic_sot_keygen(SST* sst, VSTclassic* vst){
  uint256 seed;
  if (!sst || !vst)
    assert("Key Generation algorithm has NULL input" && 0);
  set_initial_seed(&seed);
  classic_sot_keygen(&(sst->sk), vst->vk, seed);
}

/* SIGNATURE */
void sds_daisyChain_classic_sot_sig
        (Sigmaclassic *sigma, SST* sst, const uint256 m){
  uint256 hash;
  VSTclassic tmp_vst;
  SST sst_next;
  if (!sigma || !sst)
    assert("Signature algorithm has been given NULL pointer" && 0);
  // genarate sk and vk and plug vk in sigma
  sds_daisyChain_classic_sot_keygen(&sst_next, &tmp_vst);
  memcpy(sigma->vk, tmp_vst.vk, sizeof(uint256)*KEY_NUM);
  // concatenate message and verification key then hash to output a
  // string of length 256 bits
  hash_uint256_uint256doublelist(&hash, m, (const uint256 (*)[HALF_KEY_NUM]) sigma->vk);
  // sign the hash and generate sigma_prime
  classic_sot_sig(sigma->sigma_prime, sst->sk, hash);
  // update the secret state
  assign256(&(sst->sk),sst_next.sk);
}

/* VERIFICATION */
boolean sds_daisyChain_classic_sot_ver
        (VSTclassic *vst, const uint256 m,  const Sigmaclassic sigma){
  boolean b=F;
  uint256 hash;
  if (!vst)
    assert("Verification algorithm has been given NULL pointer" && 0);
  hash_uint256_uint256doublelist(&hash, m, sigma.vk);
  b = classic_sot_ver(vst->vk, hash, sigma.sigma_prime);
  if (b)
    memcpy(vst->vk, sigma.vk, sizeof(uint256)*KEY_NUM);
  return b;
}


/********* Daisy Chain SDS using optimized SOT *********/
/*******************************************************/
/* KEY GENERATION */
void sds_daisyChain_optimized_sot_keygen(SST* sst, VSToptimized* vst){
  uint256 seed;
  if (!sst || !vst) assert("No generation for NULL keys" && 0);
  set_initial_seed(&seed);
  assign256(&(sst->sk),seed);
  optimized_sot_keygen(&(sst->sk), &(vst->vk), seed);
}

/* SIGNATURE */
void sds_daisyChain_optimized_sot_sig
    (SigmaOptimized *sigma, SST* sst, const uint256 m){
  uint256 hash;
  VSToptimized tmp_vst;
  SST sst_next;
  if (!sigma || !sst)
    assert("Signature algorithm has NULL input" && 0);
  // genarate sk and vk and plug vk in sigma
  sds_daisyChain_optimized_sot_keygen(&sst_next, &tmp_vst);
  assign256(&(sigma->vk),tmp_vst.vk);
  // concatenate message and verification key then hash to output a
  // string of length 256 bits
  hash_2uint256(&hash, m, sigma->vk);
  // sign the hash and generate sigma_prime
  optimized_sot_sig(sigma->sigma_prime, sst->sk, hash);
  // update the secret state
  memcpy(&(sst->sk),&(sst_next.sk), sizeof(uint256));
}

/* VERIFICATION */
boolean sds_daisyChain_optimized_sot_ver
    (VSToptimized *vst, const uint256 m, const SigmaOptimized sigma){
  boolean b=F;
  uint256 hash;
  if (!vst)
    assert("Verification algorithm has been given NULL pointer" && 0);
  hash_2uint256(&hash, m, sigma.vk);
  b = optimized_sot_ver(vst->vk, hash, sigma.sigma_prime);
  if (b)
    assign256(&(vst->vk), sigma.vk);
  return b;
}
