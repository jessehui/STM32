void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)//NVIC分组函数
{
	u32 temp1, temp2;//temp1负责处理读进来的数据,temp2负责寄存器原有数据的处理
	temp1 = (~NVIC_Group) & 0x07;//0000 0111 只要后三位
	temp1 <<= 8;	//左移8位 移到AIRCR的8~10位
	temp2 = SCB->AIRCR;	//读取AIRCR寄存器原有值
	temp2 &= 0x0000f8ff;			//8~10位清零
	temp2 |= 0x05FA0000;		//写入钥匙
	temp1 |= temp2;
	SCB->AIRCR = temp1;	//设置分组

}











