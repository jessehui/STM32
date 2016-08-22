#include "sys.h"
#include "delay.h"

#include "led.h"
#include "timer.h"


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组为2 2位抢占优先级
	delay_init(168);
	LED_Init();		//初始化LED端口
	TIM3_Int_Init(5000-1, 8400-1);	//定时器时钟84M, 分频系数8400, 所以84M/8400 = 10KHz,计数5000次为500ms

	while(1)
	{
		LED0 = !LED0;
		delay_ms(200);	//LED0每隔200ms翻转一次, 同时LED1在定时器3每溢出一次就翻转一次
	}
}	
