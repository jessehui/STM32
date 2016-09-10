#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "wkup.h"


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//设置系统中断优先级分组2
	delay_init(168);	//初始化延时函数
	uart_init(115200);
	LED_Init();
	WKUP_Init();
	LCD_Init();
	POINT_COLOR = RED;
	LCD_ShowString(30,50,200,16,16,"WKUP TEST");
	while(1)
	{
		LED0 = !LED0;
		delay_ms(250);	//延时250ms 等待中断
	}
}