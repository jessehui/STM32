

//写寄存器函数
//regval: 寄存器值
void LCD_WR_REG(vu16 regval)
{
	regval = regval;	//-O2优化条件下, 必须插入的延时
	LCD->LCD_REG = regval;	//要写的寄存器序号
}

//写LCD数据
//data要写入的值
void LCD_WR_DATA(vu16 data)
{
	data = data;
	LCD->LCD_RAM = data;
}

//读LCD数据
u16 LCD_RD_DATA(void)
{
	vu16 ram;
	ram = LCD->LCD_RAM;
	return ram;
}


//写寄存器数据
void LCD_WriteReg(vu16 LCD_Reg, vu16 LCD_RegValue)
{
	LCD->LCD_REG = LCD_Reg;	//要写的寄存器
	LCD->LCD_RAM = LCD_RegValue;	//要写的值
}

//读寄存器数据
u16 LCD_ReadReg(vu16 LCD_Reg)
{
	LCD_WR_REG(LCD_Reg);	//写入要读的寄存器序号
	delay_us(5);
	return LCD_RD_DATA();	//返回读到的值
}

//准备开始写gRAM
void LCD_WriteRAM_Prepare(void)
{
	LCD->LCD_REG = lcddev.wramcmd;	//写命令
}

//写gRam
void LCD_WriteRAM(u16 RGB_Code)
{
	LCD->LCD_RAM = RGB_Code;	//写数据
}


//设置光标位置. 实现将 LCD 的当前操作点设置到指定坐标(x,y)
//Xpos: x位置
//Ypos: y位置
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
	if(lcddev.id == 0x9341 || lcddev.id == 0x5310)
	{
		LCD_WR_REG(lcddev.setxcmd);	//写命令: 设置x
		LCD_WR_DATA(Xpos>>8);	//x,y位置设置命令0x2A,0x2B都带4个参数(把1个start位置,1个end位置拆开),先发高8位
		LCD_WR_DATA(Xpos&0xFF);	//再发低8位

		LCD_WR_REG(lcddev.setycmd);	//设置y
		LCD_WR_DATA(Ypos>>8);
		LCD_WR_DATA(Ypos&0xFF);

	}
	else if(lcddev.id == 0x6804)
	{
		if(lcddev.dir == 1)	//如果横屏
			Xpos = lcddev.width - 1 - Xpos;	//屏幕向右旋转90度，原点座标还是设置在左上角，相对竖屏的座标：新的座标原点（0，0）相对于老的座标原点就是(0,320)
		//原来的x方向，刚好和新坐标的y方向一致，而且值也一致，所以y'=x。原来的y方向，和新坐标的x方向相反，所以新坐标的x'=320-y
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(Xpos>>8);
		LCD_WR_DATA(Xpos&0XFF); 
		LCD_WR_REG(lcddev.setycmd);
		LCD_WR_DATA(Ypos>>8);
		LCD_WR_DATA(Ypos&0XFF);

	}
	else
	{
		if(lcddev.dir==1)Xpos=lcddev.width-1-Xpos;//横屏其实就是调转 x,y 坐标 
		LCD_WriteReg(lcddev.setxcmd, Xpos);
		LCD_WriteReg(lcddev.setycmd, Ypos);
	}
}


//画点函数
//x,y 坐标
//POINT_COLOR: 颜色
void LCD_DrawPoint(u16 x, u16 y)
{
	LCD_SetCursor(x,y);
	LCD_WriteRAM_Prepare();
	LCD->LCD_RAM = POINT_COLOR;	//POINT_COLOR 是我们 定义的一个全局变量,用于存放画笔颜色
}

//读点函数
//x,y 坐标
//返回此点的颜色
u16 LCD_ReadPoint(u16 x, u16 y)
{
	vu16 r = 0, g = 0, b= 0;
	if( (x > lcddev.width) || (y > lcddev.height) ) return 0;	//超过范围直接返回
	LCD_SetCursor(x,y);

	if( lcddev.id == 0x9341 || lcddev.id == 0x6804 || lcddev.id == 0x5310) 
		LCD_WR_REG(0x2E);	//发送读Gram指令
	else if(lcddev.id==0X5510)
		LCD_WR_REG(0X2E00);//5510 发送读 GRAM 指令
	else LCD_WR_REG(R34);
	if(lcddev.id==0X9320)opt_delay(2);	//兼容性考虑

	LCD_RD_DATA();
	opt_delay(2);	//dummy read
	r = LCD_RD_DATA();

	if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X5510)
	{
		//这些要分两次读出
		opt_delay(2);
		b = LCD_RD_DATA();
		g = r&0xff;	//取低八位, 这些驱动器第一次读取的是RG的值, R在前, G在后. 各占8位.(R1G1->B1R2)
		g << = 8;	//g左移8位
	}
	if(lcddev.id==0X9325||lcddev.id==0X4535||lcddev.id==0X4531||lcddev.id==0XB505||lcddev.id==0XC505)
		return r;
	else if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X5510)
		return (((r>>11)<<11)|((g>>10)<<5)|(b>>11)); //ILI9341/NT35310/NT35510 需要公式转换一下 
	else return LCD_BGR2RGB(r); //其他 IC



}


//在指定位置显示一个字符
//num:要显示的字符
//SIZE: 字体大小
//mode: 叠加方式或者非叠加方式
void LCD_ShowChar(u16 x, u16 y, u8 num, u8 size, u8 mode)
{
	u8 temp, t1, t;
	u16 y0 = y;

	u8 csize =  (size/8) + ((size%8)?1:0)*(size/2);		//得到字体一个字符对应点阵集所占的字节数.
	num = num - ''; //得到偏移后的值
	for(t = 0;t< csize;t++)
	{
		if(size == 12) temp = asc2_1206[num][t];
		else if(size == 16) temp = asc2_1608[num][t];
		else if(size == 24) temp = asc2_2412[num][t];
		else return;

		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);

			temp<<=1;
			y++;
			if(y>=lcddev.height)return; 
			if((y-y0)==size)
			{
				//超区域了
				y=y0; 
				x++;
				if(x>=lcddev.width)return; //超区域了 break;
			}
		}
	}


}


//TFTLCD模块初始化函数
void LCD_Init(void)
{
	vu32 i =0;
	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef readWriteTiming;
	FSMC_NORSRAMTimingInitTypeDef writeTiming;

	//GPIO, FSMC 时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | 
		RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG , ENABLE);	//使能PB,PD,PE,PF,PG时钟
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);

	//GPIO 初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	//PB15推挽输出, 控制背光
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = (3<<0) | (3<<4) | (7<<8) | (3<<14);	//PD 0,1,4,5,8,9,10,14,15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	//复用输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
}





































