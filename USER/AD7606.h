#ifndef __AD7606_H
#define	__AD7606_H

#include "stm32f4xx.h"
#include "LED.h"
/*
OS0            PE2            DO 
OS1            PE3            DO
OS2            PE4            DO
RAGE           PE5            DO
CVA            PE6            DO
CVB            PF0            DO
RST            PF1            DO
RD/SCLK        PA5            DO
CS             PF2            DO
DB7/DOUT1      PA6            DI
VIO            单片机电源
DB15           GND
*/

extern uint16_t data_buffer[6];
extern char Sam_Flag;

#define  AD_OS2_0()  GPIO_ResetBits(GPIOE,GPIO_Pin_4) 
#define  AD_OS2_1()  GPIO_SetBits(GPIOE,GPIO_Pin_4)
#define  AD_OS1_0()  GPIO_ResetBits(GPIOE,GPIO_Pin_3) 
#define  AD_OS1_1()  GPIO_SetBits(GPIOE,GPIO_Pin_3)
#define  AD_OS0_0()  GPIO_ResetBits(GPIOE,GPIO_Pin_2) 
#define  AD_OS0_1()  GPIO_SetBits(GPIOE,GPIO_Pin_2)

#define  AD_CONVST_A_0()  GPIO_ResetBits(GPIOE,GPIO_Pin_6)
#define  AD_CONVST_A_1()  GPIO_SetBits(GPIOE,GPIO_Pin_6)
#define  AD_CONVST_B_0()  GPIO_ResetBits(GPIOF,GPIO_Pin_0)
#define  AD_CONVST_B_1()  GPIO_SetBits(GPIOF,GPIO_Pin_0)

#define AD_CS_0() GPIO_ResetBits(GPIOF,GPIO_Pin_2)
#define AD_CS_1() GPIO_SetBits(GPIOF,GPIO_Pin_2)

void init_GPIO(void);
void AD7606_Reset(void);
void AD7606_SetOS(uint8_t data);
void AD7606_StartConv(uint8_t cv);
void AD_SetRange(uint8_t range);
void AD7606_FREQ(uint32_t Freq);
u16 AD_Read(u16 TxData);
void AD7606_Start(uint8_t range,uint8_t setos,uint32_t Freq);
void AD7606_Stop(void);
void Ad7606_Read(void);
#endif 
