/*
@Rule：
    1、不会再更改的部分 不留空格；
    2、选择性使用 空格 断句；
    3、程序和注释内 回车一格；
    4、文初有简单注释，程序内尽可能减少注释；
    5、一行的注释直接写引号后面，多行的写引号的下一行，要有缩进；
    6、一切以美观为主，不必强求程序中间用几个空格；
@Author：landor163
@Date：2021年6月19日
*/


#include "stm32f10x.h"
#include "Buzzer.h"
#include "Exti.h"
#include "Key.h"
#include "SysTick.h"
#include "Usart.h"
#include "Hardware_I2C.h"
#include "Simulation_I2C.h"
#include "SPI.h"
#include "ADC.h"
#include "GeneralTimer_3.h"
#include "BaseTimer_6.h"
#include "Encoder.h"
#include "MPU6050.h"
#include "Find_Num.h"


int main(void)
{
	delay_init(72);
	BUZZER_GPIO_Config();
    EXTI_Key_Config();
    Key_GPIO_Config();
    SysTick_Init();
    USART_Config();
    ADCx_Init();
    GENERAL_TIM_Init();
    BASIC_TIM_Init();
    Encoder_Init_TIM2();
    Encoder_Init_TIM4();
    I2C_EE_Init();
    SPI_FLASH_Init();
    I2C_EE_Config();
	IIC_Init();
	MPU6050_initialize();
	DMP_Init();	
    while(1)
    {
		Read_DMP();
		printf ("Pitch=%f \n",Pitch);
    }
}

