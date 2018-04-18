/************************************************************
�ļ�����tcmalg.h
���ӿ⣺tcmalg.lib(��̬)
˵����ÿ�������ķ���ֵ��ʾ�ĺ�����ܲ�һ��������˵����ο������Ķ���˵��
�汾�޶���: linyang
ʱ�䣺
  2006.12.18
       ����sch��release�汾�³���.
	   ����ecc���ܴ���32�ֽڵ����ݳ���
  2006.12.18
      ʹ�ö�̬����c���Կ�(msvcrt.dll/msvcr71.dll)
  2006.12.21
		Ϊ֧�ֶ��̣߳���sms4�����е�ȫ����ԿΪ�ֲ�������
		����ģʽ��ջ�������
		ɾ����ʼ��sms4��Կ����
  2006.12.25
		Ϊ֧�ֲ���firmware����ԿЭ�̹��ܣ������������������ԿЭ�̹��ܺ�����
		��������ʹ�ã�������������;����֧��256bit�������ߣ���֧��192��������
  2006.12.31
		Ϊ֧�ֲ�����Կ���Ƿ�ƥ�䣬���������������ӿ����ڲ��ԡ�
		tcm_ecc_is_key_match ���ڲ��Թ�˽Կ�Ƿ�ƥ��
		tcm_ecc_point_from_privatekey ���ڴӹ�Կ�õ�˽Կ
		����֧��256���ߡ�
  2007.1.5
		�޸����ɵĴ�������32�ֽڶ���������������
  2007.3.29
		ԭ����SMS4ֻ�ܼ���512�ֽڳ��ȵ����ġ�����ȡ��������ơ�
************************************************************/


#ifndef HEADER_TCM_ALG_H
#define HEADER_TCM_ALG_H


