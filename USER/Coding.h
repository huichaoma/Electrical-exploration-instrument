#ifndef __Coding_H
#define __Coding_H

#include "stm32f4xx.h"
#include "Calculation.h"
#include "PWM.h"
#include "timer.h"
#include "usart1.H"
#include "Phase.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PI 3.1415926

extern char se[6];
extern double fre;

void NVIC_Configuration(void);
void decoding_1(char *st);
void decoding_2(char *st);
void Send_Data(void);
void Send_Nature(void);
void Reset(void);

#endif
