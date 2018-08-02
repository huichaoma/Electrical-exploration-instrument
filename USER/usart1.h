#ifndef __USART1_H
#define	__USART1_H

#include "stm32f4xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
	char Empty_sign;
	char num;
	char commond[15];
	struct node *next;
}Node,*pNode; 

extern pNode Stor;

//A9ÊÇTX£¬A10ÊÇRX
pNode Create_list(int n);
void USART1_Config(void);
void IWDG_Init(u8 prer,u16 rlr);
void IWDG_Feed(void);
//int fputc(int ch, FILE *f);


#endif /* __USART1_H */
