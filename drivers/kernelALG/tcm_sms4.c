//#include <malloc.h>
//#include <memory.h>
#include <linux/vmalloc.h>


//#include "tcm_sms4.h"
#include "SMS4.h"

#ifndef NULL
#define NULL    0
/*#define NULL ((void *)0)*/
#endif

/*
* func : tcm_sms4_encrypt()
* Input :  
* 		BYTE *IV : IV������Կ����һ����128bits
*		BYTE *M : ���ĵ�ַ
*		UINT32 mLen: ���ĳ���
*		BYTE *S�����ĵ�ַ
* 		BYTE *key : ��Կ
* Output :
*		��1 : ����ʧ��
*		Len : ���ܺ󳤶�
* Func Desp : SMS4�ļ�������
*/
INT32 tcm_sms4_encrypt(BYTE *IV, BYTE *M, INT32 mLen, BYTE *S, BYTE *key)
{
	INT32 iret;
	BYTE *pTempM;
	/* �����ڴ棬Ϊcbcģʽ��׼�� */
	pTempM = (BYTE *)vmalloc(mLen + 16);
	if( pTempM == NULL )
		return -1;

	memset(pTempM, 0, mLen+16);
	memcpy(pTempM, M, mLen);
	/* ��ʼCBCģʽ���� */
	iret = SMS4_E(IV, pTempM, mLen, S, key, SMS4_MODE_CBC);
	/* �ͷŷ�����ڴ� */
	vfree(pTempM);
	return iret;
}


/*
* func : tcm_sms4_decrypt()
* Input :  
* 		BYTE *IV : IV������Կ����һ����128bits
*		BYTE *M : ���ĵ�ַ
*		UINT32 mLen: ���ĳ���
*		BYTE *S�����ĵ�ַ
* 		BYTE *key : ��Կ
*		BYTE mode:SMS4ģʽѡ��
* Output :
*		��1 : ����ʧ��
*		Len : ���ܺ󳤶�
* Func Desp : SMS4�Ľ�������
*/
INT32 tcm_sms4_decrypt(BYTE *IV, BYTE *M, INT32 mLen, BYTE *S, BYTE *key)
{
	return SMS4_D(IV, M, mLen, S, key, SMS4_MODE_CBC);
}
