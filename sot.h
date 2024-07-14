#ifndef SOT_H
#define SOT_H

#include "common.h"
#include "params.h"

void classic_sot_keygen
    (uint256 *sk, uint256 vk[2][HALF_KEY_NUM], const uint256 prng_seed);
void classic_sot_sig
    (uint256 sigma[HALF_KEY_NUM], const uint256 sk, const uint256 m);
boolean classic_sot_ver
    (uint256 vk [2][HALF_KEY_NUM], const uint256 m, const uint256 sigma[HALF_KEY_NUM]);

void optimized_sot_keygen(uint256 *sk, uint256* vk, const uint256 prng_seed);
void optimized_sot_sig
    (uint256 sigma[2][HALF_KEY_NUM], const uint256 sk, const uint256 m);
boolean optimized_sot_ver
    (const uint256 vk, const uint256 m, const uint256 sigma[2][HALF_KEY_NUM]);

#endif
