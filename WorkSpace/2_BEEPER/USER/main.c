#include "BEEP.h"
#include "LED.h"
#include "sys.h"
#include "delay.h"

int main()
{
	Stm32_Clock_Init(336,8,2,7);
	delay_init(168);
	LED_Init();
	BEEP_Init();
	
	while(1)
	{
		BEEP = 1;
		LED0 = 0;
		delay_ms(50);
		BEEP = 0;
		LED0 = 1;
		delay_ms(50);
	}
}


