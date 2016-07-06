#include "KEY.h"

//共4个按键,3个是PE口(低电平有效,上拉), 1个是PA口(高电平有效,下拉)
void KEY_Init(void)
{
	RCC->AHB1ENR = 1<<4;
	RCC->AHB1ENR = 1<<0;
	GPIO_Set(GPIOE,PIN3|PIN4|PIN2,GPIO_MODE_IN,0,0,GPIO_PUPD_PU);//输入上拉
	GPIO_Set(GPIOE,PIN0,GPIO_MODE_IN,0,0,GPIO_PUPD_PD);//输入时MODE和SPEED不用设置
	
}

//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1， KEY0 按下
//2， KEY1 按下
//3， KEY2 按下
//4， KEY_UP 按下 即 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY_UP!!


u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)
		key_up=1;  //支持连按		  
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||WK_UP==1))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==0)return 1;
		else if(KEY1==0)return 2;
		else if(KEY2==0)return 3;
		else if(WK_UP==1)return 4;
	}
	else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0)
			key_up=1; 	    
 	return 0;// 无按键按下
}

//自定义按键扫描函数
//seccessional_press 0: 支持连按(可以一直出现一个现象)
//seccessional_press 1: 不支持连按(不松手就不往下进行)
//返回值: 返回mode值


u8 KEY_Scan_Custom(u8 successional_press)
{
	switch(successional_press)
	{
		case 1:
		if(KEY0==0||KEY1==0||KEY2==0||WK_UP==1)//如果任何一个按键被按下
		{
				delay_ms(30);//消抖
				if(KEY0==0) 
				{
					while(KEY0==0);//等待按键释放
					delay_ms(10);
					return 1;//工作模式1
				}
				else if(KEY1==0) 
				{
					while(KEY1==0);
					delay_ms(10);
					return 2;//工作模式1
				}
				else if(KEY2==0) 
				{
					while(KEY2==0);
					delay_ms(10);
					return 3;//工作模式1
				}
				else if(WK_UP==1) 
				{
					while(WK_UP==1);
					delay_ms(10);
					return 4;//工作模式1
				}
				return 0;
			}

		case 0://支持连按
		if(KEY0==0||KEY1==0||KEY2==0||WK_UP==1)
		{
			delay_ms(30);
			if(KEY0==0)
			{
				delay_ms(100); 
				return 1;	//等待很短的一瞬间 如果一直按下某个键 就会不断返回这个值
			}
			else if(KEY1==0)
			{
				delay_ms(100); 
				return 2;
			}
			else if(KEY2==0)
			{
				delay_ms(100); 
				return 3;
			}
			else if(WK_UP==1)
			{
				delay_ms(100); 
				return 4;
			}
			else return 0;

		}


	}



	
}



