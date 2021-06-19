/*
书写规范：
        不会再改的部分 不留空格；
        .c注释下回车两格；
        程序 和 注释 内 回车一格；
        文初有注释，则程序内尽可能减少注释；
        一行的注释直接写引号后面，多行的写引号的下一行，要有缩进；
        选择性使用空格断句；

日期：2021年6月19日
*/


#include "stm32f10x.h"
#include "Buzzer.h"
#include "Exti.h"
#include"Key.h"


int main(void)
{
    BUZZER_GPIO_Config();
    EXTI_Key_Config();
    while(1)
    {
        OFF;

        if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON )
		{
            OFF;
		}
    }
}

