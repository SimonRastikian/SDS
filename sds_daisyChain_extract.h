#ifndef SDS_DAISYCHAIN_EXTRACT_H
#define SDS_DAISYCHAIN_EXTRACT_H

#include "common.h"
#include "params.h"
#include "sds_daisyChain.h"


void sds_daisyChain_classic_sot_extract
    (SST* sst, VSTclassic vst,\
    const uint256 m0, const Sigmaclassic sigma0,\
    const uint256 m1, const Sigmaclassic sigma1);

void sds_daisyChain_optimized_sot_extract
    (SST* sst, VSToptimized vst,\
    const uint256 m0, const SigmaOptimized sigma0,\
    const uint256 m1, const SigmaOptimized sigma1);

#endif
