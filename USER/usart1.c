#include "usart1.h"


//���������ַ���
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
	
	// ���ô���1 ��USART1�� ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
	
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10
	  
	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate =115200;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
}
void IWDG_Init(u8 prer,u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //ȡ��д����
	IWDG_SetPrescaler(prer);//���÷�Ƶϵ��
	IWDG_SetReload(rlr); //����װ��ֵ
	IWDG_ReloadCounter();//����װ��
	IWDG_Enable();//ʹ��
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
 * ��������fputc
 * ����  ���ض���c�⺯��printf��USART1
 * ����  ����
 * ���  ����
 * ����  ����printf����
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
