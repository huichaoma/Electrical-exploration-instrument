#ifndef __PWM_H
#define	__PWM_H

#include "stm32f4xx.h"
#include "delay.h"

void GPIO_PWM_Init(void);
void Tim5_Init(uint16_t  arr,uint16_t  psc);
void Set_DC(void);


#endif 
