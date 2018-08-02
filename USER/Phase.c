#include "Phase.h"

char Count_Flag='0',V_Flag='0',Path_Flag=0;
u32 TIM_CAPTURE_VAL=0,TIM_Over=0;


void IO_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOG ,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOG,&GPIO_InitStructure);
}
void Tim4_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	TIM_TimeBaseInitStructure.TIM_Period =40000-1; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=42-1;  //定时器分频  Tout=((arr+1)*(psc+1))/TClk
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//初始化TIM4
		
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_ClearFlag(TIM4,TIM_IT_Update);  //清除SR中UIF标志
	TIM_SetCounter(TIM4,0);
	//TIM_Cmd(TIM4,ENABLE); 
}
void Exter_Init(char path)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	
	IO_init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);   //打开时钟
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD,EXTI_PinSource0);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD,EXTI_PinSource1);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG,EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG,EXTI_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG,EXTI_PinSource4);
	
	switch(path)
	{
		case 1:EXTI_InitStructure.EXTI_Line=EXTI_Line0|EXTI_Line3;break;
		case 2:EXTI_InitStructure.EXTI_Line=EXTI_Line0|EXTI_Line4;break;
		case 3:EXTI_InitStructure.EXTI_Line=EXTI_Line1|EXTI_Line2;break;
		case 4:EXTI_InitStructure.EXTI_Line=EXTI_Line1|EXTI_Line3;break;
		case 5:EXTI_InitStructure.EXTI_Line=EXTI_Line1|EXTI_Line4;break;
		default:EXTI_InitStructure.EXTI_Line=EXTI_Line0|EXTI_Line2;break;
	}	
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}
void EXTI_Stop(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_LineCmd=DISABLE;
	EXTI_Init(&EXTI_InitStructure);
}
double Phase_Count(char *path)
{
	int Count=0;
	double Pha=0;
	Count_Flag='0';
	V_Flag='0';
	TIM_CAPTURE_VAL=0;
	TIM_Over=0;
	if(((path[0]=='1')&&(path[2]=='1')))   //10v，10mA,外部中断D0,G2,捕获B6
		Path_Flag=0;
	if(((path[0]=='1')&&(path[3]=='1')))   //10v，1A，外部中断D0,G3，捕获B7
		 Path_Flag=1;
	if(((path[0]=='1')&&(path[4]=='1')))   //10v,15A，外部中断D0,G4，捕获B8
		 Path_Flag=2; 
	if(((path[1]=='1')&&(path[2]=='1')))   //150v,10mA，外部中断D1,G2，捕获B9
		 Path_Flag=3;
	if(((path[1]=='1')&&(path[3]=='1')))   //150v,1A，外部中断D1,G3，捕获A0
		 Path_Flag=4;
	if(((path[1]=='1')&&(path[4]=='1')))   //150v,15A，外部中断D1,G4，捕获A1       
		 Path_Flag=5;
  Exter_Init(Path_Flag);
	while(Count_Flag=='0')
	{
		if(Count>=60)//30s自动退出
		{
				Count_Flag='2';
		}
		else
		{
				delayms(500);
				Count++;
		}
	}
	EXTI_Stop();
	//printf("TIM_Over:%d,TIM_CAPTURE_VAL:%d",TIM_Over,TIM_CAPTURE_VAL);
	if(Count_Flag=='1')
		Pha=TIM_Over*0.02+TIM_CAPTURE_VAL*0.5/1000000.0;
	else
		Pha=100;
	return Pha;
}
void EXTI0_IRQHandler(void)//10v
{
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line0);
		if((V_Flag=='1')&&(Path_Flag==0||Path_Flag==1||Path_Flag==2))
		{
			TIM_Cmd(TIM4,DISABLE); 
			TIM_CAPTURE_VAL=TIM_GetCounter(TIM4);
			Count_Flag='1';
			TIM_SetCounter(TIM4,0);
		}
	}
}
void EXTI1_IRQHandler(void)//150v
{
	if(EXTI_GetITStatus(EXTI_Line1)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line1);
		if((V_Flag=='1')&&(Path_Flag==3||Path_Flag==4||Path_Flag==5))
		{
			TIM_Cmd(TIM4,DISABLE); 
			TIM_CAPTURE_VAL=TIM_GetCounter(TIM4);
			Count_Flag='1';
			TIM_SetCounter(TIM4,0);
		}
	}
}
void EXTI2_IRQHandler(void)//10mA
{
	if(EXTI_GetITStatus(EXTI_Line2)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line2);
		if((V_Flag=='0')&&(Path_Flag==0||Path_Flag==3))
		{
			TIM_Cmd(TIM4,ENABLE); 
			V_Flag='1';
		}
		else if((V_Flag=='1')&&(Path_Flag==0||Path_Flag==3))
		{
			TIM_Cmd(TIM4,DISABLE); 
			TIM_CAPTURE_VAL=TIM_GetCounter(TIM4);
			Count_Flag='2';
			TIM_SetCounter(TIM4,0);
		}	
	}
}
void EXTI3_IRQHandler(void)//1A
{
	if(EXTI_GetITStatus(EXTI_Line3)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line3);
		if((V_Flag=='0')&&(Path_Flag==1||Path_Flag==4))
		{
			TIM_Cmd(TIM4,ENABLE); 
			V_Flag='1';
		}
		else if((V_Flag=='0')&&(Path_Flag==1||Path_Flag==4))
		{
			TIM_Cmd(TIM4,DISABLE); 
			TIM_CAPTURE_VAL=TIM_GetCounter(TIM4);
			Count_Flag='2';
			TIM_SetCounter(TIM4,0);
		}	
	}
}
void EXTI4_IRQHandler(void)//15A
{
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line4);
		if((V_Flag=='0')&&(Path_Flag==2||Path_Flag==5))
		{
			TIM_Cmd(TIM4,ENABLE);
			V_Flag='1';	
		}	
		else if((V_Flag=='0')&&(Path_Flag==2||Path_Flag==5))
		{
			TIM_Cmd(TIM4,DISABLE); 
			TIM_CAPTURE_VAL=TIM_GetCounter(TIM4);
			Count_Flag='2';
			TIM_SetCounter(TIM4,0);
		}	
	}
}
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
	{
			TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
			TIM_Over++;
	}
}
