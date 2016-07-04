#include "BEEP.h"

void BEEP_Init(void)
{
	RCC->AHB1ENR |= 1<<5;
	//Port F, PIN8, 推挽输出, 拉低(未导通)
	GPIO_Set(GPIOF,PIN8,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PD);
	BEEP = 0;//初始关闭蜂鸣器
}

