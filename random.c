#include <stdio.h>
#include <assert.h>

void random_32_bytes(unsigned char * out){
  FILE *fp;
  fp = fopen("/dev/urandom", "r");
  assert("/dev/urandom file could not open or does not exist" && fp);
  assert("Could not read 32 bytes from /dev/urandom" && fread(out,1,32,fp)==32);
  fclose(fp);
}