#ifdef	__cplusplus
extern "C" {
#endif

#ifndef BOOL
typedef int BOOL;
#endif


/*************************************************************************
��Բ���߳�ʼ������
���� : tcm_ecc_init()
����ֵ���ɹ�����0�����򷵻�1
*************************************************************************/
int tcm_ecc_init();

/*************************************************************************
��Բ�����ͷ���Դ����
���� : tcm_ecc_release()
����ֵ���ɹ�����0�����򷵻�1
*************************************************************************/
int tcm_ecc_release();


/*************************************************************************
����EC�ļ����㷨
���� : tcm_ecc_encrypt()
�������  
		plaintext:		���ĵ�ַ
		uPlaintextLen:	���ĳ���
		pubkey:			��Կ��ַ
		uPubkeyLen��	��Կ����
		puCiphertextLen�������ciphertext�������Ĵ�С
������� 
		ciphertext: ���ĵ�ַ
		puCiphertextLen : ���ĳ��ȵ�ַָ�룬ָ����������ĳ���
����ֵ�����ܳɹ�����0�����򷵻�1
*************************************************************************/
int tcm_ecc_encrypt(unsigned char *plaintext, unsigned int uPlaintextLen, unsigned char *pubkey, unsigned int uPubkeyLen, unsigned char *ciphertext, unsigned int *puCiphertextLen);

/*************************************************************************
����EC�Ľ����㷨
func : tcm_ecc_decrypt()
�������  
 		ciphertext:		���ĵ�ַ
		uCiphertextLen:	���ĳ���
		prikey:			˽Կ��ַ
		uPrikeyLen��	˽Կ����
		puPlaintextLen: �����plaintext�������Ĵ�С

������� 
		plaintext:		�������
		puCiphertextLen : ���ĳ��ȵ�ַָ�룬ָ����������ĳ���

����ֵ�����ܳɹ�����0�����򷵻�1
*************************************************************************/
int tcm_ecc_decrypt(unsigned char *ciphertext, unsigned int uCiphertextLen, unsigned char *prikey, unsigned int uPrikeyLen, unsigned char *plaintext, unsigned int *puPlaintextLen);

/*************************************************************************
����EC��Բ���ߵ�����ǩ���㷨
���� : tcm_ecc_signature()
�������  
 		digest:			��Ҫ��ǩ����ժҪ
		uDigestLen:		ժҪ����
		prikey:			˽Կ��ַ
		uPrikeyLen��	˽Կ����
		puSigLen:		�����sig��������С

������� 
		sig:			���ǩ��
		puSigLen:		ǩ�����ȵ�ַָ�룬ָ�������ǩ������

����ֵ��ǩ���ɹ�����0�����򷵻�1
*************************************************************************/
int tcm_ecc_signature( unsigned char *digest, unsigned int uDigestLen,
					   unsigned char *prikey, unsigned int uPrikeyLen, 
					   /*out*/unsigned char *sig,
					   /*out*/unsigned int *puSigLen);

/*************************************************************************
����EC��Բ���ߵ�����ǩ����֤�㷨
���� : tcm_ecc_verify()
�������  
 		digest:			��Ҫ��ǩ����ժҪ
		uDigestLen:		ժҪ����
		sig:			ǩ��
		uSigLen:		ǩ���ĳ���
		pubkey:			��Կ
		uPubkeyLen:		��Կ����

������� 
		���ޣ�

����ֵ����֤�ɹ�����0�����򷵻�1
*************************************************************************/
int tcm_ecc_verify(unsigned char *digest, unsigned int uDigestLen, unsigned char *sig, unsigned int uSigLen, unsigned char *pubkey, unsigned int uPubkeyLen);



/*************************************************************************
����EC��Բ���ߵ��жϱ��ش��Ƿ��ʾ�����ϵ�һ���㣬
���� : tcm_ecc_is_point_valid()
�������  
 		point:			��Բ�����ϵĵ㣬�����Ƿ�ѹ����ʽ
		uPointLen:		��ĳ���
������� 
		���ޣ�

����ֵ����֤�ɹ�����TRUE�����򷵻�FALSE
����������ʹ��tcm_ecc_point_to_uncompressed�����õ���ѹ����ʽ�Ĵ�
*************************************************************************/
BOOL tcm_ecc_is_point_valid(unsigned char *point, unsigned int uPointLen);


/*************************************************************************
����EC��Բ���ߵ��жϱ��ش��Ƿ��ʾ�����ϵ�һ���㣬����ǣ�ת��Ϊ��ѹ����ʽ
���� : tcm_ecc_point_to_uncompressed()
�������  
 		point:			��Բ�����ϵĵ�
		uPointLen:		��ĳ���
		puUncompressedpointLen��	�����uncompressedpoint��������Сָ��

������� 
		uncompressedpoint:			��ķ�ѹ����ʽ
		puUncompressedpointLen��	����uncompressedpoint����������
����ֵ��ת���ɹ�����TRUE�����򷵻�FALSE
*************************************************************************/
BOOL tcm_ecc_point_to_uncompressed(unsigned char *point, unsigned int uPointLen,
								unsigned char *uncompressedpoint, unsigned int *puUncompressedpointLen);


/*************************************************************************
����EC��Բ������Կ�����ɺ���
���� : tcm_ecc_genkey()

�������
		���ޣ�
�������  
 		prikey:			˽Կ
		puPrikeyLen:	˽Կ��ַ
		pubkey:			��Կ
		puPubkeyLen:	��Կ��ַ	

����ֵ���ɹ�����0�����򷵻�1
*************************************************************************/
int tcm_ecc_genkey(unsigned char *prikey, unsigned int *puPrikeyLen, unsigned char *pubkey, unsigned int *puPubkeyLen);



/*************************************************************************
����EC�Ĺ��꽻����Կ�㷨��������firmware�Ĳ��Ժ�����ֻ����256bit���
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
					 );
/************************************************************************************/


/*************************************************************************
����EC��Բ���ߵĴ�˽Կ�õ���Կ
���� : tcm_ecc_point_from_privatekey()
�������  
 		prikey:			˽Կ
		uPrikeyLen:		˽Կ����
������� 
		pubkey:			��Կ
		puPubkeyLen:	��Կ����
����ֵ���ɹ�����0�����򷵻�1
*************************************************************************/
int tcm_ecc_point_from_privatekey(const unsigned char *prikey, const unsigned int uPrikeyLen, unsigned char *pubkey, unsigned int *puPubkeyLen);

/*************************************************************************
����EC��Բ���ߵ��жϹ�˽Կ���Ƿ�ƥ��
���� : tcm_ecc_is_match()
�������  
 		prikey:			��Բ�����ϵĵ㣬�����Ƿ�ѹ����ʽ
		uPrikeyLen:		˽Կ����
		pubkey:			��Կ
		uPubkeyLen:		��Կ����
������� 
		���ޣ�

����ֵ���ɹ�����TRUE�����򷵻�FALSE
������������ֻ���ڲ��ԡ�
*************************************************************************/
BOOL tcm_ecc_is_key_match(const unsigned char *prikey, const unsigned int uPrikeyLen, 
						  const unsigned char *pubkey, const unsigned int uPubkeyLen);

/************************************************************************************/
/************************************************************************************/
/* sch���� */
#ifndef uint8
#define uint8  unsigned char
#endif

#ifndef uint32
#define uint32 unsigned long int
#endif

typedef struct
{
    uint32 total[2];
    uint32 state[8];
    uint8 buffer[64];
}
sch_context;

/* sch */
void tcm_sch_starts( sch_context *ctx );
void tcm_sch_update( sch_context *ctx, uint8 *input, uint32 length );
void tcm_sch_finish( sch_context *ctx, uint8 digest[32] );

/*************************************************************************
���� : tcm_sch_hash()
�������  
		length:		��Ϣ����
		input:		��Ϣ

������� 
		digest:			���HASHֵ
����ֵ������ɹ�����0�����򷵻�1
*************************************************************************/
int tcm_sch_hash( unsigned int datalen_in, unsigned char *pdata_in, unsigned char digest[32]);


/*************************************************************************
hmac����
���� : tcm_hmac()
�������  
		text:			��Ϣ
		text_len:		��Ϣ����
 		key:			��Կ
		key_len:		��Կ����

������� 
		digest:			���HMACֵ

����ֵ������ɹ�����0�����򷵻�1
*************************************************************************/
int tcm_hmac(unsigned char *text, unsigned int text_len, unsigned char *key, unsigned int key_len, unsigned char digest[32]);


/*************************************************************************
��Կ��������
���� : tcm_kdf()
�������  
		klen:			��Ҫ����ĳ��ȣ�����ǰ����Ϊmask�������klen�Ŀռ�
		msgLen:			��Ϣ����
 		z:				�����������Ϣ
		zlen:			z�ĳ���

������� 
		mask:			����Ļ�����

����ֵ������ɹ�����0�����򷵻�1
*************************************************************************/
int tcm_kdf(/*out*/unsigned char *mask, /*in*/int klen, /*in*/unsigned char *z, /*in*/ int zlen);


/*************************************************************************
������Ϣ��hashֵ
���� : tcm_get_message_hash()
�������  
		msg:			��Ϣ
		msgLen:			��Ϣ����
 		userID:			�û���Ϣ
		uUserIDLen:		�û���Ϣ����
		pubkey:			��Կ��ַ
		uPubkeyLen:		��Կ����
		puDigestLen:	�����digest��������С

������� 
		digest:			���ժҪ
		puDigestLen;	�����digest����������

����ֵ������ɹ�����0�����򷵻�1
*************************************************************************/
int tcm_get_message_hash(unsigned char *msg, unsigned int msgLen,  
					   unsigned char  *userID, unsigned short int uUserIDLen, 
					   unsigned char *pubkey, unsigned int uPubkeyLen,
					   unsigned char *digest,
					   unsigned int *puDigestLen);


/*************************************************************************
����Za������ʹ��256bits��HASH�㷨
���� : tcm_get_usrinfo_value()
�������  
 		userID:			�û���Ϣ
		uUserIDLen:		�û���Ϣ����
		pubkey:			��Կ��ַ
		uPubkeyLen:		��Կ����

������� 
		digest:			���ժҪ

����ֵ������ɹ�����0�����򷵻�1
*************************************************************************/
int tcm_get_usrinfo_value(
					   unsigned char *userID, unsigned short int uUserIDLen, 
					   unsigned char *pubkey, unsigned int uPubkeyLen,
					   unsigned char digest[32]);


/************************************************************************************/
/************************************************************************************/
/************************************************************************************/
/* sms4�ӽ⺯�� */


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

/* sms4�Ŀ鳤 */
#define	SMS4_BLOCK_SIZE 16
/* sms4�����ܵ������ֽڳ��ȣ�ͬʱҲ����ECC */
#define SMS4_MAX_LEN	512

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


#endif	// HEADER_TCM_ALG_H
