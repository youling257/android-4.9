/******************************************
File	:debug.c 
Author	:linyang
Date	:11/21/2006
comment :���ڵ��Դ���
******************************************/

#include "debug.h"

void ASSERT(int f)
{
	if( f==1 )
	{
		return;
	}
	else
	{
// 		__asm{
// 			int 3;}
	}
}

