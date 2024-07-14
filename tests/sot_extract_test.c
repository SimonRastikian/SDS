#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "../sot.h"
#include "../sot_extract.h"

#define SEED_TEST 100

void char_to_hex(unsigned char out[], const int outlen, const unsigned char in[]);

void randomString(unsigned char message[]){
  int i, r;
  for (i=0;i<8;i++){
    r = rand();
    memcpy(message+i*sizeof(int),&r,sizeof(int));
  }
}

void test_classic_Xsot(uint256 prng_seed){
  uint256 out_key;
  uint256 sk, m1, m2;
  uint256 vk[2][HALF_KEY_NUM];
  uint256 sigma1[HALF_KEY_NUM],sigma2[HALF_KEY_NUM];

  randomString(m1.as_uchar);
  randomString(m2.as_uchar);

  classic_sot_keygen(&sk,vk,prng_seed);

  classic_sot_sig(sigma1, sk, m1);
  assert(classic_sot_ver(vk, m1, sigma1));

  classic_sot_sig(sigma2, sk, m2);
  assert(classic_sot_ver(vk, m2, sigma2));

  classic_sot_extract(&out_key, m1, sigma1, m2, sigma2);
  assert(eq256(out_key, sk));
}

void test_optimized_Xsot(uint256 prng_seed){
  uint256 out_key;
  uint256 sk, vk, m1, m2;

  uint256 sigma1[2][HALF_KEY_NUM],sigma2[2][HALF_KEY_NUM];
  randomString(m1.as_uchar);
  randomString(m2.as_uchar);

  optimized_sot_keygen(&sk, &vk, prng_seed);


  optimized_sot_sig(sigma1, sk, m1);
  assert(optimized_sot_ver(vk, m1, (const uint256 (*)[HALF_KEY_NUM]) sigma1));

  optimized_sot_sig(sigma2, sk, m2);
  assert(optimized_sot_ver(vk, m2, (const uint256 (*)[HALF_KEY_NUM]) sigma2));

  optimized_sot_extract(&out_key, m1, (const uint256 (*)[HALF_KEY_NUM]) sigma1, m2, (const uint256 (*)[HALF_KEY_NUM]) sigma2);
  assert(eq256(out_key, sk));
}


int main (void){
  int i;
  FILE *fp;
  uint256 prng_seed;
  unsigned char line[65];

  printf("Running SOT Extract tests...\n");
  srand(time(NULL));


  fp = fopen("./tests/seeds_tests", "r");
  assert("seeds_tests file cannot open" && fp);
  for(i=0;i<SEED_TEST;i++){
    assert(fread(line, 1, 65, fp)==65);
    char_to_hex(prng_seed.as_uchar, 32, line);
    test_classic_Xsot(prng_seed);
    test_optimized_Xsot(prng_seed);
  }
  fclose(fp);

  printf("\tPassed!\n");
  return 0;
}

uint8 get_val(char c) {
  if(c >= '0' && c <= '9') return c - '0';
  return c - 'A' + 10;
}

void char_to_hex(unsigned char out[], const int outlen, const unsigned char in[]){
  int i;
  for (i=0;i<outlen;i++)
    out[i] = (char)(get_val(in[i*2])<<4 | get_val(in[i*2+1]));
}
