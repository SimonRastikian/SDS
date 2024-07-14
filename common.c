#include <stdio.h>
#include <assert.h>


#include "common.h"

void print256 (const uint256 in){
  uint8 i;
  for(i=0;i<32;i++) printf("%X",in.as_uint8[i]);
}

void allzero256(uint256* out){
  uint8 i;
  for(i=0;i<32;i++) out->as_uint8[i]= 0;
}

void assign256 (uint256* out,const uint256 in){
  uint8 i;
  for (i=0;i<32;i++) out->as_uint8[i] = in.as_uint8[i];
}

void xor256 (uint256* out,const uint256 x,const uint256 y){
  uint8 i;
  for (i=0;i<32;i++) out->as_uchar[i] = x.as_uint8[i] ^ y.as_uint8[i];
}

boolean eq256 (const uint256 x, const uint256 y){
  int i;
  boolean bool=T;
  for (i=0;i<32;i++) bool &= (x.as_uchar[i] == y.as_uchar[i]);
  return bool;
}
