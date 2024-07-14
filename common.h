#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <assert.h>

typedef enum { F, T } boolean;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef union uint256_{
  unsigned char   as_uchar  [32];
  uint8           as_uint8  [32];
} uint256;

void print256 (const uint256 in);
void allzero256(uint256* out);
void assign256 (uint256 * out, const uint256 in);
void xor256 (uint256 * out, const uint256 x, const uint256 y);
boolean eq256 (const uint256 x, const uint256 y);

#endif
