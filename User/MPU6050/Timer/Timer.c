/*
@Flie：EXTI外部中断
@Hardware connection：
                    PA0  和 GND 接按键；VCC 接 按键；
                    PC13 和 GND 接按键；VCC 接 按键；
                    按键触发外部中断条件，以上为按键硬件连接；
@Program call：
                见文件 stm32f10x_it.c 中 KEY1_IRQHandler 和 KEY2_IRQHandler 函数；
                注意相关在文件在 stm32f10x_it.h 中声明；
@Author：landor163
@Date：2021年6月19日
*/


#include "timer.h"


void Timer1_Init(u16 arr,u16 psc)  
{  
	RCC->APB2ENR|=1<<11;		//TIM1时钟使能    
 	TIM1->ARR=arr;				//设定计数器自动重装值   
	TIM1->PSC=psc;				//预分频器7200,得到10Khz的计数时钟
	TIM1->DIER|=1<<0;			//允许更新中断				
	TIM1->DIER|=1<<6;			//允许触发中断	   
	TIM1->CR1|=0x01;			//使能定时器
	MY_NVIC_Init(1,3,TIM1_UP_IRQn,2);
}

