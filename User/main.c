/*********************************************************************************************

��������	��Ŀ1��������
��д�ˣ�	��giao	
��дʱ�䣺	2021.2.2
Ӳ��֧�֣�	STM32F103C8   �ⲿ����8MHz RCC����������Ƶ72MHz��  


*********************************************************************************************/
#include "stm32f10x.h" //STM32ͷ�ļ�
#include "sys.h"
#include "delay.h"
#include "rtc.h"
#include "TM1640.h"
#include "lm75a.h"
#include "touch_key.h"
#include "buzzer.h"
#include "flash.h" 
#include "relay.h"

#define S 500		   //��˸�ٶ�
#define KEY_SPEED1	100	  //������ʱ�䳤�ȣ���λ10mS��
#define KEY_SPEED2	100	  //�Ӽ��ٶ�


int main (void){//������
//	u8 c=0x01;
	u8 MENU=0;//�˵�ֵ
	u8 buffer[3];  //�¶ȱ���
	u16 i;//ʱ����˸
	u8 rup;//ʱ��д����
 	u8 a=0,c=0;
	u8 MENU2=0;	//��n������
	u8 t=0;
	u16 m=0;	//�����������������־λ
	u16 d=0;//0��1��2�˵�ѭ����ת������		   //������������u16����Ȼλ�����������



    RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 
	RELAY_Init();//�̵�����ʼ��
	delay_ms(200);
	RTC_Config();  //RTC��ʼ��
	TM1640_Init(); //TM1640��ʼ��
	I2C_Configuration();//I2C��ʼ��
	TOUCH_KEY_Init();//������ʼ��
    BUZZER_Init();//��ʼ��
    BUZZER_BEEP2();//����
	rup=0;

	RELAY_1(1);	 //c=0�̵����ſ���c=1�̵�������
	RELAY_2(1);

	for(i=0;i<2;i++)				 //ѭ��2�Σ�2��������														  ���ݶ���
		{
		t = FLASH_R(FLASH_START_ADDR+i*2);//��ָ��ҳ�ĵ�ַ��FLASH	
		alhour[i]=t/0x100;				 //�õ��߰�λ�ͳ�2^8
		almin[i]=t%0x100;
		}
	if(alhour[0]>23)
	{
	for(i=0;i<2;i++)
	   {
	   	alhour[i]=24;
    	almin[i]=0;  
	   }
	ALFLASH_W();
	}


	while(1){
			 if(rup==1){
			 rup=0;
			 rsec=0;
			 RTC_Set(ryear,rmon,rday,rhour,rmin,rsec); //ʱ��д���ж�			 			 
			 }
			 if(MENU<3||MENU>8){
			 RTC_Get();
             LM75A_GetTemp(buffer); //��ȡLM75A���¶�����
			 }



			if(MENU==0)
		   	 {		         																							 //0ԭʼʱ��������
				TM1640_display(0,ryear%100/10);	//��
				TM1640_display(1,ryear%100%10);
				TM1640_display(2,21); //-
				TM1640_display(3,rmon/10);
				TM1640_display(4,rmon%10);	// ��
				TM1640_display(5,21); //-
				TM1640_display(6,rday/10); //  ��
				TM1640_display(7,rday%10);
	
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){ //�����������ĵ�ƽ
				MENU=0;
				BUZZER_BEEP1();//��һ��																		
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){ //�����������ĵ�ƽ
				MENU=1;
				BUZZER_BEEP1();//��һ��	
		    	while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //�����������ĵ�ƽ
				MENU=2;
				BUZZER_BEEP1();//��һ��	
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //�����������ĵ�ƽ
				MENU=3;
				BUZZER_BEEP1();//��һ��	
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));			
			    }//if(MENU==0) 

		    }
    		if(MENU==1)																			     //1�¶Ȳ˵�
		     	{
				TM1640_display(0,20);	//��
				TM1640_display(1,20);
				TM1640_display(2,20);
				if(buffer[0])TM1640_display(3,21);	 //������
				else TM1640_display(3,20);
				TM1640_display(4,buffer[1]%100/10);	//�¶�
				TM1640_display(5,buffer[1]%100%10+10);
				TM1640_display(6,buffer[2]%100/10); //
				TM1640_display(7,22);	   //c
	
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){ //�����������ĵ�ƽ
				MENU=0;
				BUZZER_BEEP1();//��һ��	
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){ //�����������ĵ�ƽ
				MENU=1;
				BUZZER_BEEP1();//��һ��	
		    	while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //�����������ĵ�ƽ
				MENU=2;
				BUZZER_BEEP1();//��һ��	
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //�����������ĵ�ƽ
				MENU=3;
				BUZZER_BEEP1();//��һ��	
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
				}								
     	     	}//	if(MENU==1)

			if(MENU==2)																								   	   //2ʱ����
			    {
				RTC_Get();   			//��ˢһ��																	
				TM1640_display(0,20);	//��
				TM1640_display(1,20);
				TM1640_display(2,rhour/10); //ʱ
				TM1640_display(3,rhour%10+10);
				TM1640_display(4,rmin/10);	//��
				TM1640_display(5,rmin%10+10);
				TM1640_display(6,rsec/10); //��
				TM1640_display(7,rsec%10);
	
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){ //�����������ĵ�ƽ
				MENU=0;
				BUZZER_BEEP1();//��һ��	
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){ //�����������ĵ�ƽ
				MENU=1;
				BUZZER_BEEP1();//��һ��	
		    	while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //�����������ĵ�ƽ
				MENU=2;
				BUZZER_BEEP1();//��һ��	
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));	
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //�����������ĵ�ƽ
				MENU=3;
				BUZZER_BEEP1();//��һ��	
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
				}
				}//if(MENU==2

		   if(MENU<3)																				   //012�˵�ѭ����ʾ
		   {
		   	d++;
			if(d==2000){MENU=0;} 
			if(d==4000){MENU=1;} 
			if(d==6000){MENU=2;d=0;}	   //if�������д��{}
		   }

			if(MENU==3){		 																	      	  //3set��ʾ
			TM1640_display(0,23);	//set
			TM1640_display(1,24);
			TM1640_display(2,25);
            TM1640_display(3,20);
			TM1640_display(4,20);	//�¶�
			TM1640_display(5,20);
			TM1640_display(6,20); //
			TM1640_display(7,20);	   //c
			BUZZER_BEEP3();
			MENU2=0;
			MENU=100;

	    	}//if(MENU==3)

			if(MENU==4)
			{																					  //4��˸��
			i++;
			if(i>S)i=0;
			if(i>S/2){		 	
				TM1640_display(0,ryear%100/10);	//��								    
				TM1640_display(1,ryear%100%10);
			}else{
				TM1640_display(0,20);	//��
				TM1640_display(1,20);
			}
			TM1640_display(2,21); //-
			TM1640_display(3,rmon/10);
			TM1640_display(4,rmon%10);	// ��
			TM1640_display(5,21); //-
			TM1640_display(6,rday/10); //  ��
			TM1640_display(7,rday%10);

 			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))
			{                                                                        //�жϳ��̼�
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEY_SPEED1){ //ѭ���жϳ�������ʱ��ת
					c++;delay_ms(10); //�����жϵļ�ʱ
				}
				if(c>=KEY_SPEED1){ //��������
					//������ִ�еĳ���ŵ��˴�
					BUZZER_BEEP1();//��һ��
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						ryear++;
						if(ryear>3000)ryear=2021;
						TM1640_display(0,ryear%100/10);	//��  2018 
						TM1640_display(1,ryear%100%10);
						delay_ms(KEY_SPEED2); //��ʱ ����	                                                        ?
					}
				}else{ //��������
					if(a==0){ //�жϵ���
						//������ִ�еĳ���ŵ��˴�
						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						ryear++;
						if(ryear>3000)ryear=2021;
						BUZZER_BEEP1();//��һ��
					}
				}
				a=0;c=0; //������0
			}


			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))
			{																 //�����������ĵ�ƽ
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEY_SPEED1){ //ѭ���жϳ�������ʱ��ת
					c++;delay_ms(10); //�����жϵļ�ʱ
				}
				if(c>=KEY_SPEED1){ //��������
					//������ִ�еĳ���ŵ��˴�
					BUZZER_BEEP1();//��һ��
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						ryear--;
						if(ryear<2021)ryear=3000;
						TM1640_display(0,ryear%100/10);	//��  2018 
						TM1640_display(1,ryear%100%10);
						delay_ms(KEY_SPEED2); //��ʱ ����
					}
				}else{ //��������
					if(a==0){ //�жϵ���
						//������ִ�еĳ���ŵ��˴�
						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						ryear--;
						if(ryear<2021)ryear=3000;
						BUZZER_BEEP1();//��һ��
					}
				}
				a=0;c=0; //������0
			}


			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //�����������ĵ�ƽ
			MENU=5;
			rup=1; 
			BUZZER_BEEP1();//��һ��                                                        //ʱ���޸ı�־λ
			while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));	
			}

			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //�����������ĵ�ƽ
			MENU=0;
			rup=1;
			BUZZER_BEEP1();//��һ��                                                         //ʱ���޸ı�־λ
			while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
			}

			}//if(MENU==3)






			if(MENU==5){			   //																	  //5��˸��
			i++;
			if(i>S)i=0;
		 	
			TM1640_display(0,ryear%100/10);	//��
			TM1640_display(1,ryear%100%10);

			TM1640_display(2,21); //-
			if(i>S/2) {
				TM1640_display(3,20);
				TM1640_display(4,20);	// ��
			}else{
				TM1640_display(3,rmon/10);
				TM1640_display(4,rmon%10);	// ��
			}

			TM1640_display(5,21); //-
			TM1640_display(6,rday/10); //  ��
			TM1640_display(7,rday%10);

 			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))
			{                                                                        //�жϳ��̼�
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEY_SPEED1){ //ѭ���жϳ�������ʱ��ת
					c++;delay_ms(10); //�����жϵļ�ʱ
				}
				if(c>=KEY_SPEED1){ //��������
					//������ִ�еĳ���ŵ��˴�
					BUZZER_BEEP1();//��һ��
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						rmon++;
						if(rmon>12)rmon=1;
						TM1640_display(3,rmon/10);
						TM1640_display(4,rmon%10);	// ��
						delay_ms(KEY_SPEED2); //��ʱ ����	                                                        ?
					}
				}else{ //��������
					if(a==0)
					 { //�жϵ���
						//������ִ�еĳ���ŵ��˴�
						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						rmon++;
						if(rmon>12)rmon=1;
						BUZZER_BEEP1();//��һ��
					}
				}
				a=0;c=0; //������0
			}


			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))
			{																 //�����������ĵ�ƽ
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEY_SPEED1){ //ѭ���жϳ�������ʱ��ת
					c++;delay_ms(10); //�����жϵļ�ʱ
				}
				if(c>=KEY_SPEED1){ //��������
					//������ִ�еĳ���ŵ��˴�
					BUZZER_BEEP1();//��һ��
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						rmon--;
						if(rmon<1)rmon=12;
						TM1640_display(3,rmon/10);
						TM1640_display(4,rmon%10);	// ��
						delay_ms(KEY_SPEED2); //��ʱ ����
					}
				}else{ //��������
					if(a==0){ //�жϵ���
						//������ִ�еĳ���ŵ��˴�
						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						rmon--;
						if(rmon<1)rmon=12;
						BUZZER_BEEP1();//��һ��
					}
				}
				a=0;c=0; //������0
			}

			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //�����������ĵ�ƽ
			MENU=6;
			rup=1;
			BUZZER_BEEP1();//��һ��                                                         //ʱ���޸ı�־λ
			while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));	
			}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //�����������ĵ�ƽ
			MENU=0;
			rup=1; 
			BUZZER_BEEP1();//��һ��                                                        //ʱ���޸ı�־λ
			while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
			}

			}//if(MENU==3)

			if(MENU==6){																					//6��˸��
			i++;
			if(i>S)i=0;
		 	
			TM1640_display(0,ryear%100/10);	//��
			TM1640_display(1,ryear%100%10);

			TM1640_display(2,21); //-
			TM1640_display(3,rmon/10);
			TM1640_display(4,rmon%10);	// ��

			TM1640_display(5,21); //-
			if(i>S/2){
			TM1640_display(6,rday/10); //  ��
			TM1640_display(7,rday%10);
			}else{
			TM1640_display(6,20); //  ��
			TM1640_display(7,20);
			}
 			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))
			{                                                                        //�жϳ��̼�
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEY_SPEED1){ //ѭ���жϳ�������ʱ��ת
					c++;delay_ms(10); //�����жϵļ�ʱ
				}
				if(c>=KEY_SPEED1){ //��������
					//������ִ�еĳ���ŵ��˴�
					BUZZER_BEEP1();//��һ��
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						rday++;
						if(rday>31)rday=1;
						TM1640_display(6,rday/10); //  ��
						TM1640_display(7,rday%10);
						delay_ms(KEY_SPEED2); //��ʱ ����	                                                        ?
					}
				}else{ //��������
					if(a==0){ //�жϵ���
						//������ִ�еĳ���ŵ��˴�
						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						rday++;
						if(rday>31)rday=1;
						BUZZER_BEEP1();//��һ��
					}
				}
				a=0;c=0; //������0
			}


			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))
			{																 //�����������ĵ�ƽ
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEY_SPEED1){ //ѭ���жϳ�������ʱ��ת
					c++;delay_ms(10); //�����жϵļ�ʱ
				}
				if(c>=KEY_SPEED1){ //��������
					//������ִ�еĳ���ŵ��˴�
					BUZZER_BEEP1();//��һ��
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						rday--;
						if(rday<1)rday=31;
						TM1640_display(6,rday/10); //  ��
						TM1640_display(7,rday%10);
						delay_ms(KEY_SPEED2); //��ʱ ����
					}
				}else{ //��������
					if(a==0){ //�жϵ���
						//������ִ�еĳ���ŵ��˴�
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						rday--;
						if(rday<1)rday=31;
						BUZZER_BEEP1();//��һ��
					}
				}
				a=0;c=0; //������0
			}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //�����������ĵ�ƽ
			MENU=7;
			rup=1; 
			BUZZER_BEEP1();//��һ��                                                        //ʱ���޸ı�־λ
			while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));	
			}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //�����������ĵ�ƽ
			MENU=0;
			rup=1;  
			BUZZER_BEEP1();//��һ��                                                       //ʱ���޸ı�־λ
			while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
			}									   

			}//if(MENU==3)

		if(MENU==7){																						//7��˸ʱ
			i++;
			TM1640_display(0,20);	//��
			TM1640_display(1,20);
			if(i>S/2){
				TM1640_display(2,rhour/10); //ʱ
				TM1640_display(3,rhour%10+10);
			}else{
				TM1640_display(2,20);	//�� 
				TM1640_display(3,20);
			}
			TM1640_display(4,rmin/10);	//��
			TM1640_display(5,rmin%10+10);
			TM1640_display(6,rsec/10); //��
			TM1640_display(7,rsec%10);
			if(i>S)i=0; //���������ٶ�

 			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))
			{                                                                        //�жϳ��̼�
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEY_SPEED1){ //ѭ���жϳ�������ʱ��ת
					c++;delay_ms(10); //�����жϵļ�ʱ
				}
				if(c>=KEY_SPEED1){ //��������
					//������ִ�еĳ���ŵ��˴�
					BUZZER_BEEP1();//��һ��
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						rhour++;
						if(rhour>23)rhour=0;
						TM1640_display(2,rhour/10); //ʱ
						TM1640_display(3,rhour%10+10);
						delay_ms(KEY_SPEED2); //��ʱ ����	                                                        ?
					}
				}else{ //��������
					if(a==0){ //�жϵ���
						//������ִ�еĳ���ŵ��˴�
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						rhour++;
						if(rhour>23)rhour=0;
						BUZZER_BEEP1();//��һ��
					}
				}
				a=0;c=0; //������0
			}


			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))
			{																 //�����������ĵ�ƽ
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEY_SPEED1){ //ѭ���жϳ�������ʱ��ת
					c++;delay_ms(10); //�����жϵļ�ʱ
				}
				if(c>=KEY_SPEED1){ //��������
					//������ִ�еĳ���ŵ��˴�
					BUZZER_BEEP1();//��һ��
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						if(rhour==0)rhour=24;
						rhour--;
						TM1640_display(6,rday/10); //  ��
						TM1640_display(7,rday%10);
						delay_ms(KEY_SPEED2); //��ʱ ����
					}
				}else{ //��������
					if(a==0){ //�жϵ���
						//������ִ�еĳ���ŵ��˴�
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						if(rhour==0)rhour=24;
						rhour--;
						BUZZER_BEEP1();//��һ��		 
					}
				}
				a=0;c=0; //������0
			}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //�����������ĵ�ƽ
				MENU=8;
				rup=1;   
				BUZZER_BEEP1();//��һ��                                                      //ʱ���޸ı�־λ
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
	 		}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //�����������ĵ�ƽ
				MENU=1;
				rup=1;  
				BUZZER_BEEP1();//��һ��                                                       //ʱ���޸ı�־λ
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
	 		}
		}

		if(MENU==8){																					  //8��˸��
			i++;
			TM1640_display(0,20);	//��
			TM1640_display(1,20);

			TM1640_display(2,rhour/10); //ʱ
			TM1640_display(3,rhour%10+10);
			if(i>S/2){
			TM1640_display(4,20);	//��
			TM1640_display(5,20);
			}else{
			TM1640_display(4,rmin/10);	//��
			TM1640_display(5,rmin%10+10);
			}

			TM1640_display(6,rsec/10); //��
			TM1640_display(7,rsec%10);
			if(i>S)i=0; //���������ٶ�

 			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))
			{                                                                        //�жϳ��̼�
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEY_SPEED1){ //ѭ���жϳ�������ʱ��ת
					c++;delay_ms(10); //�����жϵļ�ʱ
				}
				if(c>=KEY_SPEED1){ //��������
				BUZZER_BEEP1();//��һ��
					//������ִ�еĳ���ŵ��˴�
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						rmin++;
						if(rmin>59)rmin=0;
						TM1640_display(4,rmin/10);	//��
						TM1640_display(5,rmin%10+10);
						delay_ms(KEY_SPEED2); //��ʱ ����	                                                        ?
					}
				}else{ //��������
					if(a==0){ //�жϵ���
						//������ִ�еĳ���ŵ��˴�
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						rmin++;
						if(rmin>59)rmin=0;
						BUZZER_BEEP1();//��һ��
					}
				}
				a=0;c=0; //������0
			}


			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))
			{																 //�����������ĵ�ƽ
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEY_SPEED1){ //ѭ���жϳ�������ʱ��ת
					c++;delay_ms(10); //�����жϵļ�ʱ
				}
				if(c>=KEY_SPEED1){ //��������
					//������ִ�еĳ���ŵ��˴�
					BUZZER_BEEP1();//��һ��
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ

						if(rmin<1)rmin=60;
						rmin--;
						TM1640_display(4,rmin/10);	//��
						TM1640_display(5,rmin%10+10);
						delay_ms(KEY_SPEED2); //��ʱ ����
					}
				}else{ //��������
					if(a==0){ //�жϵ���
						//������ִ�еĳ���ŵ��˴�
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						if(rmin<1)rmin=60;
						rmin--;
						BUZZER_BEEP1();//��һ��
					}
				}
				a=0;c=0; //������0
			}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //�����������ĵ�ƽ
				MENU=4;
				rup=1;  
				BUZZER_BEEP1();//��һ��                                                       //ʱ���޸ı�־λ
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
	 		}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //�����������ĵ�ƽ
				MENU=0;
				rup=1;  
				BUZZER_BEEP1();//��һ��                                                       //ʱ���޸ı�־λ
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
	 		}
		}

			if(MENU==100){
			
			if(MENU2==0)																							 //100set�ж�
				{
				TM1640_display(0,23);	
				TM1640_display(1,24);
				TM1640_display(2,25);
				}
			if(MENU2>=1&&MENU2<=2)	  //2��������ʾ
				{
				TM1640_display(0,26);	
				TM1640_display(1,27);
				TM1640_display(2,MENU2);
				}
			if(MENU2>=3&&MENU2<=4)		//�����¶ȿ���TC1~2
				{
				TM1640_display(0,25);	
				TM1640_display(1,22);
				TM1640_display(2,MENU2-2);						
				}		
			
	if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))
			{                                                                        //�жϳ��̼�
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEY_SPEED1){ //ѭ���жϳ�������ʱ��ת
					c++;delay_ms(10); //�����жϵļ�ʱ
				}
				if(c>=KEY_SPEED1){ //��������
				BUZZER_BEEP1();//��һ��
					//������ִ�еĳ���ŵ��˴�
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						MENU2++;
						if(MENU2>4)MENU2=0;

						delay_ms(KEY_SPEED2); //��ʱ ����	                                                        ?
					}
				}else{ //��������
					if(a==0){ //�жϵ���
						//������ִ�еĳ���ŵ��˴�
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						MENU2++;
						if(MENU2>4)MENU2=0;
						BUZZER_BEEP1();//��һ��
					}
				}
				a=0;c=0; //������0
			}


			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))
			{																 //�����������ĵ�ƽ
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEY_SPEED1){ //ѭ���жϳ�������ʱ��ת
					c++;delay_ms(10); //�����жϵļ�ʱ
				}
				if(c>=KEY_SPEED1){ //��������
					//������ִ�еĳ���ŵ��˴�
					BUZZER_BEEP1();//��һ��
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ

						if(MENU2==0)MENU2=5;
						MENU2--;

						delay_ms(KEY_SPEED2); //��ʱ ����
					}
				}else{ //��������
					if(a==0){ //�жϵ���
						//������ִ�еĳ���ŵ��˴�
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						if(MENU2==0)MENU2=5;
						MENU2--;
						BUZZER_BEEP1();//��һ��
					}
				}
				a=0;c=0; //������0
			}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //�����������ĵ�ƽ
				MENU=101+MENU2;  
				BUZZER_BEEP1();//��һ��                                                       //ʱ���޸ı�־λ
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
	 		}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //�����������ĵ�ƽ
				MENU=0; 
				BUZZER_BEEP1();//��һ��                                                       //ʱ���޸ı�־λ
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
	 		}
			}//if(MENU==3)

			if(MENU==101){MENU=4;}	 																	       	  //101��ת��4
			if(MENU>=102&&MENU<=103){MENU=150;}	//2������	 											    	   //102��103��ת��150
			if(MENU>=104&&MENU<=105){MENU=210;}	//2���¶ȿ���TC1~2               					     	   //110��111��ת��210




			if(MENU==150)
			{
			if(almin[MENU2-1]>59)almin[MENU2-1]=0;			//���������������					               	   //150����ʱ�����hour
			i++;
			if(alhour[MENU2-1]==24)		   //24��ʾOFF
				{
				TM1640_display(0,26);	
				TM1640_display(1,27);
				TM1640_display(2,MENU2);
				TM1640_display(3,20);
				TM1640_display(4,20);	

				if(i>S/2)
				    {
					TM1640_display(5,28);	
					TM1640_display(6,29);
					TM1640_display(7,29);
					}
				else
					{
					TM1640_display(5,20);	
					TM1640_display(6,20);
					TM1640_display(7,20);
					}	

				}
			else						  //��24������ʾ
				{				
				TM1640_display(0,26);	
				TM1640_display(1,27);
				TM1640_display(2,MENU2);
				if(i>S/2){
					TM1640_display(4,alhour[MENU2-1]/10); //ʱ
					TM1640_display(5,alhour[MENU2-1]%10+10);
				}else{
					TM1640_display(4,20);	//�� 
					TM1640_display(5,20);
				}
				TM1640_display(6,almin[MENU2-1]/10);	//��
				TM1640_display(7,almin[MENU2-1]%10+10);
				}

			if(i>S)i=0; //���������ٶ�

 			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))
			{                                                                        //�жϳ��̼�
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEY_SPEED1){ //ѭ���жϳ�������ʱ��ת
					c++;delay_ms(10); //�����жϵļ�ʱ
				}
				if(c>=KEY_SPEED1){ //��������
					//������ִ�еĳ���ŵ��˴�
					BUZZER_BEEP1();//��һ��
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						alhour[MENU2-1]++; //1�������� alhour[0]�洢��2�������� alhour[1]�洢
						if(alhour[MENU2-1]>24)alhour[MENU2-1]=0;
						TM1640_display(4,alhour[MENU2-1]/10); //ʱ
						TM1640_display(5,alhour[MENU2-1]%10+10);
						delay_ms(KEY_SPEED2); //��ʱ ����	                                                        ?
					}
				}else{ //��������
					if(a==0){ //�жϵ���
						//������ִ�еĳ���ŵ��˴�
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						alhour[MENU2-1]++;
						if(alhour[MENU2-1]>24)alhour[MENU2-1]=0;
						BUZZER_BEEP1();//��һ��
					}
				}
				a=0;c=0; //������0
			}


			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))
			{																 //�����������ĵ�ƽ
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEY_SPEED1){ //ѭ���жϳ�������ʱ��ת
					c++;delay_ms(10); //�����жϵļ�ʱ
				}
				if(c>=KEY_SPEED1){ //��������
					//������ִ�еĳ���ŵ��˴�
					BUZZER_BEEP1();//��һ��
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						if(alhour[MENU2-1]==0)alhour[MENU2-1]=25;
						alhour[MENU2-1]--;
						TM1640_display(4,alhour[MENU2-1]/10); //ʱ
						TM1640_display(5,alhour[MENU2-1]%10+10);
						delay_ms(KEY_SPEED2); //��ʱ ����
					}
				}else{ //��������
					if(a==0){ //�жϵ���
						//������ִ�еĳ���ŵ��˴�
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						if(alhour[MENU2-1]==0)alhour[MENU2-1]=25;
						alhour[MENU2-1]--;
						BUZZER_BEEP1();//��һ��		 
					}
				}
				a=0;c=0; //������0
			}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //�����������ĵ�ƽ
			if(alhour[MENU2-1]!=24)	MENU=151;
				BUZZER_BEEP1();//��һ�� 
				ALFLASH_W ();                                                     //ʱ���޸ı�־λ
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
	 		}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //�����������ĵ�ƽ
				MENU=0;  
				BUZZER_BEEP1();//��һ��
				ALFLASH_W ();    				                            
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
	 		}
		}

			if(MENU==151)
			{																	              //151����ʱ�����min
			i++;
				TM1640_display(0,26);	
				TM1640_display(1,27);
				TM1640_display(2,MENU2);
				TM1640_display(4,alhour[MENU2-1]/10); //ʱ
				TM1640_display(5,alhour[MENU2-1]%10+10);
				if(i>S/2){
				TM1640_display(6,almin[MENU2-1]/10);	//��
				TM1640_display(7,almin[MENU2-1]%10);
				}else{
				TM1640_display(6,20);	//��
				TM1640_display(7,20);								
				}
			if(i>S)i=0; //���������ٶ�

 			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))
			{                                                                        //�жϳ��̼�
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEY_SPEED1){ //ѭ���жϳ�������ʱ��ת
					c++;delay_ms(10); //�����жϵļ�ʱ
				}
				if(c>=KEY_SPEED1){ //��������
					//������ִ�еĳ���ŵ��˴�
					BUZZER_BEEP1();//��һ��
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						almin[MENU2-1]++; //1�������� alhour[0]�洢��2�������� alhour[1]�洢
						if(almin[MENU2-1]>59)almin[MENU2-1]=0;
						TM1640_display(6,almin[MENU2-1]/10);	//��
						TM1640_display(7,almin[MENU2-1]%10);
						delay_ms(KEY_SPEED2); //��ʱ ����	                                                        ?
					}
				}else{ //��������
					if(a==0){ //�жϵ���
						//������ִ�еĳ���ŵ��˴�
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						almin[MENU2-1]++;
						if(almin[MENU2-1]>59)almin[MENU2-1]=0;
						BUZZER_BEEP1();//��һ��
					}
				}
				a=0;c=0; //������0
			}


			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))
			{																 //�����������ĵ�ƽ
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEY_SPEED1){ //ѭ���жϳ�������ʱ��ת
					c++;delay_ms(10); //�����жϵļ�ʱ
				}
				if(c>=KEY_SPEED1){ //��������
					//������ִ�еĳ���ŵ��˴�
					BUZZER_BEEP1();//��һ��
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						if(almin[MENU2-1]==0)almin[MENU2-1]=60;
						almin[MENU2-1]--;																								
						TM1640_display(6,almin[MENU2-1]/10);	//��
						TM1640_display(7,almin[MENU2-1]%10);
						delay_ms(KEY_SPEED2); //��ʱ ����
					}
				}else{ //��������
					if(a==0){ //�жϵ���
						//������ִ�еĳ���ŵ��˴�
//						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
						if(almin[MENU2-1]==0)almin[MENU2-1]=60;
						almin[MENU2-1]--;
						BUZZER_BEEP1();//��һ��		 
					}
				}
				a=0;c=0; //������0
			}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //�����������ĵ�ƽ
				MENU=150; 
				BUZZER_BEEP1();//��һ��                                                      //ʱ���޸ı�־λ
				ALFLASH_W ();    
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
	 		}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //�����������ĵ�ƽ
				MENU=0; 
				BUZZER_BEEP1();//��һ��                                                       //ʱ���޸ı�־λ
				ALFLASH_W ();    
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
	 		}
		}


		   if(MENU<3)																					  	//<3�������𣬼��ÿ�����ӣ���ת200
		   {
		   for(i=0;i<2;i++)																				
		   if(alhour[i]==rhour&&almin[i]==rmin&&rsec==0)  //���������ʱ���򣬶���Ҫ����һ�Σ���Ȼrsec==0��ʱ�̻���
		   		{
				MENU=200;
				c=i;
				}						 
		   }

		  if(MENU==200)																					   //���������޲�����ʱ��ת
		  	{
			m++;
			if(m==500){m=0;MENU=0;}	   //��m��ʱ�ﵽ����תMENU1
			
			}


		   if(MENU==200)																				  	//200���������� 
		   {
			BUZZER_BEEP1();
			i++;
			if(i>2)
				{
				TM1640_display(0,26);	
				TM1640_display(1,27);
				TM1640_display(2,c+1);
				TM1640_display(3,20);
				TM1640_display(4,alhour[c]/10); //ʱ
				TM1640_display(5,alhour[c]%10+10);
				TM1640_display(6,almin[c]/10);	//��
				TM1640_display(7,almin[c]%10+10);
				}
			else
				{
				TM1640_display(0,20);	
				TM1640_display(1,20);
				TM1640_display(2,20);
				TM1640_display(3,20);
				TM1640_display(4,20); //ʱ
				TM1640_display(5,20);
				TM1640_display(6,20);	//��
				TM1640_display(7,20);				
				}
			if(i>4)	i=0;
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))			 //��������
				{ 
				MENU=0;   
				BUZZER_BEEP1();//��һ��                                                
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));
			    }
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))			 //��������
				{ 
				MENU=0;   
				BUZZER_BEEP1();//��һ��                                              
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
			    }
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C))			 //��������
				{ 
				MENU=0;   
				BUZZER_BEEP1();//��һ��                                               
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
			    }
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))			 //��������
				{ 
				MENU=0;   
				BUZZER_BEEP1();//��һ��                                               
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
			    }
			if(c==0)				//c==0�򿪼̵���1
				{
				RELAY_1(1);
				}
			if(c==1)						//c==1�رռ̵���1
				{
				RELAY_1(0);			
				}
		   } //(MENU==200)

