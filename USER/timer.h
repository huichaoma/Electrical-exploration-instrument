#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f4xx.h"
 
void TIME_Configuration(uint16_t arr,uint16_t psc);
void delayms(uint32_t time);
 
#endif
