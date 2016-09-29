#include "w25qxx.h"
#include "spi.h"
#include "delay.h"
#include "usart.h"

u16 W25QXX_TYPE = W25Q128;	//默认型号是W25Q128

//4Kbytes为一个Sector, 16个扇区为1个Block
//W25Q128
//容量为16M字节,共有128个Block,4096个Sector 

//初始化SPI FLASH的IO口
void W25QXX_Init(void)
{
	//使能GPIOB, G
	//W25QXX使能由PB14控制, 但是由于W25QXX和NRF共用SPI1, 
	//所以为了避免冲突, 要把两个模块都设置好
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	//片选低电平有效
	GPIO_SetBits(GPIOG, GPIO_Pin_7);	//PG7输出1, 防止NRF干扰SPI FLASH通信
	W25QXX_CS = 1;		//不选中
	SPI1_Init();
	SPI1_SetSpeed(SPI_BaudRatePrescaler_2);
	W25QXX_TYPE = W25QXX_ReadID();	//读取FLASH ID
}

//读取W25QXX的SR状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
u8 W25QXX_ReadSR(void)
{
	u8 byte = 0;
	W25QXX_CS = 0;	//打开片选
	SPI1_ReadWriteByte(W25X_ReadStatusReg);
	byte = SPI1_ReadWriteByte(0xFF);	//读取一个字节
	W25QXX_CS = 1;	//关闭片选
	return byte;
}

//写W25QXX状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写
void W25Qxx_Write_SR(u8 sr)
{
	W25QXX_CS = 0;	
	SPI1_ReadWriteByte(W25X_WriteStatusReg);
	SPI1_ReadWriteByte(sr);		//写入字节
	W25QXX_CS = 1;
}

//写使能
//将WEL置位
void W25QXX_Write_Enable(void)
{
	W25QXX_CS = 0;
	SPI1_ReadWriteByte(W25X_WriteEnable);
	W25QXX_CS = 1;
}

//写禁止
void W25QXX_Write_Disable(void)   
{  
	W25QXX_CS=0;                            //使能器件   
    SPI1_ReadWriteByte(W25X_WriteDisable);     //发送写禁止指令    
	W25QXX_CS=1;                            //取消片选     	      
} 


//读取芯片ID
u16 W25QXX_ReadID(void)
{
	u16 Temp = 0;
	W25QXX_CS = 0;	//打开片选
	SPI1_ReadWriteByte(W25X_ManufactDeviceID);
	SPI1_ReadWriteByte(0x00);	//这里三个0x00的原因是:
	SPI1_ReadWriteByte(0x00);	//根据W25Q128数据手册, 在读取指令为0x90时, 会有24bit处理时间
	SPI1_ReadWriteByte(0x00);	//就是会返回3次.之后才会读出需要的数据
	Temp |= SPI1_ReadWriteByte(0xFF) << 8;
	Temp |= SPI1_ReadWriteByte(0xFF);
	W25QXX_CS = 1;
	return Temp;
}



//读取 SPI FLASH
//在指定地址开始读取指定长度的数据
//pBuffer: 数据存储区
//ReadAddr: 开始读取的地址(24bit)
//NumByteToRead: 要读取的字节数(最大65535)
void W25QXX_Read(u8 *pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
	u16 i;
	W25QXX_CS = 0;
	SPI1_ReadWriteByte(W25X_ReadData);		//发送读取指令
	SPI1_ReadWriteByte((u8)((ReadAddr)>>16));	//发送24位地址
	SPI1_ReadWriteByte((u8)((ReadAddr)>>8));
	SPI1_ReadWriteByte((u8)((ReadAddr)>>0));
	for(i=0;i<NumByteToRead;i++)
	{
		pBuffer[i] = SPI1_ReadWriteByte(0xFF);	//循环读数
	}

	W25QXX_CS = 1;	//关闭片选
}



//SPI在一页(0~65535)内写入少于256个字节的数据
//从指定地址开始写入最大256字节的数据
//pbuffer: 数据存储区
//WriteAddr: 开始写入的地址(24Bit)
//NumByteToWrite: 要写入的字节数(最大256), 该数不应该超过该页的剩余字节数
void W25QXX_Write_Page(u8 * pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u16 i;
	W25QXX_Write_Enable();
	W25QXX_CS = 1;
	SPI1_ReadWriteByte(W25X_PageProgram);
	SPI1_ReadWriteByte((u8)(WriteAddr)>>16);	//发送地址
	SPI1_ReadWriteByte((u8)(WriteAddr)>>8);
	SPI1_ReadWriteByte((u8)WriteAddr);
	for(i=0;i<NumByteToWrite;i++)
		SPI1_ReadWriteByte(pBuffer[i]);
	W25QXX_CS = 1;
	W25QXX_Wait_Busy();
}

//无检验写SPI FLASH
//必须确保所写的地址范围内的数据全部为0XFF, 否则在非0XFF处写入的数据将失败
//具有自动换页功能
//在指定地址开始写入指定长度的数据, 但是要确保地址不越界
//pBuffer: 数据存储区
//WriteAddr: 开始写入的地址(24bit)
//NumByteToWrite: 要写入的字节数
//Check Ok
void W25QXX_Write_NoCheck(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u16 pageRemain;
	pageRemain = 256 - WriteAddr%256;	//单页剩余的字节数
	if(NumByteToWrite <= pageRemain)
		pageRemain = NumByteToWrite;
	while(1)
	{
		W25QXX_Write_Page(pBuffer, WriteAddr,pageRemain);
		if(NumByteToWrite == pageRemain)
			break;
		else	//NumByteToWrite > pageRemain
		{
			pBuffer += pageRemain;
			WriteAddr += pageRemain;

			NumByteToWrite -= pageRemain;
			if(NumByteToWrite>256)
				pageRemain = 256;	//一次可以写入256个字节
			else	pageRemain = NumByteToWrite;	//不够256个字节了
		}
	}
}










