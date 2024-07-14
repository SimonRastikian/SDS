#ifndef SDS_H
#define SDS_H

#include "common.h"
#include "params.h"

typedef struct {
  uint32 cnt;
  uint32 max_epoch;
  uint256 sk;
} SST;

typedef struct {
  uint32 cnt;
  uint32 max_epoch;
  uint256 (* vk)[2][HALF_KEY_NUM];
} VSTclassic;

typedef struct {
  uint32 cnt;
  uint32 max_epoch;
  uint256* vk;
} VSToptimized;

// It's up to the user to allocate memory for vst before keygen
void allocateVSTclassic(VSTclassic* vst, const uint32 epochs);
void allocateVSToptimized(VSToptimized* vst, const uint32 epochs);

// Similarly, the user should not forget to free the heap from memory
void freeVSTclassic(VSTclassic* vst);
void freeVSToptimized(VSToptimized* vst);

// SDS algorithms for Classic SOT.
// Careful, keygen should not be called before allocating memory for vst
void sds_classic_sot_keygen(SST* sst, VSTclassic* vst, const uint32 epochs);
void sds_classic_sot_sig(uint256 *sigma, SST* sst, const uint256 m);
boolean sds_classic_sot_ver
    (VSTclassic *vst, const uint256 m,  const uint256 *sigma);


// SDS algorithms for Optimized SOT.
// Careful, keygen should not be called before allocating memory for vst
void sds_optimized_sot_keygen(SST* sst, VSToptimized* vst, const uint32 epochs);
void sds_optimized_sot_sig
    (uint256 sigma[2][HALF_KEY_NUM], SST* sst, const uint256 m);
boolean sds_optimized_sot_ver
    (VSToptimized *vst, const uint256 m, const uint256 sigma[2][HALF_KEY_NUM]);
#endif
