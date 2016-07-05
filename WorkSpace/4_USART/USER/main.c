#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "LED.h"

int main(void)
{
	u8 t;
	u8 len;
	u16 times = 0;
	
	Stm32_Clock_Init(336,8,2,7);
	delay_init(168);
	uart_init(84,115200);
	LED_Init();
	
	while(1)
	{
		if(USART_RX_STA & 0x8000)//有信息输入
		{
			len = USART_RX_STA & 0x3fff;
			printf("\r\n The message you are sending is : \r\n");
			for(t=0;t<len;t++)
			{
				USART1->DR = USART_RX_BUF[t];
				while((USART1->SR&0x40) == 0);
				
			}
			printf("\r\n\r\n");
			USART_RX_STA = 0;
			
			
		}else
		{
			times++;
			if(times%5000 == 0)
			{
				printf("\r\n\n Serial Communication Experiment\n");
				printf("START...\n");
			}
			if(times%200 == 0)
				printf("Please Input the Data....\n");
			if(times%30 == 0) LED0 = !LED0;
			
			delay_ms(10);
		}
	}
}