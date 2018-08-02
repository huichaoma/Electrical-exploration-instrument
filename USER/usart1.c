#include "usart1.h"


//接收命令字符串
pNode Stor;

pNode Create_list(int n)
{
	int i=0;
	pNode temp;
	pNode head=(pNode)malloc(sizeof(Node));
	head->num=0;
	head->Empty_sign=0;
	
	temp=head;
	temp->next=temp;
	
	for(i=0;i<n;i++)
	{
		pNode newNode=(pNode)malloc(sizeof(Node));
		newNode->Empty_sign=0;
		newNode->num=0;
		temp->next=newNode;
		
		temp=newNode;
		temp->next=head;
	}
	
	return head;
}	
void USART1_Config(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	// 配置串口1 （USART1） 时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
	
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10
	  
	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate =115200;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断
	USART_Cmd(USART1, ENABLE);  //使能串口1 
}
void IWDG_Init(u8 prer,u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //取消写保护
	IWDG_SetPrescaler(prer);//设置分频系数
	IWDG_SetReload(rlr); //设置装载值
	IWDG_ReloadCounter();//重新装载
	IWDG_Enable();//使能
}
void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}

void USART1_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART1,USART_IT_RXNE)!=RESET)
	{
		if(Stor->Empty_sign==0)
		{
			Stor->commond[Stor->num++]=USART_ReceiveData(USART1);
			if((Stor->num==15)||(Stor->commond[Stor->num-1]=='T'))
			{
					Stor->commond[++Stor->num]='\0';
					Stor->Empty_sign=1;
					Stor=Stor->next;
			}
		}
		else
				printf("error,error");
	}
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
}

/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
#if 1
#pragma import(__use_no_semihosting)                              
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;         
void _sys_exit(int x) 
{ 
	x = x; 
} 
//__use_no_semihosting was requested, but _ttywrch was 
void _ttywrch(int ch)
{
ch = ch;
}
int fputc(int ch, FILE *f)
{
  	USART_SendData(USART1, (u8) ch);
  	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
  	return ch;
}
#endif
