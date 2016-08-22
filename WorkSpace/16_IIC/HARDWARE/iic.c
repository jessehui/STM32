#include "iic.h"

void IIC_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	//enable GPIOB

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	//普通输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	//上拉

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	IIC_SCL = 1;
	IIC_SDA = 1;
}

//I2C start signal
void IIC_Start (void)
{
	SDA_OUT();	//设置SDA线输出
	IIC_SDA = 1;  IIC_SCL = 1;	//先都拉高
	delay_us(4);
	IIC_SDA = 0; 	//SDA拉低, 此时开始传输
	delay_us(4);
	IIC_SCL = 0;	//	钳住I2C 准备发送或接受数据
	printf("start IIC \r\n");
}

//I2C stop signal
void IIC_Stop(void)
{
	SDA_OUT();	//SDA 输出
	IIC_SCL = 0; IIC_SDA = 0;	//先都拉低
	delay_us(4);
	IIC_SCL = 1; IIC_SDA = 1;	//SCL为高电平, SDA由低向高跳变 此时结束传输
	delay_us(4);	
	printf("End IIC\r\n");
}

//主机等待应答信号到来
//返回值1 : 接收应答失败; 0: 接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA_IN();
	IIC_SDA = 1;	delay_us(1);
	IIC_SCL = 1;	delay_us(1);

	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;	//读250次都没有成功就算是接收失败

		}
		
	}

	IIC_SCL = 0;
	return 0;
}


//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL = 0;	SDA_OUT();
	IIC_SDA = 0; 	delay_us(2);
	IIC_SCL = 1;	delay_us(2);
	IIC_SCL = 0;	//向发送数据的从设备发送低电平脉冲
}

//NACK应答
void IIC_Nack(void)
{
	IIC_SCL = 0; 	SDA_OUT();
	IIC_SDA = 1;	delay_us(2);
	IIC_SCL = 1;	delay_us(2);
	IIC_SCL = 0;	//向发送数据的从设备发送高电平脉冲

}

//IIC发送字节
void IIC_Send_Byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	IIC_SCL = 0;	//拉低时钟开始数据传输
	for(t=0;t<8;t++)
	{
		IIC_SDA = (txd&0x80) >> 7;	//取最低位
		txd <<= 1;	//txd = txd<<1
		delay_us(2);	
		IIC_SCL = 1;	delay_us(2);
	}
}

u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	SDA_IN();	
	for(i=0;i<8;i++)
	{
		IIC_SCL = 0;	delay_us(2);
		IIC_SCL = 1;
		receive <<= 1;		//receive先向左移1位
		if(READ_SDA) receive++;		//如果收到1, receive就+1, 最低位就和READ_SDA一样.
		//如果收到0, 就不进入if.
		delay_us(1);
	}

	if(!ack) IIC_Nack();
	else IIC_Ack();
	return receive;
}




