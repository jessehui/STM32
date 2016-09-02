

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











