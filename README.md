# Sequential Digital Signatures (SDS)

This is the C implementation of the Sequential Digital Signatures paper
[https://eprint.iacr.org/2022/995.pdf] by B. Poettering and S. Rastikian.

This implementation contains two variants of SDS:
- The first is fixes the number of signatures to perform. It is more efficient during the signing step but the keys could be large.
- The second denoted with DaisyChain allows performing an unlimited number of signatures. The keys are compact but the signing operation is slightly less performant.


## Installation
The implementation has been tested on Ubuntu 20.04 platform.
It is essential to have the C library `libssl-dev` installed.

## Running
To compile run `make`.
You can run the tests using `make run_tests`.
The measurments can be run with `make run_measurements`

## Changing Parameters
It is possible to run the implementation with different hash functions,
namely SHA2-256, SHA3-256 and HARAKA.
To switch parameters, uncomment in the file `paramsmenu.h` the corresponding line
then recompile using `make`.
