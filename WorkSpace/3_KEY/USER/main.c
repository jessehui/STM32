#include "sys.h"
#include "delay.h"
#include "LED.h"
#include "KEY.h"
#include "BEEP.h"

int main(void)
{
	u8 key;
	Stm32_Clock_Init(336,8,2,7);
	delay_init(168);
	LED_Init();
	BEEP_Init();
	KEY_Init();
	LED0 = 0;	//点亮红灯
	
	while(1)
	{
		key = KEY_Scan(0);
//		key = KEY_Scan_Custom(0);
		if(key)//有按键按下
		{
			switch(key)
			{
				case WKUP_PRES: BEEP = !BEEP;break;
				case KEY2_PRES: LED0 = !LED0;break;
				case KEY1_PRES: LED1 = ~LED1;break;
				case KEY0_PRES: LED1 = !LED1; LED0 = !LED0;break;
			}
		}
		else
			delay_ms(10);
	}
	
}

