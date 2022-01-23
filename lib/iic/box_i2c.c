/**
  ******************************************************************************
  * @file    box_i2c.c
  * @author  box
  * @version V1.0.0
  * @date    2021-6-10
  ******************************************************************************
  */

#include "box_i2c.h"

//初始化
void box_I2C_Init(void)
{
    GPIO_Init(IIC_GPIO, IIC_SCLPIN, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(IIC_GPIO, IIC_SDAPIN, GPIO_MODE_OUT_PP_HIGH_FAST);
}

//发送开始信号
void box_I2C_Start(void)
{
    SDA_OUT();   //SDA设置为输出
    SDA_HIGH();  //拉高SDA
    SCL_HIGH();  //拉高SCL
    delay_us(2); //等待2us
    SDA_LOW();   //拉低SDA，给一个由高到低的跳变
    delay_us(2); //等待2us
    SCL_LOW();   //钳住I2C时钟总线，准备发送或接收数据
}

//发送停止信号
void box_I2C_Stop(void)
{
    SDA_OUT();   //SDA设置为输出
    SDA_LOW();   //拉低SDA
    SCL_HIGH();  //拉高SCL
    delay_us(2); //等待2us
    SDA_HIGH();  //拉低SDA，给一个由低到高的跳变
    delay_us(2); //等待2us
}

//产生一个ACK应答信号
void box_I2C_Ack(void)
{
    SCL_LOW();   //拉低SCL
    SDA_OUT();   //SDA设置为输出
    SDA_LOW();   //拉低SDA
    delay_us(4); //保持4us
    SCL_HIGH();  //拉高SCL
    delay_us(4); //保持4us
    SCL_LOW();   //拉低SCL
}

//产生一个NACK非应答信号
void box_I2C_NAck(void)
{
    SCL_LOW();   //拉低SCL
    SDA_OUT();   //SDA设置为输出
    SDA_HIGH();  //拉低SDA
    delay_us(4); //保持4us
    SCL_HIGH();  //拉高SCL
    delay_us(4); //保持4us
    SCL_LOW();   //拉低SCL
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t box_I2C_Wait_Ack(void)
{
    uint8_t uTime = 0;
    SDA_IN(); //SDA设置为输入
              //	SDA_HIGH();
    SCL_HIGH();
    delay_us(4);

    while (GPIO_ReadInputPin(IIC_GPIO, IIC_SDAPIN))
    {
        uTime++;
        if (uTime > 250)
        {
            box_I2C_Stop();
            return 1;
        }
    }
    SCL_LOW(); //时钟输出0
    return 0;
}

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void box_I2C_Send_Byte(uint8_t txd)
{
    uint8_t t;              //用来计数的临时变量
    SDA_OUT();              //配置数据为输出
    SCL_LOW();              //拉低时钟开始数据传输
    for (t = 0; t < 8; t++) //发送八位
    {
        if ((txd & 0x80) >> 7) //判断最高位数据  0x1000 0000
        {
            SDA_HIGH(); //如果最高位为1，SDA线置高发送1
        }
        else
        {
            SDA_LOW(); //如果最高位为0，SDA线置低发送0
        }

        txd <<= 1; //将txd左移一位 ,等待下次发送
                   //		delay_us(1);   //对TEA5767这三个延时都是必须的
                   //		SCL_HIGH();
                   //		delay_us(1);
                   //		SCL_LOW();
                   //		delay_us(1);
    }
    box_I2C_Wait_Ack();
}

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
uint8_t box_I2C_Read_Byte()
{
    unsigned char i, receive = 0;
    SDA_OUT();  //SDA设置为输出
    SDA_HIGH(); //SDA输出一个高电平
    SDA_IN();   //SDA设置为输入
    for (i = 0; i < 8; i++)
    {
        SCL_LOW();
        delay_us(2);
        SCL_HIGH();
        delay_us(2);
        //一个时钟周期，把receive左移一位，最低位
        receive <<= 1;
        if (GPIO_ReadInputPin(IIC_GPIO, IIC_SDAPIN))
            receive++;
        SCL_LOW();
        delay_us(2);
    }
    //    if (!ack)
    //        IIC_NAck();//发送nACK
    //    else
    //        IIC_Ack(); //发送ACK
    return receive;
}

//一般的写逻辑
void box_I2C_Write(uint8_t reg_addr, uint8_t data)
{
    box_I2C_Start();                  //开始信号
    box_I2C_Send_Byte(Slave_Address); //第一个八位数据发送的是从设备地址
    box_I2C_Send_Byte(reg_addr);      //第二个八位数据发送的是从设备内部地址，一般也就是这个数据要存放在从设备的那个地址
    box_I2C_Send_Byte(data);          //第三个八位数据发送的是从设备地址，开始写入数据
    box_I2C_Stop();                   //停止信号
}
//一般的读逻辑
uint8_t box_I2c_Read(uint8_t addr)
{
    uint8_t data;
    box_I2C_Start();
    box_I2C_Send_Byte(Slave_Address);
    box_I2C_Send_Byte(addr);
    //	IIC_Stop();
    box_I2C_Start();
    box_I2C_Send_Byte(Slave_Address + 1);
    data = box_I2C_Read_Byte();
    box_I2C_NAck();
    box_I2C_Stop();
    return data;
}
