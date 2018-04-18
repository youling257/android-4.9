#ifndef HEADER_TCM_SMS4_H
#define HEADER_TCM_SMS4_H

#ifdef  __cplusplus
extern "C" {
#endif

#define	SMS4_BLOCK_SIZE 16
#define SMS4_MAX_LEN	512

#define SMS4_MODE_CBC	0
#define	SMS4_MODE_ECB	1

#ifndef UINT8
typedef unsigned char       UINT8;
#endif

#ifndef UINT32
typedef unsigned int        UINT32;
#endif


#ifndef INT32
typedef signed int          INT32;
#endif


#ifndef BYTE
typedef unsigned char       BYTE;
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
INT32 tcm_sms4_encrypt(BYTE *IV, BYTE *M, INT32 mLen, BYTE *S, BYTE *key);


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
INT32 tcm_sms4_decrypt(BYTE *IV, BYTE *M, INT32 mLen, BYTE *S, BYTE *key);



#ifdef	__cplusplus
}
#endif


#endif /* HEADER_TCM_SMS4_H */