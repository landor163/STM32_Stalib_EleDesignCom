/*
@Rule：
    1、不会再更改的部分 不留空格；
    2、选择性使用 空格 断句；
    3、程序和注释内 回车一格；
    4、文初有简单注释，程序内尽可能减少注释；
    5、一行的注释直接写引号后面，多行的写引号的下一行，要有缩进；
    6、一切以美观为主，不必强求程序中间用几个空格；
@Author：landor163
@Date：2021年6月19日
*/


#include "stm32f10x.h"
#include "Buzzer.h"
#include "Exti.h"
#include "Key.h"
#include "SysTick.h"
#include "Usart.h"
#include "Hardware_I2C.h"
#include "Simulation_I2C.h"
#include "SPI.h"
#include "ADC.h"
#include "GeneralTimer_3.h"
#include "BaseTimer_6.h"
#include "Encoder.h"


typedef enum{FAILED=0,PASSED=!FAILED}TestStatus;

/* 获取缓冲区的长度 */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress

     

/* 发送缓冲区初始化 */
uint8_t Tx_Buffer[] = "感谢您选用野火stm32开发板\r\n";
uint8_t Rx_Buffer[BufferSize];

__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

/*
@函数功能：比较两个缓冲区中的数据是否相等；
@输入：
    pBuffer1：src缓冲区指针；
    pBuffer2：dst缓冲区指针；
    BufferLength：缓冲区长度；
@输出：无；
@返回：
    FAILED：pBuffer1 不等于 pBuffer2；
    PASSED：pBuffer1 等于 pBuffer2；
*/
TestStatus Buffercmp(uint8_t* pBuffer1,uint8_t* pBuffer2,uint16_t BufferLength)
{
    while(BufferLength--)
    {
        if(*pBuffer1 != *pBuffer2)
        {
            return FAILED;
        }

        pBuffer1++;
        pBuffer2++;
    }
    return PASSED;
}

int main(void)
{
    BUZZER_GPIO_Config();
    EXTI_Key_Config();
    Key_GPIO_Config();
    SysTick_Init();
    USART_Config();
    ADCx_Init();
    GENERAL_TIM_Init();
    BASIC_TIM_Init();
    Encoder_Init_TIM2();
    Encoder_Init_TIM4();
    I2C_EE_Init();
    SPI_FLASH_Init();

    DeviceID = SPI_FLASH_ReadDeviceID();
    Delay_ms(200);
    FlashID = SPI_FLASH_ReadID();

    while(1)
    {
        if (FlashID==sFLASH_ID)//检验 SPI Flash ID;
        {
            //擦除将要写入的 SPI FLASH 扇区，FLASH写入前要先擦除;
            //这里擦除4K，即一个扇区，擦除的最小单位是扇区;
            SPI_FLASH_SectorErase(FLASH_SectorToErase);	 	 
		
            //将发送缓冲区的数据写到flash中;
            //这里写一页，一页的大小为256个字节;
            SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
		
            //将刚刚写入的数据读出来放到接收缓冲区中;
            SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
		
            //检查写入的数据与读出的数据是否相等;
            TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
		
            if( PASSED == TransferStatus1 )
            { 
                ON;
            }
        }
    }
}

