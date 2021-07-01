/*
@Flie：软件 I2C
@Hardware connection：
                    SCL 接 PB6;
                    SDA 接 PB7;
@Program call：
            uint8_t readData[10]={0};
            uint8_t writeData[8]={4,5,6,7,8,9,10,11};
            uint8_t i=0;

            while(1)
            {
                EEPROM_Byte_Write(11,55);
            
                //等待写入操作完成;
                EEPROM_WaitForWriteEnd();
            
                //写入一个字节;
                EEPROM_Byte_Write(12,52);
            
                //等待写入操作完成;
                EEPROM_WaitForWriteEnd();
            
                //addr%8==0,即为地址对齐;
                EEPROM_Page_Write(16,writeData,8);
            
                //等待写入操作完成;
                EEPROM_WaitForWriteEnd();
            
                //读取数据;
                EEPROM_Read(16,readData,8);
                for(i=0;i<8;i++)
                {	
                    printf("%d",readData[i]);	
                }
                printf("111111111111111111");
            }
@Author：landor163
@Date：2021年6月19日
*/


#include "Simulation_I2C.h"


void I2C_EE_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure; 

    EEPROM_I2C_GPIO_APBxClkCmd(EEPROM_I2C_SCL_GPIO_CLK|EEPROM_I2C_SDA_GPIO_CLK,ENABLE);     //打开 IIC GPIO 的时钟；
    
    EEPROM_I2C_APBxClkCmd(EEPROM_I2C_CLK,ENABLE);                                           //打开 IIC 外设的时钟；

    //将 IIC SCL 的GPIO配置为 推挽复用模式；
    GPIO_InitStructure.GPIO_Pin=EEPROM_I2C_SCL_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT,&GPIO_InitStructure);

    //将 IIC SDA 的GPIO配置为 浮空输入模式；
    GPIO_InitStructure.GPIO_Pin=EEPROM_I2C_SDA_GPIO_PIN;
    GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT,&GPIO_InitStructure);
        
    //配置IIC的工作参数
    I2C_InitStructure.I2C_Ack=I2C_Ack_Enable;                                               //使能应答
    I2C_InitStructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;                 //使用7位地址模式
    I2C_InitStructure.I2C_ClockSpeed=EEPROM_I2_BAUDRATE;                                    //配置SCL时钟频率
    I2C_InitStructure.I2C_DutyCycle=I2C_DutyCycle_2;
    I2C_InitStructure.I2C_Mode=I2C_Mode_I2C;
    I2C_InitStructure.I2C_OwnAddress1=STM32_I2C_OWN_ADDR;                                   //这是STM32 IIC自身设备地址，只要是总线上唯一即可
	
    I2C_Init(EEPROM_I2C,&I2C_InitStructure);
	
    //使能串口；
    I2C_Cmd(EEPROM_I2C,ENABLE);	    
}

//向 EEPROM 写入一个字节；
void EEPROM_Byte_Write(uint8_t addr,uint8_t data)
{
    I2C_GenerateSTART(EEPROM_I2C,ENABLE);                                       //产生起始信号；
        
    while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
    
    I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDR,I2C_Direction_Transmitter);      //EV5事件被检测到，发送设备地址；
    
    while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
  
    I2C_SendData(EEPROM_I2C,addr);                                              //EV6事件被检测到，发送要操作的存储单元地址；
	
    while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTING)==ERROR);
    
    I2C_SendData(EEPROM_I2C,data);                                              //EV8事件被检测到，发送要存储的数据；
	
    while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);
	
	I2C_GenerateSTOP(EEPROM_I2C,ENABLE);                                        //数据传输完成；
}

//向 EEPROM 写入多个字节（页写入）,每次写入不能超过 8个字节;
void EEPROM_Page_Write(uint8_t addr,uint8_t *data,uint8_t numByteToWrite)
{
    I2C_GenerateSTART(EEPROM_I2C,ENABLE);

    while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
    I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDR,I2C_Direction_Transmitter);

    while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
    I2C_SendData(EEPROM_I2C,addr);

    while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTING)==ERROR);
    while(numByteToWrite)
    {
        I2C_SendData(EEPROM_I2C,*data);

        while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);
        data++;
        numByteToWrite--;
    }

    I2C_GenerateSTOP(EEPROM_I2C,ENABLE);	
}

//从 EEPROM 读取数据；
void EEPROM_Read(uint8_t addr,uint8_t *data,uint8_t numByteToRead)
{
    I2C_GenerateSTART(EEPROM_I2C,ENABLE);

    while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
    I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDR,I2C_Direction_Transmitter);
	
    while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
    I2C_SendData(EEPROM_I2C,addr);
	
    while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTING)==ERROR);
    I2C_GenerateSTART(EEPROM_I2C,ENABLE);
	
    while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
    I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDR,I2C_Direction_Receiver);
	
    while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)==ERROR);
    while(numByteToRead)
    {
        if(numByteToRead==1)
        {
            I2C_AcknowledgeConfig(EEPROM_I2C,DISABLE);
        }
        
        while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_RECEIVED)==ERROR);
        *data=I2C_ReceiveData(EEPROM_I2C);
        data++;
        numByteToRead--;
    }
    I2C_GenerateSTOP(EEPROM_I2C,ENABLE);
    
    I2C_AcknowledgeConfig(EEPROM_I2C,ENABLE);
}

//等待 EEPROM 内部时序完成;
void EEPROM_WaitForWriteEnd(void)
{
    do
    {
        I2C_GenerateSTART(EEPROM_I2C,ENABLE);
		
        while(I2C_GetFlagStatus(EEPROM_I2C,I2C_FLAG_SB)==RESET);
        I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDR,I2C_Direction_Transmitter);
    }  
    while(I2C_GetFlagStatus(EEPROM_I2C,I2C_FLAG_ADDR)==RESET);
    I2C_GenerateSTOP(EEPROM_I2C,ENABLE);	
}

