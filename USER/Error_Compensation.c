#include "Error_Compensation.h"


float Select_Path(float data,uint8_t path)
{
	float Result=0;
	if(data>0)
	{
		switch(path)
		{
			case 0:Result=V10_Pos_Compensation(data);break;
			case 1:Result=V150_Pos_Compensation(data);break;
			case 2:Result=A10_Pos_Compensation(data);break;
			case 3:Result=A1_Pos_Compensation(data);break;
			case 4:Result=A15_Pos_Compensation(data);break;
			case 5:Result=V50_Compensation(data);break;
		}	
	}	
	else
	{
		switch(path)
		{
			case 0:Result=V10_Neg_Compensation(data);break;
			case 1:Result=V150_Neg_Compensation(data);break;
			case 2:Result=A10_Neg_Compensation(data);break;
			case 3:Result=A1_Neg_Compensation(data);break;
			case 4:Result=A15_Neg_Compensation(data);break;
		}	
	}	
	return Result;
}	
float V50_Compensation(float data)
{
		double Com=0;
		Com=(double)data*0.0352957646441775-0.04886719106876572;
		return (data+Com);
}	
float V10_Pos_Compensation(float data)//保留3位小数，2位浮动
{
		double Com=0;
		Com=(double)data*0.222390613067898-0.00563408087610212;
		return (data+Com);  
}	
float V10_Neg_Compensation(float data)
{
		double Com=0;
		Com=(double)data*0.221058593432517+0.0450435472296060;
		return (data+Com); 
}	
float V150_Pos_Compensation(float data)//保留3位小数,2位浮动
{
		double Com=0;
		Com=(double)0.00855161140544414*data+0.221666037771290;
		return (data+Com);
}	
float V150_Neg_Compensation(float data)
{
		double Com=0;
		Com=(double)0.00737680401701975*data+0.251826366772820;
		return (data+Com);
}	
float A10_Pos_Compensation(float data)//保留3位小数，2位变动
{
		double Com=0;
		Com=(double)-0.00244857741697068*data+0.0764776780024652;
		return (data+Com);
}	
float A10_Neg_Compensation(float data)
{
		double Com=0;
		Com=(double)-0.00103014073977929*data+0.0705238142226986;
		return (data+Com);
}	
float A1_Pos_Compensation(float data)//保留2位小数，2位变动
{
		double Com=0;
		Com=(double)0.00200146266261944*data-2.48391650109527;
		return (data+Com);
}	
float A1_Neg_Compensation(float data)
{
		double Com=0;
		Com=(double)0.00317305275669927*data-3.22510482652487;
		return (data+Com);
}	
float A15_Pos_Compensation(float data)//保留4位小数，2位变动
{
		float Com=0;
		Com=0.00141108761445109*(data-(double)0.1813)-0.17892992095438351;
		return (Com+data);
}	
float A15_Neg_Compensation(float data)
{
		float Com=0;
		Com=-0.1816;
		return (data+Com);
}	

