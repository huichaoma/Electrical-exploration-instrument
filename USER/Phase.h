#ifndef __Phase_H
#define __Phase_H

#include "stm32f4xx.h"
#include "usart1.h"
#include "timer.h"

void IO_init(void);
void Tim4_Init(void);
void Exter_Init(char path);
void EXTI_Stop(void);
double Phase_Count(char *path);


#endif


