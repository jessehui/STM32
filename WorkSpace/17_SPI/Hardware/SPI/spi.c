#include "spi.h"

void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	//使能GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	//GPIOB 3,4,5 初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;	//PB 3~5复用功能
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PdPd_UP;	//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	//GPIOB3~5 复用为 SPI1
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);

	//设置SPI单向或双向的数据模式: 双向双线全双工
	SPI_InitStructure.SPI_Direction = SPI_Direction_2lines_FullDuplex;
	//设置SPI工作模式: 设置为主SPI
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	//设置SPI的数据大小: SPI发送接收8位帧结构
	SPI_InitStucture.SPI_DataSize = SPI_DataSize_8b;
	//设置串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	//串行同步时钟的第二个跳变沿(上升或下降)数据被采样
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	//NSS信号由硬件(NSS管脚)还是软件(SSI位)管理: 内部NSS信号由SSI位控制
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	//定义波特率预分频值为256. 传输速度为84M/256 = 328.125KHz
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	//指定数据传输从MSB位还是LSB位开始: MSB
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	//CRC 计算的多项式, 提高通信可靠性, 大于1即可
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_Cmd(SPI1, ENABLE);

	SPI1_ReadWriteByte(0xff);	//启动传输






}





//SPI1速度设置函数
//SPI速度 = APB2/分频系数

void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1 &= 0xFFC7;	//位3-5 清零, 用于设置波特率
	SPI1->CR1 |= SPI_BaudRatePrescaler;	//设置SPI1速度
	SPI_Cmd(SPI1,ENABLE);


}


//SPI1 读写一个字节
//TxData: 要读入的字节
//返回: 读取的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_Flag_TXE) == RESET)
		{}//等待发送区为空
}



















