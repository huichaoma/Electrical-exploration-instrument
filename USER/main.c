#include "stm32f4xx.h"
#include "PWM.h"
#include "AD7606.h"
#include "LED.h"
#include "timer.h"
#include "Calculation.h"
#include "Coding.h"
#include "usart1.h"
#include "Phase.h"
#include "delay.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(void)
{
	pNode Car;
  Tim5_Init(1000,840);//PWM输出默认50Hz
	IWDG_Init(7,4095);//看门狗初始化
	USART1_Config(); //串口初始化
  //AD7606_Start(5,0,200);//采集频率设置  
	Tim4_Init( );
  NVIC_Configuration( );
	LED_GPIO_Config();
	Control_GPIO_config();//控制IO初始化
	LED1_ON();
	delayms(500);
	LED1_OFF();
	Car=Stor=Create_list(7);
	while(1)
	{
		//printf("Sam:%d\n",data_buffer[5]);
		//delayms(500);
			if(Car->Empty_sign==1)
			{
				//printf("Flag:%d,num:%d,Com:%s\n",Car->Empty_sign,Car->num,Car->commond);
				if((Car->num==11)&&(Car->commond[0]=='0'))
				{
						if(!strncmp(Car->commond,"01DA",4))
						{
								decoding_1(Car->commond);
						}
						else if(!strncmp(Car->commond,"01CO",4))
						{
								decoding_2(Car->commond);
						}
				}
				memset(Car->commond,0,sizeof(Car->commond));
				Car->Empty_sign=0;
				Car->num=0;
				Car=Car->next;
			}
			IWDG_Feed();
	}
}




