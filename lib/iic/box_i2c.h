/**
  ******************************************************************************
  * @file    box_i2c.h
  * @author  box
  * @version V1.0.0
  * @date    2021-6-10
  ******************************************************************************
  */

#include "sys.h"
#define IIC_GPIO GPIOB
#define IIC_SDAPIN GPIO_PIN_5
#define IIC_SCLPIN GPIO_PIN_4
#define uint8_t uint8_t
#define u16 uint16_t

#define Host_Address 0x75
#define Slave_Address 0xD0 //IIC写入时的地址字节数据，+1为读取

//IO方向设置
#define SDA_IN()                                                 \
  {                                                              \
    GPIO_Init(IIC_GPIO, IIC_SDAPIN, GPIO_MODE_OUT_PP_HIGH_FAST); \
  }
#define SDA_OUT()                                                \
  {                                                              \
    GPIO_Init(IIC_GPIO, IIC_SDAPIN, GPIO_MODE_OUT_PP_HIGH_FAST); \
  }
#define SDA_HIGH()                        \
  {                                       \
    GPIO_WriteHigh(IIC_GPIO, IIC_SDAPIN); \
  }
#define SDA_LOW()                        \
  {                                      \
    GPIO_WriteLow(IIC_GPIO, IIC_SDAPIN); \
  }
#define SCL_HIGH()                        \
  {                                       \
    GPIO_WriteHigh(IIC_GPIO, IIC_SCLPIN); \
  }
#define SCL_LOW()                        \
  {                                      \
    GPIO_WriteLow(IIC_GPIO, IIC_SCLPIN); \
  }
//IO操作函数
#define READ_SDA()                                    \
  {                                                   \
    (uint8_t) GPIO_ReadInputPin(IIC_GPIO, IIC_SDAPIN) \
  } //输入SDA

//IIC所有操作函数
void box_I2C_Init(void);             //初始化IIC的IO口
void box_I2C_Start(void);            //发送IIC开始信号
void box_I2C_Stop(void);             //发送IIC停止信号
void box_I2C_Send_Byte(uint8_t txd); //IIC发送一个字节
uint8_t box_I2C_Read_Byte(void);
uint8_t box_I2C_Wait_Ack(void); //IIC等待ACK信号
void box_I2C_Ack(void);         //IIC发送ACK信号
void box_I2C_NAck(void);        //IIC不发送ACK信号
void delay_1us(uint32_t num);
void delay_1ms(uint32_t num);
void box_I2C_Write(uint8_t reg_addr, uint8_t data);
uint8_t box_I2c_Read(uint8_t addr);