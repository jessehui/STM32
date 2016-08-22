#include "EXTI.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "beep.h"

void EXTI0_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(WK_UP == 1)
		BEEP = !BEEP;
	EXTI->PR = 1<<0;	//清除响应过的中断标志
}

void EXTI1_IRQHandler(void)
{
	delay_ms(10);
	if(KEY2 == 0)
		LED0 = !LED0;
	EXTI->PR = 1<<2;//KEY2对应PE2
}

void EXTI2_IRQHandler(void)
{
	delay_ms(10);
	if(KEY1 == 0)
		LED1 = !LED1;
	EXTI->PR = 1<<3;//KEY3对应PE3
}

void EXTI3_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY0 == 0)
	{
		LED0 = !LED0;
		LED1 = !LED1;
	}
	EXTI->PR = 1<<4;
}


void EXTIX_Init(void)
{
	KEY_Init();
	
	//配置键盘中断触发方式
	Ex_NVIC_Config(GPIO_E,2,FTIR);
	Ex_NVIC_Config(GPIO_E,3,FTIR);
	Ex_NVIC_Config(GPIO_E,4,FTIR);//上拉,默认高电平,下降沿触发
	Ex_NVIC_Config(GPIO_A,0,RTIR);//下拉,默认低电平,上升沿触发
	
	//设置中断分组
	MY_NVIC_Init(3,2,EXTI2_IRQn,2);//抢占优先级为3,子优先级为2,中断分组为第2组[2位抢占优先级(有4个抢占优先级),2位响应优先级(有4个)]
	MY_NVIC_Init(2,2,EXTI3_IRQn,2);
	MY_NVIC_Init(1,2,EXTI4_IRQn,2);
	MY_NVIC_Init(0,2,EXTI0_IRQn,2);	
	
}



