#include "Buzzer.h"


void BUZZER_GPIO_Config( void )
{
    GPIO_InitTypeDef GPIO_InitStructure;                //����һ�� GPIO_InitTypeDef���͵� �ṹ��

    RCC_APB2PeriphClockCmd( BUZZER_GPIO_CLK , ENABLE );     //�� ���Ʒ�������GPIO�Ķ˿� ʱ��

    GPIO_InitStructure.GPIO_Pin = BUZZER_GPIO_PIN ;        //ѡ�� Ҫ���Ʒ������� GPIO
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;    //���� ���ģʽ Ϊ ͨ���������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;   //���� GPIO ����Ϊ 50MHz

    GPIO_Init( BUZZER_GPIO_PORT , &GPIO_InitStructure );     //���ÿ⺯������ʼ�� ���Ʒ������� GPIO

    GPIO_ResetBits( BUZZER_GPIO_PORT , BUZZER_GPIO_PIN );      //�ر� ������
}

