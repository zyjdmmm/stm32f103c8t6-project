
/*
//�������ҳ�Ʒ
//����ϵ�п�����Ӧ�ó���
//��ע΢�Ź��ںţ����ҵ���
//���ҿ������������� www.DoYoung.net/YT 
//������ѿ����н�ѧ��Ƶ�����ؼ������ϣ�������������
//�������ݾ��� ����������ҳ www.doyoung.net
*/

/*
���޸���־��


*/



#include "flash.h"

u8 alhour[2];		   //�������ӵ����飬alhour[3]alhour[4]�洢�����¶ȵ�����
u8 almin[2];			//almin[3]almin[4]�洢�����¶ȵ�С��
//FLASHд������
void FLASH_W(u32 add,u16 dat){ //����1��32λFLASH��ַ������2��16λ����
//	 RCC_HSICmd(ENABLE); //��HSIʱ��
	 FLASH_Unlock();  //����FLASH��̲���������
     FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//�����־λ
     FLASH_ErasePage(add);     //����ָ����ַҳ
     FLASH_ProgramHalfWord(FLASH_START_ADDR,dat); //��ָ��ҳ��addr��ַ��ʼд  1��Ҫд���λ�ã�2��dat����Ҫд�������
     FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//�����־λ
     FLASH_Lock();    //����FLASH��̲���������
}

//FLASH��������
u16 FLASH_R(u32 add){ //����1��32λ����FLASH��ַ������ֵ��16λ����
	u16 a;
    a = *(u16*)(add);//��ָ��ҳ��addr��ַ��ʼ��
return a;
}

void ALFLASH_R (void)	   //������FLASH����
{


}
void ALFLASH_W (void)	   //д����FLASH����
{
u8 i;
u16 t;

FLASH_Unlock();  //����FLASH��̲���������
FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//�����־λ
FLASH_ErasePage(FLASH_START_ADDR);     //����ָ����ַҳ

for(i=0;i<2;i++)
{
	t=alhour[i]*0x100+almin[i];			 //ʱ���������t
	FLASH_ProgramHalfWord(FLASH_START_ADDR+i*2,t); //��ָ��ҳ��addr��ַ��ʼд
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//�����־λ	  ?
}
FLASH_Lock();    //����FLASH��̲���������


}


//void ALFLASH_W (void){	//д���ӵ�FLASH����
//	u16 t;
//	u8 i;
//	FLASH_Unlock();  //����FLASH��̲���������
//	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//�����־λ
//	FLASH_ErasePage(FLASH_START_ADDR);     //����ָ����ַҳ
//	for(i=0;i<2;i++){
//		t=alhour[i]*0x100+almin[i];
//		FLASH_ProgramHalfWord(FLASH_START_ADDR+i*2,t); //��ָ��ҳ��addr��ַ��ʼд
//		FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//�����־λ
//	}
//	FLASH_Lock();    //����FLASH��̲���������
//
//}





/*********************************************************************************************
 * �������� www.DoYoung.net
 * ���ҵ��� www.DoYoung.net/YT 
*********************************************************************************************/































