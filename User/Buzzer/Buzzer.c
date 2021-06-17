#include "Buzzer.h"


void BUZZER_GPIO_Config( void )
{
    GPIO_InitTypeDef GPIO_InitStructure;                //定义一个 GPIO_InitTypeDef类型的 结构体

    RCC_APB2PeriphClockCmd( BUZZER_GPIO_CLK , ENABLE );     //打开 控制蜂鸣器的GPIO的端口 时钟

    GPIO_InitStructure.GPIO_Pin = BUZZER_GPIO_PIN ;        //选择 要控制蜂鸣器的 GPIO
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;    //设置 输出模式 为 通用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;   //设置 GPIO 速率为 50MHz

    GPIO_Init( BUZZER_GPIO_PORT , &GPIO_InitStructure );     //调用库函数：初始化 控制蜂鸣器的 GPIO

    GPIO_ResetBits( BUZZER_GPIO_PORT , BUZZER_GPIO_PIN );      //关闭 蜂鸣器
}

