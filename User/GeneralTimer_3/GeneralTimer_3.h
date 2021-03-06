#ifndef GeneralTimer_3_H
#define GeneralTimer_3_H


#include "stm32f10x.h"


#define            GENERAL_TIM                   TIM3
#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK               RCC_APB1Periph_TIM3
#define            GENERAL_TIM_Period            9      //定时器周期；设定自动重载寄存器的值；范围为 0 - 65535；
#define            GENERAL_TIM_Prescaler         71     //预分频器；范围为 0 - 65535 ，实现 1 - 65536 分频；

//输出比较通道 1
#define            GENERAL_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM_CH1_PORT          GPIOA
#define            GENERAL_TIM_CH1_PIN           GPIO_Pin_6
//输出比较通道 2
#define            GENERAL_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM_CH2_PORT          GPIOA
#define            GENERAL_TIM_CH2_PIN           GPIO_Pin_7
//输出比较通道 3
#define            GENERAL_TIM_CH3_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            GENERAL_TIM_CH3_PORT          GPIOB
#define            GENERAL_TIM_CH3_PIN           GPIO_Pin_0
//输出比较通道 4
#define            GENERAL_TIM_CH4_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            GENERAL_TIM_CH4_PORT          GPIOB
#define            GENERAL_TIM_CH4_PIN           GPIO_Pin_1


void GENERAL_ccr1_Mode_Config(int a);
void GENERAL_ccr2_Mode_Config(int a);
void GENERAL_ccr3_Mode_Config(int a);
void GENERAL_ccr4_Mode_Config(int a);
void GENERAL_TIM_Init(void);

#endif /*GeneralTimer_3_H*/

