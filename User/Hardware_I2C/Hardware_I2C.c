/*
@Flie：硬件 I2C
@Hardware connection：
                    SCL 接 PB6;
                    SDA 接 PB7;
@Program call：
            while(1)
            {
                if(I2C_Test() ==1)
                {
                    ON;
                }
                else
                {

                }
            }
@Author：landor163
@Date：2021年6月19日
*/


#include "Hardware_I2C.h"


uint16_t EEPROM_ADDRESS;
static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;   
static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);


static void I2C_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 

    EEPROM_I2C_APBxClock_FUN(EEPROM_I2C_CLK,ENABLE);

    EEPROM_I2C_GPIO_APBxClock_FUN(EEPROM_I2C_GPIO_CLK,ENABLE);

    GPIO_InitStructure.GPIO_Pin=EEPROM_I2C_SCL_PIN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;       //开漏输出；
    GPIO_Init(EEPROM_I2C_SCL_PORT,&GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin=EEPROM_I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
    GPIO_Init(EEPROM_I2C_SDA_PORT,&GPIO_InitStructure);
}

//工作模式配置；
static void I2C_Mode_Configu(void)
{
    I2C_InitTypeDef I2C_InitStructure;

    I2C_InitStructure.I2C_Mode=I2C_Mode_I2C;                                    //高电平数据稳定，低电平数据变化 SCL 时钟线的占空比；
    I2C_InitStructure.I2C_DutyCycle=I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1=I2Cx_OWN_ADDRESS7; 
    I2C_InitStructure.I2C_Ack=I2C_Ack_Enable ;
    I2C_InitStructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;     //寻址模式；
    I2C_InitStructure.I2C_ClockSpeed=I2C_Speed;                                 //通信速率；

    I2C_Init(EEPROM_I2Cx,&I2C_InitStructure);

    I2C_Cmd(EEPROM_I2Cx,ENABLE);   
}

void I2C_EE_Init(void)
{
    I2C_GPIO_Config(); 
    I2C_Mode_Configu();

//选择 EEPROM 的设备地址；
#ifdef EEPROM_Block0_ADDRESS
    EEPROM_ADDRESS=EEPROM_Block0_ADDRESS;       //选择 EEPROM Block0 来写入；
#endif

#ifdef EEPROM_Block1_ADDRESS
    EEPROM_ADDRESS=EEPROM_Block1_ADDRESS;
#endif

#ifdef EEPROM_Block2_ADDRESS
    EEPROM_ADDRESS=EEPROM_Block2_ADDRESS;
#endif

#ifdef EEPROM_Block3_ADDRESS
    EEPROM_ADDRESS=EEPROM_Block3_ADDRESS;
#endif
}

