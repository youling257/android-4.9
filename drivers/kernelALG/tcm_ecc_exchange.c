/******************************************
File	:tcm_ecc_exchange.c 
Author	:linyang
Date	:12/23/2006
Comment :���ڲ���firmware����ԿЭ�̹���
******************************************/

#include "openssl/bn.h"
#include "openssl/ec_operations.h"
#include "openssl/crypto.h"
#include "tcm_ecc.h"
#include "tcm_hash.h"
#include "tcm_bn.h"
#include <linux/string.h>

/*************************************************************************
����EC�Ĺ��꽻����Կ�㷨����������firmware�Ĳ��Ժ�����ֻ����256bit���
���� : tcm_ecc_exchange()
�������  
		fA:				��ΪTRUEʱ��Ϊ���𷽣�����Ϊ��Ӧ��
		prikey_A��		
						��faΪTRUEʱ��Ϊ���𷽵ľ�̬��Կ˽Կ��
						����Ϊ��Ӧ���ľ�̬��Կ˽Կ��
		pubkey_A��
						��faΪTRUEʱ��Ϊ���𷽵ľ�̬��Կ��Կ��
						����Ϊ��Ӧ���ľ�̬��Կ��Կ��
		prikey_RA��		
						��faΪTRUEʱ��Ϊ���𷽵���ʱ��Կ˽Կ��
						����Ϊ��Ӧ������ʱ��Կ˽Կ��
		pubkey_RA
						��faΪTRUEʱ��Ϊ���𷽵���ʱ��Կ��Կ��
						����Ϊ��Ӧ������ʱ��Կ��Կ��
		pubkey_B
						��faΪTRUEʱ��Ϊ��Ӧ���ľ�̬��Կ��Կ��
						����Ϊ���𷽵ľ�̬��Կ��Կ��
		pubkey_RB
						��faΪTRUEʱ��Ϊ��Ӧ������ʱ��Կ��Կ��
						����Ϊ���𷽵���ʱ��Կ��Կ��
		Za
						�������û���ժҪ��Ϣ
		Zb
						��Ӧ���û���ժҪ��Ϣ
������� 
 						
		key				�������Կ
		S1 
						��faΪTRUEʱ��Ϊ��Ӧ����S1��
						����Ϊ���𷽵�SB��
		Sa
						��faΪTRUEʱ��Ϊ��Ӧ����Sa��
						����Ϊ���𷽵�S2��

����ֵ������ɹ�����0�����򷵻�1
*************************************************************************/
int tcm_ecc_exchange(BOOL fA,
					 unsigned char prikey_A[32], unsigned char pubkey_A[65],
					 unsigned char prikey_RA[32], unsigned char pubkey_RA[65],
					 unsigned char pubkey_B[65], unsigned char pubkey_RB[65], 
					 unsigned char Za[32], unsigned char Zb[32],
					 /*out*/unsigned char key[16],
					 /*out*/unsigned char S1[32], 
					 /*out*/unsigned char Sa[32]
					 )
{
	unsigned int i;
	unsigned char str_x[32]={0};
	unsigned char str_y[32]={0};
	//
	unsigned char str_buffer[7*32]={0};
	unsigned char str_temp65[65]={0};
	unsigned char str_temp32[32]={0};


	BIGNUM 	*N;			/* ���� */
	BIGNUM	*one;
	BIGNUM	*da;
	BIGNUM	*ra;
	BIGNUM	*ta;
	BIGNUM	*x1, *y1, *x_1, *r_A;
	BIGNUM	*x2, *y2, *x_2;
	BIGNUM	*x_temp, *y_temp;
	BIGNUM	*xu, *yu;
	BIGNUM	*bn_127, *temp;

	EC_POINT *P_PB, *P_RB, *P_temp;

	BN_CTX *ctx;

	/* ֻ��֤256vbit����Կ����Э�� */
	if( g_uNumbits != 256 )
		return 1;

	//������Կ��Ҫ�Ƿ�ѹ����ʽ�����򷵻�ʧ��
	if( pubkey_A[0] != 0x04 ||
		pubkey_B[0] != 0x04 ||
		pubkey_RB[0] != 0x04)
	{
		return 1;
	}

	//
	ctx= BN_CTX_new();
	//
	N=BN_new();
	//
	one = BN_new();
	//
	da = BN_new();
	ra = BN_new();
	ta = BN_new();
	//
	x1 = BN_new();
	y1 = BN_new();
	x_1 = BN_new();
	r_A = BN_new();
	//
	x2 = BN_new();
	y2 = BN_new();
	x_2 = BN_new();
	//
	x_temp = BN_new();
	y_temp = BN_new();
	//
	xu = BN_new();
	yu = BN_new();
	//
	temp = BN_new();
	bn_127 = BN_new();
	//
	P_PB = EC_POINT_new();
	P_RB = EC_POINT_new();
	P_temp = EC_POINT_new();

	if ( ctx == NULL || N == NULL ||
		one == NULL ||
		da == NULL || ra == NULL || ta == NULL ||
		x1 == NULL || y1 == NULL || x_1 == NULL || r_A == NULL ||
		x2 == NULL || y2 == NULL || x_2 == NULL ||
		x_temp == NULL || y_temp == NULL ||
		xu == NULL || yu == NULL ||
		temp == NULL || bn_127 == NULL ||
		P_PB == NULL ||
		P_RB == NULL ||
		P_temp == NULL
	)
	{
		return 1;
	}

	EC_GROUP_get_order(group,N);	/* �� */
//	BN_copy(p,&group->p); /* ������ */

	/* ȡ��r_A */
	for (i = 0; i < (g_uNumbits/8); i++) {
		str_x[i] = prikey_RA[i];
	}
	BN_bin2bn(str_x, g_uNumbits/8, r_A);

	/* ȡ��x1 */
	for (i = 0; i < (g_uNumbits/8); i++) {
		str_x[i] = pubkey_RA[1+i];
		str_y[i] = pubkey_RA[1+g_uNumbits/8+i];
	}
	BN_bin2bn(str_x, g_uNumbits/8, x1);
	BN_bin2bn(str_y, g_uNumbits/8, y1);

	/* ȡ��x2 */
	for (i = 0; i < (g_uNumbits/8); i++) {
		str_x[i] = pubkey_RB[1+i];
		str_y[i] = pubkey_RB[1+g_uNumbits/8+i];
	}
	BN_bin2bn(str_x, g_uNumbits/8, x2);
	BN_bin2bn(str_y, g_uNumbits/8, y2);

	/* ����Կ�ַ���(48�ֽ�)�ֳ�������(24, 24)�ֽ� */
	for (i = 0; i < (g_uNumbits/8); i++) {
		str_x[i] = pubkey_B[1+i];
		str_y[i] = pubkey_B[1+g_uNumbits/8 + i];
	}
	BN_bin2bn(str_x, g_uNumbits/8, x_temp);
	BN_bin2bn(str_y, g_uNumbits/8, y_temp);
	BN_hex2bn(&one, "1");
	/* ���ɹ�Կ��Pb */
	EC_POINT_set_point(P_PB, x_temp, y_temp, one);

	/* ����Կ�ַ���(48�ֽ�)�ֳ�������(24, 24)�ֽ� */
	for (i = 0; i < (g_uNumbits/8); i++) {
		str_x[i] = pubkey_RB[1+i];
		str_y[i] = pubkey_RB[1+g_uNumbits/8 + i];
	}
	BN_bin2bn(str_x, g_uNumbits/8, x_temp);
	BN_bin2bn(str_y, g_uNumbits/8, y_temp);
	BN_hex2bn(&one, "1");
	/* ���ɹ�Կ��Rb */
	EC_POINT_set_point(P_RB, x_temp, y_temp, one);


	/* �õ�da */
	for (i = 0; i < (g_uNumbits/8); i++) {
		str_x[i] = prikey_A[i];
	}
	BN_bin2bn(str_x, g_uNumbits/8, da);

	/* �õ�ra */
	for (i = 0; i < (g_uNumbits/8); i++) {
		str_x[i] = prikey_RA[i];
	}
	BN_bin2bn(str_x, g_uNumbits/8, ra);
	

	/*����x_1 */
	/*����2^127*/
	if( g_uNumbits == 256 )
	{
	//	BN_hex2bn(&bn_127, "0FFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF");
		BN_hex2bn(&bn_127, "80000000000000000000000000000000");
	}
	else /*if( g_uNumbits == 128 )*/
	{
		BN_dec2bn(&bn_127, "95");
	}

	/* ���� 2^127-1 */
	BN_sub(temp,bn_127,one);
	BN_nnmod(temp,temp,N,ctx);

	/*����x1&(2^127-1)*/
	/*��ʱʹ��str_x��str_y*/
	tcm_bn_bn2bin(x1, 32, str_x);
	tcm_bn_bn2bin(temp, 16, str_y);
	for(i=0;i<16;i++)
	{
		str_x[i] = str_x[i+16] & str_y[i];
	}
	BN_bin2bn(str_x, 16, temp);
	BN_add(x_1, bn_127, temp);

	
#ifdef TEST
{
	char *str;
	str = BN_bn2hex(x_1);
	printf("x_1: %s\n",str);
	OPENSSL_free(str);
}
#endif

	/* ����ta */
	/* ����x_1*ra */
	BN_mul(temp, x_1, ra, ctx);
	BN_nnmod(temp, temp, N, ctx);

	BN_add(ta, da, temp);
	BN_nnmod(ta, ta, N, ctx);
#ifdef TEST
{
	char *str;
	str = BN_bn2hex(ta);
	printf("ta: %s\n",str);
	OPENSSL_free(str);
}
#endif

	/* ����x_2 */
	/* ���� 2^127-1 */
	BN_sub(temp,bn_127,one);
	BN_nnmod(temp,temp,N,ctx);

	/*����x2&(2^127-1)*/
	/*��ʱʹ��str_x��str_y*/
	tcm_bn_bn2bin(x2, 32, str_x);
	tcm_bn_bn2bin(temp, 16, str_y);
	for(i=0;i<16;i++)
	{
		str_x[i] = str_x[i+16] & str_y[i];
	}
	BN_bin2bn(str_x, 16, temp);
	BN_add(x_2, bn_127, temp);
#ifdef TEST
{
	char *str;
	str = BN_bn2hex(x_2);
	printf("x_2: %s\n",str);
	OPENSSL_free(str);
}
#endif

	/* ����[h*ta[Pb+[x_2]Rb] */
	/* �ȼ��� [x_2]RB */
	EC_POINT_mul(group, P_temp, x_2, P_RB);
#ifdef TEST
{
		char *str;
		BIGNUM *xb0, *yb0;
		xb0 = BN_new();
		yb0 = BN_new();
		EC_POINT_affine2gem(group,P_temp,P_temp);
		EC_POINT_get_point(P_temp,xb0,yb0,temp);

		str = BN_bn2hex(xb0);
		printf("xb0: %s\n",str);
		OPENSSL_free(str);

		str = BN_bn2hex(yb0);
		printf("yb0: %s\n",str);
		OPENSSL_free(str);

		BN_free(xb0);
		BN_free(yb0);
}
#endif

	/* �ټ��� PB+[x_2]RB */
	EC_POINT_add(group, P_temp, P_PB, P_temp);
#ifdef TEST
{
		char *str;
		BIGNUM *xb1, *yb1;
		xb1 = BN_new();
		yb1 = BN_new();
		EC_POINT_affine2gem(group,P_temp,P_temp);
		EC_POINT_get_point(P_temp,xb1,yb1,temp);

		str = BN_bn2hex(xb1);
		printf("xb1: %s\n",str);
		OPENSSL_free(str);

		str = BN_bn2hex(yb1);
		printf("yb1: %s\n",str);
		OPENSSL_free(str);

		BN_free(xb1);
		BN_free(yb1);
}
#endif
	EC_POINT_mul(group, P_temp, ta, P_temp);
	EC_POINT_affine2gem(group, P_temp, P_temp);
	EC_POINT_get_point(P_temp, xu, yu, temp);

#ifdef TEST
{
		char *str;
		str = BN_bn2hex(xu);
		printf("xu: %s\n",str);
		OPENSSL_free(str);

		str = BN_bn2hex(yu);
		printf("yu: %s\n",str);
		OPENSSL_free(str);
}
#endif
	/* ����KDF(xu||yu||Za||Zb, klen) */
	tcm_bn_bn2bin(xu, 32, &str_buffer[0]);
	tcm_bn_bn2bin(yu, 32, &str_buffer[32]);
	memcpy(&str_buffer[32*2], Za, 32);
	memcpy(&str_buffer[32*3], Zb, 32);

#ifdef TEST
{
	printf("xu||yu||Za||Zb:\n");
	for(i=0;i<32*4;i++)
	{
		if( i%32 == 0 && i!=0)
			printf("\n");
		if( i%4 == 0 )
			printf(" ");
		printf("%.2X", str_buffer[i]);
	}
	printf("\n\n");
}
#endif

	//
	tcm_kdf(key, 16, str_buffer, 4*32);
#ifdef TEST
{
	printf("KA\n");
	for(i=0;i<16;i++)
	{
		if( i%32 == 0 && i!=0)
			printf("\n");
		if( i%4 == 0 )
			printf(" ");
		printf("%.2X", key[i]);
	}
	printf("\n\n");
}
#endif


	/* ����S1 */
	if( fA == TRUE )
	{//������
		tcm_bn_bn2bin(xu, 32, &str_buffer[0]);
		memcpy(&str_buffer[32], Za, 32);
		memcpy(&str_buffer[32*2], Zb, 32);
		tcm_bn_bn2bin(x1, 32, &str_buffer[32*3]);
		tcm_bn_bn2bin(y1, 32, &str_buffer[32*4]);
		tcm_bn_bn2bin(x2, 32, &str_buffer[32*5]);
		tcm_bn_bn2bin(y2, 32, &str_buffer[32*6]);
	}
	else//������
	{
		tcm_bn_bn2bin(xu, 32, &str_buffer[0]);
		memcpy(&str_buffer[32], Za, 32);
		memcpy(&str_buffer[32*2], Zb, 32);
		tcm_bn_bn2bin(x2, 32, &str_buffer[32*3]);
		tcm_bn_bn2bin(y2, 32, &str_buffer[32*4]);
		tcm_bn_bn2bin(x1, 32, &str_buffer[32*5]);
		tcm_bn_bn2bin(y1, 32, &str_buffer[32*6]);
	}

#ifdef TEST
{
	printf("xu||Za||Zb||x1||y1||x2||y2:\n");
	for(i=0;i<32*7;i++)
	{
		if( i%32 == 0 && i!=0)
			printf("\n");
		if( i%4 == 0 )
			printf(" ");
		printf("%.2X", str_buffer[i]);
	}
	printf("\n\n");
}
#endif

	tcm_sch_hash(32*7, str_buffer, str_temp32);
#ifdef TEST
{
	printf("Hash(xu||Za||Zb||x1||y1||x2||y2):\n");
	for(i=0;i<32*7;i++)
	{
		if( i%32 == 0 && i!=0)
			printf("\n");
		if( i%4 == 0 )
			printf(" ");
		printf("%.2X", str_buffer[i]);
	}
	printf("\n\n");
}
#endif

	str_temp65[0] = 0x02;
	tcm_bn_bn2bin(yu, 32, &str_temp65[1]);
	memcpy(&str_temp65[1+32], str_temp32, 32);

#ifdef TEST
{
	printf("0x02||yu||Hash(xu||Za||Zb||x1||y1||x2||y2):\n");
	for(i=0;i<32*7;i++)
	{
		if( i%32 == 0 && i!=0)
			printf("\n");
		if( i%4 == 0 )
			printf(" ");
		printf("%.2X", str_buffer[i]);
	}
	printf("\n\n");
}
#endif
	if( fA == TRUE )
	{
		tcm_sch_hash(65, str_temp65, S1);
	}
	else
	{
		tcm_sch_hash(65, str_temp65, Sa);
	}
#ifdef TEST
{
	if( fA == TRUE )
	{
		printf("S1:\n");
		for(i=0;i<32;i++)
		{
			if( i%32 == 0 && i!=0)
				printf("\n");
			if( i%4 == 0 )
				printf(" ");
			printf("%.2X", S1[i]);
		}
	}
	else
	{
		printf("SB:\n");
		for(i=0;i<32;i++)
		{
			if( i%32 == 0 && i!=0)
				printf("\n");
			if( i%4 == 0 )
				printf(" ");
			printf("%.2X", Sa[i]);
		}
	}
	printf("\n\n");
}
#endif

	/* ����Sa */
	tcm_sch_hash(32*7, str_buffer, str_temp32);
#ifdef TEST
{
	printf("Hash(xu||Za||Zb||x1||y1||x2||y2):\n");
	for(i=0;i<32*7;i++)
	{
		if( i%32 == 0 && i!=0)
			printf("\n");
		if( i%4 == 0 )
			printf(" ");
		printf("%.2X", str_temp32[i]);
	}
	printf("\n\n");
}
#endif

	str_temp65[0] = 0x03;
	tcm_bn_bn2bin(yu, 32, &str_temp65[1]);
	memcpy(&str_temp65[1+32], str_temp32, 32);
#ifdef TEST
{
	printf("0x03||yu||Hash(xu||Za||Zb||x1||y1||x2||y2):\n");
	for(i=0;i<32*7;i++)
	{
		if( i%32 == 0 && i!=0)
			printf("\n");
		if( i%4 == 0 )
			printf(" ");
		printf("%.2X", str_buffer[i]);
	}
	printf("\n\n");
}
#endif
	if( fA == TRUE )
	{
		tcm_sch_hash(65, str_temp65, Sa);
	}
	else
	{
		tcm_sch_hash(65, str_temp65, S1);
	}
#ifdef TEST
{
	if( fA == TRUE )
	{
		printf("SA:\n");
		for(i=0;i<32;i++)
		{
			if( i%32 == 0 && i!=0)
				printf("\n");
			if( i%4 == 0 )
				printf(" ");
			printf("%.2X", Sa[i]);
		}
	}
	else
	{
		printf("S2:\n");
		for(i=0;i<32;i++)
		{
			if( i%32 == 0 && i!=0)
				printf("\n");
			if( i%4 == 0 )
				printf(" ");
			printf("%.2X", S1[i]);
		}

	}

	printf("\n\n");
}
#endif

	BN_free(N);
	BN_free(one);
	BN_free(da);
	BN_free(ra);
	BN_free(ta);
	BN_free(x1);
	BN_free(y1);
	BN_free(x_1);
	BN_free(r_A);
	BN_free(x2);
	BN_free(y2);
	BN_free(x_2);
	BN_free(x_temp);
	BN_free(y_temp);
	BN_free(xu);
	BN_free(yu);
	BN_free(bn_127);
	BN_free(temp);

	EC_POINT_free(P_PB);
	EC_POINT_free(P_RB);
	EC_POINT_free(P_temp);
	
	BN_CTX_free(ctx);

	return 0;
}

