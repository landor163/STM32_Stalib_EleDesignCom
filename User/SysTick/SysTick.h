#ifndef SysTick_H
#define SysTick_H


#include "stm32f10x.h"
#include "core_cm3.h"
#include "misc.h"


void SysTick_Init(void);
void Delay_us(__IO u32 nTime);

#define Delay_ms(x) Delay_us(100*x)

void TimingDelay_Decrement(void);
void SysTick_Delay_Us( __IO uint32_t us);
void SysTick_Delay_Ms( __IO uint32_t ms);

#endif /*SysTick_H*/

