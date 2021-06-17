#include "stm32f10x.h"
#include "Buzzer.h"
#include "Exti.h"


int main ( void )
{
	BUZZER_GPIO_Config(  );       //蜂鸣器
    EXTI_Key_Config(  );
    while( 1 )
    {
        //ON;     //蜂鸣器
    }
}

