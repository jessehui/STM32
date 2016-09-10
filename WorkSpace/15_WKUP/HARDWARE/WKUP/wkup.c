//////////////////////////////////////////////////////////////////////////////////	 
//需求
//通过长按(3 秒)KEY_UP 按键开机, 并且通过 DS0 的闪烁指示程序已经开始运行,再次长按该键,
//则进入待机模式,DS0 关闭,程 序停止运行。类似于手机的开关机。
//////////////////////////////////////////////////////////////////////////////////	 


#include "wkup.h"
#include "led.h"
#include "delay.h"
#include "usart.h"


//系统进入待机模式
void Sys_Enter_Standby(void)
{
	RCC_AHB1PeriphResetCmd(0x04FF,ENABLE);	//第一个参数是uint32_t RCC_AHB1Periph, 这里是要复位所有IO口
	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_PWR, ENABLE); //使能PWR时钟
	PWR_BackupAccessCmd(ENABLE);	//后备区域访问使能

	//关闭相关RTC中断
	RTC_ITConfig(RTC_IT_TS|RTC_IT_WUT|RTC_IT_ALRB|RTC_IT_ALRA,DISABLE);//关闭RTC相关中断，可能在RTC实验打开了。
	RTC_ClearITPendingBit(RTC_IT_TS|RTC_IT_WUT|RTC_IT_ALRB|RTC_IT_ALRA);//清楚RTC相关中断标志位。
	
	PWR_ClearFlag(PWR_FLAG_WU);	//清除Wake-up标志
	PWR_WakeUpPinCmd(ENABLE);	//设置WKUP引脚用于唤醒
	PWR_EnterSTANDBYMode();		//进入待机模式
}

//检测WKUP引脚的信号
//返回值1: 连续按下3s以上
//     0: 错误的触发
u8 Check_WKUP(void)
{
	u8 t = 0;
	u8 tx = 0; 	//记录松开的次数
	LED0 = 0;	//点亮DS0
	while(1)
	{
		if(WKUP_KD)
		{
			t++;
			tx = 0;
		}
		else
		{
			tx++;
			if(tx>3)	//超过90ms没有wakeup信号
			{
				LED0 = 1;
				return 0;	//直接返回	
			}
		}
		delay_ms(30);
		if(t>=100)	//按下超过3s
		{
			LED0 = 0;	//点亮DS0
			return 1;
		}
	}

}



void WKUP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);	//使能SYSCFG时钟(控制中断必须使能SYSCFG时钟)

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	//PA0 ,先定index
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	//输入
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;	//输入 ,无所谓
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//输入, 输出最高频率无所谓
	GPIO_InitStructure.GPIO_PuPd = GPIO_PdPd_Down;	//下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化PA0端口

	//检查是否正常开机
	if(Check_WKUP() == 0)	//没有按下
	{
		Sys_Enter_Standby();	//没有按下, 直接待机模式
	}
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);	//中断线0连接PA端口(PA0)

	EXTI_InitStructure.EXTI_Line = EXTI_Line0;	//line0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//上升沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;	//使能
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;	//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级2
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);//配置NVIC







}



//中断服务程序, 检测到PA0的一个上升沿. 外部中断0 可以映射到任意的0号引脚上(PB, PC, PD...)
void EXTI0_IRQHandler(void)	
{
	EXTI_ClearITPendingBit(EXTI_Line0);//清除中断标志
	if(Check_WKUP())//收到信号
	{
		Sys_Enter_Standby();
	}
}




















