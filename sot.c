#include <string.h>

#include "sot.h"
#include "hash.h"
#include "rand/rng.h"

// This function generates half of the secret keys using PRNG
// The other half should be later on XORed with the seed in sk_gen
static void half_sk_gen(uint256* half_sk, const uint256 seed){
  int i;
  AES_XOF_struct ctx;
  // for Bertram Poettering Simon Rastikian
  unsigned char diversifier[] = "B.P.S.R.";
  // initialize correctly the seed expander
  // diversifier is of 8 bytes
  if (seedexpander_init(&ctx, (unsigned char *) seed.as_uchar, diversifier, HALF_KEY_SIZE+1))
    assert("Seed Expander Init Failed: RNG_BAD_MAXLEN" && 0);

  // generate half of the secret keys with seedexpander
  for (i=0;i<HALF_KEY_NUM;i++){
    switch(seedexpander(&ctx,half_sk[i].as_uchar,SEED_SIZE)){
      case RNG_BAD_OUTBUF:
        assert("Seed Expander Failed: RNG_BAD_OUTBUF" && 0);
        break;
      case RNG_BAD_REQ_LEN:
        assert("Seed Expander Failed: RNG_BAD_REQ_LEN" && 0);
        break;
      default:
        break;
    }
  }
}

// This function calls half_sk_gen to generate half of the secret keys using PRNG
// The other half is then XORed with the seed
static void sk_gen(uint256 sk[2][HALF_KEY_NUM], const uint256 seed){
  int i;

  // secrets[0] contains the list of secrets generated with PRNG
  half_sk_gen(sk[0],seed);

  // secrets[1] contains the list of secrets generated with PRNG XORed with seed
  for (i=0;i<HALF_KEY_NUM;i++) xor256(&(sk[1][i]), sk[0][i], seed);
}

// The core function of the SOT key generation
// Inputs a seed and outputs the secret key and the verification key(s)
static void core_gen
    (uint256 sk[2][HALF_KEY_NUM], uint256 vk[2][HALF_KEY_NUM],const uint256 seed){

  sk_gen(sk,seed);

  for (size_t i=0;i<HALF_KEY_NUM;i++) {
    // vk[0] contains the list of hashed secrets[0]
    hash_uint256_2int(&(vk[0][i]), sk[0][i],0,i);
    // vk[1] contains the list of hashed secrets[1]
    hash_uint256_2int(&(vk[1][i]), sk[1][i],1,i);
  }
}


/********* CLASSICAL SOT *********/
/*********************************/
/* KEY GENERATION */
// The theoretical paper defines keygen with no input and two outputs
// For security reasons, we do not make use of malloc function
void classic_sot_keygen(uint256 *sk, uint256 vk[2][HALF_KEY_NUM], const uint256 prng_seed){
  uint256 secrets[2][HALF_KEY_NUM];
  core_gen(secrets,vk, prng_seed);
  // assign sk = seed
  assign256(sk, prng_seed);
}

/* SIGNATURE */
// The theoretical paper defines sig with two inputs and one outputs
// For security reasons, we do not make use of malloc function
void classic_sot_sig(uint256 sigma[HALF_KEY_NUM],const uint256 sk,const uint256 m){
  int i,b;
  int cnt;
  uint256 secrets[2][HALF_KEY_NUM];
  // we have seed == sk so use PRNG(sk)
  sk_gen(secrets, sk);

  cnt =-1;
  for(i=0;i<HALF_KEY_NUM;i++){
    // (m_1...m_n) <- m
    if ((i%8)==0) cnt++;
    // b <- m_i
    b = (m.as_uint8[cnt]>>(i%8)) & 1;
    // sigma_i <- sk_i^b
    assign256(sigma+i, secrets[b][i]);
  }
}

/* VERIFICATION */
// The theoretical paper defines sig with two inputs and one outputs
// For security reasons, we do not make use of malloc function
boolean classic_sot_ver
    (uint256 vk [2][HALF_KEY_NUM],const uint256 m, const uint256 sigma[HALF_KEY_NUM]){

  int i,b;
  int cnt;
  uint256 hash;
  boolean bool=T;
  cnt = -1;
  for(i=0;i<HALF_KEY_NUM;i++){
    // (m_1...m_n) <-- m
    if ((i%8)==0) cnt++;
    // b <-- m_i
    b = (m.as_uint8[cnt]>>(i%8)) & 1;
    //  hash <-- H(sigma_i)
    hash_uint256_2int(&hash,sigma[i],b,i);
    //  Test hash == vk_i^b
    bool &= eq256(vk[b][i], hash);
  }
  return bool;
}

/********* SPACE OPTIMIZED SOT *********/
/****************************************/
/* KEY GENERATION */
// The theoretical paper defines keygen with no input and two outputs
// For security reasons, we do not make use of malloc function
void optimized_sot_keygen(uint256 *sk, uint256* vk, const uint256 prng_seed){
  uint256 secrets [2][HALF_KEY_NUM];
  uint256 ver_keys [2][HALF_KEY_NUM];

  core_gen(secrets,ver_keys, prng_seed);

  // assign sk = seed
  assign256(sk, prng_seed);
  hash_uint256doublelist(vk, (const uint256 (*)[HALF_KEY_NUM]) ver_keys);
}

/* SIGNATURE */
// The theoretical paper defines sig with two inputs and one outputs
// For security reasons, we do not make use of malloc function
void optimized_sot_sig
    (uint256 sigma[2][HALF_KEY_NUM],const uint256 sk,const uint256 m){

  int i,b;
  int cnt;
  uint256 secrets[2][HALF_KEY_NUM];

  // we have seed == sk so use PRNG(sk)
  sk_gen(secrets,sk);

  cnt =-1;
  for(i=0;i<HALF_KEY_NUM;i++){
    // (m_1...m_n) <-- m
    if ((i%8)==0) cnt++;
    // b <- m_i
    b = (m.as_uint8[cnt]>>(i%8)) & 1;
    // sigma_i^0 <-- sk_i^b
    assign256(&(sigma[0][i]), secrets[b][i]);
    // sigma_i^1 <--
    hash_uint256_2int(&(sigma[1][i]), secrets[1-b][i],1-b,i);
  }
}

/* VERIFICATION */
boolean optimized_sot_ver
    (const uint256 vk, const uint256 m, const uint256 sigma[2][HALF_KEY_NUM]){

  int i;
  int cnt;
  uint8 b;
  uint256 ver[2][HALF_KEY_NUM];
  uint256 hash;
  boolean bool;

  cnt = -1;
  for(i=0;i<HALF_KEY_NUM;i++){
    // (m_1...m_n) <-- m
    if ((i%8)==0) cnt++;
    // b <-- m_i
    b = (m.as_uint8[cnt]>>(i%8)) & 1;
    //  ver_i^b <-- H(sigma_i)
    hash_uint256_2int(&(ver[b][i]),sigma[0][i],b,i);
    assign256(&(ver[1-b][i]),sigma[1][i]);
  }
  hash_uint256doublelist(&hash, (const uint256 (*)[HALF_KEY_NUM]) ver);
  bool = eq256(hash,vk);
  return bool;
}
