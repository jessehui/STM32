#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "adc.h"


int main(void)
{
	u16 adcx;	//用于显示整数部分
	float temp;	//用于显示小数部分
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//设置系统中断优先级分组2
	delay_init(168);	//初始化延时函数
	uart_init(115200);
	LED_Init();
	LCD_Init();
	Adc_Init();
	POINT_COLOR = RED;	//红字
	LCD_ShowString(30,50,200,16,16,"ADC TEST");

	POINT_COLOR = BLUE;	//蓝字
	LCD_ShowString(30,130,200,16,16,"ADC1_CH5_VAL:");
	LCD_ShowString(30,150,200,16,16,"ADC1_CH5_VOL: 0.000v");
	while(1)
	{
		adcx = Get_Adc_Average(ADC_Channel_5, 20);	//获取通道5的转换值, 20次取平均
		LCD_ShowxNum(134, 130, adcx, 4,16,0);	//显示采样后的原始值
		temp = (float)adcx*(3.3/4096);	//获取计算后的带小数的实际电压值
		adcx = temp;	//赋值整数部分给adcx, adcx本身为u16
		LCD_ShowxNum(134,150,adcs,1,16,0);

		temp -= adcx;	//获取小数部分
		temp *= 1000;	//小数部分*1000
		LCD_ShowxNum(150,150,temp,3,16,0x80); //显示小数部分

		LED0 = !LED0;
		delay_ms(250);	//延时250ms 等待中断
	}
}