/*
@函数功能：将 缓冲区 中的数据写到 I2C EEPROM 中；
@输入：
    pBuffer：缓冲区指针；
    WriteAddr：写地址；
    NumByteToWrite：写的字节数；
@输出：无；
*/
void I2C_EE_BufferWrite(u8* pBuffer,u8 WriteAddr,u16 NumByteToWrite)
{
    u8 NumOfPage=0,NumOfSingle=0,Addr=0,count=0;

    Addr=WriteAddr%I2C_PageSize;
    count=I2C_PageSize-Addr;
    NumOfPage=NumByteToWrite/I2C_PageSize;
    NumOfSingle=NumByteToWrite%I2C_PageSize;

    if(Addr==0)                 //如果 NumOfPage 和 I2C_PageSize 对齐；
    {
        if(NumOfPage==0)        //如果 NumByteToWrite 小于 I2C_PageSize；
        {
            I2C_EE_PageWrite(pBuffer,WriteAddr,NumOfSingle);
            I2C_EE_WaitEepromStandbyState();
        }
        else                    //如果 NumByteToWrite 大于 I2C_PageSize；
        {
            while(NumOfPage--)
            {
                I2C_EE_PageWrite(pBuffer,WriteAddr,I2C_PageSize); 
                I2C_EE_WaitEepromStandbyState();
                WriteAddr+=I2C_PageSize;
                pBuffer+=I2C_PageSize;
            }

            if(NumOfSingle!=0)
            {
                I2C_EE_PageWrite(pBuffer,WriteAddr,NumOfSingle);
                I2C_EE_WaitEepromStandbyState();
            }
        }
    }
    else                        //如果 NumOfPage 和 I2C_PageSize 不齐；
    {
        if(NumOfPage==0)        //如果 NumByteToWrite 小于 I2C_PageSize；
        {
            I2C_EE_PageWrite(pBuffer,WriteAddr,NumOfSingle);
            I2C_EE_WaitEepromStandbyState();
        }
        else                    //如果 NumByteToWrite 大于 I2C_PageSize；
        {
            NumByteToWrite-=count;
            NumOfPage=NumByteToWrite/I2C_PageSize;
            NumOfSingle=NumByteToWrite%I2C_PageSize;	
      
            if(count!=0)
            {
                I2C_EE_PageWrite(pBuffer,WriteAddr,count);
                I2C_EE_WaitEepromStandbyState();
                WriteAddr+=count;
                pBuffer+=count;
            } 
      
            while(NumOfPage--)
            {
                I2C_EE_PageWrite(pBuffer,WriteAddr,I2C_PageSize);
                I2C_EE_WaitEepromStandbyState();
                WriteAddr+=I2C_PageSize;
                pBuffer+=I2C_PageSize;  
            }

            if(NumOfSingle!=0)
            {
                I2C_EE_PageWrite(pBuffer,WriteAddr,NumOfSingle); 
                I2C_EE_WaitEepromStandbyState();
            }
        }
    }  
}

/*
@函数功能：写 一个字节 到 I2C EEPROM 中；
@输入：
    pBuffer：缓冲区指针；
    WriteAddr：写地址；
@输出：简单的标志而已；
*/
uint32_t I2C_EE_ByteWrite(u8* pBuffer,u8 WriteAddr)
{
    I2C_GenerateSTART(EEPROM_I2Cx,ENABLE);                                              //发送 STRAT 条件；

    I2CTimeout=I2CT_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_MODE_SELECT))                    //在 EV5 上测试并清除；
    {
        if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(0);
    }

    I2CTimeout=I2CT_FLAG_TIMEOUT;
    I2C_Send7bitAddress(EEPROM_I2Cx,EEPROM_ADDRESS,I2C_Direction_Transmitter);          //发送 EEPROM 地址进行写入；
    while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))      //在 EV6 上测试并清除；
    {
        if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(1);
    }
    I2C_SendData(EEPROM_I2Cx,WriteAddr);                                                //发送要写入的 EEPROM 的内部地址；


    I2CTimeout=I2CT_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED))               //在 EV8 上测试并清除；
    {
        if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(2);
    }
    I2C_SendData(EEPROM_I2Cx,*pBuffer);                                                 //发送要写入的字节；


    I2CTimeout=I2CT_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED))               //在 EV8 上测试并清除；
    {
        if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(3);
    }

    I2C_GenerateSTOP(EEPROM_I2Cx,ENABLE);                                               //发送停止条件；

    return 1;
}

