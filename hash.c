#include <stddef.h>
#include <string.h>

#include "hash.h"
#include "hash/sha2.h"
#include "hash/haraka.h"
#include "hash/fips202.h"

// hash/sha2.h hash/haraka.h hash/fips202.h used functions
// void sha256(uint8_t out[32], const uint8_t *in, size_t inlen);
// void sha3_256(uint8_t h[32], const uint8_t *in, size_t inlen);
// void haraka256(unsigned char *out, const unsigned char *in);
static void hash(uint256* out,const unsigned char *in, const size_t inlen){
  #ifdef HARAKA
    haraka256(out->as_uchar, in);
  #elif defined(SHA_3_256)
    sha3_256(out->as_uint8, (const uint8 *)in, inlen);
  #elif defined(SHA_256)
    sha256(out->as_uint8, (const uint8 *)in, inlen);
  #else
    __builtin_unreachable();
  #endif
}

void hash_uint256_1int
    (uint256* out, const uint256 in, const int i){
  const size_t size = sizeof(uint256) + sizeof(int);
  unsigned char msg [size];
  memcpy(msg, in.as_uchar, sizeof(uint256));
  memcpy(msg+sizeof(uint256), &i, sizeof(int));
  hash(out, msg, size);
}

void hash_2uint256
    (uint256* out, const uint256 in1, const uint256 in2){
    const size_t size = sizeof(uint256)*2;
    unsigned char msg [size];
    memcpy(msg, in1.as_uchar, sizeof(uint256));
    memcpy(msg+sizeof(uint256), &in2,  sizeof(uint256));
    hash(out, msg, size);
}


void hash_uint256_2int
    (uint256* out, const uint256 in, const int b, const int i){
  const size_t size = sizeof(uint256) + 2*sizeof(int);
  unsigned char msg [size];
  memcpy(msg, in.as_uchar, sizeof(uint256));
  memcpy(msg+sizeof(uint256), &i, sizeof(int));
  memcpy(msg+sizeof(uint256)+sizeof(int), &b, sizeof(int));
  hash(out, msg, size);
}

/* Not sure whether the following is theoretically better than what's below
void hash_uint256doublelist_2int
    (uint256* out,const uint256 in[2][HALF_KEY_NUM], const int b, const int i){
  unsigned int j;
  const size_t size = KEY_NUM*sizeof(uint256) + 2*sizeof(int);
  unsigned char msg [size];

  for (j=0;j<HALF_KEY_NUM;j++) {
    memcpy(&(msg[j*sizeof(uint256)*2]), in[0][j].as_uchar, sizeof(uint256));
    memcpy(&(msg[(j+1)*sizeof(uint256)*2]), in[1][j].as_uchar, sizeof(uint256));
  }
  memcpy(&(msg[KEY_NUM]), i, sizeof(int));
  memcpy(&(msg[KEY_NUM+sizeof(int)]), b, sizeof(int));
  hash(out,(const unsigned char *) msg, size);
}
*/

void hash_uint256doublelist(uint256* out, const uint256 in[2][HALF_KEY_NUM]){
  unsigned int i;
  const size_t uchar256_size = sizeof(uint8)*32;
  const size_t size = KEY_NUM*uchar256_size;
  unsigned char msg[size];

  for (i=0;i<HALF_KEY_NUM;i++) {
    memcpy(msg+(i*2)*uchar256_size, in[0][i].as_uchar, uchar256_size);
    memcpy(msg+(i*2+1)*uchar256_size, in[1][i].as_uchar, uchar256_size);
  }
  hash(out,(const unsigned char *) msg, size);
}

void hash_uint256_uint256doublelist(uint256* out, const uint256 in1, const uint256 in2[2][HALF_KEY_NUM]){
  unsigned int i;
  const size_t uchar256_size = sizeof(uint8)*32;
  const size_t size = (KEY_NUM+1)*uchar256_size; // size of (2*HALF_KEY_NUM + 1)*uint256
  unsigned char msg[size];

  memcpy(msg, in1.as_uchar, uchar256_size);
  for (i=0;i<HALF_KEY_NUM;i++) {
    memcpy(msg+(i*2+1)*uchar256_size, in2[0][i].as_uchar, uchar256_size);
    memcpy(msg+(i*2+2)*uchar256_size, in2[1][i].as_uchar, uchar256_size);
  }
  hash(out,(const unsigned char *) msg, size);
}
