#ifndef PARAMS_H
#define PARAMS_H

#include "paramsmenu.h"

#ifdef HARAKA
  #undef SHA_3_256
  #undef SHA_256
#elif defined(SHA_3_256)
  #undef HARAKA
  #undef SHA_256
#else
  #define SHA_256
  #undef HARAKA
  #undef SHA_3_256
#endif

/* SOT part */
#define MSG_SIZE        256                 // size is determined by HASH
#define SEED_SIZE       256                 // PRNG seeds of size 32 bytes
#define KEY_NUM         MSG_SIZE*2          // choice between msg bit 0 and 1
#define HALF_KEY_NUM    KEY_NUM/2           // generated number of bytes by PRNG
#define KEY_SIZE        SEED_SIZE*KEY_NUM   // because of sk_1 = sk_0 xor seed
#define HALF_KEY_SIZE   KEY_SIZE/2          // generated number of bytes by PRNG

#endif
