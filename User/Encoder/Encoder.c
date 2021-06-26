/*
@Flie：编码器 - 定时器2、4 - 直流电机
@Hardware connection：取决于具体使用的
@Program call：
            int Encoder_left,Encoder_right;
            int main(void)
            {
                while(1)
                {
                    Encoder_left=Read_Encoder(2);
                    Encoder_right=Read_Encoder(4);
                }
            }
@Author：landor163
@Date：2021年6月25日
*/


#include "Encoder.h"


//把TIM2初始化为编码器接口模式;
void Encoder_Init_TIM2(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
    TIM_ICInitTypeDef TIM_ICInitStructure;  
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能 A 端口时钟
	
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;//引脚A0、A1；
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
  
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler=0x0;
    TIM_TimeBaseStructure.TIM_Period=ENCODER_TIM_PERIOD;
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
    TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter=10;
    TIM_ICInit(TIM2,&TIM_ICInitStructure);
    TIM_ClearFlag(TIM2,TIM_FLAG_Update);
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM_SetCounter(TIM2,0);
    TIM_Cmd(TIM2, ENABLE); 
}

void Encoder_Init_TIM4(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
    TIM_ICInitTypeDef TIM_ICInitStructure;  
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能 B 端口时钟
	
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;//引脚B6、B7；
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
  
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler=0x0;
    TIM_TimeBaseStructure.TIM_Period=ENCODER_TIM_PERIOD;
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
    TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter=10;
    TIM_ICInit(TIM4,&TIM_ICInitStructure);
    TIM_ClearFlag(TIM4,TIM_FLAG_Update);
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
    TIM_SetCounter(TIM4,0);
    TIM_Cmd(TIM4,ENABLE); 
}

int Read_Encoder(u8 TIMX)
{
    int Encoder_TIM;    
    switch(TIMX)
    {
        case 1: Encoder_TIM=(short)TIM1->CNT;TIM1->CNT=0;break;
        case 2: Encoder_TIM=(short)TIM2->CNT;TIM2->CNT=0;break;
        case 3: Encoder_TIM=(short)TIM3->CNT;TIM3->CNT=0;break;	
        case 4: Encoder_TIM=(short)TIM4->CNT;TIM4->CNT=0;break;	
        default: Encoder_TIM=0;
    }
    return Encoder_TIM;
}



