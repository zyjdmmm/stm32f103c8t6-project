#ifndef __FLASH_H
#define __FLASH_H 			   
#include "sys.h"
 
#define FLASH_START_ADDR  0x0801f000	  //д�����ʼ��ַ	  ��øú���4����

extern u8 alhour[2];		   //�������ӵ�����
extern u8 almin[2];

void FLASH_W(u32 add,u16 dat);
u16 FLASH_R(u32 add);
void ALFLASH_W(void);
#endif





























