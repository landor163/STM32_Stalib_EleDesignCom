#ifndef ADC_H
#define ADC_H


#include "stm32f10x.h"


//转换通道个数
#define     NOFCHANEL    6


#define     ADC_APBxClock_FUN           RCC_APB2PeriphClockCmd
#define     ADC_CLK                     RCC_APB2Periph_ADC1

#define     ADC_GPIO_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define     ADC_GPIO_CLK                RCC_APB2Periph_GPIOC  
#define     ADC_PORT                    GPIOC


#define     ADC_PIN1                    GPIO_Pin_0
#define     ADC_CHANNEL1                ADC_Channel_10

#define     ADC_PIN2                    GPIO_Pin_1
#define     ADC_CHANNEL2                ADC_Channel_11

#define     ADC_PIN3                    GPIO_Pin_2
#define     ADC_CHANNEL3                ADC_Channel_12

#define     ADC_PIN4                    GPIO_Pin_3
#define     ADC_CHANNEL4                ADC_Channel_13

#define     ADC_PIN5                    GPIO_Pin_4
#define     ADC_CHANNEL5                ADC_Channel_14

#define     ADC_PIN6                    GPIO_Pin_5
#define     ADC_CHANNEL6                ADC_Channel_15


//ADC1 用的 DMA1 的通道1；
//ADC3 用的 DMA2 的通道5；
//ADC2 没有 DMA 功能；
#define     ADC_x                       ADC1
#define     ADC_DMA_CHANNEL             DMA1_Channel1
#define     ADC_DMA_CLK                 RCC_AHBPeriph_DMA1


void ADCx_Init(void);

#endif /*ADC_H*/

