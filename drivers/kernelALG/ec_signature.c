/******************************************
File	:ec_signature.c 
Author	:linyang
Date	:11/21/2006
comment :�����㷨
******************************************/

#include "openssl/bn.h"
#include "openssl/bnEx.h"
#include "openssl/crypto.h"
#include "openssl/ec_operations.h"
#include "tcm_rand.h"
#include "tcm_bn.h"
//#include <malloc.h>
#include <linux/vmalloc.h>
/*
����EC��Բ���ߵ�����ǩ���㷨
�����ǩ���ַ���sig = Mr || Ms
���룺group������p, a, b, N
���룺����G(Gx,Gy)
���룺ǩ��˽Կda
���룺��ǩ������Ϣmsg
���룺��ǩ����Ϣ�ĳ���msgLen
���룺sta�˵�mac��ַ
*/
int ECC_Signature(/*out*/unsigned char *pSignature,
				  const EC_GROUP *group, 
				  const EC_POINT *G, 
				  const BIGNUM *da, 
				  /*in*/unsigned char *pDigest)
{
	unsigned char*	pTemp_k = NULL;		/* ����� */

	BIGNUM 		*e;			/* ��ϣ�Ӵ�ֵ��Ӧ��GF(p)�е����� */
	BIGNUM		*k;			/* �������Ӧ��GF(p)�е����� */
	BIGNUM 		*i, *tmp;
	BIGNUM 		*Vy;
	BIGNUM 		*N;			/* ���� */
	BIGNUM 		*s;
	BIGNUM 		*r;

	EC_POINT 	*V; 
	BN_CTX 		*ctx = BN_CTX_new();

	//
	k = BN_new();
	Vy = BN_new();
	i = BN_new();
	s = BN_new();
	r = BN_new();
	N = BN_new();
	tmp=BN_new();
	e=BN_new();
	V = EC_POINT_new();
	//
	pTemp_k = (unsigned char*)vmalloc(RANDOM_LEN);
	//

	if ( e == NULL || k == NULL || Vy == NULL || i == NULL || s == NULL ||
		 r == NULL || N == NULL || tmp == NULL || V == NULL || ctx == NULL ||
		 pTemp_k == NULL)
	{
		return 1;
	}
	
	EC_GROUP_get_order(group,N);
	/* A2 */
	/* e=CH(M) */
	/* ����ϣ�Ӵ�ֵת��Ϊ������ */
	/* ʹ����Կ���ȣ����������� */
	BN_bin2bn(pDigest, g_uNumbits/8, e);

#ifdef TEST
	{
		char *str;
		str = BN_bn2hex(e);
		printf("e: %s\n",str);
		OPENSSL_free(str);
	}
#endif

	/* A3 */
	/* ��������� */
step3:
	tcm_rng(g_uNumbits/8, pTemp_k );
	BN_bin2bn(pTemp_k, g_uNumbits/8, k);	
	/* ȷʹk����[1, N-1] */
	BN_nnmod(k, k, N, ctx);
	/* ���ﻹ�����ж��Ƿ�Ϊ0 */
	if( BN_is_zero(k) )
	{
#ifdef TEST
		printf("k is zeor\n");
#endif
		goto step3;
	}

#ifdef TEST_FIXED
{
	// ʹ�ù̶������
	if( g_uNumbits == 256 )
	{
		// for Fp-256
		BN_hex2bn(&k,"6CB28D99385C175C94F94E934817663FC176D925DD72B727260DBAAE1FB2F96F");
	}
	else
	{
		// for Fp-192
		BN_hex2bn(&k,"79443C2BB962971437ACB6246EA7E14165700FD733E14569");
	}
}
#endif
	
	/* A4 */
	/* ������Բ���ߵ� (x,y) = [k]G */
	EC_POINT_mul(group, V, k, G);
	if (EC_POINT_is_at_infinity(group,V))
		goto step3;

	/* �������굽�������� */
	EC_POINT_affine2gem(group, V, V);
	EC_POINT_get_point(V, i, Vy, tmp);

#ifdef TEST
	{

		char *str;
		EC_POINT_get_point(V, i, Vy, tmp);
		str = BN_bn2hex(i);
		printf("x1: %s\n",str);
		OPENSSL_free(str);

		str = BN_bn2hex(Vy);
		printf("y1: %s\n",str);
		OPENSSL_free(str);

		str = BN_bn2hex(tmp);
		printf("z1: %s\n",str);
		OPENSSL_free(str);
	}
#endif

	/* A5 */
	/* ���� r=(e+x1) mod n */
	BN_add(r, e, i);
	BN_nnmod(r, r, N, ctx);
	/* �� r=0 ��r+k=n���򷵻�A3 */
	if(BN_is_zero(r))
		goto step3;

	/* �����Ƿ�r+k=n��������򷵻�A3 */
	BN_add(tmp, r, k);
	if(BN_cmp(tmp, N) == 0 )
		goto step3;


#ifdef TEST
	{
		char *str;
		str = BN_bn2hex(r);
		printf("r: %s\n",str);
		OPENSSL_free(str);
	}
#endif


	/* A6 */
	/* ���� (1+da) �����(k-rda) mod n */

	/* ���� k-rda */
	BN_mul(tmp, r, da, ctx);
	BN_sub(s, k, tmp);
	/* ���� (1+da) ���� */
	BN_dec2bn(&i,"1");
	BN_add(tmp, i, da);
	BN_div_mod(s, s, tmp, N);


#ifdef TEST
	{
		char *str;
		str = BN_bn2hex(s);
		printf("s: %s\n",str);
		OPENSSL_free(str);
	}
#endif
	
	/* A7 */
	/* ��ϢM��ǩ��Ϊ (r,s) */
	BN_lshift(r,r,8*g_uNumbits/8);
	BN_add(r,r,s);
	//
	tcm_bn_bn2bin(r, 2*g_uSCH_Numbits/8, pSignature);

  	BN_free(e);
	BN_free(Vy);
	BN_free(i);
	BN_free(k);
	BN_free(s);
	BN_free(N);
	BN_free(tmp);
	BN_free(r);
	BN_CTX_free(ctx);
	EC_POINT_free(V);
	//
	vfree(pTemp_k);
	
	return 0;
}
