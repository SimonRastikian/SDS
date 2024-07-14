#ifndef HASH_H
#define HASH_H

#include "common.h"
#include "params.h"

void hash_uint256_1int
    (uint256* out, const uint256 in, const int i);

void hash_2uint256
    (uint256* out, const uint256 in1, const uint256 in2);

void hash_uint256_2int
    (uint256* out, const uint256 in, const int b, const int i);

void hash_uint256doublelist
    (uint256* out, const uint256 in[2][HALF_KEY_NUM]);

void hash_uint256_uint256doublelist
    (uint256* out, const uint256 in1, const uint256 in2[2][HALF_KEY_NUM]);

/* Not sure whether the following is theoretically better than what's below
void hash_uint256doublelist_2int
    (uint256* out,const uint256 in[2][HALF_KEY_NUM], const int b, const int i);
*/
#endif
