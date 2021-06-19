#ifndef Key_H
#define Key_H


#include "stm32f10x.h"


#define    KEY1_GPIO_CLK    RCC_APB2Periph_GPIOA
#define    KEY1_GPIO_PORT   GPIOA			   
#define    KEY1_GPIO_PIN    GPIO_Pin_0

#define    KEY2_GPIO_CLK    RCC_APB2Periph_GPIOC
#define    KEY2_GPIO_PORT   GPIOC		   
#define    KEY2_GPIO_PIN    GPIO_Pin_13
//按键按下为高电平，如下；反之，互换；
#define KEY_ON	1
#define KEY_OFF	0


void Key_GPIO_Config(void);
uint8_t Key_Scan( GPIO_TypeDef* GPIOx , uint16_t GPIO_Pin );

#endif /*Key_H*/

