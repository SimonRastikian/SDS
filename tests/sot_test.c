#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "../sot.h"

#define SEED_TEST 100



void char_to_hex(unsigned char out[], const int outlen, const unsigned char in[]);

void test_classic_sot(unsigned char message[], uint256 PRNG_SEED){
  uint256 sk, m;
  uint256 vk[2][HALF_KEY_NUM];
  uint256 sigma[HALF_KEY_NUM];
  memcpy(m.as_uchar, message, 32);

  classic_sot_keygen(&sk,vk, PRNG_SEED);
  classic_sot_sig(sigma, sk, m);
  assert(classic_sot_ver(vk, m, sigma));
}

void test_optimized_sot(unsigned char message[], uint256 PRNG_SEED){
  uint256 sk, vk, m;
  uint256 sigma[2][HALF_KEY_NUM];
  memcpy(m.as_uchar, message, 32);

  optimized_sot_keygen(&sk, &vk, PRNG_SEED);
  optimized_sot_sig(sigma, sk, m);
  assert(optimized_sot_ver(vk, m, (const uint256 (*)[HALF_KEY_NUM]) sigma));
}

int main (void){
  int i;
  uint256 PRNG_SEED;
  FILE *fp;
  unsigned char line[65];
  unsigned char message[32] = "What 32 bytes message do I sign?";


  printf("Running SOT tests...\n");

  fp = fopen("./tests/seeds_tests", "r");
  assert("seeds_tests file cannot open" && fp);
  for(i=0;i<SEED_TEST;i++){
    assert(fread(line, 1, 65, fp)==65);
    char_to_hex(PRNG_SEED.as_uchar, 32, line);
    test_classic_sot(message, PRNG_SEED);
    test_optimized_sot(message, PRNG_SEED);
  }
  fclose(fp);

  printf("\tPassed!\n");
  return 0;
}


uint8 get_val(unsigned char c) {
  if(c >= '0' && c <= '9') return c - '0';
  return c - 'A' + 10;
}

void char_to_hex(unsigned char out[], const int outlen, const unsigned char in[]){
  int i;
  for (i=0;i<outlen;i++)
    out[i] = (char)(get_val(in[i*2])<<4 | get_val(in[i*2+1]));
}
