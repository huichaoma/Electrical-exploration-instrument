#include "Coding.h"


char se[6]="01001\0";  //不同量程选择
double fre=50;         //PWM频率

void NVIC_Configuration(void) 
{  
  NVIC_InitTypeDef NVIC_Init_Structure; 

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
  NVIC_Init_Structure.NVIC_IRQChannel = USART1_IRQn; 
  NVIC_Init_Structure.NVIC_IRQChannelPreemptionPriority = 2; 
  NVIC_Init_Structure.NVIC_IRQChannelSubPriority = 2; 
  NVIC_Init_Structure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_Init_Structure); 
	
	NVIC_Init_Structure.NVIC_IRQChannel = EXTI0_IRQn; 
  NVIC_Init_Structure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_Init_Structure.NVIC_IRQChannelSubPriority = 0; 
  NVIC_Init_Structure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_Init_Structure); 
	
	NVIC_Init_Structure.NVIC_IRQChannel = EXTI1_IRQn; 
  NVIC_Init_Structure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_Init_Structure.NVIC_IRQChannelSubPriority = 0; 
  NVIC_Init_Structure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_Init_Structure);

	NVIC_Init_Structure.NVIC_IRQChannel = EXTI2_IRQn; 
  NVIC_Init_Structure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_Init_Structure.NVIC_IRQChannelSubPriority = 0; 
  NVIC_Init_Structure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_Init_Structure); 
	
	NVIC_Init_Structure.NVIC_IRQChannel = EXTI3_IRQn; 
  NVIC_Init_Structure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_Init_Structure.NVIC_IRQChannelSubPriority = 0; 
  NVIC_Init_Structure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_Init_Structure); 
	
	NVIC_Init_Structure.NVIC_IRQChannel = EXTI4_IRQn; 
  NVIC_Init_Structure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_Init_Structure.NVIC_IRQChannelSubPriority = 0; 
  NVIC_Init_Structure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_Init_Structure); 
	
	NVIC_Init_Structure.NVIC_IRQChannel = TIM7_IRQn; 
  NVIC_Init_Structure.NVIC_IRQChannelPreemptionPriority = 3; 
  NVIC_Init_Structure.NVIC_IRQChannelSubPriority = 3; 
  NVIC_Init_Structure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_Init_Structure);  
	

	NVIC_Init_Structure.NVIC_IRQChannel = TIM5_IRQn; 
  NVIC_Init_Structure.NVIC_IRQChannelPreemptionPriority = 0; 
  NVIC_Init_Structure.NVIC_IRQChannelSubPriority = 0; 
  NVIC_Init_Structure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_Init_Structure); 
	
	NVIC_Init_Structure.NVIC_IRQChannel =TIM3_IRQn; 
  NVIC_Init_Structure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_Init_Structure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_Init_Structure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_Init_Structure); 
	
	NVIC_Init_Structure.NVIC_IRQChannel =TIM4_IRQn; 
  NVIC_Init_Structure.NVIC_IRQChannelPreemptionPriority = 3; 
  NVIC_Init_Structure.NVIC_IRQChannelSubPriority = 2; 
  NVIC_Init_Structure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_Init_Structure);
	
	/*NVIC_Init_Structure.NVIC_IRQChannel =TIM2_IRQn; 
  NVIC_Init_Structure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_Init_Structure.NVIC_IRQChannelSubPriority = 0; 
  NVIC_Init_Structure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_Init_Structure);*/
}
void decoding_1(char *st)  //设置交流频率，返回交流频率
{
	char p_1[5],i=0;
	for(i=0;i<4;i++)
			p_1[i]=st[i+4];                                  
	p_1[4]='\0';
		

	if(!strncmp(p_1,"00NN",4))
	{
			GPIO_ResetBits(GPIOF,GPIO_Pin_8);
			printf("01RE00NNST");
	}
	else if(!strncmp(p_1,"00FF",4))
	{
			GPIO_SetBits(GPIOF,GPIO_Pin_8);
			printf("01RE00FFST");
	}
	else
	{
			fre=atof(p_1);
			if(fre>1)
			{
					Tim5_Init((int)(50000/fre),840);//默认PWM周期，分频系数  count=50000/f 
					printf("01RE00ACST");
			}
			else if(fre>0)
			{
					//printf("%f \n",fre);
					Tim5_Init((int)(1000/fre),42000);//默认PWM周期，分频系数  count=1000/f 
					printf("01RE00ACST");
			}
			else
			{
					Set_DC();
					printf("01RE00DCST");
			}
		}
}
void decoding_2(char *st)//直流设置、量程选择
{
	char p_2[5];
	unsigned int i;
	double Power=0;
		
	for(i=0;i<4;i++)                        
			p_2[i]=st[i+4];	
	p_2[4]='\0';

	if(!strncmp(p_2,"010V",4))
	{
			GPIO_ResetBits(GPIOF,GPIO_Pin_11);
			GPIO_SetBits(GPIOF,GPIO_Pin_12);
			printf("01RE010VST");
			se[0]='1';se[1]='0';
	}
	else if(!strncmp(p_2,"150V",4))
	{
			GPIO_ResetBits(GPIOF,GPIO_Pin_12);
			GPIO_SetBits(GPIOF,GPIO_Pin_11);
			printf("01RE150VST");
			se[0]='0';se[1]='1';
	}
	else if(!strncmp(p_2,"10mA",4))
	{
			GPIO_ResetBits(GPIOF,GPIO_Pin_13);
			GPIO_SetBits(GPIOF,GPIO_Pin_14);
			GPIO_SetBits(GPIOF,GPIO_Pin_15);
			printf("01RE10mAST");
			se[2]='1';se[3]='0';se[4]='0';
	}
	else if(!strncmp(p_2,"001A",4))
	{
			GPIO_ResetBits(GPIOF,GPIO_Pin_14);
			GPIO_SetBits(GPIOF,GPIO_Pin_13);
			GPIO_SetBits(GPIOF,GPIO_Pin_15);
			printf("01RE001AST");
			se[2]='0';se[3]='1';se[4]='0';
	}
	else if(!strncmp(p_2,"015A",4))
	{
			GPIO_ResetBits(GPIOF,GPIO_Pin_15);
			GPIO_SetBits(GPIOF,GPIO_Pin_14);
			GPIO_SetBits(GPIOF,GPIO_Pin_13);
			printf("01RE015AST");
			se[2]='0';se[3]='0';se[4]='1';
	}
	else if(!strncmp(p_2,"SAMP",4))
	{
			printf("01RESAMPST");
			Send_Data();
	}
	else if(!strncmp(p_2,"WMPW",4))
	{
			printf("01REWMPWST");
			for(i=0;i<20;i++)
			{
				Send_Data();
				Delay(2000);
				IWDG_Feed();
			}	
	}
	else if(!strncmp(p_2,"NANA",4))
	{
			printf("01RENANAST");
			Send_Nature();
	}
	else if(!strncmp(p_2,"WNAW",4))
	{
			printf("01REWNAWST");
			for(i=0;i<20;i++)
			{
				Send_Nature();
				Delay(1000);
				IWDG_Feed();
			}
	}
	else if(!strncmp(p_2,"POWE",4))
	{
			//printf("POWER");
			Power=Transformation_Data(0,5,0);
			printf("01MO%fST",Power);
	}
	else if(!strncmp(p_2,"CHAG",4))
	{
			printf("01RECHAGST");
			for(i=1;i<501;i++)
			{
					Tim5_Init((int)(50000/i),840);	
					Delay(2000);
					Send_Data();
					IWDG_Feed();
			}
	}
	else if(!strncmp(p_2,"1234",4))
	{
			printf("01RE1234ST");
			Reset();
	}					
}
void Send_Data(void) //发送数据
{
	double temp_V=0,temp_I=0,phase=0,angle=0;
	
	if(((se[0]=='1')&&(se[1]=='0')))
	{
		temp_V=Transformation_Data(fre,0,0);
		temp_V=Precision_Control(temp_V,3);
	}
	if(((se[0]=='0')&&(se[1]=='1')))
	{
		temp_V=Transformation_Data(fre,1,0);
		temp_V=Precision_Control(temp_V,3);
	}
	if(((se[2]=='1')&&(se[3]=='0')&&(se[4]=='0')))
	{
		temp_I=Transformation_Data(fre,2,0);
		temp_I=Precision_Control(temp_I,3);
	}
	if(((se[2]=='0')&&(se[3]=='1')&&(se[4]=='0')))
	{
		temp_I=Transformation_Data(fre,3,0);
		temp_I=Precision_Control(temp_I,2);
	}
	if(((se[2]=='0')&&(se[3]=='0')&&(se[4]=='1')))
	{
		temp_I=Transformation_Data(fre,4,0);
		temp_I=Precision_Control(temp_I,4);
	}
	if(fre>0)
	{
			IWDG_Feed();
			phase=Phase_Count(se);
			if(phase>=100)
				angle=4;
			else
			{
				if(phase>0.5/fre)
					angle=(phase-1.0/fre)*fre*PI;
				else
					angle=phase*fre*PI;
			}
			angle=Precision_Control(angle,6);
			printf("01MV%fST01MI%fST01MP%fST",temp_V,temp_I,angle);
	}
	else
	{
		  printf("01MV%fST01MI%fST",temp_V,temp_I);
	}
					
}
void Send_Nature(void) //发送数据
{
	double temp_V=0;
	
	if(((se[0]=='1')&&(se[1]=='0')))
	{
		temp_V=Transformation_Data(0,0,0);
		temp_V=Precision_Control(temp_V,3);
	}
	if(((se[0]=='0')&&(se[1]=='1')))
	{
		temp_V=Transformation_Data(0,1,0);
		temp_V=Precision_Control(temp_V,3);
	}
	printf("01MV%fST",temp_V);						
}
void Reset(void)
{
	//__set_FAULTMASK(1);//关闭所有中断
	NVIC_SystemReset();//复位
}
