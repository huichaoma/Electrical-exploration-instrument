#include "LED.h"

//GPIO配置
void LED_GPIO_Config(void)
{	
  GPIO_InitTypeDef  GPIO_InitStructure;	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
  GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	//初始化两个IO
  LED1_OFF();
	LED2_OFF();
}
/*
各量程IO引脚
	F8     Power_Key
	F9     LED0
	F10    LED1
  F11     10V
  F12    150V
	F13    10mA
  F14    1A
	F15    15A
*/
void Control_GPIO_config(void)
{
	GPIO_InitTypeDef GPIO_F_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	GPIO_F_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_F_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_F_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_F_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_F_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(GPIOF, &GPIO_F_InitStructure);
	
	GPIO_SetBits(GPIOF,GPIO_Pin_8);//关闭电源
	GPIO_SetBits(GPIOF,GPIO_Pin_11);
	GPIO_SetBits(GPIOF,GPIO_Pin_12);//关闭所有通道
	GPIO_SetBits(GPIOF,GPIO_Pin_13);
	GPIO_SetBits(GPIOF,GPIO_Pin_14);
	GPIO_SetBits(GPIOF,GPIO_Pin_15);
}
