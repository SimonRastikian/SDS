#ifndef SDS_EXTRACT_H
#define SDS_EXTRACT_H

#include "common.h"
#include "params.h"
#include "sds.h"

void sds_classic_sot_extract
    (SST* sst, VSTclassic vst,\
    const uint256 m0, const uint256 sigma0[HALF_KEY_NUM],\
    const uint256 m1, const uint256 sigma1[HALF_KEY_NUM]);

void sds_optimized_sot_extract
    (SST* sst, VSToptimized vst,\
    const uint256 m0, const uint256 sigma0[2][HALF_KEY_NUM],\
    const uint256 m1, const uint256 sigma1[2][HALF_KEY_NUM]);

#endif
