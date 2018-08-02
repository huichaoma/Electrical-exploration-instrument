#include "AD7606.h"

uint16_t data_buffer[6];   //AD采集存储
char Sam_Flag='0';         //采样完成标志位

void init_GPIO(void)
{
	GPIO_InitTypeDef  GPIOE_InitStructure,GPIOF_InitStructure,GPIOA_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SYSCFG , ENABLE);	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOA, ENABLE);//开启GPIOF的外设时钟	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	GPIOF_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2; 
	GPIOF_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIOF_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度50MHz
	GPIOF_InitStructure.GPIO_OType = GPIO_OType_PP;     //推挽输出
	GPIOF_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF,&GPIOF_InitStructure);	
	
	GPIOE_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6; 
	GPIOE_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIOE_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度100MHz
	GPIOE_InitStructure.GPIO_OType = GPIO_OType_PP;     //推挽输出
	GPIOE_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE,&GPIOE_InitStructure);	
	
		
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1); 
	
	GPIOA_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIOA_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIOA_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIOA_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOA_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIOA_InitStructure);
	
	
	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//收发模式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;// 16位数据模式
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//同步时钟空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; //捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //NSS信号硬件管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;//预分频16 
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //数据从MSB开始
	SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure); 
	SPI_Cmd(SPI1, ENABLE);
	AD_Read(0xff);//启动传输	
}

void AD7606_Reset(void)
{
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);
	GPIO_SetBits(GPIOF,GPIO_Pin_1);
	GPIO_SetBits(GPIOF,GPIO_Pin_1);
	GPIO_SetBits(GPIOF,GPIO_Pin_1);
	GPIO_SetBits(GPIOF,GPIO_Pin_1);
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);
}
void AD7606_SetOS(uint8_t data)
{
	switch(data)
	{
		case 1:AD_OS2_0();AD_OS1_0();AD_OS0_1();break;
		case 2:AD_OS2_0();AD_OS1_1();AD_OS0_0();break;
		case 3:AD_OS2_0();AD_OS1_1();AD_OS0_1();break;
		case 4:AD_OS2_1();AD_OS1_0();AD_OS0_0();break;
		case 5:AD_OS2_1();AD_OS1_0();AD_OS0_1();break;
		case 6:AD_OS2_1();AD_OS1_1();AD_OS0_0();break;
		default:AD_OS2_0();AD_OS1_0();AD_OS0_0();break;
	}
}
void AD7606_StartConv(uint8_t cv)
{
	switch(cv)
	{
		case 1:AD_CONVST_A_0();AD_CONVST_A_0();AD_CONVST_A_0();AD_CONVST_A_1();break;
		case 2:AD_CONVST_B_0();AD_CONVST_B_0();AD_CONVST_B_0();AD_CONVST_B_1();break;
		default:return;
	}
}
void AD_SetRange(uint8_t range)
{
	if(range==10)
		GPIO_SetBits(GPIOE,GPIO_Pin_5);
	else 
		GPIO_ResetBits(GPIOE,GPIO_Pin_5);
}
void AD7606_FREQ(uint32_t Freq)
{
	uint16_t Prescaler,Period;
	TIM_TimeBaseInitTypeDef TIM3_TimeBaseInitStructure;
	if (Freq == 0)
	{
		return;		
	}
	else if (Freq <= 100)   
	{
		Prescaler = 42000;	
		Period = 2000/Freq;			 	
	}
	else if (Freq <= 200000)	
	{
		Prescaler = 21;		
		Period = 4000000/Freq;
	}	
	else	
	{
		return;
	}
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  //使能TIM3时钟
  TIM3_TimeBaseInitStructure.TIM_Period = Period-1; 	//自动重装载值
	TIM3_TimeBaseInitStructure.TIM_Prescaler=Prescaler-1;  //定时器分频  Tout=((arr+1)*(psc+1))/TClk
	TIM3_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM3_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM3,&TIM3_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ClearFlag(TIM3,TIM_IT_Update);  //清除SR中UIF标志
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); 
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
}
uint16_t AD_Read(uint16_t TxData)
{		
	u8 retry=0;				 	
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)
			return 0;
  }			  
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据
	
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
  {
		retry++;
		if(retry>200)
			return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据					    
}
void Ad7606_Read(void)
{
	AD_CS_0();
	data_buffer[0]=AD_Read(0);
	data_buffer[1]=AD_Read(1);
	data_buffer[2]=AD_Read(2);
	data_buffer[3]=AD_Read(3);
	data_buffer[4]=AD_Read(4);
	data_buffer[5]=AD_Read(5);
	AD_CS_1();
	AD7606_StartConv(1);
	AD7606_StartConv(2);
}
void AD7606_Start(uint8_t range,uint8_t setos,uint32_t Freq)
{
	init_GPIO();
	AD7606_Reset();
  AD_SetRange(range);
  AD7606_SetOS(setos);
	AD7606_StartConv(1);
	AD7606_StartConv(2);
	AD7606_FREQ(Freq);
}
void AD7606_Stop(void)
{
	TIM_Cmd(TIM3, DISABLE);
}
void TIM3_IRQHandler(void) //采集数据
{
	LED2_ON();
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
	{
		 Sam_Flag='1';
		 Ad7606_Read();
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除SR中UIF标志
	LED2_OFF();
}
