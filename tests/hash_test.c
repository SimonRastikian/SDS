#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "../hash.h"
#include "../hash/sha2.h"
#include "../hash/fips202.h"
#include "../hash/haraka.h"

void char_to_hex(char out[], const int outlen, const char in[]);

char str1[32] = \
    "Please give me the hash... ish!!";
char sha_256_str1[] = \
    "7534CFBCCAD94E39D0A2D061E7940E425424014466FE58BED23AEC692A95F5CD";
char sha_3_256_str1[] = \
    "9AC2B7F8A9604659406A378AEB9EEC2C988FEA230F470298032719B3A3CA94AE";
char sha_256_str1_0[] = \
    "262BF6B1A8E32FD5C9B89FDBB44A8AAF8774A43A4CD94A6468081FA854A247BD";
char sha_3_256_str1_2_6[] = \
    "4D338A522461BC3FFA68619FD0D6C461F7A7FABC3A28B4456EAE88C1CD5B81E1";

char str2[32] = \
    "A thirty two characters sentence";
char sha_256_str2[] = \
    "3BA9271BD0E2F095B455220FB5AA91B472695C9025FBECB546C33CCD63F62FA6";
char sha_3_256_str2[] = \
    "7FCFDFCCE9956C7A62B6BA72360AD56E2E6361D3675C0689C4D7074507DC3173";
char sha_3_256_str2_32[] = \
    "47E255755355DABC57786C9F3B3E6DCBC303722343F9F6AE401E82F5A1CA2C94";
char sha_256_str2_1_3[] = \
    "9C14CBAA0E6E2CCC34CEA36417EE85A38325347119FD20542806893A3F83BEEB";


int main (void){
  char str[32];
  uint256 out;

  printf("Running little endian hash tests...\n");

  // testing hash functions -- could not test Haraka function
  sha256(out.as_uint8,(const uint8 *)str1, 32);
  char_to_hex(str,32,sha_256_str1);
  assert(memcmp(str,out.as_uchar,32)==0);

  sha256(out.as_uint8,(const uint8 *)str2, 32);
  char_to_hex(str,32,sha_256_str2);
  assert(memcmp(str,out.as_uchar,32)==0);


  sha3_256(out.as_uint8,(const uint8 *)str1,32);
  char_to_hex(str,32,sha_3_256_str1);
  assert(memcmp(str,out.as_uchar,32)==0);

  sha3_256(out.as_uint8,(const uint8 *)str2,32);
  char_to_hex(str,32,sha_3_256_str2);
  assert(memcmp(str,out.as_uchar,32)==0);


  #ifdef SHA_256
  memcpy(in.as_uchar,str1,32);
  hash_uint256_1int (&out, in, 0);
  char_to_hex(str,32,sha_256_str1_0);
  assert(memcmp(str,out.as_uchar,32)==0);

  memcpy(in.as_uchar,str2,32);
  hash_uint256_2int (&out, in, 1,3);
  char_to_hex(str,32,sha_256_str2_1_3);
  assert(memcmp(str,out.as_uchar,32)==0);
  #endif

  #ifdef SHA_3_256
  memcpy(in.as_uchar,str1,32);
  hash_uint256_2int (&out, in, 2,6);
  char_to_hex(str,32,sha_3_256_str1_2_6);
  assert(memcmp(str,out.as_uchar,32)==0);

  memcpy(in.as_uchar,str2,32);
  hash_uint256_1int (&out, in, 32);
  char_to_hex(str,32,sha_3_256_str2_32);
  assert(memcmp(str,out.as_uchar,32)==0);
  #endif

  uint256 in2[2][HALF_KEY_NUM];
  hash_uint256doublelist(&out, (const uint256 (*)[HALF_KEY_NUM]) in2);
  printf("\tPassed!\n");
  return 0;
}

uint8 get_val(char c) {
  if(c >= '0' && c <= '9') return c - '0';
  return c - 'A' + 10;
}

void char_to_hex(char out[], const int outlen, const char in[]){
  int i;
  for (i=0;i<outlen;i++)
    out[i] = (char)(get_val(in[i*2])<<4 | get_val(in[i*2+1]));
}
