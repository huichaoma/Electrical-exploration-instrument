 #include "PWM.h"

char flag_1=0;

void GPIO_PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); //使能PORTC时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;   //上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure);  
	GPIO_ResetBits(GPIOE,GPIO_Pin_0);
	GPIO_ResetBits(GPIOE,GPIO_Pin_1);
}
void Tim5_Init(uint16_t  arr,uint16_t  psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	GPIO_PWM_Init();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  //TIM3时钟使能
  TIM_TimeBaseStructure.TIM_Prescaler=psc-1; //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数
  TIM_TimeBaseStructure.TIM_Period=arr-1; //自动重装载值
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM5,ENABLE);	
}
void Set_DC(void)
{
	TIM_Cmd(TIM5,DISABLE);
	GPIO_ResetBits(GPIOE,GPIO_Pin_1);
	GPIO_SetBits(GPIOE,GPIO_Pin_0);
}
void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5,TIM_IT_Update)==SET)
	{
		if(flag_1==1)
		{
			flag_1=0;
			GPIO_ResetBits(GPIOE,GPIO_Pin_1);
			Delay(700);
			GPIO_SetBits(GPIOE,GPIO_Pin_0);
		}
		else
		{
			flag_1=1;
			GPIO_ResetBits(GPIOE,GPIO_Pin_0);
			Delay(700);
			GPIO_SetBits(GPIOE,GPIO_Pin_1);
		}
	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
}
