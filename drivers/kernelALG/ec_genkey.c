/* ec_genkey.c */

#include "openssl/bn.h"
#include "openssl/bnEx.h"
#include "openssl/crypto.h"
#include "openssl/ec_operations.h"
#include "tcm_rand.h"
#include "tcm_ecc.h"
#include "tcm_bn.h"
//#include <malloc.h>
//#include <memory.h>
#include <linux/vmalloc.h>


/*************************************************************************
����EC��˽Կ�����㷨
func : ECC_gen_d()
�������  
 		prikey:			˽Կ������
		puPrikeyLen:	�����prikey����������

������� 
 		prikey:			���ص�˽Կ
		puPrikeyLen:	�����prikey��С

����ֵ��������0�����򷵻�1
*************************************************************************/
int ECC_gen_d(unsigned char *prikey, unsigned int *puPrikeyLen)
{
	unsigned char*	pTemp_k = NULL;		/* ����� */
	BIGNUM 		*N;					/* ���� */
	BIGNUM		*kt;
	BN_CTX 		*ctx;
	//
	// ����ĳ��Ȳ����������˽Կ�����빫Կ����
	if( prikey == NULL  || *puPrikeyLen < g_uNumbits/8 )
	{
		return 1;
	}
	//

	N = BN_new();
	kt = BN_new();
	ctx= BN_CTX_new();
	pTemp_k = (unsigned char*)vmalloc(RANDOM_LEN);
	//
	if ( kt == NULL || ctx == NULL || pTemp_k == NULL )
	{
		return 1;
	}

	EC_GROUP_get_order(group, N);

	/* step 1 */
step1:

	tcm_rng(g_uNumbits/8, pTemp_k );
	BN_bin2bn(pTemp_k, g_uNumbits/8, kt);
	BN_nnmod(kt, kt, N, ctx);					/* ȷʹkt����[1, N-1] */
	/* kt��������������[1, n-1] */
	/* ���ﻹ�����ж��Ƿ�Ϊ0 */
	if( BN_is_zero(kt) )
	{
#ifdef TEST
		printf("kt is zeor\n");
#endif
		goto step1;
	}
	tcm_bn_bn2bin(kt, g_uNumbits/8, prikey);					/* ����ת��Ϊ˽Կ */
	*puPrikeyLen = g_uNumbits/8;
	//
	BN_free(N);
	BN_free(kt);
	BN_CTX_free(ctx);
	vfree(pTemp_k);

	return 0;
}


/*
ECC��Կ�����ɺ���
���룺sta�˵�mac��ַ
�����˽ԿpPrikey_out��˽Կ����pPrikeyLen_out
      ��ԿpPubkey_out, ��Կ����pPubkeyLen_out
	  ���ط�ѹ����ʽ
���أ��ɹ�����0�����򷵻�1
*/
int tcm_ecc_genkey(unsigned char *prikey, unsigned int *puPrikeyLen, unsigned char *pubkey, unsigned int *puPubkeyLen)
{
	int iret;
	/* ������˽Կ */
	iret = ECC_gen_d(prikey, puPrikeyLen);
	if( iret == 1 )
		return 1;

	iret = tcm_ecc_point_from_privatekey(prikey, *puPrikeyLen, pubkey, puPubkeyLen);

	if( iret == 1 )
		return 1;
	return 0;
}


