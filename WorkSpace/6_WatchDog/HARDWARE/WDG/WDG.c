#include "WDG.h"
//prer:分频数:0~7(只有低 3 位有效!)
//rlr:自动重装载值,0~0XFFF.
//分频因子=4*2^prer.但最大值只能是 256!
//rlr:重装载寄存器值:低 11 位有效.
//时间计算(大概):Tout=((4*2^prer)*rlr)/32 (ms)

void IWDG_Init(u8 prer, u16 rlr)//常用prer = 4, rlr = 500, 此时溢出时间为1s
{
	//注意: 直接写入键值, 不需要用与或
	IWDG->KR = 0x5555;		//使能对IWDG->PR和IWDG->RLR的写
	IWDG->PR = prer;
	IWDG->RLR = rlr;
	IWDG->KR = 0xAAAA;		//装载数值
	IWDG->KR = 0xCCCC;		//使能看门狗
}

//每隔一段时间 喂狗
void IWDG_Feed(void)
{
	IWDG->KR = 0xAAAA;	//装载数值
}



















