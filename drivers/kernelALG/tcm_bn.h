/* tcm_bn.h*/
/*
��дbn����
*/

#ifndef HEADER_TCM_BN_H
#define HEADER_TCM_BN_H


#ifdef	__cplusplus
extern "C" {
#endif


/*
������λΪ0��Ҳ��Ҫת������������ͬBN_bn2bin
linyang 2007.1.5
*/
int tcm_bn_bn2bin(const BIGNUM *a, int len, unsigned char *to);


#ifdef	__cplusplus
}
#endif


#endif	// HEADER_TCM_ECC_H