/*
@函数功能：在 EEPROM 的一个 写循环 中可以写 多个字节，但一次写入的字节数不能超过 EEPROM 页的大小，每页 8 个字节；
@输入：
    pBuffer：缓冲区指针；
    WriteAddr：写地址；
    NumByteToWrite：写的字节数；
@输出：简单的标志而已；
*/
uint32_t I2C_EE_PageWrite(u8* pBuffer,u8 WriteAddr,u8 NumByteToWrite)
{
    I2CTimeout=I2CT_LONG_TIMEOUT;

    while(I2C_GetFlagStatus(EEPROM_I2Cx,I2C_FLAG_BUSY))   
    {
        if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(4);
    }
    
    I2C_GenerateSTART(EEPROM_I2Cx,ENABLE);                                              //发送 STRAT 条件；

    I2CTimeout=I2CT_FLAG_TIMEOUT;

    while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_MODE_SELECT))                    //在 EV5 上测试并清除；
    {
        if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(5);
    }
    
    I2C_Send7bitAddress(EEPROM_I2Cx,EEPROM_ADDRESS,I2C_Direction_Transmitter);          //发送 EEPROM 地址进行写入；
  
    I2CTimeout=I2CT_FLAG_TIMEOUT;

    while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))      //在 EV5 上测试并清除；
    {
        if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(6);
    }
     
    I2C_SendData(EEPROM_I2Cx,WriteAddr);                                                //发送要写入的 EEPROM 的内部地址;

    I2CTimeout=I2CT_FLAG_TIMEOUT;
    
    while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED))               //在 EV8 上测试并清除；
    {
        if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(7);
    }
    
    while(NumByteToWrite--)                                                             //当有数据要写入时;
    {
        
        I2C_SendData(EEPROM_I2Cx,*pBuffer);                                             //发送当前字节；
        
        pBuffer++;                                                                      //指向下一个要写入的字节;
  
        I2CTimeout=I2CT_FLAG_TIMEOUT;
        
        while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED))           //在 EV8 上测试并清除；
        {
            if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(8);
        } 
    }
    I2C_GenerateSTOP(EEPROM_I2Cx,ENABLE);                                               //发送停止条件;
  
    return 1;
}

/*
@函数功能：在 EEPROM 里面读取一块数据；
@输入：
    pBuffer：缓冲区指针；
    ReadAddr：数据地址；
    NumByteToRead：读取的字节数；
@输出：简单的标志而已；
*/
uint32_t I2C_EE_BufferRead(u8* pBuffer,u8 ReadAddr,u16 NumByteToRead)
{  
    I2CTimeout=I2CT_LONG_TIMEOUT;
  
    while(I2C_GetFlagStatus(EEPROM_I2Cx,I2C_FLAG_BUSY))                                 //*((u8 *)0x4001080c) |= 0x80;
    {
        if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(9);
    }
    
    I2C_GenerateSTART(EEPROM_I2Cx,ENABLE);                                              //发送 STRAT 条件；
  
    I2CTimeout=I2CT_FLAG_TIMEOUT;                                                       //*((u8 *)0x4001080c) &=~ 0x80;
    
    while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_MODE_SELECT))                    //在 EV5 上测试并清除；
    {
        if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(10);
    }
    
    I2C_Send7bitAddress(EEPROM_I2Cx,EEPROM_ADDRESS,I2C_Direction_Transmitter);          //发送 EEPROM 地址进行写入；

    I2CTimeout=I2CT_FLAG_TIMEOUT;
    
    while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))      //在 EV5 上测试并清除；
    {
        if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(11);
    }
    
    I2C_Cmd(EEPROM_I2Cx,ENABLE);                                                        //通过再次设置 PE 位清除 EV6；

    I2C_SendData(EEPROM_I2Cx,ReadAddr);                                                 //发送要写入的 EEPROM 的内部地址；
   
    I2CTimeout=I2CT_FLAG_TIMEOUT;
    
    while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED))               //在 EV5 上测试并清除；
    {
        if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(12);
    }
     
    I2C_GenerateSTART(EEPROM_I2Cx,ENABLE);                                              //第二次发送 STRAT 条件；
  
    I2CTimeout=I2CT_FLAG_TIMEOUT;
    
    while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_MODE_SELECT))                    //在 EV5 上测试并清除；
    {
        if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(13);
    }
    
    I2C_Send7bitAddress(EEPROM_I2Cx,EEPROM_ADDRESS,I2C_Direction_Receiver);             //发送 EEPROM 地址以供读取；
  
    I2CTimeout=I2CT_FLAG_TIMEOUT;
    
    while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))         //在 EV6 上测试并清除；
    {
        if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(14);
    }
    
    while(NumByteToRead)                                                                //当有数据要读取时；
    {
        if(NumByteToRead==1)
        {
            
            I2C_AcknowledgeConfig(EEPROM_I2Cx,DISABLE);                                 //禁用确认；
            
            I2C_GenerateSTOP(EEPROM_I2Cx,ENABLE);                                       //发送停止条件；
        }
          
        I2CTimeout=I2CT_LONG_TIMEOUT;                                                   //在 EV7 上测试并清除；
    
		while(I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_RECEIVED)==0)  
		{
            if((I2CTimeout--)==0) return I2C_TIMEOUT_UserCallback(3);
		}

        {
            *pBuffer=I2C_ReceiveData(EEPROM_I2Cx);                                      //从 EEPROM 中读取一个字节；
            
            pBuffer++;                                                                  //指向将保存读取的字节的下一个位置；
            
            NumByteToRead--;                                                            //减少读取字节计数器；
        }   
    }
    
    I2C_AcknowledgeConfig(EEPROM_I2Cx,ENABLE);                                          //启用确认以准备另一次接收；
  
    return 1;
}

