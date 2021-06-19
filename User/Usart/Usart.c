/*
名称：串口通讯

要点：
	在 配套头文件 里面直接 选择相应的 宏
*/


#include "Usart.h"


static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
    NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;

    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	//打开GPIO时钟
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	//打开串口时钟
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	//将 Usart 的 Tx 的GPIO 配置位 推挽复用
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

    //将 Usart 的 Rx 的GPIO 配置位 浮空输入
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	//配置 串口工作参数
	//配置 波特率
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	//配置 针数据 字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	//配置 停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	//配置 校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	//配置 硬件流 控制
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	//配置 工作模式 收发一体
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//完成串口初始化配置
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
	//串口中断 优先级 配置
	NVIC_Configuration();
	
	//使能 串口接收 中断
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);	
	
	//使能 串口
	USART_Cmd(DEBUG_USARTx, ENABLE);	    
}

//发送一个字节
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	//发送 一个字节 数据 到Usart
	USART_SendData(pUSARTx,ch);

	//等待 发送数据寄存器 为空
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

//发送八位数组
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
    uint8_t i;
	
	for(i=0; i<num; i++)
    {
	    /* ·¢ËÍÒ»¸ö×Ö½ÚÊý¾Ýµ½USART */
	    Usart_SendByte(pUSARTx,array[i]);	
  
    }
	//等待发送完成
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

//发送字符串
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
    unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(*(str + k)!='\0');
  
    //等待发送完成
    while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

//发送一个 16 位数
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	//取高八位
	temp_h = (ch&0XFF00)>>8;
	//取低八位
	temp_l = ch&0XFF;
	
	//发高八位
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	//发低八位
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

//重定向 printf 函数到串口
int fputc(int ch, FILE *f)
{
    //发送一个字节数据到串口
    USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
    //等待发送完毕
    while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
    return (ch);
}

//重定向 scanf 函数到串口
int fgetc(FILE *f)
{
    //等待串口输入数据
    while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

    return (int)USART_ReceiveData(DEBUG_USARTx);
}

