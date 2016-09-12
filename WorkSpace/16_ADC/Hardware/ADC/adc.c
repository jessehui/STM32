#include "adc.h"
#include "delay.h"

void Adc_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	//先初始化ADC1通道5 IO口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;	//模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//不带上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化

	//复位ADC1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);	//ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);	//复位结束

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;	//独立模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;	//采样阶段之间的延迟5个时钟
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disable;	//不使用DMA 
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;	//预分频
	ADC_CommonInit(&ADC_CommonInitStructure);	//初始化

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//右对齐
	ADC_InitStructure.ADC_NbrofConversion = 1;	//1个转换在规则序列中
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_Cmd(ADC1, ENABLE);	//开启AD转换器


}

//获得ADC的值
//ch: ADC_channels(取值范围ADC_Channel_0 ~ 16)
//返回值: 转换结果
u16 Get_Adc(u8 ch)
{
	//设置指定ADC的规则组通道, 1个序列, 采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles);
	ADC_SoftwareStartConv(ADC1);
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));	//等待转换结束
	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果

}



//获取通道ch的转换值, 取times次, 然后平均
//ch: 通道编号
//times: 获取次数
//返回值: 通道ch的times次转换结果的平均值


u16 Get_Adc_Average(u8 ch, u8 times)
{
	u32 temp_val = 0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val += Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
}






