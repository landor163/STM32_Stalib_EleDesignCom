/*
名称：EXTI外部中断

小知识：
      EXTI 初始化结构体
      typedef struct
      {
        uint32_t EXTI_Line;               //中断线
        
        EXTIMode_TypeDef EXTI_Mode;       //中断模式

        EXTITrigger_TypeDef EXTI_Trigger; //触发类型：上升沿触发/下降沿触发/都触发；

        FunctionalState EXTI_LineCmd;     //EXTI 使能
      }EXTI_InitTypeDef;

      静态函数不需要再次声明；

      目前要通用比较难，只能按键触发中断；
      按键只是为了产生 电平变化，以此来 触发中断；

要点：
    初始化需要用来产生中断的 GPIO；
    初始化Exti；
    配置NVIC；
        中断源；
        抢占优先级；
        子优先级；
        中断使能 或 失能；
    编写中断服务函数；

硬件连接：
        PA0  和 GND 接按键；VCC 接 按键；
        PC13 和 GND 接按键；VCC 接 按键；
*/


#include "Exti.h"


static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;                      //定义一个 NVIC_InitTypeDef 类型的 结构体;
  
  NVIC_PriorityGroupConfig( NVIC_PriorityGroup_1 );         //配置 NVIC 为 优先级组1
  
  NVIC_InitStructure.NVIC_IRQChannel= KEY1_INT_EXTI_IRQ ;   //配置 中断源：按键1

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1 ; //配置 抢先优先级

  NVIC_InitStructure.NVIC_IRQChannelSubPriority= 1 ;        //配置 子优先级

  NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE ;           //使能中断通道
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

