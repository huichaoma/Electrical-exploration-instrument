#ifndef __Calculation_H
#define __Calculation_H

#include "stm32f4xx.h"
#include  "AD7606.h"
#include "usart1.h"
#include "Error_Compensation.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define Num_Buffer 256

extern float Data[Num_Buffer];

double Transformation_Data(double fre,uint8_t path,uint8_t al);
uint16_t Point_Samp(double fre);
void Collection_data(double fre,uint8_t path);
double Filter_AC(float *data,uint8_t path,uint16_t Periodic_Len,uint16_t len);
double Filter_DC(float *data,uint8_t path,uint16_t len,uint8_t algor);
float Data_T(uint32_t data ,uint8_t path);
void BubbleSort(float *da,uint16_t len);
float RCF(float *da,uint16_t len,float amp,uint8_t M);
float MF(float *da,uint8_t mem);
float Data_10V(double data);
float Data_50V(double data);
float Data_150V(double data);
float Data_10mA(double data);
float Data_1A(double data);
float Data_15A(double data);
float Precision_Control(float data,int len);

#endif

