#ifndef __Error_H
#define __Error_H


#include "stm32f4xx.h"
#include "math.h"

float Select_Path(float data,uint8_t path);
float V10_Pos_Compensation(float data);
float V10_Neg_Compensation(float data);
float V50_Compensation(float data);
float V150_Pos_Compensation(float data);
float V150_Neg_Compensation(float data);
float A10_Pos_Compensation(float data);
float A10_Neg_Compensation(float data);
float A1_Pos_Compensation(float data);
float A1_Neg_Compensation(float data);
float A15_Pos_Compensation(float data);
float A15_Neg_Compensation(float data);



#endif