//等待 EEPROM 待机状态；
void I2C_EE_WaitEepromStandbyState(void)
{
    vu16 SR1_Tmp = 0;

    do
    {
        I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);                                             //发送 STRAT 条件；
        
        SR1_Tmp = I2C_ReadRegister(EEPROM_I2Cx, I2C_Register_SR1);                          //读取 I2C1 SR1 寄存器；

        I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);        //发送 EEPROM 地址进行写入；
    }while(!(I2C_ReadRegister(EEPROM_I2Cx, I2C_Register_SR1) & 0x0002));
    
    I2C_ClearFlag(EEPROM_I2Cx, I2C_FLAG_AF);                                                //清除 AF 标志；
     
    I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);                                                  //停止条件；
}

/*
@函数功能：超时情况的基本管理；
@输入：errorCode：错误代码，可以用来定位是哪个环节出错；
@输出：返回 0 ，表示 I2C 读取失败；
*/
static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
    EEPROM_ERROR("I2C 等待超时!errorCode = %d",errorCode);      //阻止通信和所有进程;
  
    return 0;
}


#define     EEP_Firstpage       0x00
uint8_t I2c_Buf_Write[256];
uint8_t I2c_Buf_Read[256];


/*
@函数功能：I2C 读写测试；
@输入：无；
@输出：正常返回1，异常返回0；
*/
uint8_t I2C_Test(void)
{
    uint16_t i;

    printf("写入的数据\n\r");
    
    for (i=0;i<=255;i++)                                        //填充缓冲;
    {   
        I2c_Buf_Write[i]=i;

        printf("0x%02X",I2c_Buf_Write[i]);

        if(i%16==15)
        {
            printf("\n\r");
        }  
    }

    I2C_EE_BufferWrite(I2c_Buf_Write,EEP_Firstpage,256);        //将I2c_Buf_Write中顺序递增的数据写入EERPOM中;

    EEPROM_INFO("\n\r写成功\n\r");

    EEPROM_INFO("\n\r读出的数据\n\r");

    I2C_EE_BufferRead(I2c_Buf_Read,EEP_Firstpage,256);          //将EEPROM读出数据顺序保持到I2c_Buf_Read中;

    for (i=0;i<256;i++)                                         //将I2c_Buf_Read中的数据通过串口打印;
    {	
        if(I2c_Buf_Read[i]!=I2c_Buf_Write[i])
        {
            EEPROM_ERROR("0x%02X",I2c_Buf_Read[i]);
            EEPROM_ERROR("错误: I2C EEPROM写入与读出的数据不一致\n\r");
            return 0;
        }
        printf("0x%02X",I2c_Buf_Read[i]);
        if(i%16==15)    
            printf("\n\r");
	}
    EEPROM_INFO("I2C(AT24C02)读写测试成功\n\r");
  
    return 1;
}

