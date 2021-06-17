#ifndef BUZZER_H
#define BUZZER_H


#include "stm32f10x.h"


#define     BUZZER_GPIO_CLK     RCC_APB2Periph_GPIOA
#define     BUZZER_GPIO_PIN     GPIO_Pin_8
#define     BUZZER_GPIO_PORT    GPIOA


void BUZZER_GPIO_Config(void);

#endif /*BUZZER_H*/

