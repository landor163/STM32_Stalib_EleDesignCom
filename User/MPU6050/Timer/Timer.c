/*
@Flie��EXTI�ⲿ�ж�
@Hardware connection��
                    PA0  �� GND �Ӱ�����VCC �� ������
                    PC13 �� GND �Ӱ�����VCC �� ������
                    ���������ⲿ�ж�����������Ϊ����Ӳ�����ӣ�
@Program call��
                ���ļ� stm32f10x_it.c �� KEY1_IRQHandler �� KEY2_IRQHandler ������
                ע��������ļ��� stm32f10x_it.h ��������
@Author��landor163
@Date��2021��6��19��
*/


#include "timer.h"


void Timer1_Init(u16 arr,u16 psc)  
{  
	RCC->APB2ENR|=1<<11;		//TIM1ʱ��ʹ��    
 	TIM1->ARR=arr;				//�趨�������Զ���װֵ   
	TIM1->PSC=psc;				//Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��
	TIM1->DIER|=1<<0;			//��������ж�				
	TIM1->DIER|=1<<6;			//�������ж�	   
	TIM1->CR1|=0x01;			//ʹ�ܶ�ʱ��
	MY_NVIC_Init(1,3,TIM1_UP_IRQn,2);
}

