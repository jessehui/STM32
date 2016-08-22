#include "WDG.h"
#include "delay.h"
#include "LED.h"

int main(void)
{
	Stm32_Clock_Init(336,8,2,7);
	delay_init(168);
	LED_Init();
	LED0 = 0;	//点亮
	delay_ms(300);

	WWDG_Init(0x7f,0x5f,3);	//计数器初值为0x7f, 门限值0x5f, 分频数为2^3
	while(1)
	{
		LED0 = 1;
	}
}

