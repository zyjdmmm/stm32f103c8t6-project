/*********************************************************************************************

程序名：	项目1：电子钟
编写人：	阿giao	
编写时间：	2021.2.2
硬件支持：	STM32F103C8   外部晶振8MHz RCC函数设置主频72MHz　  


*********************************************************************************************/
#include "stm32f10x.h" //STM32头文件
#include "sys.h"
#include "delay.h"
#include "rtc.h"
#include "TM1640.h"
#include "lm75a.h"
#include "touch_key.h"
#include "buzzer.h"
#include "flash.h" 
#include "relay.h"

#define S 500		   //闪烁速度
#define KEY_SPEED1	100	  //长按的时间长度（单位10mS）
#define KEY_SPEED2	100	  //加减速度


int main (void){//主程序
//	u8 c=0x01;
	u8 MENU=0;//菜单值
	u8 buffer[3];  //温度变量
	u16 i;//时钟闪烁
	u8 rup;//时钟写入标记
 	u8 a=0,c=0;
	u8 MENU2=0;	//第n个闹钟
	u8 t=0;
	u16 m=0;	//闹钟跳出不再鸣响标志位
	u16 d=0;//0，1，2菜单循环跳转计数器		   //用作计算器用u16，不然位数不够会溢出



    RCC_Configuration(); //系统时钟初始化 
	RELAY_Init();//继电器初始化
	delay_ms(200);
	RTC_Config();  //RTC初始化
	TM1640_Init(); //TM1640初始化
	I2C_Configuration();//I2C初始化
	TOUCH_KEY_Init();//按键初始化
    BUZZER_Init();//初始化
    BUZZER_BEEP2();//开机
	rup=0;

	RELAY_1(1);	 //c=0继电器放开，c=1继电器吸合
	RELAY_2(1);

	for(i=0;i<2;i++)				 //循环2次，2两个闹钟														  数据读出
		{
		t = FLASH_R(FLASH_START_ADDR+i*2);//从指定页的地址读FLASH	
		alhour[i]=t/0x100;				 //得到高八位就除2^8
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
			 RTC_Set(ryear,rmon,rday,rhour,rmin,rsec); //时间写入判断			 			 
			 }
			 if(MENU<3||MENU>8){
			 RTC_Get();
             LM75A_GetTemp(buffer); //读取LM75A的温度数据
			 }



			if(MENU==0)
		   	 {		         																							 //0原始时钟年月日
				TM1640_display(0,ryear%100/10);	//年
				TM1640_display(1,ryear%100%10);
				TM1640_display(2,21); //-
				TM1640_display(3,rmon/10);
				TM1640_display(4,rmon%10);	// 月
				TM1640_display(5,21); //-
				TM1640_display(6,rday/10); //  日
				TM1640_display(7,rday%10);
	
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){ //读触摸按键的电平
				MENU=0;
				BUZZER_BEEP1();//响一声																		
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){ //读触摸按键的电平
				MENU=1;
				BUZZER_BEEP1();//响一声	
		    	while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //读触摸按键的电平
				MENU=2;
				BUZZER_BEEP1();//响一声	
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //读触摸按键的电平
				MENU=3;
				BUZZER_BEEP1();//响一声	
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));			
			    }//if(MENU==0) 

		    }
    		if(MENU==1)																			     //1温度菜单
		     	{
				TM1640_display(0,20);	//无
				TM1640_display(1,20);
				TM1640_display(2,20);
				if(buffer[0])TM1640_display(3,21);	 //正负号
				else TM1640_display(3,20);
				TM1640_display(4,buffer[1]%100/10);	//温度
				TM1640_display(5,buffer[1]%100%10+10);
				TM1640_display(6,buffer[2]%100/10); //
				TM1640_display(7,22);	   //c
	
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){ //读触摸按键的电平
				MENU=0;
				BUZZER_BEEP1();//响一声	
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){ //读触摸按键的电平
				MENU=1;
				BUZZER_BEEP1();//响一声	
		    	while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //读触摸按键的电平
				MENU=2;
				BUZZER_BEEP1();//响一声	
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //读触摸按键的电平
				MENU=3;
				BUZZER_BEEP1();//响一声	
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
				}								
     	     	}//	if(MENU==1)

			if(MENU==2)																								   	   //2时分秒
			    {
				RTC_Get();   			//多刷一次																	
				TM1640_display(0,20);	//无
				TM1640_display(1,20);
				TM1640_display(2,rhour/10); //时
				TM1640_display(3,rhour%10+10);
				TM1640_display(4,rmin/10);	//分
				TM1640_display(5,rmin%10+10);
				TM1640_display(6,rsec/10); //秒
				TM1640_display(7,rsec%10);
	
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){ //读触摸按键的电平
				MENU=0;
				BUZZER_BEEP1();//响一声	
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){ //读触摸按键的电平
				MENU=1;
				BUZZER_BEEP1();//响一声	
		    	while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //读触摸按键的电平
				MENU=2;
				BUZZER_BEEP1();//响一声	
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));	
				}
				if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //读触摸按键的电平
				MENU=3;
				BUZZER_BEEP1();//响一声	
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
				}
				}//if(MENU==2

		   if(MENU<3)																				   //012菜单循环显示
		   {
		   	d++;
			if(d==2000){MENU=0;} 
			if(d==4000){MENU=1;} 
			if(d==6000){MENU=2;d=0;}	   //if后面最好写上{}
		   }

			if(MENU==3){		 																	      	  //3set显示
			TM1640_display(0,23);	//set
			TM1640_display(1,24);
			TM1640_display(2,25);
            TM1640_display(3,20);
			TM1640_display(4,20);	//温度
			TM1640_display(5,20);
			TM1640_display(6,20); //
			TM1640_display(7,20);	   //c
			BUZZER_BEEP3();
			MENU2=0;
			MENU=100;

	    	}//if(MENU==3)

			if(MENU==4)
			{																					  //4闪烁年
			i++;
			if(i>S)i=0;
			if(i>S/2){		 	
				TM1640_display(0,ryear%100/10);	//年								    
				TM1640_display(1,ryear%100%10);
			}else{
				TM1640_display(0,20);	//无
				TM1640_display(1,20);
			}
			TM1640_display(2,21); //-
			TM1640_display(3,rmon/10);
			TM1640_display(4,rmon%10);	// 月
			TM1640_display(5,21); //-
			TM1640_display(6,rday/10); //  日
			TM1640_display(7,rday%10);

 			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))
			{                                                                        //判断长短键
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEY_SPEED1){ //循环判断长按，到时跳转
					c++;delay_ms(10); //长按判断的计时
				}
				if(c>=KEY_SPEED1){ //长键处理
					//长按后执行的程序放到此处
					BUZZER_BEEP1();//响一声
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
//						i=S/2+1;//能让调时过程中时数一直显示
						ryear++;
						if(ryear>3000)ryear=2021;
						TM1640_display(0,ryear%100/10);	//年  2018 
						TM1640_display(1,ryear%100%10);
						delay_ms(KEY_SPEED2); //延时 毫秒	                                                        ?
					}
				}else{ //单击处理
					if(a==0){ //判断单击
						//单击后执行的程序放到此处
						i=S/2+1;//能让调时过程中时数一直显示
						ryear++;
						if(ryear>3000)ryear=2021;
						BUZZER_BEEP1();//响一声
					}
				}
				a=0;c=0; //参数清0
			}


			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))
			{																 //读触摸按键的电平
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEY_SPEED1){ //循环判断长按，到时跳转
					c++;delay_ms(10); //长按判断的计时
				}
				if(c>=KEY_SPEED1){ //长键处理
					//长按后执行的程序放到此处
					BUZZER_BEEP1();//响一声
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
//						i=S/2+1;//能让调时过程中时数一直显示
						ryear--;
						if(ryear<2021)ryear=3000;
						TM1640_display(0,ryear%100/10);	//年  2018 
						TM1640_display(1,ryear%100%10);
						delay_ms(KEY_SPEED2); //延时 毫秒
					}
				}else{ //单击处理
					if(a==0){ //判断单击
						//单击后执行的程序放到此处
						i=S/2+1;//能让调时过程中时数一直显示
						ryear--;
						if(ryear<2021)ryear=3000;
						BUZZER_BEEP1();//响一声
					}
				}
				a=0;c=0; //参数清0
			}


			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //读触摸按键的电平
			MENU=5;
			rup=1; 
			BUZZER_BEEP1();//响一声                                                        //时间修改标志位
			while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));	
			}

			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //读触摸按键的电平
			MENU=0;
			rup=1;
			BUZZER_BEEP1();//响一声                                                         //时间修改标志位
			while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
			}

			}//if(MENU==3)






			if(MENU==5){			   //																	  //5闪烁月
			i++;
			if(i>S)i=0;
		 	
			TM1640_display(0,ryear%100/10);	//年
			TM1640_display(1,ryear%100%10);

			TM1640_display(2,21); //-
			if(i>S/2) {
				TM1640_display(3,20);
				TM1640_display(4,20);	// 月
			}else{
				TM1640_display(3,rmon/10);
				TM1640_display(4,rmon%10);	// 月
			}

			TM1640_display(5,21); //-
			TM1640_display(6,rday/10); //  日
			TM1640_display(7,rday%10);

 			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))
			{                                                                        //判断长短键
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEY_SPEED1){ //循环判断长按，到时跳转
					c++;delay_ms(10); //长按判断的计时
				}
				if(c>=KEY_SPEED1){ //长键处理
					//长按后执行的程序放到此处
					BUZZER_BEEP1();//响一声
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
//						i=S/2+1;//能让调时过程中时数一直显示
						rmon++;
						if(rmon>12)rmon=1;
						TM1640_display(3,rmon/10);
						TM1640_display(4,rmon%10);	// 月
						delay_ms(KEY_SPEED2); //延时 毫秒	                                                        ?
					}
				}else{ //单击处理
					if(a==0)
					 { //判断单击
						//单击后执行的程序放到此处
						i=S/2+1;//能让调时过程中时数一直显示
						rmon++;
						if(rmon>12)rmon=1;
						BUZZER_BEEP1();//响一声
					}
				}
				a=0;c=0; //参数清0
			}


			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))
			{																 //读触摸按键的电平
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEY_SPEED1){ //循环判断长按，到时跳转
					c++;delay_ms(10); //长按判断的计时
				}
				if(c>=KEY_SPEED1){ //长键处理
					//长按后执行的程序放到此处
					BUZZER_BEEP1();//响一声
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
//						i=S/2+1;//能让调时过程中时数一直显示
						rmon--;
						if(rmon<1)rmon=12;
						TM1640_display(3,rmon/10);
						TM1640_display(4,rmon%10);	// 月
						delay_ms(KEY_SPEED2); //延时 毫秒
					}
				}else{ //单击处理
					if(a==0){ //判断单击
						//单击后执行的程序放到此处
						i=S/2+1;//能让调时过程中时数一直显示
						rmon--;
						if(rmon<1)rmon=12;
						BUZZER_BEEP1();//响一声
					}
				}
				a=0;c=0; //参数清0
			}

			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //读触摸按键的电平
			MENU=6;
			rup=1;
			BUZZER_BEEP1();//响一声                                                         //时间修改标志位
			while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));	
			}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //读触摸按键的电平
			MENU=0;
			rup=1; 
			BUZZER_BEEP1();//响一声                                                        //时间修改标志位
			while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
			}

			}//if(MENU==3)

			if(MENU==6){																					//6闪烁天
			i++;
			if(i>S)i=0;
		 	
			TM1640_display(0,ryear%100/10);	//年
			TM1640_display(1,ryear%100%10);

			TM1640_display(2,21); //-
			TM1640_display(3,rmon/10);
			TM1640_display(4,rmon%10);	// 月

			TM1640_display(5,21); //-
			if(i>S/2){
			TM1640_display(6,rday/10); //  日
			TM1640_display(7,rday%10);
			}else{
			TM1640_display(6,20); //  日
			TM1640_display(7,20);
			}
 			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))
			{                                                                        //判断长短键
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEY_SPEED1){ //循环判断长按，到时跳转
					c++;delay_ms(10); //长按判断的计时
				}
				if(c>=KEY_SPEED1){ //长键处理
					//长按后执行的程序放到此处
					BUZZER_BEEP1();//响一声
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
//						i=S/2+1;//能让调时过程中时数一直显示
						rday++;
						if(rday>31)rday=1;
						TM1640_display(6,rday/10); //  日
						TM1640_display(7,rday%10);
						delay_ms(KEY_SPEED2); //延时 毫秒	                                                        ?
					}
				}else{ //单击处理
					if(a==0){ //判断单击
						//单击后执行的程序放到此处
						i=S/2+1;//能让调时过程中时数一直显示
						rday++;
						if(rday>31)rday=1;
						BUZZER_BEEP1();//响一声
					}
				}
				a=0;c=0; //参数清0
			}


			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))
			{																 //读触摸按键的电平
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEY_SPEED1){ //循环判断长按，到时跳转
					c++;delay_ms(10); //长按判断的计时
				}
				if(c>=KEY_SPEED1){ //长键处理
					//长按后执行的程序放到此处
					BUZZER_BEEP1();//响一声
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
//						i=S/2+1;//能让调时过程中时数一直显示
						rday--;
						if(rday<1)rday=31;
						TM1640_display(6,rday/10); //  日
						TM1640_display(7,rday%10);
						delay_ms(KEY_SPEED2); //延时 毫秒
					}
				}else{ //单击处理
					if(a==0){ //判断单击
						//单击后执行的程序放到此处
//						i=S/2+1;//能让调时过程中时数一直显示
						rday--;
						if(rday<1)rday=31;
						BUZZER_BEEP1();//响一声
					}
				}
				a=0;c=0; //参数清0
			}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //读触摸按键的电平
			MENU=7;
			rup=1; 
			BUZZER_BEEP1();//响一声                                                        //时间修改标志位
			while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));	
			}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //读触摸按键的电平
			MENU=0;
			rup=1;  
			BUZZER_BEEP1();//响一声                                                       //时间修改标志位
			while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
			}									   

			}//if(MENU==3)

		if(MENU==7){																						//7闪烁时
			i++;
			TM1640_display(0,20);	//无
			TM1640_display(1,20);
			if(i>S/2){
				TM1640_display(2,rhour/10); //时
				TM1640_display(3,rhour%10+10);
			}else{
				TM1640_display(2,20);	//无 
				TM1640_display(3,20);
			}
			TM1640_display(4,rmin/10);	//分
			TM1640_display(5,rmin%10+10);
			TM1640_display(6,rsec/10); //秒
			TM1640_display(7,rsec%10);
			if(i>S)i=0; //调节闪的速度

 			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))
			{                                                                        //判断长短键
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEY_SPEED1){ //循环判断长按，到时跳转
					c++;delay_ms(10); //长按判断的计时
				}
				if(c>=KEY_SPEED1){ //长键处理
					//长按后执行的程序放到此处
					BUZZER_BEEP1();//响一声
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
//						i=S/2+1;//能让调时过程中时数一直显示
						rhour++;
						if(rhour>23)rhour=0;
						TM1640_display(2,rhour/10); //时
						TM1640_display(3,rhour%10+10);
						delay_ms(KEY_SPEED2); //延时 毫秒	                                                        ?
					}
				}else{ //单击处理
					if(a==0){ //判断单击
						//单击后执行的程序放到此处
//						i=S/2+1;//能让调时过程中时数一直显示
						rhour++;
						if(rhour>23)rhour=0;
						BUZZER_BEEP1();//响一声
					}
				}
				a=0;c=0; //参数清0
			}


			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))
			{																 //读触摸按键的电平
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEY_SPEED1){ //循环判断长按，到时跳转
					c++;delay_ms(10); //长按判断的计时
				}
				if(c>=KEY_SPEED1){ //长键处理
					//长按后执行的程序放到此处
					BUZZER_BEEP1();//响一声
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
//						i=S/2+1;//能让调时过程中时数一直显示
						if(rhour==0)rhour=24;
						rhour--;
						TM1640_display(6,rday/10); //  日
						TM1640_display(7,rday%10);
						delay_ms(KEY_SPEED2); //延时 毫秒
					}
				}else{ //单击处理
					if(a==0){ //判断单击
						//单击后执行的程序放到此处
//						i=S/2+1;//能让调时过程中时数一直显示
						if(rhour==0)rhour=24;
						rhour--;
						BUZZER_BEEP1();//响一声		 
					}
				}
				a=0;c=0; //参数清0
			}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //读触摸按键的电平
				MENU=8;
				rup=1;   
				BUZZER_BEEP1();//响一声                                                      //时间修改标志位
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
	 		}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //读触摸按键的电平
				MENU=1;
				rup=1;  
				BUZZER_BEEP1();//响一声                                                       //时间修改标志位
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
	 		}
		}

		if(MENU==8){																					  //8闪烁分
			i++;
			TM1640_display(0,20);	//无
			TM1640_display(1,20);

			TM1640_display(2,rhour/10); //时
			TM1640_display(3,rhour%10+10);
			if(i>S/2){
			TM1640_display(4,20);	//分
			TM1640_display(5,20);
			}else{
			TM1640_display(4,rmin/10);	//分
			TM1640_display(5,rmin%10+10);
			}

			TM1640_display(6,rsec/10); //秒
			TM1640_display(7,rsec%10);
			if(i>S)i=0; //调节闪的速度

 			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))
			{                                                                        //判断长短键
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEY_SPEED1){ //循环判断长按，到时跳转
					c++;delay_ms(10); //长按判断的计时
				}
				if(c>=KEY_SPEED1){ //长键处理
				BUZZER_BEEP1();//响一声
					//长按后执行的程序放到此处
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
//						i=S/2+1;//能让调时过程中时数一直显示
						rmin++;
						if(rmin>59)rmin=0;
						TM1640_display(4,rmin/10);	//分
						TM1640_display(5,rmin%10+10);
						delay_ms(KEY_SPEED2); //延时 毫秒	                                                        ?
					}
				}else{ //单击处理
					if(a==0){ //判断单击
						//单击后执行的程序放到此处
//						i=S/2+1;//能让调时过程中时数一直显示
						rmin++;
						if(rmin>59)rmin=0;
						BUZZER_BEEP1();//响一声
					}
				}
				a=0;c=0; //参数清0
			}


			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))
			{																 //读触摸按键的电平
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEY_SPEED1){ //循环判断长按，到时跳转
					c++;delay_ms(10); //长按判断的计时
				}
				if(c>=KEY_SPEED1){ //长键处理
					//长按后执行的程序放到此处
					BUZZER_BEEP1();//响一声
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
//						i=S/2+1;//能让调时过程中时数一直显示

						if(rmin<1)rmin=60;
						rmin--;
						TM1640_display(4,rmin/10);	//分
						TM1640_display(5,rmin%10+10);
						delay_ms(KEY_SPEED2); //延时 毫秒
					}
				}else{ //单击处理
					if(a==0){ //判断单击
						//单击后执行的程序放到此处
//						i=S/2+1;//能让调时过程中时数一直显示
						if(rmin<1)rmin=60;
						rmin--;
						BUZZER_BEEP1();//响一声
					}
				}
				a=0;c=0; //参数清0
			}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //读触摸按键的电平
				MENU=4;
				rup=1;  
				BUZZER_BEEP1();//响一声                                                       //时间修改标志位
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
	 		}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //读触摸按键的电平
				MENU=0;
				rup=1;  
				BUZZER_BEEP1();//响一声                                                       //时间修改标志位
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
	 		}
		}

			if(MENU==100){
			
			if(MENU2==0)																							 //100set判断
				{
				TM1640_display(0,23);	
				TM1640_display(1,24);
				TM1640_display(2,25);
				}
			if(MENU2>=1&&MENU2<=2)	  //2个闹钟显示
				{
				TM1640_display(0,26);	
				TM1640_display(1,27);
				TM1640_display(2,MENU2);
				}
			if(MENU2>=3&&MENU2<=4)		//两个温度控制TC1~2
				{
				TM1640_display(0,25);	
				TM1640_display(1,22);
				TM1640_display(2,MENU2-2);						
				}		
			
	if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))
			{                                                                        //判断长短键
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEY_SPEED1){ //循环判断长按，到时跳转
					c++;delay_ms(10); //长按判断的计时
				}
				if(c>=KEY_SPEED1){ //长键处理
				BUZZER_BEEP1();//响一声
					//长按后执行的程序放到此处
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
//						i=S/2+1;//能让调时过程中时数一直显示
						MENU2++;
						if(MENU2>4)MENU2=0;

						delay_ms(KEY_SPEED2); //延时 毫秒	                                                        ?
					}
				}else{ //单击处理
					if(a==0){ //判断单击
						//单击后执行的程序放到此处
//						i=S/2+1;//能让调时过程中时数一直显示
						MENU2++;
						if(MENU2>4)MENU2=0;
						BUZZER_BEEP1();//响一声
					}
				}
				a=0;c=0; //参数清0
			}


			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))
			{																 //读触摸按键的电平
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEY_SPEED1){ //循环判断长按，到时跳转
					c++;delay_ms(10); //长按判断的计时
				}
				if(c>=KEY_SPEED1){ //长键处理
					//长按后执行的程序放到此处
					BUZZER_BEEP1();//响一声
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
//						i=S/2+1;//能让调时过程中时数一直显示

						if(MENU2==0)MENU2=5;
						MENU2--;

						delay_ms(KEY_SPEED2); //延时 毫秒
					}
				}else{ //单击处理
					if(a==0){ //判断单击
						//单击后执行的程序放到此处
//						i=S/2+1;//能让调时过程中时数一直显示
						if(MENU2==0)MENU2=5;
						MENU2--;
						BUZZER_BEEP1();//响一声
					}
				}
				a=0;c=0; //参数清0
			}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //读触摸按键的电平
				MENU=101+MENU2;  
				BUZZER_BEEP1();//响一声                                                       //时间修改标志位
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
	 		}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //读触摸按键的电平
				MENU=0; 
				BUZZER_BEEP1();//响一声                                                       //时间修改标志位
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
	 		}
			}//if(MENU==3)

			if(MENU==101){MENU=4;}	 																	       	  //101跳转到4
			if(MENU>=102&&MENU<=103){MENU=150;}	//2个闹钟	 											    	   //102到103跳转到150
			if(MENU>=104&&MENU<=105){MENU=210;}	//2个温度控制TC1~2               					     	   //110到111跳转到210




			if(MENU==150)
			{
			if(almin[MENU2-1]>59)almin[MENU2-1]=0;			//解决分钟乱码问题					               	   //150闹钟时间调整hour
			i++;
			if(alhour[MENU2-1]==24)		   //24显示OFF
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
			else						  //非24正常显示
				{				
				TM1640_display(0,26);	
				TM1640_display(1,27);
				TM1640_display(2,MENU2);
				if(i>S/2){
					TM1640_display(4,alhour[MENU2-1]/10); //时
					TM1640_display(5,alhour[MENU2-1]%10+10);
				}else{
					TM1640_display(4,20);	//无 
					TM1640_display(5,20);
				}
				TM1640_display(6,almin[MENU2-1]/10);	//分
				TM1640_display(7,almin[MENU2-1]%10+10);
				}

			if(i>S)i=0; //调节闪的速度

 			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))
			{                                                                        //判断长短键
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEY_SPEED1){ //循环判断长按，到时跳转
					c++;delay_ms(10); //长按判断的计时
				}
				if(c>=KEY_SPEED1){ //长键处理
					//长按后执行的程序放到此处
					BUZZER_BEEP1();//响一声
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
//						i=S/2+1;//能让调时过程中时数一直显示
						alhour[MENU2-1]++; //1号闹钟用 alhour[0]存储，2号闹钟用 alhour[1]存储
						if(alhour[MENU2-1]>24)alhour[MENU2-1]=0;
						TM1640_display(4,alhour[MENU2-1]/10); //时
						TM1640_display(5,alhour[MENU2-1]%10+10);
						delay_ms(KEY_SPEED2); //延时 毫秒	                                                        ?
					}
				}else{ //单击处理
					if(a==0){ //判断单击
						//单击后执行的程序放到此处
//						i=S/2+1;//能让调时过程中时数一直显示
						alhour[MENU2-1]++;
						if(alhour[MENU2-1]>24)alhour[MENU2-1]=0;
						BUZZER_BEEP1();//响一声
					}
				}
				a=0;c=0; //参数清0
			}


			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))
			{																 //读触摸按键的电平
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEY_SPEED1){ //循环判断长按，到时跳转
					c++;delay_ms(10); //长按判断的计时
				}
				if(c>=KEY_SPEED1){ //长键处理
					//长按后执行的程序放到此处
					BUZZER_BEEP1();//响一声
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
//						i=S/2+1;//能让调时过程中时数一直显示
						if(alhour[MENU2-1]==0)alhour[MENU2-1]=25;
						alhour[MENU2-1]--;
						TM1640_display(4,alhour[MENU2-1]/10); //时
						TM1640_display(5,alhour[MENU2-1]%10+10);
						delay_ms(KEY_SPEED2); //延时 毫秒
					}
				}else{ //单击处理
					if(a==0){ //判断单击
						//单击后执行的程序放到此处
//						i=S/2+1;//能让调时过程中时数一直显示
						if(alhour[MENU2-1]==0)alhour[MENU2-1]=25;
						alhour[MENU2-1]--;
						BUZZER_BEEP1();//响一声		 
					}
				}
				a=0;c=0; //参数清0
			}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //读触摸按键的电平
			if(alhour[MENU2-1]!=24)	MENU=151;
				BUZZER_BEEP1();//响一声 
				ALFLASH_W ();                                                     //时间修改标志位
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
	 		}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //读触摸按键的电平
				MENU=0;  
				BUZZER_BEEP1();//响一声
				ALFLASH_W ();    				                            
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
	 		}
		}

			if(MENU==151)
			{																	              //151闹钟时间调整min
			i++;
				TM1640_display(0,26);	
				TM1640_display(1,27);
				TM1640_display(2,MENU2);
				TM1640_display(4,alhour[MENU2-1]/10); //时
				TM1640_display(5,alhour[MENU2-1]%10+10);
				if(i>S/2){
				TM1640_display(6,almin[MENU2-1]/10);	//分
				TM1640_display(7,almin[MENU2-1]%10);
				}else{
				TM1640_display(6,20);	//分
				TM1640_display(7,20);								
				}
			if(i>S)i=0; //调节闪的速度

 			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))
			{                                                                        //判断长短键
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEY_SPEED1){ //循环判断长按，到时跳转
					c++;delay_ms(10); //长按判断的计时
				}
				if(c>=KEY_SPEED1){ //长键处理
					//长按后执行的程序放到此处
					BUZZER_BEEP1();//响一声
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
//						i=S/2+1;//能让调时过程中时数一直显示
						almin[MENU2-1]++; //1号闹钟用 alhour[0]存储，2号闹钟用 alhour[1]存储
						if(almin[MENU2-1]>59)almin[MENU2-1]=0;
						TM1640_display(6,almin[MENU2-1]/10);	//分
						TM1640_display(7,almin[MENU2-1]%10);
						delay_ms(KEY_SPEED2); //延时 毫秒	                                                        ?
					}
				}else{ //单击处理
					if(a==0){ //判断单击
						//单击后执行的程序放到此处
//						i=S/2+1;//能让调时过程中时数一直显示
						almin[MENU2-1]++;
						if(almin[MENU2-1]>59)almin[MENU2-1]=0;
						BUZZER_BEEP1();//响一声
					}
				}
				a=0;c=0; //参数清0
			}


			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))
			{																 //读触摸按键的电平
				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEY_SPEED1){ //循环判断长按，到时跳转
					c++;delay_ms(10); //长按判断的计时
				}
				if(c>=KEY_SPEED1){ //长键处理
					//长按后执行的程序放到此处
					BUZZER_BEEP1();//响一声
					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
//						i=S/2+1;//能让调时过程中时数一直显示
						if(almin[MENU2-1]==0)almin[MENU2-1]=60;
						almin[MENU2-1]--;																								
						TM1640_display(6,almin[MENU2-1]/10);	//分
						TM1640_display(7,almin[MENU2-1]%10);
						delay_ms(KEY_SPEED2); //延时 毫秒
					}
				}else{ //单击处理
					if(a==0){ //判断单击
						//单击后执行的程序放到此处
//						i=S/2+1;//能让调时过程中时数一直显示
						if(almin[MENU2-1]==0)almin[MENU2-1]=60;
						almin[MENU2-1]--;
						BUZZER_BEEP1();//响一声		 
					}
				}
				a=0;c=0; //参数清0
			}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //读触摸按键的电平
				MENU=150; 
				BUZZER_BEEP1();//响一声                                                      //时间修改标志位
				ALFLASH_W ();    
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
	 		}
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //读触摸按键的电平
				MENU=0; 
				BUZZER_BEEP1();//响一声                                                       //时间修改标志位
				ALFLASH_W ();    
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
	 		}
		}


		   if(MENU<3)																					  	//<3闹钟响起，检查每个闹钟，跳转200
		   {
		   for(i=0;i<2;i++)																				
		   if(alhour[i]==rhour&&almin[i]==rmin&&rsec==0)  //上面别用延时程序，而是要遍历一次，不然rsec==0的时刻会错过
		   		{
				MENU=200;
				c=i;
				}						 
		   }

		  if(MENU==200)																					   //闹钟响起无操作到时跳转
		  	{
			m++;
			if(m==500){m=0;MENU=0;}	   //用m计时达到后跳转MENU1
			
			}


		   if(MENU==200)																				  	//200闹钟响起处理 
		   {
			BUZZER_BEEP1();
			i++;
			if(i>2)
				{
				TM1640_display(0,26);	
				TM1640_display(1,27);
				TM1640_display(2,c+1);
				TM1640_display(3,20);
				TM1640_display(4,alhour[c]/10); //时
				TM1640_display(5,alhour[c]%10+10);
				TM1640_display(6,almin[c]/10);	//分
				TM1640_display(7,almin[c]%10+10);
				}
			else
				{
				TM1640_display(0,20);	
				TM1640_display(1,20);
				TM1640_display(2,20);
				TM1640_display(3,20);
				TM1640_display(4,20); //时
				TM1640_display(5,20);
				TM1640_display(6,20);	//分
				TM1640_display(7,20);				
				}
			if(i>4)	i=0;
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))			 //跳出闹钟
				{ 
				MENU=0;   
				BUZZER_BEEP1();//响一声                                                
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A));
			    }
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))			 //跳出闹钟
				{ 
				MENU=0;   
				BUZZER_BEEP1();//响一声                                              
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B));
			    }
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C))			 //跳出闹钟
				{ 
				MENU=0;   
				BUZZER_BEEP1();//响一声                                               
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
			    }
			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D))			 //跳出闹钟
				{ 
				MENU=0;   
				BUZZER_BEEP1();//响一声                                               
				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
			    }
			if(c==0)				//c==0打开继电器1
				{
				RELAY_1(1);
				}
			if(c==1)						//c==1关闭继电器1
				{
				RELAY_1(0);			
				}
		   } //(MENU==200)

