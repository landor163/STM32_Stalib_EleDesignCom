#ifndef Buzzer_H
#define Buzzer_H


#include "stm32f10x.h"


#define     BUZZER_GPIO_CLK     RCC_APB2Periph_GPIOA
#define     BUZZER_GPIO_PIN     GPIO_Pin_8
#define     BUZZER_GPIO_PORT    GPIOA

#define     ON      GPIO_SetBits(BUZZER_GPIO_PORT,BUZZER_GPIO_PIN);
#define     OFF     GPIO_ResetBits(BUZZER_GPIO_PORT,BUZZER_GPIO_PIN);


void BUZZER_GPIO_Config( void );

#endif /*Buzzer_H*/

