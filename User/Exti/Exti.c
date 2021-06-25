/*
@Flie：EXTI外部中断
@Hardware connection：
                    PA0  和 GND 接按键；VCC 接 按键；
                    PC13 和 GND 接按键；VCC 接 按键；
                    按键触发外部中断条件，以上为按键硬件连接；
@Program call：
                见文件 stm32f10x_it.c 中 KEY1_IRQHandler 和 KEY2_IRQHandler 函数；
                注意相关在文件在 stm32f10x_it.h 中声明；
*/


#include "Exti.h"


static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;                      //定义一个 NVIC_InitTypeDef 类型的 结构体;
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);           //配置 NVIC 为 优先级组1
  
  NVIC_InitStructure.NVIC_IRQChannel= KEY1_INT_EXTI_IRQ ;   //配置 中断源：按键1

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;   //配置 抢先优先级

  NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;          //配置 子优先级

  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;             //使能中断通道
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel= KEY2_INT_EXTI_IRQ ;   //配置 中断源：按键2
  NVIC_Init(&NVIC_InitStructure);                           //其他使用上面的配置
}

void EXTI_Key_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd( KEY1_INT_GPIO_CLK ,ENABLE);                           //开启 按键 GPIO口 的时钟
												
	NVIC_Configuration();                                                         //配置 NVIC 中断
	
/******************************KEY1配置******************************/
  GPIO_InitStructure.GPIO_Pin= KEY1_INT_GPIO_PIN ;                              //选择 按键用到的 GPIO

  GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN_FLOATING ;                         //配置为 浮空输入
  GPIO_Init( KEY1_INT_GPIO_PORT ,&GPIO_InitStructure);

  GPIO_EXTILineConfig( KEY1_INT_EXTI_PORTSOURCE , KEY1_INT_EXTI_PINSOURCE );    //选择 Exti 的 信号源
  EXTI_InitStructure.EXTI_Line= KEY1_INT_EXTI_LINE ;
	
  EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;                             //Exti 为 中断模式

  EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;                          //上升沿中断
	
  EXTI_InitStructure.EXTI_LineCmd=ENABLE;                                       //使能 中断
  EXTI_Init(&EXTI_InitStructure);

/******************************KEY2配置******************************/
  GPIO_InitStructure.GPIO_Pin= KEY2_INT_GPIO_PIN ;                              //选择 按键用到的 GPIO

  GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN_FLOATING ;                         //配置为 浮空输入
  GPIO_Init( KEY2_INT_GPIO_PORT ,&GPIO_InitStructure);

  GPIO_EXTILineConfig( KEY2_INT_EXTI_PORTSOURCE , KEY2_INT_EXTI_PINSOURCE );    //选择 Exti 的 信号源
  EXTI_InitStructure.EXTI_Line= KEY2_INT_EXTI_LINE ;
	
  EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;                             //Exti 为 中断模式

  EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;                         //下降沿中断

  EXTI_InitStructure.EXTI_LineCmd=ENABLE;                                       //使能 中断
  EXTI_Init(&EXTI_InitStructure);
}