/*************************************************************************
����EC��Բ���ߵĴ�˽Կ�õ���Կ
���� : tcm_ecc_point_from_privatekey()
�������  
 		prikey:			˽Կ
������� 
		pubkey:			��Կ

����ֵ���ɹ�����0�����򷵻�1
*************************************************************************/
int tcm_ecc_point_from_privatekey(const unsigned char *prikey, const unsigned int uPrikeyLen, unsigned char *pubkey, unsigned int *puPubkeyLen)
{
	BIGNUM 		*N;					/* ���� */
	BIGNUM		*kt;
	BIGNUM		*x;
	BIGNUM		*y;
	EC_POINT	*Pt;
	EC_POINT	*Pz;
	//
	BN_CTX 		*ctx;
	//

	if( prikey == NULL || pubkey == NULL || puPubkeyLen == NULL )
	{
		return 1;
	}
	// ����ĳ��Ȳ����������˽Կ�����빫Կ����
	if( (uPrikeyLen != g_uNumbits/8) || (*puPubkeyLen < 1 + 2 * g_uNumbits/8) )
	{
		return 1;
	}


	ctx= BN_CTX_new();
	N = BN_new();
	kt = BN_new();
	x = BN_new();
	y = BN_new();
	Pt = EC_POINT_new();
	Pz = EC_POINT_new();
	//

	if ( N == NULL || kt == NULL || x == NULL || y == NULL ||
		 Pt == NULL || Pz == NULL || ctx == NULL 
		)
	{
		return 1;
	}

	EC_GROUP_get_order(group, N);

	/* step 1 */
	BN_bin2bn(prikey, g_uNumbits/8, kt);
	/* ȷʹkt����[1, N-1], �Ƿ�Ϊ0��ʱ���ж� */
	BN_nnmod(kt, kt, N, ctx);

	/* step 2 */
	EC_POINT_mul(group, Pt, kt, G);		/* ���㹫Կ */
	EC_POINT_affine2gem(group, Pt, Pz);	
	EC_POINT_get_point(Pz, x, y, kt);

/*#ifdef TEST
{
		char *str;
		str = BN_bn2hex(x);
		printf("x: %s\n",str);
		OPENSSL_free(str);

		str = BN_bn2hex(y);
		printf("y: %s\n",str);
		OPENSSL_free(str);
}
#endif*/

	BN_hex2bn(&kt, "04");
	//
	BN_lshift(kt, kt, g_uNumbits);
	BN_add(kt, kt, x);
	//
	BN_lshift(kt, kt, g_uNumbits);
	BN_add(kt, kt, y);
	//
	tcm_bn_bn2bin(kt, 1 + 2 * g_uNumbits/8, pubkey);

	/* ���÷��صĹ�Կ��С */
	*puPubkeyLen = 1 + 2 * g_uNumbits/8;

#ifdef TEST
{
		char *str;
		str = BN_bn2hex(kt);
		printf("04||x||y: %s\n",str);
		OPENSSL_free(str);
}
#endif

	BN_free(N);
	BN_free(kt);
	BN_free(x);
	BN_free(y);
	EC_POINT_free(Pt);
	EC_POINT_free(Pz);
	BN_CTX_free(ctx);

	return 0;
}


/*************************************************************************
����EC��Բ���ߵ��жϹ�˽Կ���Ƿ�ƥ��
���� : tcm_ecc_is_match()
�������  
 		prikey:			��Բ�����ϵĵ㣬�����Ƿ�ѹ����ʽ
		pubkey:			˽Կ
������� 
		���ޣ�

����ֵ���ɹ�����TRUE�����򷵻�FALSE
������������ֻ���ڲ��ԡ�����ֻ����С��256bit(����)�����ߡ�
*************************************************************************/
BOOL tcm_ecc_is_key_match(const unsigned char *prikey, const unsigned int uPrikeyLen, 
						  const unsigned char *pubkey, const unsigned int uPubkeyLen)
{
	int iret;
	unsigned char pubkey_temp[65];
	unsigned int uPubkey_tempLen = 65;

	if( prikey == NULL || pubkey == NULL)
	{
		return 1;
	}
	if( (uPrikeyLen != g_uNumbits/8) || (uPubkeyLen != 1 + 2 * g_uNumbits/8) )
	{
		return 1;
	}

	iret = tcm_ecc_point_from_privatekey(prikey, uPrikeyLen, pubkey_temp, &uPubkey_tempLen);
	if( iret == 1 )
		return FALSE;
	if( memcmp(pubkey_temp, pubkey, uPubkey_tempLen) != 0 )
		return FALSE;
	return TRUE;
}
