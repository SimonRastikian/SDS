#include <stdlib.h>

#include "sds.h"
#include "sot.h"
#include "hash.h"
#include "random.h"

// This is the function that will initialize the seed
static void set_initial_seed(uint256* init_seed){
  random_32_bytes(init_seed->as_uchar);
}


// Dynamically allocating VST
void allocateVSTclassic(VSTclassic* vst, const uint32 epochs){
  if(vst){
    vst->cnt = 0;
    vst->max_epoch = epochs;
    vst->vk = malloc(epochs*sizeof(uint256 [2][HALF_KEY_NUM]));
    assert("Failed to allocate memory for verification state" && vst->vk);
  }
}

void allocateVSToptimized(VSToptimized* vst, const uint32 epochs){
  if(vst){
    vst->cnt = 0;
    vst->max_epoch = epochs;
    vst->vk = malloc(epochs*sizeof(uint256));
    assert("Failed to allocate memory for verification state" && vst->vk);
  }
}

void freeVSTclassic(VSTclassic* vst){
  if(vst)
    if(vst->vk)
        free(vst->vk);
}

void freeVSToptimized(VSToptimized* vst){
  if(vst)
    if(vst->vk)
        free(vst->vk);
}


/********* SDS using classic SOT *********/
/*****************************************/
/* KEY GENERATION */
void sds_classic_sot_keygen(SST* sst, VSTclassic* vst, const uint32 epochs){
  int i;
  uint256 tmp_sk, seed;
  if (!epochs) assert("No generation for 0 epochs" && 0);
  if (!sst || !vst) assert("No generation NULL keys" && 0);

  set_initial_seed(&seed);

  // sst = (seed,0)
  assign256(&(sst->sk), seed);

  sst->cnt = 0;
  sst->max_epoch = epochs;
  // vst = (vk[.],0)
  for (i=0;i<epochs;i++){
    classic_sot_keygen(&tmp_sk, vst->vk[i], seed);
    hash_uint256_1int (&seed, seed, i);
  }

  vst->cnt = 0;
  vst->max_epoch = epochs;
}

/* SIGNATURE */
void sds_classic_sot_sig (uint256 *sigma, SST* sst, const uint256 m){
  if (!sigma || !sst)
    assert("Signature algorithm has been given NULL pointer" && 0);
  if (sst->cnt == sst->max_epoch)
    assert ("The number of signatures has exceeded the maximum" && 0);

  // This specific implementation for our sot allows us to skip the generation
  // part : classic_sot_keygen(&sk, vk, sst->sk);
  classic_sot_sig(sigma,sst->sk,m);
  hash_uint256_1int (&(sst->sk), sst->sk, sst->cnt);
  sst->cnt ++;
}

/* VERIFICATION */
boolean sds_classic_sot_ver
    (VSTclassic *vst, const uint256 m,  const uint256 *sigma){
  boolean b=F;
  if (!vst)
    assert("Verification algorithm has been given NULL pointer" && 0);
  if (vst->cnt == vst->max_epoch)
    assert ("The number of valid verifications has exceeded the maximum" && 0);
  b = classic_sot_ver(vst->vk[vst->cnt], m, sigma);
  if (b) vst->cnt++ ;
  return b;
}


/********* SDS using optimized SOT *********/
/*******************************************/
/* KEY GENERATION */
void sds_optimized_sot_keygen(SST* sst, VSToptimized* vst, const uint32 epochs){
  int i;
  uint256 tmp_sk, seed;

  if (epochs==0) assert("No generation for 0 epochs" && 0);
  if (!sst || !vst) assert("No generation for NULL keys" && 0);

  set_initial_seed(&seed);
  // sst = (seed,0)
  // In Optimized SOT: sk1 = seed
  assign256(&(sst->sk),seed);
  sst->cnt = 0;
  sst->max_epoch = epochs;
  // vst = (vk[.],0)
  for (i=0;i<epochs;i++){
    optimized_sot_keygen(&tmp_sk, &(vst->vk[i]), seed);
    // because tmp_sk == seed then one can hash seed directly
    hash_uint256_1int (&seed, seed, i);
  }
  vst->cnt = 0;
  vst->max_epoch = epochs;
}

/* SIGNATURE */
void sds_optimized_sot_sig
    (uint256 sigma[2][HALF_KEY_NUM], SST* sst, const uint256 m){
  if (!sst)
    assert("Signature algorithm has been given NULL pointer" && 0);
  if (sst->cnt == sst->max_epoch)
    assert ("The number of signatures has exceeded the maximum" && 0);
  // optimized_sot_keygen(&sk, &vk, sst->sk);
  optimized_sot_sig(sigma,sst->sk,m);
  hash_uint256_1int (&(sst->sk), sst->sk, sst->cnt);
  sst->cnt ++;
}

/* VERIFICATION */
boolean sds_optimized_sot_ver
    (VSToptimized *vst, const uint256 m,  const uint256 sigma[2][HALF_KEY_NUM]){
  boolean b=F;
  if (!vst)
    assert("Verification algorithm has been given NULL pointer" && 0);
  if (vst->cnt == vst->max_epoch)
    assert ("The number of valid verifications has exceeded the maximum" && 0);
  b = optimized_sot_ver(vst->vk[vst->cnt], m, sigma);
  if (b) vst->cnt++ ;
  return b;
}
