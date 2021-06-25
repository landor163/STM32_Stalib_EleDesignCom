/*
@Flie：ADC电压采集 - 多通道 DMA 读取
@Hardware connection：
                    取决于使用的具体的ADC通道；
                    用作ADC采集的 IO 必须没有复用，否则采集电压会有影响；
                    PC0 在指南者接了 蜂鸣器，和SPI flash的片选，可能会有误差，是不是这么接的 不确定；
@Program call：
			while(1)
			{
				extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];     //ADC1 转换的电压值通过 MDA 的方式传到 SRAM；
				float ADC_ConvertedValueLocal[NOFCHANEL];               //用于保存转换计算后的电压值；

				ADC_ConvertedValueLocal[0] =(float) ADC_ConvertedValue[0]/4096*3.3;
				ADC_ConvertedValueLocal[1] =(float) ADC_ConvertedValue[1]/4096*3.3;
				ADC_ConvertedValueLocal[2] =(float) ADC_ConvertedValue[2]/4096*3.3;
				ADC_ConvertedValueLocal[3] =(float) ADC_ConvertedValue[3]/4096*3.3;
				ADC_ConvertedValueLocal[4] =(float) ADC_ConvertedValue[4]/4096*3.3;
				ADC_ConvertedValueLocal[5] =(float) ADC_ConvertedValue[5]/4096*3.3;
			}
@Author：landor163
@Date：2021年6月25日
*/


#include "ADC.h"


__IO uint16_t ADC_ConvertedValue[NOFCHANEL]={0,0,0,0};


static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	ADC_GPIO_APBxClock_FUN(ADC_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=ADC_PIN1|ADC_PIN2|ADC_PIN3|ADC_PIN4|ADC_PIN5|ADC_PIN6;

	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	
	GPIO_Init(ADC_PORT,&GPIO_InitStructure);				
}

static void ADCx_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	//打开 DMA 时钟
	RCC_AHBPeriphClockCmd(ADC_DMA_CLK,ENABLE);
	//打开 ADC 时钟
	ADC_APBxClock_FUN(ADC_CLK,ENABLE);
	
	//复位 DMA 控制器
	DMA_DeInit(ADC_DMA_CHANNEL);
	
	//配置 DMA 初始化结构体
	DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)(&(ADC_x->DR));                   //外设基址为：ADC 数据寄存器地址
	DMA_InitStructure.DMA_MemoryBaseAddr=(u32)ADC_ConvertedValue;                   //存储器地址；
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;                                //数据源来自外设；
	DMA_InitStructure.DMA_BufferSize=NOFCHANEL;                                     //缓冲区大小，应该等于数据目的地大小；
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;                  //外设寄存器只有一个，地址不用递增；
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;                           //存储器地址递增；
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;       //外设数据大小为 半字，即 两个字节；
    DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;               //内存数据大小也为半字，跟外设数据大小相同；	
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;	                                //循环传输模式；
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;                               //DMA 传输通道优先级为 高，当使用一个 DMA 通道时，优先级不影响；
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;                                      //禁止存储器到存储器模式，因为是外设到存储器；	
	DMA_Init(ADC_DMA_CHANNEL,&DMA_InitStructure);
	DMA_Cmd(ADC_DMA_CHANNEL,ENABLE);                                                //使能 DMA 通道；
	
	//配置 ADC 模式
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;                                //只使用一个 ADC ，属于单模式；
	ADC_InitStructure.ADC_ScanConvMode=ENABLE;                                      //扫描模式；
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;                                //连续转换模式；
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;               //不用外部触发转换，软件开启即可；
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;                            //转换结果右对齐；
	ADC_InitStructure.ADC_NbrOfChannel=NOFCHANEL;                                   //转换通道个数；
	ADC_Init(ADC_x,&ADC_InitStructure);
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);                                               //配置 ADC 时钟 CLK2 为8分频，即9MHz；
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL1, 1, ADC_SampleTime_55Cycles5);     //配置 ADC 通道的 转换顺序 和 采样时间；
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL2, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL3, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL4, 4, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL5, 5, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL6, 6, ADC_SampleTime_55Cycles5);
	ADC_DMACmd(ADC_x, ENABLE);                                                      //使能 ADC DMA 请求；
	ADC_Cmd(ADC_x, ENABLE);                                                         //开启 ADC ，并开始转换；
	ADC_ResetCalibration(ADC_x);                                                    //初始化 ADC 校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC_x));                                    //等待校准寄存器初始化完成
	ADC_StartCalibration(ADC_x);                                                    //ADC 开始校准
	while(ADC_GetCalibrationStatus(ADC_x));                                         //等待校准完成
	ADC_SoftwareStartConvCmd(ADC_x, ENABLE);                                        //由于没有使用外部触发转换，所以软件触发 ADC 转换；
}

void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
}