//	   if(MENU==210)																								 //210温度控制整数
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
////					if(!buffer[0])TM1640_display(3,20);  //正负号
////					else TM1640_display(3,21);
//					TM1640_display(4,alhour[MENU2-1]%100/10);	//温度 
//					TM1640_display(5,alhour[MENU2-1]%10+10);
//					TM1640_display(7,22); //c
//				}
//			}else{
//				if(alhour[MENU2-1]==0){
//					TM1640_display(4,20); //OFF闪烁的无显示 
//					TM1640_display(5,20);
//					TM1640_display(6,20);
//				}else{
//					TM1640_display(4,20);	//无 
//					TM1640_display(5,20);
//				}
//			}
//			if(alhour[MENU2-1]!=0){
//				TM1640_display(6,almin[MENU2-1]%10); //
//			}
//			if(i>1400)i=0; //调节闪的速度
//			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){//判断长短键
//				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEY_SPEED1){ //循环判断长按，到时跳转
//					c++;delay_ms(10); //长按判断的计时
//				}
//				if(c>=KEY_SPEED1){ //长键处理
//					//长按后执行的程序放到此处
//					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
//						i=701;//能让调时过程中时数一直显示
//						alhour[MENU2-1]++;
//						if(alhour[MENU2-1]>60)alhour[MENU2-1]=0;
//						TM1640_display(4,alhour[MENU2-1]/10); //时
//						TM1640_display(5,alhour[MENU2-1]%10+10);
//						delay_ms(KEY_SPEED2); //延时 毫秒
//						BUZZER_BEEP1();//蜂鸣器音1
//					}
//				}else{ //单击处理
//					if(a==0){ //判断单击
//						//单击后执行的程序放到此处
//						i=701;//能让调时过程中时数一直显示
//						alhour[MENU2-1]++;
//						if(alhour[MENU2-1]>60)alhour[MENU2-1]=0;
//						BUZZER_BEEP1();//蜂鸣器音1
//					}
//				}
//				a=0;c=0; //参数清0
//			}
//			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){//判断长短键
//				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEY_SPEED1){ //循环判断长按，到时跳转
//					c++;delay_ms(10); //长按判断的计时
//				}
//				if(c>=KEY_SPEED1){ //长键处理
//					//长按后执行的程序放到此处
//					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
//						i=701;//能让调时过程中时数一直显示
//						if(alhour[MENU2-1]==0)alhour[MENU2-1]=61;
//						alhour[MENU2-1]--;
//						TM1640_display(4,alhour[MENU2-1]/10); //时
//						TM1640_display(5,alhour[MENU2-1]%10+10);
//						delay_ms(KEY_SPEED2); //延时 毫秒
//						BUZZER_BEEP1();//蜂鸣器音1
//					}
//				}else{ //单击处理
//					if(a==0){ //判断单击
//						//单击后执行的程序放到此处
//						i=701;//能让调时过程中时数一直显示
//						if(alhour[MENU2-1]==0)alhour[MENU2-1]=61;
//						alhour[MENU2-1]--;
//						BUZZER_BEEP1();//蜂鸣器音1
//					}
//				}
//				a=0;c=0; //参数清0
//			}
//			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //读触摸按键的电平
//				if(alhour[MENU2-1]!=0)MENU=211;
//				BUZZER_BEEP1();//蜂鸣器音1
//				ALFLASH_W(); //把数据写入到FLASH中
//				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
//	 		}
//			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //读触摸按键的电平
//				MENU=1;
//				rup=1;//时间修改的标志位置1
//				ALFLASH_W(); //把数据写入到FLASH中
//				BUZZER_BEEP4();//蜂鸣器音1
//				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
//	 		}
//		} //(MENU==210)
//
//			if(MENU==151)																						  //211温度控制小数
//			{																	           
//			i++;
//				TM1640_display(0,26);	
//				TM1640_display(1,27);
//				TM1640_display(2,MENU2);
//				TM1640_display(4,alhour[MENU2-1]/10); //时
//				TM1640_display(5,alhour[MENU2-1]%10+10);
//				if(i>S/2){
//				TM1640_display(6,almin[MENU2-1]/10);	//分
//				TM1640_display(7,almin[MENU2-1]%10);
//				}else{
//				TM1640_display(6,20);	//分
//				TM1640_display(7,20);								
//				}
//			if(i>S)i=0; //调节闪的速度
//
// 			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))
//			{                                                                        //判断长短键
//				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A))&&c<KEY_SPEED1){ //循环判断长按，到时跳转
//					c++;delay_ms(10); //长按判断的计时
//				}
//				if(c>=KEY_SPEED1){ //长键处理
//					//长按后执行的程序放到此处
//					BUZZER_BEEP1();//响一声
//					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)){
////						i=S/2+1;//能让调时过程中时数一直显示
//						almin[MENU2-1]++; //1号闹钟用 alhour[0]存储，2号闹钟用 alhour[1]存储
//						if(almin[MENU2-1]>59)almin[MENU2-1]=0;
//						TM1640_display(6,almin[MENU2-1]/10);	//分
//						TM1640_display(7,almin[MENU2-1]%10);
//						delay_ms(KEY_SPEED2); //延时 毫秒	                                                        ?
//					}
//				}else{ //单击处理
//					if(a==0){ //判断单击
//						//单击后执行的程序放到此处
////						i=S/2+1;//能让调时过程中时数一直显示
//						almin[MENU2-1]++;
//						if(almin[MENU2-1]>59)almin[MENU2-1]=0;
//						BUZZER_BEEP1();//响一声
//					}
//				}
//				a=0;c=0; //参数清0
//			}
//
//
//			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))
//			{																 //读触摸按键的电平
//				while((!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B))&&c<KEY_SPEED1){ //循环判断长按，到时跳转
//					c++;delay_ms(10); //长按判断的计时
//				}
//				if(c>=KEY_SPEED1){ //长键处理
//					//长按后执行的程序放到此处
//					BUZZER_BEEP1();//响一声
//					while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)){
////						i=S/2+1;//能让调时过程中时数一直显示
//						if(almin[MENU2-1]==0)almin[MENU2-1]=60;
//						almin[MENU2-1]--;																								
//						TM1640_display(6,almin[MENU2-1]/10);	//分
//						TM1640_display(7,almin[MENU2-1]%10);
//						delay_ms(KEY_SPEED2); //延时 毫秒
//					}
//				}else{ //单击处理
//					if(a==0){ //判断单击
//						//单击后执行的程序放到此处
////						i=S/2+1;//能让调时过程中时数一直显示
//						if(almin[MENU2-1]==0)almin[MENU2-1]=60;
//						almin[MENU2-1]--;
//						BUZZER_BEEP1();//响一声		 
//					}
//				}
//				a=0;c=0; //参数清0
//			}
//			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)){ //读触摸按键的电平
//				MENU=150; 
//				BUZZER_BEEP1();//响一声                                                      //时间修改标志位
//				ALFLASH_W ();    
//				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C));
//	 		}
//			if(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)){ //读触摸按键的电平
//				MENU=0; 
//				BUZZER_BEEP1();//响一声                                                       //时间修改标志位
//				ALFLASH_W ();    
//				while(!GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D));
//	 		}
//		}	   
	   


		} //while(1)

} //main
 


/*********************************************************************************************/
/*

【变量定义】
u32     a; //定义32位无符号变量a
u16     a; //定义16位无符号变量a
u8     a; //定义8位无符号变量a
vu32     a; //定义易变的32位无符号变量a
vu16     a; //定义易变的 16位无符号变量a
vu8     a; //定义易变的 8位无符号变量a
uc32     a; //定义只读的32位无符号变量a
uc16     a; //定义只读 的16位无符号变量a
uc8     a; //定义只读 的8位无符号变量a

#define ONE  1   //宏定义

delay_us(1); //延时1微秒
delay_ms(1); //延时1毫秒
delay_s(1); //延时1秒

GPIO_WriteBit(LEDPORT,LED1,(BitAction)(1)); //LED控制

*/



