/*
名称：GPIO输入

要点：
	使能 GPIO端口时钟；
	初始化GPIO目标引脚为 浮空输入

硬件连接：
		PA0  和 GND 接按键；VCC 接 按键；
        PC13 和 GND 接按键；VCC 接 按键；
*/

#include"Key.h"


void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd( KEY1_GPIO_CLK | KEY2_GPIO_CLK ,ENABLE);
	//Key1
	GPIO_InitStructure.GPIO_Pin= KEY1_GPIO_PIN ; 
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN_FLOATING ;       //设置为 浮空输入
	GPIO_Init( KEY1_GPIO_PORT ,&GPIO_InitStructure);
	//Key2
	GPIO_InitStructure.GPIO_Pin= KEY2_GPIO_PIN ; 
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN_FLOATING ; 
	GPIO_Init( KEY2_GPIO_PORT ,&GPIO_InitStructure);	
}

/*
函数功能：检测是否有按键按下；

输入：
    GPIOx：x可以是A、B、C、D、E；
    GPIO_Pin：等待读取的端口位；

输出：
    KEY_ON：有按键按下；
    KEY_OFF：没有按键按下；
*/
uint8_t Key_Scan( GPIO_TypeDef* GPIOx , uint16_t GPIO_Pin )
{			
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )            //检测 是否 有按键按下
	{	 
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);     //等待释放
		return 	KEY_ON;	 
	}
	else
		return KEY_OFF;
}

