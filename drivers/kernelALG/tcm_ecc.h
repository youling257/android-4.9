/* tcm_ecc.h*/
/* ����ĵ��еĺ�����Ҫ������ */

#ifndef HEADER_TCM_ECC_H
#define HEADER_TCM_ECC_H


#ifdef	__cplusplus
extern "C" {
#endif
#ifndef BOOL
typedef int BOOL;
#endif
/* 
1. ����ֵΪint���͵ĺ�����0��ʾ�ɹ���1��ʾʧ�ܡ�
2. ����ֵΪBOOL���͵ĺ�����TRUE��ʾ�ɹ���FALSE��ʾʧ�� 
*/


/*************************************************************************
��Բ���߳�ʼ������
���� : tcm_ecc_init()
����ֵ���ɹ�����0�����򷵻�1
*************************************************************************/
int tcm_ecc_init(void);

/*************************************************************************
��Բ�����ͷ���Դ����
���� : tcm_ecc_release()
����ֵ���ɹ�����0�����򷵻�1
*************************************************************************/
int tcm_ecc_release(void);


/* ���ڲ��Թ����㷨������256bit */
int tcm_ecc_init_test256(void);
/* ���ڲ��Թ����㷨������192bit */
int tcm_ecc_init_test192(void);



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
����EC��˽Կ�����㷨
func : ECC_gen_d()
�������  
 		prikey:			˽Կ������
		puPrikeyLen:	�����prikey����������

������� 
 		prikey:			���ص�˽Կ
		puPrikeyLen:	�����prikey��С

����ֵ�����ܳɹ�����0�����򷵻�1
*************************************************************************/
int ECC_gen_d(unsigned char *prikey, unsigned int *puPrikeyLen);

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

#ifdef	__cplusplus
}
#endif


#endif	// HEADER_TCM_ECC_H
