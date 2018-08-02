#include "timer.h"

uint32_t tim;
//定时器7配置
void TIME_Configuration(uint16_t arr,uint16_t psc)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  //使能TIM7时钟
  TIM_TimeBaseInitStructure.TIM_Period = arr-1; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc-1;  //定时器分频  Tout=((arr+1)*(psc+1))/TClk
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);//初始化TIM7
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
	TIM_ClearFlag(TIM7,TIM_IT_Update);  //清除SR中UIF标志
	TIM_Cmd(TIM7,ENABLE); //使能定时器7
}
void delayms(uint32_t time) //0.1ms
{
	tim=time;
	TIME_Configuration(100,840);
	while(tim);
}
void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET)
	{
			TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
			tim--;
	}
}
