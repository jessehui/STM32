#ifndef __IIC_H
#define __IIC_H

#include "sys.h"

//先清零后设置 养成好习惯. *2是因为对应设置有2位
#define SDA_IN()	{GPIOB->MODER &= ~(3 << (9*2)); GPIOB->MODER |= (0 << (9*2));}	
#define SDA_OUT()	{GPIOB->MODER &= ~(3 << (9*2)); GPIOB->MODER |= (1 << (9*2));}

//地址宏 方便使用的时候直接调用
#define IIC_SCL		PBout(8)		
#define IIC_SDA 	PBout(9)
#define READ_SDA 	PBin(9)

void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Byte(u8 txd);
u8 IIC_Read_Byte(unsigned char ack);
u8 IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_Nack(void);

#endif