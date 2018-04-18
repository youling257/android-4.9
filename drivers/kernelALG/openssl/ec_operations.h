/* ec_operations.h */

#ifndef HEADER_EC_OPERATION_H
#define HEADER_EC_OPERATION_H

#ifdef	__cplusplus
extern "C" {
#endif


#ifndef BN_ULONG
#define BN_ULONG	unsigned long long
#endif


#ifndef BOOL
typedef int   BOOL;
#endif

#ifndef NULL
#define NULL    0
#endif

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif


typedef struct bignum_st
	{
	BN_ULONG *d;	/* Pointer to an array of 'BN_BITS2' bit chunks. */
	int top;	/* Index of last used d +1. */
	/* The next are internal book keeping for bn_expand. */
	int dmax;	/* Size of the d array. */
	int neg;	/* one if the number is negative */
	int flags;
	} BIGNUM;

typedef struct ec_point_st {
	BIGNUM X;
	BIGNUM Y;
	BIGNUM Z; /* Jacobian projective coordinates:
	           * (X, Y, Z)  represents  (X/Z^2, Y/Z^3)  if  Z != 0 */
	int Z_is_one; /* enable optimized point arithmetics for special case */

} EC_POINT /* EC_POINT */;

typedef struct ec_group_st {
	BIGNUM p; /* Field specification.
	               * For curves over GF(p), this is the modulus. */

	BIGNUM a, b; /* Curve coefficients.
	              * (Here the assumption is that BIGNUMs can be used
	              * or abused for all kinds of fields, not just GF(p).)
	              * For characteristic  > 3,  the curve is defined
	              * by a Weierstrass equation of the form
	              *     y^2 = x^3 + a*x + b.
	              */
	int a_is_minus3; /* enable optimized point arithmetics for special case */

	EC_POINT *generator; /* optional */
	BIGNUM order, cofactor;
}EC_GROUP /* EC_GROUP */;


/* ������������ */
//#define TEST
/* ��������У�ʹ���ı��еĹ̶����� */
//#define TEST_FIXED


/* ��Բ����Ⱥgroup�ͻ���G��ȫ�ֱ��� */
extern EC_GROUP *group;
extern EC_POINT *G;

/* ��Կ���� */
extern unsigned int g_uNumbits;
/* ��ǰʹ�õ�HASH����,������ǩ��ǰ�Ĵ��� */
extern unsigned int g_uSCH_Numbits;


/* ��ѹ����ĳ��ȣ�Ҳ�ǹ�Կ���� */
#define PUBKEY_LEN	( 1+2*(g_uNumbits/8) )

/* sch�Ӵճ��� */
/* Hash��kdf��ʹ��256bit�� */
#define HASH_NUMBITS	256
#define KDF_NUMBITS		HASH_NUMBITS

/* ���������������128bitsΪ��λ�ģ����ա��֡���� */
#define	RANDOM_LEN	((1+(g_uNumbits-1)/128)*16)



EC_POINT *EC_POINT_new(void);
void EC_POINT_free(EC_POINT *point);
int EC_POINT_is_at_infinity(const EC_GROUP *group,const EC_POINT *point);
int EC_POINT_set_to_infinity(const EC_GROUP *group,EC_POINT *point);
int EC_POINT_copy(EC_POINT *dest, const EC_POINT *src);
void EC_POINT_print(EC_POINT *P);
int EC_POINT_set_point(EC_POINT *point,const BIGNUM *x,const BIGNUM *y,const BIGNUM *z);
int EC_POINT_get_point(const EC_POINT *point,BIGNUM *x,BIGNUM *y,BIGNUM *z);
int EC_POINT_invert(const EC_GROUP *group,EC_POINT *point);
int EC_POINT_affine2gem(const EC_GROUP *group,const EC_POINT *P,EC_POINT *R);

/* ��������ļӷ� */
int EC_POINT_add(const EC_GROUP *group, EC_POINT *R, const EC_POINT *P0,const EC_POINT *P1);
int EC_POINT_sub(const EC_GROUP *group, EC_POINT *R, const EC_POINT *P0, const EC_POINT *P1);
int EC_POINT_mul(const EC_GROUP *group,EC_POINT *S,const BIGNUM *n, const EC_POINT *P);
int EC_POINT_dbl(const EC_GROUP *group, EC_POINT *R, const EC_POINT *P);

EC_GROUP *EC_GROUP_new(void);
void EC_GROUP_free(EC_GROUP *group);
int EC_GROUP_set_curve_GFp(EC_GROUP *group, const BIGNUM *p, const BIGNUM *a, const BIGNUM *b);
int EC_GROUP_get_curve_GFp(const EC_GROUP *group, BIGNUM *p, BIGNUM *a, BIGNUM *b);
int EC_GROUP_set_generator(EC_GROUP *group, const EC_POINT *generator, const BIGNUM *order, const BIGNUM *cofactor);
int EC_GROUP_set_order(EC_GROUP *group,const  BIGNUM *order);
int EC_GROUP_get_order(const EC_GROUP *group, BIGNUM *r);
int EC_GROUP_get_cofactor(const EC_GROUP *group, BIGNUM *cofactor);
/* 2006.12.4 linyang �������������ӵĺ��� */
int EC_GROUP_set_cofactor(EC_GROUP *group, const BIGNUM *cofactor);
/* �жϵ��Ƿ��������� */
BOOL EC_POINT_is_on_curve(const EC_GROUP *group, const EC_POINT *point);

/****************************************************************************************/

int ECC_Signature(unsigned char *pSignature, const EC_GROUP *group, const EC_POINT *G, const BIGNUM *ka, unsigned char *pDigest);
int ECC_Verify(const EC_GROUP *group, const EC_POINT *G, const EC_POINT *Pa, unsigned char *pDigest, unsigned char *pSignature);
int ECC_Encrypt(unsigned char *cipher,const EC_GROUP *group,const EC_POINT *G,const EC_POINT *Pb,unsigned char *msg,const int msgLen);
int ECC_Decrypt(unsigned char *msg,const EC_GROUP *group,unsigned char *cipher,unsigned int cipherLen,const BIGNUM *kb);

/*****************************************************************************************/

#ifdef	__cplusplus
}
#endif

#endif
