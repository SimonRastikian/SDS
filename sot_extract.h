#ifndef SOT_EXTRACT_H
#define SOT_EXTRACT_H

#include "common.h"
#include "params.h"

void classic_sot_extract
    (uint256 *out_key,\
    const uint256 m0, const uint256 sigma0[HALF_KEY_NUM],\
    const uint256 m1, const uint256 sigma1[HALF_KEY_NUM]);

void optimized_sot_extract
    (uint256 *out_key,\
    const uint256 m0, const uint256 sigma0[2][HALF_KEY_NUM],\
    const uint256 m1, const uint256 sigma1[2][HALF_KEY_NUM]);

#endif
