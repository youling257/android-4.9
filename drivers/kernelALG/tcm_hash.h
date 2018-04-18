/* tcm_hash.*/
/* ����ĵ��еĺ�����Ҫ������ */

#ifndef HEADER_TCM_HASH_H
#define HEADER_TCM_HASH_H
#include "sch.h"

#ifdef	__cplusplus
extern "C" {
#endif

void tcm_sch_starts( sch_context *ctx );
void tcm_sch_update( sch_context *ctx, uint8 *input, uint32 length );
void tcm_sch_finish( sch_context *ctx, uint8 digest[32] );


/*��ϣ����*/
/*************************************************************************
���� : tcm_sch_hash()
�������  
		length:		��Ϣ����
		input:		��Ϣ

������� 
		digest:			���HASHֵ
����ֵ������ɹ�����0�����򷵻�1
*************************************************************************/
int tcm_sch_hash( unsigned int length, unsigned char *input, unsigned char digest[32]);
int tcm_sch_192( unsigned int length, unsigned char *input, unsigned char digest[24]);
int tcm_sch_256( unsigned int length, unsigned char *input, unsigned char digest[32]);


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


#ifdef	__cplusplus
}
#endif


#endif	// HEADER_TCM_HASH_H






