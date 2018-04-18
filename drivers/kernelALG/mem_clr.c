/* mem_clr.c */

//#include <string.h>
#include <linux/string.h>
//#include "openssl/crypto.h"

unsigned char cleanse_ctr = 0;

void OPENSSL_cleanse(void *ptr, size_t len)
	{
	unsigned char *p = ptr;
	size_t loop = len;
	while(loop--)
		{
		*(p++) = cleanse_ctr;
		cleanse_ctr += (17 + (unsigned char)((int)p & 0xF));
		}
	if(memchr(ptr, cleanse_ctr, len))
		cleanse_ctr += 63;
	}
