/*
@Flie：系统定时器
@Author：landor163
@Date：2021年6月19日
*/


#include "SysTick.h"


static __IO u32 TimingDelay;//下面会用到的一个变量


void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
//	if (SysTick_Config(SystemFrequency / 100000))	// ST3.0.0库版本
	if (SysTick_Config(SystemCoreClock / 100000))	// ST3.5.0库版本
	{ 
		while(1);
	}
}

//微秒延时程序，10us一个单位；
void Delay_us(__IO u32 nTime)
{ 
	TimingDelay=nTime;	
	//使能 滴答定时器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;

	while(TimingDelay!=0);
}

//获取节拍程序，中断 SysTick_Handler() 中有调用；
void TimingDelay_Decrement(void)
{
	if (TimingDelay!=0x00)
	{ 
		TimingDelay--;
	}
}

#if 0
// 这个 固件库函数 在 core_cm3.h 中
static __INLINE uint32_t SysTick_Config(uint32_t ticks)
{ 
    //reload寄存器为24bit，最大为2^24
    if(ticks > SysTick_LOAD_RELOAD_Msk)  return (1);
  
    //配置reload寄存器初始值
    SysTick->LOAD=(ticks&SysTick_LOAD_RELOAD_Msk)-1;
	
    //配置中断优先级为 1<<4-1 =15,优先级为最低
    NVIC_SetPriority(SysTick_IRQn,(1<<__NVIC_PRIO_BITS)-1); 
	
    //配置counter计数器的值
    SysTick->VAL=0;
	
	//配置 systick的 时钟为 72M
	//使能 中断
	//使能 systick
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk| 
                   SysTick_CTRL_TICKINT_Msk| 
                   SysTick_CTRL_ENABLE_Msk;                    
    return (0); 
}
#endif

void SysTick_Delay_Us(__IO uint32_t us)
{
	uint32_t i;
	SysTick_Config(SystemCoreClock/1000000);
	
	for(i=0;i<us;i++)
	{
		//当计数器的值减小到0的时候， CRTL 寄存器的 位16 置1
		while(!((SysTick->CTRL)&(1<<16)));
	}
	//关闭systick定时器
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Delay_Ms(__IO uint32_t ms)
{
	uint32_t i;	
	SysTick_Config(SystemCoreClock/1000);
	
	for(i=0;i<ms;i++)
	{
		//当计数器的值减小到0的时候， CRTL 寄存器的 位16 置1
		//当置1 时，读取该位会 清0
		while(!((SysTick->CTRL)&(1<<16)));
	}
	//关闭systick定时器
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
}

