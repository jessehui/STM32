#include "24cxx.h"
#include "delay.h"

void AT24CXX_Init(void)
{
	IIC_Init();
}

//从24cxx指定地址读出一个数据
//ReadAddr 开始读数的地址
//返回: 读到的数据
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{
	u8 temp = 0;
	u8 returnValue1, returnValue2, returnValue3;
	IIC_Start();

	if(EE_TYPE > AT24C16)
	{
		IIC_Send_Byte(0xA0);	//1010 0000. 1010是从器件地址, 最低位0表示对从设备写
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr >> 8);	// 发送高八位
	}

	else//EETYPE小于16, 地址位可能需要8位,9位, 10位等. 
		//多于8位就要用地址寄存器的位来补充. ReadAddr/256算出需要补充几位作为地址位. 
		//然后要左移1位, 因为最低位表示读或写
		IIC_Send_Byte(0xA0+((ReadAddr/256)<<1));

		returnValue1 = IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr%256);
		returnValue2 = IIC_Wait_Ack();
		IIC_Start();	
		IIC_Send_Byte(0xA1);	//主机要读数据
		returnValue3 = IIC_Wait_Ack();
		temp = IIC_Read_Byte(0);
		IIC_Stop();	//产生停止条件
		return temp;
}

void AT24CXX_WriteOneByte(u16 WriteAddr, u8 DataToWrite)
{
	IIC_Start();
	if(EE_TYPE > AT24C16)
	{
		IIC_Send_Byte(0xA0);	//发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr >> 8);	//发送高地址
	}

	else
		IIC_Send_Byte(0xA0+((WriteAddr/256)<<1);

		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr%256);	//发送低地址
		IIC_Wait_Ack();
		IIC_Send_Byte(DataToWrite);		//发送字节
		IIC_Wait_Ack();
		IIC_Stop();	
		delay_ms(10);
}

//指定地址开始写入长度为len的数据
void AT24CXX_WriteLenByte(u16 WriteAddr, u32 DataToWrite, u8 Len)
{
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t, (DataToWrite>>(8*t))&0xff);
	}
}



//指定地址读出长度为len的数据(len为2或4, 即数据为16bit或32bit)
u32 AT24CXX_ReadLenByte(u16 ReadAddr, u8 Len)
{
	u8 t;
	u32 temp = 0;
	for(t=0;t<Len;t++)
	{
		temp <<= 8;	//temp共32位
		temp += AT24CXX_ReadOneByte(ReadAddr+Len-t-1);
	}

	return temp;
}


//检查AT24CXX是否正常, 这里用24XX最后一个地址(255)来存储标志字
//1: 检测失败		0: 检测成功
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp = AT24CXX_ReadOneByte(255);	//避免每次开机都写
	if(temp == 0x55) 
		return 0;

	else	//排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(255,0x55);	//在地址255处写入0x55
		temp = AT24CXX_ReadOneByte(255);
		if(temp == 0x55)
			return 0;
	}

	return 1;

}


//从指定地址开始读出指定个数的数据
//readaddr: 开始读出的地址 对24c02为0~255
//pbuffer:	数据数组首地址
//numtoread: 要读出数据的个数
void AT24CXX_Read(u16 ReadAddr, u8 *pBuffer, u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++ = AT24CXX_ReadOneByte(ReadAddr++);
		NumToRead--;
	}
}



//指定地址开始写入指定个数的数据
void AT24CXX_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr++, *pBuffer++);
	}
}





















