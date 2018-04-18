/* tcm_rand.h */

#ifndef HEADER_RAND_H
#define HEADER_RAND_H

#ifdef  __cplusplus
extern "C" {
#endif

/*�������������*/
int tcm_bn_pseudo_rand(BIGNUM *rnd, int bits);
int tcm_rng( unsigned int rng_len, unsigned char *prngdata_out);


#ifdef  __cplusplus
}
#endif

#endif	// HEADER_RAND_H