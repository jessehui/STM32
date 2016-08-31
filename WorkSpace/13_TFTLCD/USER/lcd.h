

//LCD重要参数集
typedef struct{
	u16 width;
	u16 height;
	u16 id;
	u8 dir;	//方向: 0竖屏 1横屏
	u16 wramcmd;	//写ram命令
	u16 setxcmd;	//设置x指令
	u16 setycmd; 	
}_lcd_dev;
extern _lcd_dev lcddev;	//为什么要加extern









typedef struct {
	vu16 LCD_REG;
	vu16 LCD_RAM;	//vu16 即 volatile uint16
}LCD_Typedef;

//使用NOR/SRAM的BANK1. sect4 地址位HADDR[27,26]=11; 
//A6为数据/命令区分线(本来A0~A18为SRAM的地址线, FSMC上有对应的控制线)	

//注意16位数据总线时, STM32内部地址会右移一位对齐.
#define LCD_BASE ( (u32)(0x6c000000|0x7e ) )
//0x6c00 0000是Bank1.sect4对应的地址. 0x7e是为了A6来控制命令/数据区分线

#define LCD ((LCD_Typedef *)LCD_BASE)
//可以得到 LCD->LCD_REG 的地址就是 0X6C00,007E,对应 A6 的状态为 0(即 RS=0)
//而 LCD-> LCD_RAM 的地址就是 0X6C00,0080(结构体地址自增),对应 A6 的状态为 1










