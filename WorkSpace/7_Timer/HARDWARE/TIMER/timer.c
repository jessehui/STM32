#include "timer.c"
#include "led.c"

//通用定时器3中断初始化
//arr: 自动重装初值
//psc: 时钟预分频数

void TIM3_Int_Init (u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	// enable TIMER 3 's clock

	TIM_TimeBaseInitStructure.TIM_Period = arr;	//自动重装
	TIM_TimeBaseInitStructure.TIM_Prescaler =  psc;	//预分频数
	Tim_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

/* Initializes the TIMx Time Base Unit peripheral according to 
           the specified parameters in the TIM_TimeBaseInitStruct.*/
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure); //initialize TIMER 3

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);	//允许定时器3更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	//定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;	//响应优先级为3
	NVIC_InitStructure.NVIC_IRQChannnelCmd = ENABLE;	

	NVIC_Init(&NVIC_InitStructure);		

	TIM_Cmd(TIM3, ENABLE);	//使能定时器3



}


//定时器3中断函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)	//Checks whether the TIM interrupt has occurred or not.
	{
		LED1 = !LED1;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);	//清除中断标志位
}















