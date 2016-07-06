#include "LED.h"
#include "USART.h"
#include "WDG.h"
#include "KEY.h"
#include "BEEP.h"
#include "delay.h"



int TEMP(void)// main函数
{
	Stm32_Clock_Init(336,8,2,7);
	delay_init(168);
	uart_init(84,115200);
	KEY_Init();
	LED_Init();
	IWDG_Init(4,500);
	BEEP_Init();

	u8 i;
	for(i=0;i<10;i++)		//每次重启, BEEP会响
	{
		BEEP = 1;  
		delay_ms(50);
		BEEP = 0;
		delay_ms(50);
	}


	LED1 = 0;//正常工作指示灯

	while(1)
	{
		if( KEY_Scan(0) == WKUP_PRES )//如果WK_UP被按下, 则喂一次狗
		{
			IWDG_Feed();
			printf("Dog is fed.\n");
			printf("\r\n");
		}		
		delay_ms(10);
	}
}

