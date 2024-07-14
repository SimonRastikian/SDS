#ifndef SDS_DAISYCHAIN_H
#define SDS_DAISYCHAIN_H

#include "common.h"
#include "params.h"

typedef struct {
  uint256 sk;
} SST;

typedef struct {
  uint256 vk[2][HALF_KEY_NUM];
} VSTclassic;

typedef struct {
  uint256 vk[2][HALF_KEY_NUM];
  uint256 sigma_prime[HALF_KEY_NUM];
} Sigmaclassic;

typedef struct {
  uint256 vk;
} VSToptimized;

typedef struct {
  uint256 vk;
  uint256 sigma_prime[2][HALF_KEY_NUM];
} SigmaOptimized;


// SDS algorithms for Classic SOT.
// Careful, keygen should not be called before allocating memory for vst
void sds_daisyChain_classic_sot_keygen
    (SST* sst, VSTclassic* vst);
void sds_daisyChain_classic_sot_sig
    (Sigmaclassic *sigma, SST* sst, const uint256 m);
boolean sds_daisyChain_classic_sot_ver
    (VSTclassic *vst, const uint256 m, const Sigmaclassic sigma);

// SDS algorithms for Optimized SOT.
// Careful, keygen should not be called before allocating memory for vst
void sds_daisyChain_optimized_sot_keygen
    (SST* sst, VSToptimized* vst);
void sds_daisyChain_optimized_sot_sig
    (SigmaOptimized *sigma, SST* sst, const uint256 m);
boolean sds_daisyChain_optimized_sot_ver
    (VSToptimized *vst, const uint256 m, const SigmaOptimized sigma);
#endif