//	   if(MENU==210)																								 //210�¶ȿ�������
//	   {		
//			i++;
//
//
//				TM1640_display(0,25);	
//				TM1640_display(1,22);
//				TM1640_display(2,MENU2-2);
//
//			if(i>S/2){
//				if(alhour[MENU2-1]==0){
//					TM1640_display(4,28); //OFF
//					TM1640_display(5,29);
//					TM1640_display(6,29);
//					TM1640_display(7,20); //
//				}else{
////					if(!buffer[0])TM1640_display(3,20);  //������
////					else TM1640_display(3,21);
//					TM1640_display(4,alhour[MENU2-1]%100/10);	//�¶� 
//					TM1640_display(5,alhour[MENU2-1]%10+10);
//					TM1640_display(7,22); //c
//				}
//			}else{
//				if(alhour[MENU2-1]==0){
//					TM1640_display(4,20); //OFF��˸������ʾ 
//					TM1640_display(5,20);
//					TM1640_display(6,20);
//				}else{
//					TM1640_display(4,20);	//�� 
//					TM1640_display(5,20);
//				}
//			}
//			if(alhour[MENU2-1]!=0){
//				TM1640_display(6,almin[MENU2-1]%10); //
//			}
//			if(i>1400)i=0; //���������ٶ�
//			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){//�жϳ��̼�
//				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEY_SPEED1){ //ѭ���жϳ�������ʱ��ת
//					c++;delay_ms(10); //�����жϵļ�ʱ
//				}
//				if(c>=KEY_SPEED1){ //��������
//					//������ִ�еĳ���ŵ��˴�
//					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
//						i=701;//���õ�ʱ������ʱ��һֱ��ʾ
//						alhour[MENU2-1]++;
//						if(alhour[MENU2-1]>60)alhour[MENU2-1]=0;
//						TM1640_display(4,alhour[MENU2-1]/10); //ʱ
//						TM1640_display(5,alhour[MENU2-1]%10+10);
//						delay_ms(KEY_SPEED2); //��ʱ ����
//						BUZZER_BEEP1();//��������1
//					}
//				}else{ //��������
//					if(a==0){ //�жϵ���
//						//������ִ�еĳ���ŵ��˴�
//						i=701;//���õ�ʱ������ʱ��һֱ��ʾ
//						alhour[MENU2-1]++;
//						if(alhour[MENU2-1]>60)alhour[MENU2-1]=0;
//						BUZZER_BEEP1();//��������1
//					}
//				}
//				a=0;c=0; //������0
//			}
//			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){//�жϳ��̼�
//				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEY_SPEED1){ //ѭ���жϳ�������ʱ��ת
//					c++;delay_ms(10); //�����жϵļ�ʱ
//				}
//				if(c>=KEY_SPEED1){ //��������
//					//������ִ�еĳ���ŵ��˴�
//					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
//						i=701;//���õ�ʱ������ʱ��һֱ��ʾ
//						if(alhour[MENU2-1]==0)alhour[MENU2-1]=61;
//						alhour[MENU2-1]--;
//						TM1640_display(4,alhour[MENU2-1]/10); //ʱ
//						TM1640_display(5,alhour[MENU2-1]%10+10);
//						delay_ms(KEY_SPEED2); //��ʱ ����
//						BUZZER_BEEP1();//��������1
//					}
//				}else{ //��������
//					if(a==0){ //�жϵ���
//						//������ִ�еĳ���ŵ��˴�
//						i=701;//���õ�ʱ������ʱ��һֱ��ʾ
//						if(alhour[MENU2-1]==0)alhour[MENU2-1]=61;
//						alhour[MENU2-1]--;
//						BUZZER_BEEP1();//��������1
//					}
//				}
//				a=0;c=0; //������0
//			}
//			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //�����������ĵ�ƽ
//				if(alhour[MENU2-1]!=0)MENU=211;
//				BUZZER_BEEP1();//��������1
//				ALFLASH_W(); //������д�뵽FLASH��
//				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
//	 		}
//			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //�����������ĵ�ƽ
//				MENU=1;
//				rup=1;//ʱ���޸ĵı�־λ��1
//				ALFLASH_W(); //������д�뵽FLASH��
//				BUZZER_BEEP4();//��������1
//				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
//	 		}
//		} //(MENU==210)
//
//			if(MENU==151)																						  //211�¶ȿ���С��
//			{																	           
//			i++;
//				TM1640_display(0,26);	
//				TM1640_display(1,27);
//				TM1640_display(2,MENU2);
//				TM1640_display(4,alhour[MENU2-1]/10); //ʱ
//				TM1640_display(5,alhour[MENU2-1]%10+10);
//				if(i>S/2){
//				TM1640_display(6,almin[MENU2-1]/10);	//��
//				TM1640_display(7,almin[MENU2-1]%10);
//				}else{
//				TM1640_display(6,20);	//��
//				TM1640_display(7,20);								
//				}
//			if(i>S)i=0; //���������ٶ�
//
// 			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))
//			{                                                                        //�жϳ��̼�
//				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEY_SPEED1){ //ѭ���жϳ�������ʱ��ת
//					c++;delay_ms(10); //�����жϵļ�ʱ
//				}
//				if(c>=KEY_SPEED1){ //��������
//					//������ִ�еĳ���ŵ��˴�
//					BUZZER_BEEP1();//��һ��
//					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
////						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
//						almin[MENU2-1]++; //1�������� alhour[0]�洢��2�������� alhour[1]�洢
//						if(almin[MENU2-1]>59)almin[MENU2-1]=0;
//						TM1640_display(6,almin[MENU2-1]/10);	//��
//						TM1640_display(7,almin[MENU2-1]%10);
//						delay_ms(KEY_SPEED2); //��ʱ ����	                                                        ?
//					}
//				}else{ //��������
//					if(a==0){ //�жϵ���
//						//������ִ�еĳ���ŵ��˴�
////						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
//						almin[MENU2-1]++;
//						if(almin[MENU2-1]>59)almin[MENU2-1]=0;
//						BUZZER_BEEP1();//��һ��
//					}
//				}
//				a=0;c=0; //������0
//			}
//
//
//			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))
//			{																 //�����������ĵ�ƽ
//				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEY_SPEED1){ //ѭ���жϳ�������ʱ��ת
//					c++;delay_ms(10); //�����жϵļ�ʱ
//				}
//				if(c>=KEY_SPEED1){ //��������
//					//������ִ�еĳ���ŵ��˴�
//					BUZZER_BEEP1();//��һ��
//					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
////						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
//						if(almin[MENU2-1]==0)almin[MENU2-1]=60;
//						almin[MENU2-1]--;																								
//						TM1640_display(6,almin[MENU2-1]/10);	//��
//						TM1640_display(7,almin[MENU2-1]%10);
//						delay_ms(KEY_SPEED2); //��ʱ ����
//					}
//				}else{ //��������
//					if(a==0){ //�жϵ���
//						//������ִ�еĳ���ŵ��˴�
////						i=S/2+1;//���õ�ʱ������ʱ��һֱ��ʾ
//						if(almin[MENU2-1]==0)almin[MENU2-1]=60;
//						almin[MENU2-1]--;
//						BUZZER_BEEP1();//��һ��		 
//					}
//				}
//				a=0;c=0; //������0
//			}
//			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //�����������ĵ�ƽ
//				MENU=150; 
//				BUZZER_BEEP1();//��һ��                                                      //ʱ���޸ı�־λ
//				ALFLASH_W ();    
//				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
//	 		}
//			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //�����������ĵ�ƽ
//				MENU=0; 
//				BUZZER_BEEP1();//��һ��                                                       //ʱ���޸ı�־λ
//				ALFLASH_W ();    
//				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
//	 		}
//		}	   
	   


		} //while(1)

} //main
 


/*********************************************************************************************/
/*

���������塿
u32     a; //����32λ�޷��ű���a
u16     a; //����16λ�޷��ű���a
u8     a; //����8λ�޷��ű���a
vu32     a; //�����ױ��32λ�޷��ű���a
vu16     a; //�����ױ�� 16λ�޷��ű���a
vu8     a; //�����ױ�� 8λ�޷��ű���a
uc32     a; //����ֻ����32λ�޷��ű���a
uc16     a; //����ֻ�� ��16λ�޷��ű���a
uc8     a; //����ֻ�� ��8λ�޷��ű���a

#define ONE  1   //�궨��

delay_us(1); //��ʱ1΢��
delay_ms(1); //��ʱ1����
delay_s(1); //��ʱ1��

GPIO_WriteBit(LEDPORT,LED1,(BitAction)(1)); //LED����

*/



