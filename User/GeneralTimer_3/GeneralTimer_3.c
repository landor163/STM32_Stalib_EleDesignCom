/*
@Flie：通用定时器 - PWM输出
@Hardware connection：取决于具体使用的 输出比较通道；
@Program call：
@Author：landor163
@Date：2021年6月25日
*/


#include "GeneralTimer_3.h"


static void GENERAL_TIM_GPIO_Config(void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //输出比较通道 1 GPIO 初始化
    RCC_APB2PeriphClockCmd(GENERAL_TIM_CH1_GPIO_CLK,ENABLE);
    GPIO_InitStructure.GPIO_Pin=GENERAL_TIM_CH1_PIN;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GENERAL_TIM_CH1_PORT,&GPIO_InitStructure);
	
	//输出比较通道 2 GPIO 初始化
    RCC_APB2PeriphClockCmd(GENERAL_TIM_CH2_GPIO_CLK,ENABLE);
    GPIO_InitStructure.GPIO_Pin=GENERAL_TIM_CH2_PIN;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GENERAL_TIM_CH2_PORT,&GPIO_InitStructure);
	
    //输出比较通道 3 GPIO 初始化
    RCC_APB2PeriphClockCmd(GENERAL_TIM_CH3_GPIO_CLK,ENABLE);
    GPIO_InitStructure.GPIO_Pin=GENERAL_TIM_CH3_PIN;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GENERAL_TIM_CH3_PORT,&GPIO_InitStructure);
	
    //输出比较通道 4 GPIO 初始化
    RCC_APB2PeriphClockCmd(GENERAL_TIM_CH4_GPIO_CLK,ENABLE);
    GPIO_InitStructure.GPIO_Pin=GENERAL_TIM_CH4_PIN;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GENERAL_TIM_CH4_PORT,&GPIO_InitStructure);
}

static void GENERAL_TIM_Mode_Config(void)
{
    GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK,ENABLE);//开启定时器时钟，即内部时钟 CK_INT=72M;

    //时基结构体初始化；
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;                  //配置周期这里配置为100K；
	TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM_Period;            //自动重新装载寄存器的值，累计 GENERAL_TIM_Period +1 个频率后产生一个更新或中断；
	TIM_TimeBaseStructure.TIM_Prescaler=GENERAL_TIM_Prescaler;     //驱动 CNT 计数器的时钟=Fck_int/(psc+1)；
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;           //时钟分频因子，配置死区时间时需要用到；
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;       //计数器计数模式，设置为向上计数；
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;                  //重复计数器的值；
	TIM_TimeBaseInit(GENERAL_TIM,&TIM_TimeBaseStructure);

	//输出比较结构体初始化
	uint16_t CCR1_Val=5;//占空比配置
	uint16_t CCR2_Val=4;
	uint16_t CCR3_Val=3;
	uint16_t CCR4_Val=2;
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//配置为 PWM 模式 1
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//输出使能
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//输出通道电平极性配置
	
	//输出比较通道 1
	TIM_OCInitStructure.TIM_Pulse=CCR1_Val;
	TIM_OC1Init(GENERAL_TIM,&TIM_OCInitStructure);
	TIM_OC1PreloadConfig(GENERAL_TIM,TIM_OCPreload_Enable);
	
	//输出比较通道 2
	TIM_OCInitStructure.TIM_Pulse=CCR2_Val;
	TIM_OC2Init(GENERAL_TIM,&TIM_OCInitStructure);
	TIM_OC2PreloadConfig(GENERAL_TIM,TIM_OCPreload_Enable);
	
	//输出比较通道 3
	TIM_OCInitStructure.TIM_Pulse=CCR3_Val;
	TIM_OC3Init(GENERAL_TIM,&TIM_OCInitStructure);
	TIM_OC3PreloadConfig(GENERAL_TIM,TIM_OCPreload_Enable);
	
	//输出比较通道 4
	TIM_OCInitStructure.TIM_Pulse=CCR4_Val;
	TIM_OC4Init(GENERAL_TIM,&TIM_OCInitStructure);
	TIM_OC4PreloadConfig(GENERAL_TIM,TIM_OCPreload_Enable);
	
	TIM_Cmd(GENERAL_TIM, ENABLE);//使能计数器
}

void GENERAL_TIM_Init(void)
{
    GENERAL_TIM_GPIO_Config();
    GENERAL_TIM_Mode_Config();		
}

