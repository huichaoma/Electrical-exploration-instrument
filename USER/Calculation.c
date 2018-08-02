#include "Calculation.h"

/*
power的校准值放大100倍
10v通道校准值放大1000倍
150v通道校准值放大100倍
10mA通道校准值放大1000倍
1A通道校准值放大10倍
15A通道校准值放大1000倍
传感器正输入  传感器反输入
*/

float Data[Num_Buffer]={0};

double Transformation_Data(double fre,uint8_t path,uint8_t al)
{
		double Temp_Data=0;
		memset(Data,0,sizeof(Data));
		Collection_data(fre,path);
		if(fre==0)
		{
			Temp_Data=Filter_DC(Data,path,100,al);
		}	
		else if(fre>0)
		{
			Temp_Data=Filter_AC(Data,path,32,256);
		}	
		return Temp_Data;
}										 
uint16_t Point_Samp(double fre)
{
		uint16_t sam;
			if(fre<=0)
			{
					AD7606_Start(5,0,200);
					sam=200;
			}
			else if(fre<=700)
			{
					AD7606_Start(5,0,fre*32);
					sam=256;
			}
		/*	else if(fre<=1300)
			{
						 AD7606_Start(5,0,(uint32_t)(fre*32));
						 sam=256;
			}
			else if(fre<2400)
			{
							AD7606_Start(5,0,(uint32_t)(fre*8));
							sam=128;
			}
			else if(fre<=3000)
			{
							AD7606_Start(5,0,(uint32_t)(fre*8));
							sam=128;
			}*/
	return sam;
}	
//AD存储位，信号频率，通道数(0-7)，交直流选择('0'直流，'1'交流)
void Collection_data(double fre,uint8_t path)
{
	uint16_t i=0,sam_point=0;
	sam_point=Point_Samp(fre);
	while(i<sam_point)
		if(Sam_Flag=='1')
		{
				Sam_Flag='0';
				Data[i++]=data_buffer[path];
				IWDG_Feed();
		}	
	AD7606_Stop();
}
double Filter_AC(float *data,uint8_t path,uint16_t Periodic_Sam,uint16_t len)//????
{
		int Num_Data=0,i=0;
		float Tem[8]={0};
		double Calcul=0;
		for(Num_Data=0;Num_Data<Periodic_Sam;Num_Data++)
		{
				for(i=0;i<(int)(len/Periodic_Sam);i++)
				{		
					Tem[i]=data[Num_Data+Periodic_Sam*i];
				}
				BubbleSort(Tem,8);
				data[Num_Data]=RCF(Tem,8,100,3);
				//printf("nu:%f\n",data[Num_Data]);
		}
		for(Num_Data=0;Num_Data<Periodic_Sam;Num_Data++)
		{
			data[Num_Data]=Data_T(data[Num_Data],path);
			data[Num_Data]=Select_Path(data[Num_Data],path);
			Calcul+=data[Num_Data]*data[Num_Data];
		}
		return (sqrt(Calcul/Periodic_Sam));
}
double Filter_DC(float *data,uint8_t path,uint16_t len,uint8_t algor)//???
{
		double Calcul=0;
		BubbleSort(data,len);
		if(algor==0)
			Calcul=MF(data,100);
		else if(algor==1)
		  Calcul=RCF(data,len,30,(uint16_t)(len/2));
		
		//printf("num:%f\n",Calcul);
		Calcul=Data_T(Calcul,path);
		Calcul=Select_Path(Calcul,path);
		return Calcul;
}
float Data_T(uint32_t data,uint8_t path)
{
	float temp_data;
			switch (path)
			{
				case 0:temp_data=Data_10V(data);break;
				case 1:temp_data=Data_150V(data);break;
				case 2:temp_data=Data_10mA(data);break;
				case 3:temp_data=Data_1A(data);break;
			  case 4:temp_data=Data_15A(data);break;
				case 5:temp_data=Data_50V(data);break;
				default:break;
			}
	return temp_data;
}
void BubbleSort(float *da,uint16_t len)
{
	uint16_t i=0,j=0;
	uint32_t tem;
	for(i=0;i<len;i++)           
		for(j=i+1;j<len;j++)
		{
			if(da[i]<da[j])
				{
					tem=da[j];
					da[j]=da[i];
					da[i]=tem;
				}
		}
}	
float RCF(float *da,uint16_t len,float amp,uint8_t M) 
{
	uint16_t i=0,j=0;
	float sum;
	uint32_t tem=da[M];
	for(i=0,j=0;i<len;i++)
	{
		if(fabs(da[i]-tem)<=amp) 
		{
				sum+=da[i];
				j++;
		}
	}
	return (sum/j);
}
float MF(float *da,uint8_t mem)
{
	float sum=0;
	uint16_t i=0;
	for(i=1;i<mem;i++)
		sum+=da[i];
	return (sum/(mem-1));
}

float Data_10V(double data)
{
	float temp;
	if(data>32767)
	{
		temp=(65536-data)*10/32768.0;
	}
	else
	{
		temp=-(data*10/32768.0);
	}	
	return temp;
}
float Data_50V(double data)
{
	float temp;
	if(data>32767)
	{
		temp=(65536-data)*50/32768.0;
	}
	else
	{
		temp=-(data*50/32768.0);
	}	
	return temp;
}
float Data_150V(double data)
{
	float temp;
	if(data>32767)
	{
		temp=(65536-data)*150/32768.0;
	}
	else
	{
		temp=-(data*150/32768.0);
	}
	return temp;
}
float Data_10mA(double data)
{
	float temp;
	if(data>32767)
	{
		temp=((65536-data)*10/32768.0); 
	}	
	else
	{
		temp=-(data*10/32768.0);	
	}
	return temp;
}
float Data_1A(double data)
{
	float temp;
	if(data<32768)
	{
		temp=-((data*5/32768.0)-2.5)*500;
	}	
	return temp;
}
float Data_15A(double data)
{
	float temp;
	if(data>32767)
	{
		temp=(65536-data)*5*3.75/32768.0;
	}
	else
	{
		temp=-(data*5*3.75/32768);
	}
	return temp;
}
float Precision_Control(float data,int len)
{
		float Result=0;
		if(data>0)
		{
				Result=((int)(data*pow((double)10,(double)len)+0.5))/pow((double)10,(double)len)/1.0;
		}
		else
		{
				Result=((int)(data*pow((double)10,(double)len)+0.5))/pow((double)10,(double)len)/1.0;
		}	
		return Result;
}


