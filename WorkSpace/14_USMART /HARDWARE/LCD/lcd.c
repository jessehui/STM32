#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "usart.h"	 
#include "delay.h"	 
//////////////////////////////////////////////////////////////////////////////////	 
//±æ≥Ã–Ú÷ªπ©—ßœ∞ π”√£¨Œ¥æ≠◊˜’ﬂ–Ìø…£¨≤ªµ√”√”⁄∆‰À¸»Œ∫Œ”√Õæ
//ALIENTEK STM32F407ø™∑¢∞Â
//2.4¥Á/2.8¥Á/3.5¥Á/4.3¥Á/7¥Á TFT“∫æß«˝∂Ø	  
//÷ß≥÷«˝∂ØIC–Õ∫≈∞¸¿®:ILI9341/ILI9325/RM68042/RM68021/ILI9320/ILI9328/LGDP4531/LGDP4535/
//                  SPFD5408/1505/B505/C505/NT35310/NT35510/SSD1963µ»		    
//’˝µ„‘≠◊”@ALIENTEK
//ºº ı¬€Ã≥:www.openedv.com
//¥¥Ω®»’∆⁄:2010/7/4
//∞Ê±æ£∫V3.0
//∞Ê»®À˘”–£¨µ¡∞Ê±ÿæø°£
//Copyright(C) π„÷› ––«“ÌµÁ◊”ø∆ºº”–œﬁπ´Àæ 2014-2024
//All rights reserved	
//********************************************************************************
//V1.2–ﬁ∏ƒÀµ√˜
//÷ß≥÷¡ÀSPFD5408µƒ«˝∂Ø,¡ÌÕ‚∞—“∫æßID÷±Ω”¥Ú”°≥…HEX∏Ò Ω.∑Ω±„≤Èø¥LCD«˝∂ØIC.
//V1.3
//º”»Î¡ÀøÏÀŸIOµƒ÷ß≥÷
//–ﬁ∏ƒ¡À±≥π‚øÿ÷∆µƒº´–‘£®  ”√”⁄V1.8º∞“‘∫Ûµƒø™∑¢∞Â∞Ê±æ£©
//∂‘”⁄1.8∞Ê±æ÷Æ«∞(≤ª∞¸¿®1.8)µƒ“∫æßƒ£øÈ,«Î–ﬁ∏ƒLCD_Init∫Ø ˝µƒLCD_LED=1;Œ™LCD_LED=1;
//V1.4
//–ﬁ∏ƒ¡ÀLCD_ShowChar∫Ø ˝£¨ π”√ª≠µ„π¶ƒ‹ª≠◊÷∑˚°£
//º”»Î¡À∫· ˙∆¡œ‘ æµƒ÷ß≥÷
//V1.5 20110730
//1,–ﬁ∏ƒ¡ÀB505“∫æß∂¡—’…´”–ŒÛµƒbug.
//2,–ﬁ∏ƒ¡ÀøÏÀŸIOº∞∫· ˙∆¡µƒ…Ë÷√∑Ω Ω.
//V1.6 20111116
//1,º”»Î∂‘LGDP4535“∫æßµƒ«˝∂Ø÷ß≥÷
//V1.7 20120713
//1,‘ˆº”LCD_RD_DATA∫Ø ˝
//2,‘ˆº”∂‘ILI9341µƒ÷ß≥÷
//3,‘ˆº”ILI9325µƒ∂¿¡¢«˝∂Ø¥˙¬Î
//4,‘ˆº”LCD_Scan_Dir∫Ø ˝(…˜÷ÿ π”√)	  
//6,¡ÌÕ‚–ﬁ∏ƒ¡À≤ø∑÷‘≠¿¥µƒ∫Ø ˝,“‘  ”¶9341µƒ≤Ÿ◊˜
//V1.8 20120905
//1,º”»ÎLCD÷ÿ“™≤Œ ˝…Ë÷√Ω·ππÃÂlcddev
//2,º”»ÎLCD_Display_Dir∫Ø ˝,÷ß≥÷‘⁄œﬂ∫· ˙∆¡«–ªª
//V1.9 20120911
//1,–¬‘ˆRM68042«˝∂Ø£®ID:6804£©£¨µ´ «6804≤ª÷ß≥÷∫·∆¡œ‘ æ£°£°‘≠“Ú£∫∏ƒ±‰…®√Ë∑Ω Ω£¨
//µº÷¬6804◊¯±Í…Ë÷√ ß–ß£¨ ‘π˝∫‹∂‡∑Ω∑®∂º≤ª––£¨‘› ±ŒﬁΩ‚°£
//V2.0 20120924
//‘⁄≤ª”≤º˛∏¥Œªµƒ«Èøˆœ¬,ILI9341µƒID∂¡»°ª·±ªŒÛ∂¡≥…9300,–ﬁ∏ƒLCD_Init,Ω´Œﬁ∑® ∂±
//µƒ«Èøˆ£®∂¡µΩIDŒ™9300/∑«∑®ID£©,«ø÷∆÷∏∂®«˝∂ØICŒ™ILI9341£¨÷¥––9341µƒ≥ı ºªØ°£
//V2.1 20120930
//–ﬁ’˝ILI9325∂¡—’…´µƒbug°£
//V2.2 20121007
//–ﬁ’˝LCD_Scan_Dirµƒbug°£
//V2.3 20130120
//–¬‘ˆ6804÷ß≥÷∫·∆¡œ‘ æ
//V2.4 20131120
//1,–¬‘ˆNT35310£®ID:5310£©«˝∂Ø∆˜µƒ÷ß≥÷
//2,–¬‘ˆLCD_Set_Window∫Ø ˝,”√”⁄…Ë÷√¥∞ø⁄,∂‘øÏÀŸÃÓ≥‰,±»Ωœ”–”√,µ´ «∏√∫Ø ˝‘⁄∫·∆¡ ±,≤ª÷ß≥÷6804.
//V2.5 20140211
//1,–¬‘ˆNT35510£®ID:5510£©«˝∂Ø∆˜µƒ÷ß≥÷
//V2.6 20140504
//1,–¬‘ˆASCII 24*24◊÷ÃÂµƒ÷ß≥÷(∏¸∂‡◊÷ÃÂ”√ªßø…“‘◊‘––ÃÌº”)  
//2,–ﬁ∏ƒ≤ø∑÷∫Ø ˝≤Œ ˝,“‘÷ß≥÷MDK -O2”≈ªØ
//3,’Î∂‘9341/35310/35510,–¥ ±º‰…Ë÷√Œ™◊ÓøÏ,æ°ø…ƒ‹µƒÃ·∏ﬂÀŸ∂»
//4,»•µÙ¡ÀSSD1289µƒ÷ß≥÷,“ÚŒ™1289 µ‘⁄ «Ã´¬˝¡À,∂¡÷‹∆⁄“™1us...ºÚ÷±∆Ê›‚.≤ª  ∫œF4 π”√
//5,–ﬁ’˝68042º∞C505µ»ICµƒ∂¡—’…´∫Ø ˝µƒbug.
//V2.7 20140710
//1,–ﬁ’˝LCD_Color_Fill∫Ø ˝µƒ“ª∏ˆbug. 
//2,–ﬁ’˝LCD_Scan_Dir∫Ø ˝µƒ“ª∏ˆbug.
//V2.8 20140721
//1,Ω‚æˆMDK π”√-O2”≈ªØ ±LCD_ReadPoint∫Ø ˝∂¡µ„ ß–ßµƒŒ Ã‚.
//2,–ﬁ’˝LCD_Scan_Dir∫·∆¡ ±…Ë÷√µƒ…®√Ë∑Ω Ωœ‘ æ≤ª»´µƒbug.
//V2.9 20141130
//1,–¬‘ˆ∂‘SSD1963 LCDµƒ÷ß≥÷.
//2,–¬‘ˆLCD_SSD_BackLightSet∫Ø ˝
//3,»°œ˚ILI93XXµƒRxxºƒ¥Ê∆˜∂®“Â
//V3.0 20150423
//–ﬁ∏ƒSSD1963 LCD∆¡µƒ«˝∂Ø≤Œ ˝.
//////////////////////////////////////////////////////////////////////////////////	 

//LCDµƒª≠± —’…´∫Õ±≥æ∞…´	   
u16 POINT_COLOR=0x0000;	//ª≠± —’…´
u16 BACK_COLOR=0xFFFF;  //±≥æ∞…´ 
  
//π‹¿ÌLCD÷ÿ“™≤Œ ˝
//ƒ¨»œŒ™ ˙∆¡
_lcd_dev lcddev;
	 
//–¥ºƒ¥Ê∆˜∫Ø ˝
//regval:ºƒ¥Ê∆˜÷µ
void LCD_WR_REG(vu16 regval)
{   
	regval=regval;		// π”√-O2”≈ªØµƒ ±∫Ú,±ÿ–Î≤Â»Îµƒ—” ±
	LCD->LCD_REG=regval;//–¥»Î“™–¥µƒºƒ¥Ê∆˜–Ú∫≈	 
}
//–¥LCD ˝æ›
//data:“™–¥»Îµƒ÷µ
void LCD_WR_DATA(vu16 data)
{	  
	data=data;			// π”√-O2”≈ªØµƒ ±∫Ú,±ÿ–Î≤Â»Îµƒ—” ±
	LCD->LCD_RAM=data;		 
}
//∂¡LCD ˝æ›
//∑µªÿ÷µ:∂¡µΩµƒ÷µ
u16 LCD_RD_DATA(void)
{
	vu16 ram;			//∑¿÷π±ª”≈ªØ
	ram=LCD->LCD_RAM;	
	return ram;	 
}					   
//–¥ºƒ¥Ê∆˜
//LCD_Reg:ºƒ¥Ê∆˜µÿ÷∑
//LCD_RegValue:“™–¥»Îµƒ ˝æ›
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;		//–¥»Î“™–¥µƒºƒ¥Ê∆˜–Ú∫≈	 
	LCD->LCD_RAM = LCD_RegValue;//–¥»Î ˝æ›	    		 
}	   
//∂¡ºƒ¥Ê∆˜
//LCD_Reg:ºƒ¥Ê∆˜µÿ÷∑
//∑µªÿ÷µ:∂¡µΩµƒ ˝æ›
u16 LCD_ReadReg(u16 LCD_Reg)
{										   
	LCD_WR_REG(LCD_Reg);		//–¥»Î“™∂¡µƒºƒ¥Ê∆˜–Ú∫≈
	delay_us(5);		  
	return LCD_RD_DATA();		//∑µªÿ∂¡µΩµƒ÷µ
}   
//ø™ º–¥GRAM
void LCD_WriteRAM_Prepare(void)
{
 	LCD->LCD_REG=lcddev.wramcmd;	  
}	 
//LCD–¥GRAM
//RGB_Code:—’…´÷µ
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD->LCD_RAM = RGB_Code;//–¥ Æ¡˘ŒªGRAM
}
//¥”ILI93xx∂¡≥ˆµƒ ˝æ›Œ™GBR∏Ò Ω£¨∂¯Œ“√«–¥»Îµƒ ±∫ÚŒ™RGB∏Ò Ω°£
//Õ®π˝∏√∫Ø ˝◊™ªª
//c:GBR∏Ò Ωµƒ—’…´÷µ
//∑µªÿ÷µ£∫RGB∏Ò Ωµƒ—’…´÷µ
u16 LCD_BGR2RGB(u16 c)
{
	u16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
} 
//µ±mdk -O1 ±º‰”≈ªØ ±–Ë“™…Ë÷√
//—” ±i
void opt_delay(u8 i)
{
	while(i--);
}
//∂¡»°∏ˆƒ≥µ„µƒ—’…´÷µ	 
//x,y:◊¯±Í
//∑µªÿ÷µ:¥Àµ„µƒ—’…´
u16 LCD_ReadPoint(u16 x,u16 y)
{
 	u16 r=0,g=0,b=0;
	if(x>=lcddev.width||y>=lcddev.height)return 0;	//≥¨π˝¡À∑∂Œß,÷±Ω”∑µªÿ		   
	LCD_SetCursor(x,y);	    
	if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X1963)LCD_WR_REG(0X2E);//9341/6804/3510/1963 ∑¢ÀÕ∂¡GRAM÷∏¡Ó
	else if(lcddev.id==0X5510)LCD_WR_REG(0X2E00);	//5510 ∑¢ÀÕ∂¡GRAM÷∏¡Ó
	else LCD_WR_REG(0X22);      		 			//∆‰À˚IC∑¢ÀÕ∂¡GRAM÷∏¡Ó
	if(lcddev.id==0X9320)opt_delay(2);				//FOR 9320,—” ±2us	    
 	r=LCD_RD_DATA();								//dummy Read	   
	if(lcddev.id==0X1963)return r;					//1963÷±Ω”∂¡æÕø…“‘ 
	opt_delay(2);	  
 	r=LCD_RD_DATA();  		  						// µº ◊¯±Í—’…´
 	if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X5510)		//9341/NT35310/NT35510“™∑÷2¥Œ∂¡≥ˆ
 	{
		opt_delay(2);	  
		b=LCD_RD_DATA(); 
		g=r&0XFF;		//∂‘”⁄9341/5310/5510,µ⁄“ª¥Œ∂¡»°µƒ «RGµƒ÷µ,R‘⁄«∞,G‘⁄∫Û,∏˜’º8Œª
		g<<=8;
	} 
	if(lcddev.id==0X9325||lcddev.id==0X4535||lcddev.id==0X4531||lcddev.id==0XB505||lcddev.id==0XC505)return r;	//’‚º∏÷÷IC÷±Ω”∑µªÿ—’…´÷µ
	else if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X5510)return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));//ILI9341/NT35310/NT35510–Ë“™π´ Ω◊™ªª“ªœ¬
	else return LCD_BGR2RGB(r);						//∆‰À˚IC
}			 
//LCDø™∆Ùœ‘ æ
void LCD_DisplayOn(void)
{					   
	if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X1963)LCD_WR_REG(0X29);	//ø™∆Ùœ‘ æ
	else if(lcddev.id==0X5510)LCD_WR_REG(0X2900);	//ø™∆Ùœ‘ æ
	else LCD_WriteReg(0X07,0x0173); 				 	//ø™∆Ùœ‘ æ
}	 
//LCDπÿ±’œ‘ æ
void LCD_DisplayOff(void)
{	   
	if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X1963)LCD_WR_REG(0X28);	//πÿ±’œ‘ æ
	else if(lcddev.id==0X5510)LCD_WR_REG(0X2800);	//πÿ±’œ‘ æ
	else LCD_WriteReg(0X07,0x0);//πÿ±’œ‘ æ 
}   
//…Ë÷√π‚±ÍŒª÷√
//Xpos:∫·◊¯±Í
//Ypos:◊›◊¯±Í
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	 
 	if(lcddev.id==0X9341||lcddev.id==0X5310)
	{		    
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF); 			 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(Ypos>>8);LCD_WR_DATA(Ypos&0XFF); 		
	}else if(lcddev.id==0X6804)
	{
		if(lcddev.dir==1)Xpos=lcddev.width-1-Xpos;//∫·∆¡ ±¥¶¿Ì
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF); 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(Ypos>>8);LCD_WR_DATA(Ypos&0XFF); 
	}else if(lcddev.id==0X1963)
	{  			 		
		if(lcddev.dir==0)//x◊¯±Í–Ë“™±‰ªª
		{
			Xpos=lcddev.width-1-Xpos;
			LCD_WR_REG(lcddev.setxcmd); 
			LCD_WR_DATA(0);LCD_WR_DATA(0); 		
			LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF);		 	 
		}else
		{
			LCD_WR_REG(lcddev.setxcmd); 
			LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF); 		
			LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);		 	 			
		}	
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(Ypos>>8);LCD_WR_DATA(Ypos&0XFF); 		
		LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF); 			 		
		
	}else if(lcddev.id==0X5510)
	{
		LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(Xpos>>8); 		
		LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(Xpos&0XFF);			 
		LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(Ypos>>8);  		
		LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(Ypos&0XFF);			
	}else
	{
		if(lcddev.dir==1)Xpos=lcddev.width-1-Xpos;//∫·∆¡∆‰ µæÕ «µ˜◊™x,y◊¯±Í
		LCD_WriteReg(lcddev.setxcmd, Xpos);
		LCD_WriteReg(lcddev.setycmd, Ypos);
	}	 
} 		 
//…Ë÷√LCDµƒ◊‘∂Ø…®√Ë∑ΩœÚ
//◊¢“‚:∆‰À˚∫Ø ˝ø…ƒ‹ª· ‹µΩ¥À∫Ø ˝…Ë÷√µƒ”∞œÏ(”»∆‰ «9341/6804’‚¡Ω∏ˆ∆Ê›‚),
//À˘“‘,“ª∞„…Ë÷√Œ™L2R_U2Dº¥ø…,»Áπ˚…Ë÷√Œ™∆‰À˚…®√Ë∑Ω Ω,ø…ƒ‹µº÷¬œ‘ æ≤ª’˝≥£.
//dir:0~7,¥˙±Ì8∏ˆ∑ΩœÚ(æﬂÃÂ∂®“Âº˚lcd.h)
//9320/9325/9328/4531/4535/1505/b505/5408/9341/5310/5510/1963µ»IC“—æ≠ µº ≤‚ ‘	   	   
void LCD_Scan_Dir(u8 dir)
{
	u16 regval=0;
	u16 dirreg=0;
	u16 temp;  
	if((lcddev.dir==1&&lcddev.id!=0X6804&&lcddev.id!=0X1963)||(lcddev.dir==0&&lcddev.id==0X1963))//∫·∆¡ ±£¨∂‘6804∫Õ1963≤ª∏ƒ±‰…®√Ë∑ΩœÚ£° ˙∆¡ ±1963∏ƒ±‰∑ΩœÚ
	{			   
		switch(dir)//∑ΩœÚ◊™ªª
		{
			case 0:dir=6;break;
			case 1:dir=7;break;
			case 2:dir=4;break;
			case 3:dir=5;break;
			case 4:dir=1;break;
			case 5:dir=0;break;
			case 6:dir=3;break;
			case 7:dir=2;break;	     
		}
	} 
	if(lcddev.id==0x9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X5510||lcddev.id==0X1963)//9341/6804/5310/5510/1963,Ãÿ ‚¥¶¿Ì
	{
		switch(dir)
		{
			case L2R_U2D://¥”◊ÛµΩ”“,¥”…œµΩœ¬
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U://¥”◊ÛµΩ”“,¥”œ¬µΩ…œ
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D://¥””“µΩ◊Û,¥”…œµΩœ¬
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U://¥””“µΩ◊Û,¥”œ¬µΩ…œ
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R://¥”…œµΩœ¬,¥”◊ÛµΩ”“
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://¥”…œµΩœ¬,¥””“µΩ◊Û
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://¥”œ¬µΩ…œ,¥”◊ÛµΩ”“
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://¥”œ¬µΩ…œ,¥””“µΩ◊Û
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
		if(lcddev.id==0X5510)dirreg=0X3600;
		else dirreg=0X36;
 		if((lcddev.id!=0X5310)&&(lcddev.id!=0X5510)&&(lcddev.id!=0X1963))regval|=0X08;//5310/5510/1963≤ª–Ë“™BGR   
		if(lcddev.id==0X6804)regval|=0x02;//6804µƒBIT6∫Õ9341µƒ∑¥¡À	   
		LCD_WriteReg(dirreg,regval);
		if(lcddev.id!=0X1963)//1963≤ª◊ˆ◊¯±Í¥¶¿Ì
		{
			if(regval&0X20)
			{
				if(lcddev.width<lcddev.height)//ΩªªªX,Y
				{
					temp=lcddev.width;
					lcddev.width=lcddev.height;
					lcddev.height=temp;
				}
			}else  
			{
				if(lcddev.width>lcddev.height)//ΩªªªX,Y
				{
					temp=lcddev.width;
					lcddev.width=lcddev.height;
					lcddev.height=temp;
				}
			}  
		}
		if(lcddev.id==0X5510)
		{
			LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setxcmd+2);LCD_WR_DATA((lcddev.width-1)>>8); 
			LCD_WR_REG(lcddev.setxcmd+3);LCD_WR_DATA((lcddev.width-1)&0XFF); 
			LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setycmd+2);LCD_WR_DATA((lcddev.height-1)>>8); 
			LCD_WR_REG(lcddev.setycmd+3);LCD_WR_DATA((lcddev.height-1)&0XFF);
		}else
		{
			LCD_WR_REG(lcddev.setxcmd); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);
			LCD_WR_REG(lcddev.setycmd); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);  
		}
  	}else 
	{
		switch(dir)
		{
			case L2R_U2D://¥”◊ÛµΩ”“,¥”…œµΩœ¬
				regval|=(1<<5)|(1<<4)|(0<<3); 
				break;
			case L2R_D2U://¥”◊ÛµΩ”“,¥”œ¬µΩ…œ
				regval|=(0<<5)|(1<<4)|(0<<3); 
				break;
			case R2L_U2D://¥””“µΩ◊Û,¥”…œµΩœ¬
				regval|=(1<<5)|(0<<4)|(0<<3);
				break;
			case R2L_D2U://¥””“µΩ◊Û,¥”œ¬µΩ…œ
				regval|=(0<<5)|(0<<4)|(0<<3); 
				break;	 
			case U2D_L2R://¥”…œµΩœ¬,¥”◊ÛµΩ”“
				regval|=(1<<5)|(1<<4)|(1<<3); 
				break;
			case U2D_R2L://¥”…œµΩœ¬,¥””“µΩ◊Û
				regval|=(1<<5)|(0<<4)|(1<<3); 
				break;
			case D2U_L2R://¥”œ¬µΩ…œ,¥”◊ÛµΩ”“
				regval|=(0<<5)|(1<<4)|(1<<3); 
				break;
			case D2U_R2L://¥”œ¬µΩ…œ,¥””“µΩ◊Û
				regval|=(0<<5)|(0<<4)|(1<<3); 
				break;	 
		} 
		dirreg=0X03;
		regval|=1<<12; 
		LCD_WriteReg(dirreg,regval);
	}
}     
//ª≠µ„
//x,y:◊¯±Í
//POINT_COLOR:¥Àµ„µƒ—’…´
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		//…Ë÷√π‚±ÍŒª÷√ 
	LCD_WriteRAM_Prepare();	//ø™ º–¥»ÎGRAM
	LCD->LCD_RAM=POINT_COLOR; 
}
//øÏÀŸª≠µ„
//x,y:◊¯±Í
//color:—’…´
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
{	   
	if(lcddev.id==0X9341||lcddev.id==0X5310)
	{
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF);  			 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF); 		 	 
	}else if(lcddev.id==0X5510)
	{
		LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(x>>8);  
		LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(x&0XFF);	  
		LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(y>>8);  
		LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(y&0XFF); 
	}else if(lcddev.id==0X1963)
	{
		if(lcddev.dir==0)x=lcddev.width-1-x;
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF); 		
		LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF); 		
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF); 		
		LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF); 		
	}else if(lcddev.id==0X6804)
	{		    
		if(lcddev.dir==1)x=lcddev.width-1-x;//∫·∆¡ ±¥¶¿Ì
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF);			 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF); 		
	}else
	{
 		if(lcddev.dir==1)x=lcddev.width-1-x;//∫·∆¡∆‰ µæÕ «µ˜◊™x,y◊¯±Í
		LCD_WriteReg(lcddev.setxcmd,x);
		LCD_WriteReg(lcddev.setycmd,y);
	}			 
	LCD->LCD_REG=lcddev.wramcmd; 
	LCD->LCD_RAM=color; 
}	 
//SSD1963 ±≥π‚…Ë÷√
//pwm:±≥π‚µ»º∂,0~100.‘Ω¥Û‘Ω¡¡.
void LCD_SSD_BackLightSet(u8 pwm)
{	
	LCD_WR_REG(0xBE);	//≈‰÷√PWM ‰≥ˆ
	LCD_WR_DATA(0x05);	//1…Ë÷√PWM∆µ¬ 
	LCD_WR_DATA(pwm*2.55);//2…Ë÷√PWM’ºø’±»
	LCD_WR_DATA(0x01);	//3…Ë÷√C
	LCD_WR_DATA(0xFF);	//4…Ë÷√D
	LCD_WR_DATA(0x00);	//5…Ë÷√E
	LCD_WR_DATA(0x00);	//6…Ë÷√F
}

//…Ë÷√LCDœ‘ æ∑ΩœÚ
//dir:0, ˙∆¡£ª1,∫·∆¡
void LCD_Display_Dir(u8 dir)
{
	if(dir==0)			// ˙∆¡
	{
		lcddev.dir=0;	// ˙∆¡
		lcddev.width=240;
		lcddev.height=320;
		if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;  	 
			if(lcddev.id==0X6804||lcddev.id==0X5310)
			{
				lcddev.width=320;
				lcddev.height=480;
			}
		}else if(lcddev.id==0x5510)
		{
			lcddev.wramcmd=0X2C00;
	 		lcddev.setxcmd=0X2A00;
			lcddev.setycmd=0X2B00; 
			lcddev.width=480;
			lcddev.height=800;
		}else if(lcddev.id==0X1963)
		{
			lcddev.wramcmd=0X2C;	//…Ë÷√–¥»ÎGRAMµƒ÷∏¡Ó 
			lcddev.setxcmd=0X2B;	//…Ë÷√–¥X◊¯±Í÷∏¡Ó
			lcddev.setycmd=0X2A;	//…Ë÷√–¥Y◊¯±Í÷∏¡Ó
			lcddev.width=480;		//…Ë÷√øÌ∂»480
			lcddev.height=800;		//…Ë÷√∏ﬂ∂»800  
		}else
		{
			lcddev.wramcmd=0X22;
	 		lcddev.setxcmd=0X20;
			lcddev.setycmd=0X21;  
		}
	}else 				//∫·∆¡
	{	  				
		lcddev.dir=1;	//∫·∆¡
		lcddev.width=320;
		lcddev.height=240;
		if(lcddev.id==0X9341||lcddev.id==0X5310)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;  	 
		}else if(lcddev.id==0X6804)	 
		{
 			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2B;
			lcddev.setycmd=0X2A; 
		}else if(lcddev.id==0x5510)
		{
			lcddev.wramcmd=0X2C00;
	 		lcddev.setxcmd=0X2A00;
			lcddev.setycmd=0X2B00; 
			lcddev.width=800;
			lcddev.height=480;
		}else if(lcddev.id==0X1963)
		{
			lcddev.wramcmd=0X2C;	//…Ë÷√–¥»ÎGRAMµƒ÷∏¡Ó 
			lcddev.setxcmd=0X2A;	//…Ë÷√–¥X◊¯±Í÷∏¡Ó
			lcddev.setycmd=0X2B;	//…Ë÷√–¥Y◊¯±Í÷∏¡Ó
			lcddev.width=800;		//…Ë÷√øÌ∂»800
			lcddev.height=480;		//…Ë÷√∏ﬂ∂»480  
		}else
		{
			lcddev.wramcmd=0X22;
	 		lcddev.setxcmd=0X21;
			lcddev.setycmd=0X20;  
		}
		if(lcddev.id==0X6804||lcddev.id==0X5310)
		{ 	 
			lcddev.width=480;
			lcddev.height=320; 			
		}
	} 
	LCD_Scan_Dir(DFT_SCAN_DIR);	//ƒ¨»œ…®√Ë∑ΩœÚ
}	 
//…Ë÷√¥∞ø⁄,≤¢◊‘∂Ø…Ë÷√ª≠µ„◊¯±ÍµΩ¥∞ø⁄◊Û…œΩ«(sx,sy).
//sx,sy:¥∞ø⁄∆ º◊¯±Í(◊Û…œΩ«)
//width,height:¥∞ø⁄øÌ∂»∫Õ∏ﬂ∂»,±ÿ–Î¥Û”⁄0!!
//¥∞ÃÂ¥Û–°:width*height. 
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{    
	u8 hsareg,heareg,vsareg,veareg;
	u16 hsaval,heaval,vsaval,veaval; 
	u16 twidth,theight;
	twidth=sx+width-1;
	theight=sy+height-1;
	if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X6804||(lcddev.dir==1&&lcddev.id==0X1963))
	{
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(sx>>8); 
		LCD_WR_DATA(sx&0XFF);	 
		LCD_WR_DATA(twidth>>8); 
		LCD_WR_DATA(twidth&0XFF);  
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(sy>>8); 
		LCD_WR_DATA(sy&0XFF); 
		LCD_WR_DATA(theight>>8); 
		LCD_WR_DATA(theight&0XFF); 
	}else if(lcddev.id==0X1963)//1963 ˙∆¡Ãÿ ‚¥¶¿Ì
	{
		sx=lcddev.width-width-sx; 
		height=sy+height-1; 
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(sx>>8); 
		LCD_WR_DATA(sx&0XFF);	 
		LCD_WR_DATA((sx+width-1)>>8); 
		LCD_WR_DATA((sx+width-1)&0XFF);  
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(sy>>8); 
		LCD_WR_DATA(sy&0XFF); 
		LCD_WR_DATA(height>>8); 
		LCD_WR_DATA(height&0XFF); 		
	}else if(lcddev.id==0X5510)
	{
		LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(sx>>8);  
		LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(sx&0XFF);	  
		LCD_WR_REG(lcddev.setxcmd+2);LCD_WR_DATA(twidth>>8);   
		LCD_WR_REG(lcddev.setxcmd+3);LCD_WR_DATA(twidth&0XFF);   
		LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(sy>>8);   
		LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(sy&0XFF);  
		LCD_WR_REG(lcddev.setycmd+2);LCD_WR_DATA(theight>>8);   
		LCD_WR_REG(lcddev.setycmd+3);LCD_WR_DATA(theight&0XFF);  
	}else	//∆‰À˚«˝∂ØIC
	{
		if(lcddev.dir==1)//∫·∆¡
		{
			//¥∞ø⁄÷µ
			hsaval=sy;				
			heaval=theight;
			vsaval=lcddev.width-twidth-1;
			veaval=lcddev.width-sx-1;				
		}else
		{ 
			hsaval=sx;				
			heaval=twidth;
			vsaval=sy;
			veaval=theight;
		} 
		hsareg=0X50;heareg=0X51;//ÀÆ∆Ω∑ΩœÚ¥∞ø⁄ºƒ¥Ê∆˜
		vsareg=0X52;veareg=0X53;//¥π÷±∑ΩœÚ¥∞ø⁄ºƒ¥Ê∆˜	   							  
		//…Ë÷√ºƒ¥Ê∆˜÷µ
		LCD_WriteReg(hsareg,hsaval);
		LCD_WriteReg(heareg,heaval);
		LCD_WriteReg(vsareg,vsaval);
		LCD_WriteReg(veareg,veaval);		
		LCD_SetCursor(sx,sy);	//…Ë÷√π‚±ÍŒª÷√
	}
}
//≥ı ºªØlcd
//∏√≥ı ºªØ∫Ø ˝ø…“‘≥ı ºªØ∏˜÷÷ILI93XX“∫æß,µ´ «∆‰À˚∫Ø ˝ «ª˘”⁄ILI9320µƒ!!!
//‘⁄∆‰À˚–Õ∫≈µƒ«˝∂Ø–æ∆¨…œ√ª”–≤‚ ‘! 
void LCD_Init(void)
{ 	
	vu32 i=0;
	
  GPIO_InitTypeDef  GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG, ENABLE);// πƒ‹PD,PE,PF,PG ±÷”  
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);// πƒ‹FSMC ±÷”  
	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//PB15 Õ∆ÕÏ ‰≥ˆ,øÿ÷∆±≥π‚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//∆’Õ® ‰≥ˆƒ£ Ω
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Õ∆ÕÏ ‰≥ˆ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//…œ¿≠
  GPIO_Init(GPIOB, &GPIO_InitStructure);//≥ı ºªØ //PB15 Õ∆ÕÏ ‰≥ˆ,øÿ÷∆±≥π‚
	
  GPIO_InitStructure.GPIO_Pin = (3<<0)|(3<<4)|(7<<8)|(3<<14);//PD0,1,4,5,8,9,10,14,15 AF OUT
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//∏¥”√ ‰≥ˆ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Õ∆ÕÏ ‰≥ˆ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//…œ¿≠
  GPIO_Init(GPIOD, &GPIO_InitStructure);//≥ı ºªØ  
	
  GPIO_InitStructure.GPIO_Pin = (0X1FF<<7);//PE7~15,AF OUT
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//∏¥”√ ‰≥ˆ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Õ∆ÕÏ ‰≥ˆ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//…œ¿≠
  GPIO_Init(GPIOE, &GPIO_InitStructure);//≥ı ºªØ  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//PF12,FSMC_A6
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//∏¥”√ ‰≥ˆ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Õ∆ÕÏ ‰≥ˆ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//…œ¿≠
  GPIO_Init(GPIOF, &GPIO_InitStructure);//≥ı ºªØ  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//PF12,FSMC_A6
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//∏¥”√ ‰≥ˆ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//Õ∆ÕÏ ‰≥ˆ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//…œ¿≠
  GPIO_Init(GPIOG, &GPIO_InitStructure);//≥ı ºªØ 

  GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);//PD0,AF12
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);//PD1,AF12
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC); 
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_FSMC); 
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);//PD15,AF12
 
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);//PE7,AF12
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC);//PE15,AF12
 
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource12,GPIO_AF_FSMC);//PF12,AF12
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource12,GPIO_AF_FSMC);


  readWriteTiming.FSMC_AddressSetupTime = 0XF;	 //µÿ÷∑Ω®¡¢ ±º‰£®ADDSET£©Œ™16∏ˆHCLK 1/168M=6ns*16=96ns	
  readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //µÿ÷∑±£≥÷ ±º‰£®ADDHLD£©ƒ£ ΩAŒ¥”√µΩ	
  readWriteTiming.FSMC_DataSetupTime = 60;			// ˝æ›±£¥Ê ±º‰Œ™60∏ˆHCLK	=6*60=360ns
  readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
  readWriteTiming.FSMC_CLKDivision = 0x00;
  readWriteTiming.FSMC_DataLatency = 0x00;
  readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ƒ£ ΩA 
    

	writeTiming.FSMC_AddressSetupTime =9;	      //µÿ÷∑Ω®¡¢ ±º‰£®ADDSET£©Œ™9∏ˆHCLK =54ns 
  writeTiming.FSMC_AddressHoldTime = 0x00;	 //µÿ÷∑±£≥÷ ±º‰£®A		
  writeTiming.FSMC_DataSetupTime = 8;		 // ˝æ›±£¥Ê ±º‰Œ™6ns*9∏ˆHCLK=54ns
  writeTiming.FSMC_BusTurnAroundDuration = 0x00;
  writeTiming.FSMC_CLKDivision = 0x00;
  writeTiming.FSMC_DataLatency = 0x00;
  writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ƒ£ ΩA 

 
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;//  ’‚¿ÔŒ“√« π”√NE4 £¨“≤æÕ∂‘”¶BTCR[6],[7]°£
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // ≤ª∏¥”√ ˝æ›µÿ÷∑
  FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//¥Ê¥¢∆˜ ˝æ›øÌ∂»Œ™16bit   
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  ¥Ê¥¢∆˜–¥ πƒ‹
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // ∂¡–¥ π”√≤ªÕ¨µƒ ±–Ú
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //∂¡–¥ ±–Ú
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //–¥ ±–Ú

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //≥ı ºªØFSMC≈‰÷√

  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  //  πƒ‹BANK1 
		
 	delay_ms(50); // delay 50 ms 
 	LCD_WriteReg(0x0000,0x0001);
	delay_ms(50); // delay 50 ms 
  	lcddev.id = LCD_ReadReg(0x0000);   
   	if(lcddev.id<0XFF||lcddev.id==0XFFFF||lcddev.id==0X9300)//∂¡µΩID≤ª’˝»∑,–¬‘ˆlcddev.id==0X9300≈–∂œ£¨“ÚŒ™9341‘⁄Œ¥±ª∏¥Œªµƒ«Èøˆœ¬ª·±ª∂¡≥…9300
	{	
 		//≥¢ ‘9341 IDµƒ∂¡»°		
		LCD_WR_REG(0XD3);				   
		lcddev.id=LCD_RD_DATA();	//dummy read 	
 		lcddev.id=LCD_RD_DATA();	//∂¡µΩ0X00
  		lcddev.id=LCD_RD_DATA();   	//∂¡»°93								   
 		lcddev.id<<=8;
		lcddev.id|=LCD_RD_DATA();  	//∂¡»°41 	   			   
 		if(lcddev.id!=0X9341)		//∑«9341,≥¢ ‘ «≤ª «6804
		{	
 			LCD_WR_REG(0XBF);				   
			lcddev.id=LCD_RD_DATA(); 	//dummy read 	 
	 		lcddev.id=LCD_RD_DATA();   	//∂¡ªÿ0X01			   
	 		lcddev.id=LCD_RD_DATA(); 	//∂¡ªÿ0XD0 			  	
	  		lcddev.id=LCD_RD_DATA();	//’‚¿Ô∂¡ªÿ0X68 
			lcddev.id<<=8;
	  		lcddev.id|=LCD_RD_DATA();	//’‚¿Ô∂¡ªÿ0X04	  
			if(lcddev.id!=0X6804)		//“≤≤ª «6804,≥¢ ‘ø¥ø¥ «≤ª «NT35310
			{ 
				LCD_WR_REG(0XD4);				   
				lcddev.id=LCD_RD_DATA();//dummy read  
				lcddev.id=LCD_RD_DATA();//∂¡ªÿ0X01	 
				lcddev.id=LCD_RD_DATA();//∂¡ªÿ0X53	
				lcddev.id<<=8;	 
				lcddev.id|=LCD_RD_DATA();	//’‚¿Ô∂¡ªÿ0X10	 
				if(lcddev.id!=0X5310)		//“≤≤ª «NT35310,≥¢ ‘ø¥ø¥ «≤ª «NT35510
				{
					LCD_WR_REG(0XDA00);	
					lcddev.id=LCD_RD_DATA();		//∂¡ªÿ0X00	 
					LCD_WR_REG(0XDB00);	
					lcddev.id=LCD_RD_DATA();		//∂¡ªÿ0X80
					lcddev.id<<=8;	
					LCD_WR_REG(0XDC00);	
					lcddev.id|=LCD_RD_DATA();		//∂¡ªÿ0X00		
					if(lcddev.id==0x8000)lcddev.id=0x5510;//NT35510∂¡ªÿµƒID «8000H,Œ™∑Ω±„«¯∑÷,Œ“√««ø÷∆…Ë÷√Œ™5510
					if(lcddev.id!=0X5510)			//“≤≤ª «NT5510,≥¢ ‘ø¥ø¥ «≤ª «SSD1963
					{
						LCD_WR_REG(0XA1);
						lcddev.id=LCD_RD_DATA();
						lcddev.id=LCD_RD_DATA();	//∂¡ªÿ0X57
						lcddev.id<<=8;	 
						lcddev.id|=LCD_RD_DATA();	//∂¡ªÿ0X61	
						if(lcddev.id==0X5761)lcddev.id=0X1963;//SSD1963∂¡ªÿµƒID «5761H,Œ™∑Ω±„«¯∑÷,Œ“√««ø÷∆…Ë÷√Œ™1963
					}
				}
			}
 		}  	
	} 
	if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X5510||lcddev.id==0X1963)//»Áπ˚ «’‚º∏∏ˆIC,‘Ú…Ë÷√WR ±–ÚŒ™◊ÓøÏ
	{
		//÷ÿ–¬≈‰÷√–¥ ±–Úøÿ÷∆ºƒ¥Ê∆˜µƒ ±–Ú   	 							    
		FSMC_Bank1E->BWTR[6]&=~(0XF<<0);//µÿ÷∑Ω®¡¢ ±º‰(ADDSET)«Â¡„ 	 
		FSMC_Bank1E->BWTR[6]&=~(0XF<<8);// ˝æ›±£¥Ê ±º‰«Â¡„
		FSMC_Bank1E->BWTR[6]|=3<<0;		//µÿ÷∑Ω®¡¢ ±º‰(ADDSET)Œ™3∏ˆHCLK =18ns  	 
		FSMC_Bank1E->BWTR[6]|=2<<8; 	// ˝æ›±£¥Ê ±º‰(DATAST)Œ™6ns*3∏ˆHCLK=18ns
	}else if(lcddev.id==0X6804||lcddev.id==0XC505)	//6804/C505ÀŸ∂»…œ≤ª»•,µ√ΩµµÕ
	{
		//÷ÿ–¬≈‰÷√–¥ ±–Úøÿ÷∆ºƒ¥Ê∆˜µƒ ±–Ú   	 							    
		FSMC_Bank1E->BWTR[6]&=~(0XF<<0);//µÿ÷∑Ω®¡¢ ±º‰(ADDSET)«Â¡„ 	 
		FSMC_Bank1E->BWTR[6]&=~(0XF<<8);// ˝æ›±£¥Ê ±º‰«Â¡„
		FSMC_Bank1E->BWTR[6]|=10<<0;	//µÿ÷∑Ω®¡¢ ±º‰(ADDSET)Œ™10∏ˆHCLK =60ns  	 
		FSMC_Bank1E->BWTR[6]|=12<<8; 	// ˝æ›±£¥Ê ±º‰(DATAST)Œ™6ns*13∏ˆHCLK=78ns
	}
 	printf(" LCD ID:%x\r\n",lcddev.id); //¥Ú”°LCD ID   
	if(lcddev.id==0X9341)	//9341≥ı ºªØ
	{	 
		LCD_WR_REG(0xCF);  
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0xC1); 
		LCD_WR_DATA(0X30); 
		LCD_WR_REG(0xED);  
		LCD_WR_DATA(0x64); 
		LCD_WR_DATA(0x03); 
		LCD_WR_DATA(0X12); 
		LCD_WR_DATA(0X81); 
		LCD_WR_REG(0xE8);  
		LCD_WR_DATA(0x85); 
		LCD_WR_DATA(0x10); 
		LCD_WR_DATA(0x7A); 
		LCD_WR_REG(0xCB);  
		LCD_WR_DATA(0x39); 
		LCD_WR_DATA(0x2C); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x34); 
		LCD_WR_DATA(0x02); 
		LCD_WR_REG(0xF7);  
		LCD_WR_DATA(0x20); 
		LCD_WR_REG(0xEA);  
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_REG(0xC0);    //Power control 
		LCD_WR_DATA(0x1B);   //VRH[5:0] 
		LCD_WR_REG(0xC1);    //Power control 
		LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0] 
		LCD_WR_REG(0xC5);    //VCM control 
		LCD_WR_DATA(0x30); 	 //3F
		LCD_WR_DATA(0x30); 	 //3C
		LCD_WR_REG(0xC7);    //VCM control2 
		LCD_WR_DATA(0XB7); 
		LCD_WR_REG(0x36);    // Memory Access Control 
		LCD_WR_DATA(0x48); 
		LCD_WR_REG(0x3A);   
		LCD_WR_DATA(0x55); 
		LCD_WR_REG(0xB1);   
		LCD_WR_DATA(0x00);   
		LCD_WR_DATA(0x1A); 
		LCD_WR_REG(0xB6);    // Display Function Control 
		LCD_WR_DATA(0x0A); 
		LCD_WR_DATA(0xA2); 
		LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
		LCD_WR_DATA(0x00); 
		LCD_WR_REG(0x26);    //Gamma curve selected 
		LCD_WR_DATA(0x01); 
		LCD_WR_REG(0xE0);    //Set Gamma 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x2A); 
		LCD_WR_DATA(0x28); 
		LCD_WR_DATA(0x08); 
		LCD_WR_DATA(0x0E); 
		LCD_WR_DATA(0x08); 
		LCD_WR_DATA(0x54); 
		LCD_WR_DATA(0XA9); 
		LCD_WR_DATA(0x43); 
		LCD_WR_DATA(0x0A); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 		 
		LCD_WR_REG(0XE1);    //Set Gamma 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x15); 
		LCD_WR_DATA(0x17); 
		LCD_WR_DATA(0x07); 
		LCD_WR_DATA(0x11); 
		LCD_WR_DATA(0x06); 
		LCD_WR_DATA(0x2B); 
		LCD_WR_DATA(0x56); 
		LCD_WR_DATA(0x3C); 
		LCD_WR_DATA(0x05); 
		LCD_WR_DATA(0x10); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x3F); 
		LCD_WR_DATA(0x3F); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_REG(0x2B); 
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x01);
		LCD_WR_DATA(0x3f);
		LCD_WR_REG(0x2A); 
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xef);	 
		LCD_WR_REG(0x11); //Exit Sleep
		delay_ms(120);
		LCD_WR_REG(0x29); //display on	
	}else if(lcddev.id==0x6804) //6804≥ı ºªØ
	{
		LCD_WR_REG(0X11);
		delay_ms(20);
		LCD_WR_REG(0XD0);//VCI1  VCL  VGH  VGL DDVDH VREG1OUT power amplitude setting
		LCD_WR_DATA(0X07); 
		LCD_WR_DATA(0X42); 
		LCD_WR_DATA(0X1D); 
		LCD_WR_REG(0XD1);//VCOMH VCOM_AC amplitude setting
		LCD_WR_DATA(0X00);
		LCD_WR_DATA(0X1a);
		LCD_WR_DATA(0X09); 
		LCD_WR_REG(0XD2);//Operational Amplifier Circuit Constant Current Adjust , charge pump frequency setting
		LCD_WR_DATA(0X01);
		LCD_WR_DATA(0X22);
		LCD_WR_REG(0XC0);//REV SM GS 
		LCD_WR_DATA(0X10);
		LCD_WR_DATA(0X3B);
		LCD_WR_DATA(0X00);
		LCD_WR_DATA(0X02);
		LCD_WR_DATA(0X11);
		
		LCD_WR_REG(0XC5);// Frame rate setting = 72HZ  when setting 0x03
		LCD_WR_DATA(0X03);
		
		LCD_WR_REG(0XC8);//Gamma setting
		LCD_WR_DATA(0X00);
		LCD_WR_DATA(0X25);
		LCD_WR_DATA(0X21);
		LCD_WR_DATA(0X05);
		LCD_WR_DATA(0X00);
		LCD_WR_DATA(0X0a);
		LCD_WR_DATA(0X65);
		LCD_WR_DATA(0X25);
		LCD_WR_DATA(0X77);
		LCD_WR_DATA(0X50);
		LCD_WR_DATA(0X0f);
		LCD_WR_DATA(0X00);	  
						  
   		LCD_WR_REG(0XF8);
		LCD_WR_DATA(0X01);	  

 		LCD_WR_REG(0XFE);
 		LCD_WR_DATA(0X00);
 		LCD_WR_DATA(0X02);
		
		LCD_WR_REG(0X20);//Exit invert mode

		LCD_WR_REG(0X36);
		LCD_WR_DATA(0X08);//‘≠¿¥ «a
		
		LCD_WR_REG(0X3A);
		LCD_WR_DATA(0X55);//16Œªƒ£ Ω	  
		LCD_WR_REG(0X2B);
		LCD_WR_DATA(0X00);
		LCD_WR_DATA(0X00);
		LCD_WR_DATA(0X01);
		LCD_WR_DATA(0X3F);
		
		LCD_WR_REG(0X2A);
		LCD_WR_DATA(0X00);
		LCD_WR_DATA(0X00);
		LCD_WR_DATA(0X01);
		LCD_WR_DATA(0XDF);
		delay_ms(120);
		LCD_WR_REG(0X29); 	 
 	}else if(lcddev.id==0x5310)
	{ 
		LCD_WR_REG(0xED);
		LCD_WR_DATA(0x01);
		LCD_WR_DATA(0xFE);

		LCD_WR_REG(0xEE);
		LCD_WR_DATA(0xDE);
		LCD_WR_DATA(0x21);

		LCD_WR_REG(0xF1);
		LCD_WR_DATA(0x01);
		LCD_WR_REG(0xDF);
		LCD_WR_DATA(0x10);

		//VCOMvoltage//
		LCD_WR_REG(0xC4);
		LCD_WR_DATA(0x8F);	  //5f

		LCD_WR_REG(0xC6);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xE2);
		LCD_WR_DATA(0xE2);
		LCD_WR_DATA(0xE2);
		LCD_WR_REG(0xBF);
		LCD_WR_DATA(0xAA);

		LCD_WR_REG(0xB0);
		LCD_WR_DATA(0x0D);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x0D);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x11);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x19);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x21);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x2D);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x3D);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x5D);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x5D);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xB1);
		LCD_WR_DATA(0x80);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x8B);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x96);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xB2);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x02);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x03);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xB3);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xB4);
		LCD_WR_DATA(0x8B);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x96);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xA1);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xB5);
		LCD_WR_DATA(0x02);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x03);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x04);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xB6);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xB7);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x3F);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x5E);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x64);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x8C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xAC);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xDC);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x70);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x90);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xEB);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xDC);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xB8);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xBA);
		LCD_WR_DATA(0x24);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xC1);
		LCD_WR_DATA(0x20);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x54);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xFF);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xC2);
		LCD_WR_DATA(0x0A);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x04);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xC3);
		LCD_WR_DATA(0x3C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x3A);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x39);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x37);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x3C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x36);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x32);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x2F);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x2C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x29);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x26);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x24);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x24);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x23);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x3C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x36);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x32);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x2F);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x2C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x29);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x26);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x24);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x24);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x23);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xC4);
		LCD_WR_DATA(0x62);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x05);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x84);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xF0);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x18);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xA4);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x18);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x50);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x0C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x17);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x95);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xF3);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xE6);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xC5);
		LCD_WR_DATA(0x32);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x44);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x65);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x76);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x88);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xC6);
		LCD_WR_DATA(0x20);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x17);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x01);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xC7);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xC8);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xC9);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xE0);
		LCD_WR_DATA(0x16);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x1C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x21);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x36);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x46);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x52);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x64);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x7A);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x8B);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x99);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xA8);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xB9);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xC4);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xCA);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xD2);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xD9);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xE0);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xF3);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xE1);
		LCD_WR_DATA(0x16);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x1C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x22);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x36);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x45);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x52);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x64);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x7A);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x8B);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x99);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xA8);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xB9);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xC4);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xCA);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xD2);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xD8);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xE0);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xF3);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xE2);
		LCD_WR_DATA(0x05);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x0B);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x1B);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x34);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x44);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x4F);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x61);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x79);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x88);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x97);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xA6);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xB7);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xC2);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xC7);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xD1);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xD6);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xDD);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xF3);
		LCD_WR_DATA(0x00);
		LCD_WR_REG(0xE3);
		LCD_WR_DATA(0x05);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xA);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x1C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x33);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x44);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x50);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x62);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x78);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x88);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x97);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xA6);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xB7);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xC2);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xC7);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xD1);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xD5);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xDD);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xF3);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xE4);
		LCD_WR_DATA(0x01);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x01);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x02);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x2A);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x3C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x4B);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x5D);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x74);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x84);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x93);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xA2);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xB3);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xBE);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xC4);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xCD);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xD3);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xDD);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xF3);
		LCD_WR_DATA(0x00);
		LCD_WR_REG(0xE5);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x02);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x29);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x3C);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x4B);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x5D);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x74);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x84);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x93);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xA2);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xB3);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xBE);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xC4);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xCD);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xD3);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xDC);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xF3);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xE6);
		LCD_WR_DATA(0x11);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x34);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x56);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x76);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x77);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x66);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x88);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x99);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xBB);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x99);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x66);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x55);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x55);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x45);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x43);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x44);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xE7);
		LCD_WR_DATA(0x32);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x55);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x76);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x66);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x67);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x67);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x87);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x99);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xBB);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x99);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x77);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x44);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x56);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x23); 
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x33);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x45);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xE8);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x99);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x87);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x88);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x77);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x66);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x88);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xAA);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xBB);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x99);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x66);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x55);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x55);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x44);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x44);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x55);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xE9);
		LCD_WR_DATA(0xAA);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0x00);
		LCD_WR_DATA(0xAA);

		LCD_WR_REG(0xCF);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xF0);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x50);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xF3);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0xF9);
		LCD_WR_DATA(0x06);
		LCD_WR_DATA(0x10);
		LCD_WR_DATA(0x29);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0x3A);
		LCD_WR_DATA(0x55);	//66

		LCD_WR_REG(0x11);
		delay_ms(100);
		LCD_WR_REG(0x29);
		LCD_WR_REG(0x35);
		LCD_WR_DATA(0x00);

		LCD_WR_REG(0x51);
		LCD_WR_DATA(0xFF);
		LCD_WR_REG(0x53);
		LCD_WR_DATA(0x2C);
		LCD_WR_REG(0x55);
		LCD_WR_DATA(0x82);
		LCD_WR_REG(0x2c);
	}else if(lcddev.id==0x5510)
	{
		LCD_WriteReg(0xF000,0x55);
		LCD_WriteReg(0xF001,0xAA);
		LCD_WriteReg(0xF002,0x52);
		LCD_WriteReg(0xF003,0x08);
		LCD_WriteReg(0xF004,0x01);
		//AVDD Set AVDD 5.2V
		LCD_WriteReg(0xB000,0x0D);
		LCD_WriteReg(0xB001,0x0D);
		LCD_WriteReg(0xB002,0x0D);
		//AVDD ratio
		LCD_WriteReg(0xB600,0x34);
		LCD_WriteReg(0xB601,0x34);
		LCD_WriteReg(0xB602,0x34);
		//AVEE -5.2V
		LCD_WriteReg(0xB100,0x0D);
		LCD_WriteReg(0xB101,0x0D);
		LCD_WriteReg(0xB102,0x0D);
		//AVEE ratio
		LCD_WriteReg(0xB700,0x34);
		LCD_WriteReg(0xB701,0x34);
		LCD_WriteReg(0xB702,0x34);
		//VCL -2.5V
		LCD_WriteReg(0xB200,0x00);
		LCD_WriteReg(0xB201,0x00);
		LCD_WriteReg(0xB202,0x00);
		//VCL ratio
		LCD_WriteReg(0xB800,0x24);
		LCD_WriteReg(0xB801,0x24);
		LCD_WriteReg(0xB802,0x24);
		//VGH 15V (Free pump)
		LCD_WriteReg(0xBF00,0x01);
		LCD_WriteReg(0xB300,0x0F);
		LCD_WriteReg(0xB301,0x0F);
		LCD_WriteReg(0xB302,0x0F);
		//VGH ratio
		LCD_WriteReg(0xB900,0x34);
		LCD_WriteReg(0xB901,0x34);
		LCD_WriteReg(0xB902,0x34);
		//VGL_REG -10V
		LCD_WriteReg(0xB500,0x08);
		LCD_WriteReg(0xB501,0x08);
		LCD_WriteReg(0xB502,0x08);
		LCD_WriteReg(0xC200,0x03);
		//VGLX ratio
		LCD_WriteReg(0xBA00,0x24);
		LCD_WriteReg(0xBA01,0x24);
		LCD_WriteReg(0xBA02,0x24);
		//VGMP/VGSP 4.5V/0V
		LCD_WriteReg(0xBC00,0x00);
		LCD_WriteReg(0xBC01,0x78);
		LCD_WriteReg(0xBC02,0x00);
		//VGMN/VGSN -4.5V/0V
		LCD_WriteReg(0xBD00,0x00);
		LCD_WriteReg(0xBD01,0x78);
		LCD_WriteReg(0xBD02,0x00);
		//VCOM
		LCD_WriteReg(0xBE00,0x00);
		LCD_WriteReg(0xBE01,0x64);
		//Gamma Setting
		LCD_WriteReg(0xD100,0x00);
		LCD_WriteReg(0xD101,0x33);
		LCD_WriteReg(0xD102,0x00);
		LCD_WriteReg(0xD103,0x34);
		LCD_WriteReg(0xD104,0x00);
		LCD_WriteReg(0xD105,0x3A);
		LCD_WriteReg(0xD106,0x00);
		LCD_WriteReg(0xD107,0x4A);
		LCD_WriteReg(0xD108,0x00);
		LCD_WriteReg(0xD109,0x5C);
		LCD_WriteReg(0xD10A,0x00);
		LCD_WriteReg(0xD10B,0x81);
		LCD_WriteReg(0xD10C,0x00);
		LCD_WriteReg(0xD10D,0xA6);
		LCD_WriteReg(0xD10E,0x00);
		LCD_WriteReg(0xD10F,0xE5);
		LCD_WriteReg(0xD110,0x01);
		LCD_WriteReg(0xD111,0x13);
		LCD_WriteReg(0xD112,0x01);
		LCD_WriteReg(0xD113,0x54);
		LCD_WriteReg(0xD114,0x01);
		LCD_WriteReg(0xD115,0x82);
		LCD_WriteReg(0xD116,0x01);
		LCD_WriteReg(0xD117,0xCA);
		LCD_WriteReg(0xD118,0x02);
		LCD_WriteReg(0xD119,0x00);
		LCD_WriteReg(0xD11A,0x02);
		LCD_WriteReg(0xD11B,0x01);
		LCD_WriteReg(0xD11C,0x02);
		LCD_WriteReg(0xD11D,0x34);
		LCD_WriteReg(0xD11E,0x02);
		LCD_WriteReg(0xD11F,0x67);
		LCD_WriteReg(0xD120,0x02);
		LCD_WriteReg(0xD121,0x84);
		LCD_WriteReg(0xD122,0x02);
		LCD_WriteReg(0xD123,0xA4);
		LCD_WriteReg(0xD124,0x02);
		LCD_WriteReg(0xD125,0xB7);
		LCD_WriteReg(0xD126,0x02);
		LCD_WriteReg(0xD127,0xCF);
		LCD_WriteReg(0xD128,0x02);
		LCD_WriteReg(0xD129,0xDE);
		LCD_WriteReg(0xD12A,0x02);
		LCD_WriteReg(0xD12B,0xF2);
		LCD_WriteReg(0xD12C,0x02);
		LCD_WriteReg(0xD12D,0xFE);
		LCD_WriteReg(0xD12E,0x03);
		LCD_WriteReg(0xD12F,0x10);
		LCD_WriteReg(0xD130,0x03);
		LCD_WriteReg(0xD131,0x33);
		LCD_WriteReg(0xD132,0x03);
		LCD_WriteReg(0xD133,0x6D);
		LCD_WriteReg(0xD200,0x00);
		LCD_WriteReg(0xD201,0x33);
		LCD_WriteReg(0xD202,0x00);
		LCD_WriteReg(0xD203,0x34);
		LCD_WriteReg(0xD204,0x00);
		LCD_WriteReg(0xD205,0x3A);
		LCD_WriteReg(0xD206,0x00);
		LCD_WriteReg(0xD207,0x4A);
		LCD_WriteReg(0xD208,0x00);
		LCD_WriteReg(0xD209,0x5C);
		LCD_WriteReg(0xD20A,0x00);

		LCD_WriteReg(0xD20B,0x81);
		LCD_WriteReg(0xD20C,0x00);
		LCD_WriteReg(0xD20D,0xA6);
		LCD_WriteReg(0xD20E,0x00);
		LCD_WriteReg(0xD20F,0xE5);
		LCD_WriteReg(0xD210,0x01);
		LCD_WriteReg(0xD211,0x13);
		LCD_WriteReg(0xD212,0x01);
		LCD_WriteReg(0xD213,0x54);
		LCD_WriteReg(0xD214,0x01);
		LCD_WriteReg(0xD215,0x82);
		LCD_WriteReg(0xD216,0x01);
		LCD_WriteReg(0xD217,0xCA);
		LCD_WriteReg(0xD218,0x02);
		LCD_WriteReg(0xD219,0x00);
		LCD_WriteReg(0xD21A,0x02);
		LCD_WriteReg(0xD21B,0x01);
		LCD_WriteReg(0xD21C,0x02);
		LCD_WriteReg(0xD21D,0x34);
		LCD_WriteReg(0xD21E,0x02);
		LCD_WriteReg(0xD21F,0x67);
		LCD_WriteReg(0xD220,0x02);
		LCD_WriteReg(0xD221,0x84);
		LCD_WriteReg(0xD222,0x02);
		LCD_WriteReg(0xD223,0xA4);
		LCD_WriteReg(0xD224,0x02);
		LCD_WriteReg(0xD225,0xB7);
		LCD_WriteReg(0xD226,0x02);
		LCD_WriteReg(0xD227,0xCF);
		LCD_WriteReg(0xD228,0x02);
		LCD_WriteReg(0xD229,0xDE);
		LCD_WriteReg(0xD22A,0x02);
		LCD_WriteReg(0xD22B,0xF2);
		LCD_WriteReg(0xD22C,0x02);
		LCD_WriteReg(0xD22D,0xFE);
		LCD_WriteReg(0xD22E,0x03);
		LCD_WriteReg(0xD22F,0x10);
		LCD_WriteReg(0xD230,0x03);
		LCD_WriteReg(0xD231,0x33);
		LCD_WriteReg(0xD232,0x03);
		LCD_WriteReg(0xD233,0x6D);
		LCD_WriteReg(0xD300,0x00);
		LCD_WriteReg(0xD301,0x33);
		LCD_WriteReg(0xD302,0x00);
		LCD_WriteReg(0xD303,0x34);
		LCD_WriteReg(0xD304,0x00);
		LCD_WriteReg(0xD305,0x3A);
		LCD_WriteReg(0xD306,0x00);
		LCD_WriteReg(0xD307,0x4A);
		LCD_WriteReg(0xD308,0x00);
		LCD_WriteReg(0xD309,0x5C);
		LCD_WriteReg(0xD30A,0x00);

		LCD_WriteReg(0xD30B,0x81);
		LCD_WriteReg(0xD30C,0x00);
		LCD_WriteReg(0xD30D,0xA6);
		LCD_WriteReg(0xD30E,0x00);
		LCD_WriteReg(0xD30F,0xE5);
		LCD_WriteReg(0xD310,0x01);
		LCD_WriteReg(0xD311,0x13);
		LCD_WriteReg(0xD312,0x01);
		LCD_WriteReg(0xD313,0x54);
		LCD_WriteReg(0xD314,0x01);
		LCD_WriteReg(0xD315,0x82);
		LCD_WriteReg(0xD316,0x01);
		LCD_WriteReg(0xD317,0xCA);
		LCD_WriteReg(0xD318,0x02);
		LCD_WriteReg(0xD319,0x00);
		LCD_WriteReg(0xD31A,0x02);
		LCD_WriteReg(0xD31B,0x01);
		LCD_WriteReg(0xD31C,0x02);
		LCD_WriteReg(0xD31D,0x34);
		LCD_WriteReg(0xD31E,0x02);
		LCD_WriteReg(0xD31F,0x67);
		LCD_WriteReg(0xD320,0x02);
		LCD_WriteReg(0xD321,0x84);
		LCD_WriteReg(0xD322,0x02);
		LCD_WriteReg(0xD323,0xA4);
		LCD_WriteReg(0xD324,0x02);
		LCD_WriteReg(0xD325,0xB7);
		LCD_WriteReg(0xD326,0x02);
		LCD_WriteReg(0xD327,0xCF);
		LCD_WriteReg(0xD328,0x02);
		LCD_WriteReg(0xD329,0xDE);
		LCD_WriteReg(0xD32A,0x02);
		LCD_WriteReg(0xD32B,0xF2);
		LCD_WriteReg(0xD32C,0x02);
		LCD_WriteReg(0xD32D,0xFE);
		LCD_WriteReg(0xD32E,0x03);
		LCD_WriteReg(0xD32F,0x10);
		LCD_WriteReg(0xD330,0x03);
		LCD_WriteReg(0xD331,0x33);
		LCD_WriteReg(0xD332,0x03);
		LCD_WriteReg(0xD333,0x6D);
		LCD_WriteReg(0xD400,0x00);
		LCD_WriteReg(0xD401,0x33);
		LCD_WriteReg(0xD402,0x00);
		LCD_WriteReg(0xD403,0x34);
		LCD_WriteReg(0xD404,0x00);
		LCD_WriteReg(0xD405,0x3A);
		LCD_WriteReg(0xD406,0x00);
		LCD_WriteReg(0xD407,0x4A);
		LCD_WriteReg(0xD408,0x00);
		LCD_WriteReg(0xD409,0x5C);
		LCD_WriteReg(0xD40A,0x00);
		LCD_WriteReg(0xD40B,0x81);

		LCD_WriteReg(0xD40C,0x00);
		LCD_WriteReg(0xD40D,0xA6);
		LCD_WriteReg(0xD40E,0x00);
		LCD_WriteReg(0xD40F,0xE5);
		LCD_WriteReg(0xD410,0x01);
		LCD_WriteReg(0xD411,0x13);
		LCD_WriteReg(0xD412,0x01);
		LCD_WriteReg(0xD413,0x54);
		LCD_WriteReg(0xD414,0x01);
		LCD_WriteReg(0xD415,0x82);
		LCD_WriteReg(0xD416,0x01);
		LCD_WriteReg(0xD417,0xCA);
		LCD_WriteReg(0xD418,0x02);
		LCD_WriteReg(0xD419,0x00);
		LCD_WriteReg(0xD41A,0x02);
		LCD_WriteReg(0xD41B,0x01);
		LCD_WriteReg(0xD41C,0x02);
		LCD_WriteReg(0xD41D,0x34);
		LCD_WriteReg(0xD41E,0x02);
		LCD_WriteReg(0xD41F,0x67);
		LCD_WriteReg(0xD420,0x02);
		LCD_WriteReg(0xD421,0x84);
		LCD_WriteReg(0xD422,0x02);
		LCD_WriteReg(0xD423,0xA4);
		LCD_WriteReg(0xD424,0x02);
		LCD_WriteReg(0xD425,0xB7);
		LCD_WriteReg(0xD426,0x02);
		LCD_WriteReg(0xD427,0xCF);
		LCD_WriteReg(0xD428,0x02);
		LCD_WriteReg(0xD429,0xDE);
		LCD_WriteReg(0xD42A,0x02);
		LCD_WriteReg(0xD42B,0xF2);
		LCD_WriteReg(0xD42C,0x02);
		LCD_WriteReg(0xD42D,0xFE);
		LCD_WriteReg(0xD42E,0x03);
		LCD_WriteReg(0xD42F,0x10);
		LCD_WriteReg(0xD430,0x03);
		LCD_WriteReg(0xD431,0x33);
		LCD_WriteReg(0xD432,0x03);
		LCD_WriteReg(0xD433,0x6D);
		LCD_WriteReg(0xD500,0x00);
		LCD_WriteReg(0xD501,0x33);
		LCD_WriteReg(0xD502,0x00);
		LCD_WriteReg(0xD503,0x34);
		LCD_WriteReg(0xD504,0x00);
		LCD_WriteReg(0xD505,0x3A);
		LCD_WriteReg(0xD506,0x00);
		LCD_WriteReg(0xD507,0x4A);
		LCD_WriteReg(0xD508,0x00);
		LCD_WriteReg(0xD509,0x5C);
		LCD_WriteReg(0xD50A,0x00);
		LCD_WriteReg(0xD50B,0x81);

		LCD_WriteReg(0xD50C,0x00);
		LCD_WriteReg(0xD50D,0xA6);
		LCD_WriteReg(0xD50E,0x00);
		LCD_WriteReg(0xD50F,0xE5);
		LCD_WriteReg(0xD510,0x01);
		LCD_WriteReg(0xD511,0x13);
		LCD_WriteReg(0xD512,0x01);
		LCD_WriteReg(0xD513,0x54);
		LCD_WriteReg(0xD514,0x01);
		LCD_WriteReg(0xD515,0x82);
		LCD_WriteReg(0xD516,0x01);
		LCD_WriteReg(0xD517,0xCA);
		LCD_WriteReg(0xD518,0x02);
		LCD_WriteReg(0xD519,0x00);
		LCD_WriteReg(0xD51A,0x02);
		LCD_WriteReg(0xD51B,0x01);
		LCD_WriteReg(0xD51C,0x02);
		LCD_WriteReg(0xD51D,0x34);
		LCD_WriteReg(0xD51E,0x02);
		LCD_WriteReg(0xD51F,0x67);
		LCD_WriteReg(0xD520,0x02);
		LCD_WriteReg(0xD521,0x84);
		LCD_WriteReg(0xD522,0x02);
		LCD_WriteReg(0xD523,0xA4);
		LCD_WriteReg(0xD524,0x02);
		LCD_WriteReg(0xD525,0xB7);
		LCD_WriteReg(0xD526,0x02);
		LCD_WriteReg(0xD527,0xCF);
		LCD_WriteReg(0xD528,0x02);
		LCD_WriteReg(0xD529,0xDE);
		LCD_WriteReg(0xD52A,0x02);
		LCD_WriteReg(0xD52B,0xF2);
		LCD_WriteReg(0xD52C,0x02);
		LCD_WriteReg(0xD52D,0xFE);
		LCD_WriteReg(0xD52E,0x03);
		LCD_WriteReg(0xD52F,0x10);
		LCD_WriteReg(0xD530,0x03);
		LCD_WriteReg(0xD531,0x33);
		LCD_WriteReg(0xD532,0x03);
		LCD_WriteReg(0xD533,0x6D);
		LCD_WriteReg(0xD600,0x00);
		LCD_WriteReg(0xD601,0x33);
		LCD_WriteReg(0xD602,0x00);
		LCD_WriteReg(0xD603,0x34);
		LCD_WriteReg(0xD604,0x00);
		LCD_WriteReg(0xD605,0x3A);
		LCD_WriteReg(0xD606,0x00);
		LCD_WriteReg(0xD607,0x4A);
		LCD_WriteReg(0xD608,0x00);
		LCD_WriteReg(0xD609,0x5C);
		LCD_WriteReg(0xD60A,0x00);
		LCD_WriteReg(0xD60B,0x81);

		LCD_WriteReg(0xD60C,0x00);
		LCD_WriteReg(0xD60D,0xA6);
		LCD_WriteReg(0xD60E,0x00);
		LCD_WriteReg(0xD60F,0xE5);
		LCD_WriteReg(0xD610,0x01);
		LCD_WriteReg(0xD611,0x13);
		LCD_WriteReg(0xD612,0x01);
		LCD_WriteReg(0xD613,0x54);
		LCD_WriteReg(0xD614,0x01);
		LCD_WriteReg(0xD615,0x82);
		LCD_WriteReg(0xD616,0x01);
		LCD_WriteReg(0xD617,0xCA);
		LCD_WriteReg(0xD618,0x02);
		LCD_WriteReg(0xD619,0x00);
		LCD_WriteReg(0xD61A,0x02);
		LCD_WriteReg(0xD61B,0x01);
		LCD_WriteReg(0xD61C,0x02);
		LCD_WriteReg(0xD61D,0x34);
		LCD_WriteReg(0xD61E,0x02);
		LCD_WriteReg(0xD61F,0x67);
		LCD_WriteReg(0xD620,0x02);
		LCD_WriteReg(0xD621,0x84);
		LCD_WriteReg(0xD622,0x02);
		LCD_WriteReg(0xD623,0xA4);
		LCD_WriteReg(0xD624,0x02);
		LCD_WriteReg(0xD625,0xB7);
		LCD_WriteReg(0xD626,0x02);
		LCD_WriteReg(0xD627,0xCF);
		LCD_WriteReg(0xD628,0x02);
		LCD_WriteReg(0xD629,0xDE);
		LCD_WriteReg(0xD62A,0x02);
		LCD_WriteReg(0xD62B,0xF2);
		LCD_WriteReg(0xD62C,0x02);
		LCD_WriteReg(0xD62D,0xFE);
		LCD_WriteReg(0xD62E,0x03);
		LCD_WriteReg(0xD62F,0x10);
		LCD_WriteReg(0xD630,0x03);
		LCD_WriteReg(0xD631,0x33);
		LCD_WriteReg(0xD632,0x03);
		LCD_WriteReg(0xD633,0x6D);
		//LV2 Page 0 enable
		LCD_WriteReg(0xF000,0x55);
		LCD_WriteReg(0xF001,0xAA);
		LCD_WriteReg(0xF002,0x52);
		LCD_WriteReg(0xF003,0x08);
		LCD_WriteReg(0xF004,0x00);
		//Display control
		LCD_WriteReg(0xB100, 0xCC);
		LCD_WriteReg(0xB101, 0x00);
		//Source hold time
		LCD_WriteReg(0xB600,0x05);
		//Gate EQ control
		LCD_WriteReg(0xB700,0x70);
		LCD_WriteReg(0xB701,0x70);
		//Source EQ control (Mode 2)
		LCD_WriteReg(0xB800,0x01);
		LCD_WriteReg(0xB801,0x03);
		LCD_WriteReg(0xB802,0x03);
		LCD_WriteReg(0xB803,0x03);
		//Inversion mode (2-dot)
		LCD_WriteReg(0xBC00,0x02);
		LCD_WriteReg(0xBC01,0x00);
		LCD_WriteReg(0xBC02,0x00);
		//Timing control 4H w/ 4-delay
		LCD_WriteReg(0xC900,0xD0);
		LCD_WriteReg(0xC901,0x02);
		LCD_WriteReg(0xC902,0x50);
		LCD_WriteReg(0xC903,0x50);
		LCD_WriteReg(0xC904,0x50);
		LCD_WriteReg(0x3500,0x00);
		LCD_WriteReg(0x3A00,0x55);  //16-bit/pixel
		LCD_WR_REG(0x1100);
		delay_us(120);
		LCD_WR_REG(0x2900);
	}else if(lcddev.id==0x9325)//9325
	{
		LCD_WriteReg(0x00E5,0x78F0); 
		LCD_WriteReg(0x0001,0x0100); 
		LCD_WriteReg(0x0002,0x0700); 
		LCD_WriteReg(0x0003,0x1030); 
		LCD_WriteReg(0x0004,0x0000); 
		LCD_WriteReg(0x0008,0x0202);  
		LCD_WriteReg(0x0009,0x0000);
		LCD_WriteReg(0x000A,0x0000); 
		LCD_WriteReg(0x000C,0x0000); 
		LCD_WriteReg(0x000D,0x0000);
		LCD_WriteReg(0x000F,0x0000);
		//power on sequence VGHVGL
		LCD_WriteReg(0x0010,0x0000);   
		LCD_WriteReg(0x0011,0x0007);  
		LCD_WriteReg(0x0012,0x0000);  
		LCD_WriteReg(0x0013,0x0000); 
		LCD_WriteReg(0x0007,0x0000); 
		//vgh 
		LCD_WriteReg(0x0010,0x1690);   
		LCD_WriteReg(0x0011,0x0227);
		//delayms(100);
		//vregiout 
		LCD_WriteReg(0x0012,0x009D); //0x001b
		//delayms(100); 
		//vom amplitude
		LCD_WriteReg(0x0013,0x1900);
		//delayms(100); 
		//vom H
		LCD_WriteReg(0x0029,0x0025); 
		LCD_WriteReg(0x002B,0x000D); 
		//gamma
		LCD_WriteReg(0x0030,0x0007);
		LCD_WriteReg(0x0031,0x0303);
		LCD_WriteReg(0x0032,0x0003);// 0006
		LCD_WriteReg(0x0035,0x0206);
		LCD_WriteReg(0x0036,0x0008);
		LCD_WriteReg(0x0037,0x0406); 
		LCD_WriteReg(0x0038,0x0304);//0200
		LCD_WriteReg(0x0039,0x0007); 
		LCD_WriteReg(0x003C,0x0602);// 0504
		LCD_WriteReg(0x003D,0x0008); 
		//ram
		LCD_WriteReg(0x0050,0x0000); 
		LCD_WriteReg(0x0051,0x00EF);
		LCD_WriteReg(0x0052,0x0000); 
		LCD_WriteReg(0x0053,0x013F);  
		LCD_WriteReg(0x0060,0xA700); 
		LCD_WriteReg(0x0061,0x0001); 
		LCD_WriteReg(0x006A,0x0000); 
		//
		LCD_WriteReg(0x0080,0x0000); 
		LCD_WriteReg(0x0081,0x0000); 
		LCD_WriteReg(0x0082,0x0000); 
		LCD_WriteReg(0x0083,0x0000); 
		LCD_WriteReg(0x0084,0x0000); 
		LCD_WriteReg(0x0085,0x0000); 
		//
		LCD_WriteReg(0x0090,0x0010); 
		LCD_WriteReg(0x0092,0x0600); 
		
		LCD_WriteReg(0x0007,0x0133);
		LCD_WriteReg(0x00,0x0022);//
	}else if(lcddev.id==0x9328)//ILI9328   OK  
	{
  		LCD_WriteReg(0x00EC,0x108F);// internal timeing      
 		LCD_WriteReg(0x00EF,0x1234);// ADD        
		//LCD_WriteReg(0x00e7,0x0010);      
        //LCD_WriteReg(0x0000,0x0001);//ø™∆Ùƒ⁄≤ø ±÷”
        LCD_WriteReg(0x0001,0x0100);     
        LCD_WriteReg(0x0002,0x0700);//µÁ‘¥ø™∆Ù                    
		//LCD_WriteReg(0x0003,(1<<3)|(1<<4) ); 	//65K  RGB
		//DRIVE TABLE(ºƒ¥Ê∆˜ 03H)
		//BIT3=AM BIT4:5=ID0:1
		//AM ID0 ID1   FUNCATION
		// 0  0   0	   R->L D->U
		// 1  0   0	   D->U	R->L
		// 0  1   0	   L->R D->U
		// 1  1   0    D->U	L->R
		// 0  0   1	   R->L U->D
		// 1  0   1    U->D	R->L
		// 0  1   1    L->R U->D ’˝≥£æÕ”√’‚∏ˆ.
		// 1  1   1	   U->D	L->R
        LCD_WriteReg(0x0003,(1<<12)|(3<<4)|(0<<3) );//65K    
        LCD_WriteReg(0x0004,0x0000);                                   
        LCD_WriteReg(0x0008,0x0202);	           
        LCD_WriteReg(0x0009,0x0000);         
        LCD_WriteReg(0x000a,0x0000);//display setting         
        LCD_WriteReg(0x000c,0x0001);//display setting          
        LCD_WriteReg(0x000d,0x0000);//0f3c          
        LCD_WriteReg(0x000f,0x0000);
		//µÁ‘¥≈‰÷√
        LCD_WriteReg(0x0010,0x0000);   
        LCD_WriteReg(0x0011,0x0007);
        LCD_WriteReg(0x0012,0x0000);                                                                 
        LCD_WriteReg(0x0013,0x0000);                 
     	LCD_WriteReg(0x0007,0x0001);                 
       	delay_ms(50); 
        LCD_WriteReg(0x0010,0x1490);   
        LCD_WriteReg(0x0011,0x0227);
        delay_ms(50); 
        LCD_WriteReg(0x0012,0x008A);                  
        delay_ms(50); 
        LCD_WriteReg(0x0013,0x1a00);   
        LCD_WriteReg(0x0029,0x0006);
        LCD_WriteReg(0x002b,0x000d);
        delay_ms(50); 
        LCD_WriteReg(0x0020,0x0000);                                                            
        LCD_WriteReg(0x0021,0x0000);           
		delay_ms(50); 
		//Ÿ§¬Ì–£’˝
        LCD_WriteReg(0x0030,0x0000); 
        LCD_WriteReg(0x0031,0x0604);   
        LCD_WriteReg(0x0032,0x0305);
        LCD_WriteReg(0x0035,0x0000);
        LCD_WriteReg(0x0036,0x0C09); 
        LCD_WriteReg(0x0037,0x0204);
        LCD_WriteReg(0x0038,0x0301);        
        LCD_WriteReg(0x0039,0x0707);     
        LCD_WriteReg(0x003c,0x0000);
        LCD_WriteReg(0x003d,0x0a0a);
        delay_ms(50); 
        LCD_WriteReg(0x0050,0x0000); //ÀÆ∆ΩGRAM∆ ºŒª÷√ 
        LCD_WriteReg(0x0051,0x00ef); //ÀÆ∆ΩGRAM÷’÷πŒª÷√                    
        LCD_WriteReg(0x0052,0x0000); //¥π÷±GRAM∆ ºŒª÷√                    
        LCD_WriteReg(0x0053,0x013f); //¥π÷±GRAM÷’÷πŒª÷√  
 
         LCD_WriteReg(0x0060,0xa700);        
        LCD_WriteReg(0x0061,0x0001); 
        LCD_WriteReg(0x006a,0x0000);
        LCD_WriteReg(0x0080,0x0000);
        LCD_WriteReg(0x0081,0x0000);
        LCD_WriteReg(0x0082,0x0000);
        LCD_WriteReg(0x0083,0x0000);
        LCD_WriteReg(0x0084,0x0000);
        LCD_WriteReg(0x0085,0x0000);
      
        LCD_WriteReg(0x0090,0x0010);     
        LCD_WriteReg(0x0092,0x0600);  
        //ø™∆Ùœ‘ æ…Ë÷√    
        LCD_WriteReg(0x0007,0x0133); 
	}else if(lcddev.id==0x9320)//≤‚ ‘OK.
	{
		LCD_WriteReg(0x00,0x0000);
		LCD_WriteReg(0x01,0x0100);	//Driver Output Contral.
		LCD_WriteReg(0x02,0x0700);	//LCD Driver Waveform Contral.
		LCD_WriteReg(0x03,0x1030);//Entry Mode Set.
		//LCD_WriteReg(0x03,0x1018);	//Entry Mode Set.
	
		LCD_WriteReg(0x04,0x0000);	//Scalling Contral.
		LCD_WriteReg(0x08,0x0202);	//Display Contral 2.(0x0207)
		LCD_WriteReg(0x09,0x0000);	//Display Contral 3.(0x0000)
		LCD_WriteReg(0x0a,0x0000);	//Frame Cycle Contal.(0x0000)
		LCD_WriteReg(0x0c,(1<<0));	//Extern Display Interface Contral 1.(0x0000)
		LCD_WriteReg(0x0d,0x0000);	//Frame Maker Position.
		LCD_WriteReg(0x0f,0x0000);	//Extern Display Interface Contral 2.	    
		delay_ms(50); 
		LCD_WriteReg(0x07,0x0101);	//Display Contral.
		delay_ms(50); 								  
		LCD_WriteReg(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4));	//Power Control 1.(0x16b0)
		LCD_WriteReg(0x11,0x0007);								//Power Control 2.(0x0001)
		LCD_WriteReg(0x12,(1<<8)|(1<<4)|(0<<0));				//Power Control 3.(0x0138)
		LCD_WriteReg(0x13,0x0b00);								//Power Control 4.
		LCD_WriteReg(0x29,0x0000);								//Power Control 7.
	
		LCD_WriteReg(0x2b,(1<<14)|(1<<4));	    
		LCD_WriteReg(0x50,0);	//Set X Star
		//ÀÆ∆ΩGRAM÷’÷πŒª÷√Set X End.
		LCD_WriteReg(0x51,239);	//Set Y Star
		LCD_WriteReg(0x52,0);	//Set Y End.t.
		LCD_WriteReg(0x53,319);	//
	
		LCD_WriteReg(0x60,0x2700);	//Driver Output Control.
		LCD_WriteReg(0x61,0x0001);	//Driver Output Control.
		LCD_WriteReg(0x6a,0x0000);	//Vertical Srcoll Control.
	
		LCD_WriteReg(0x80,0x0000);	//Display Position? Partial Display 1.
		LCD_WriteReg(0x81,0x0000);	//RAM Address Start? Partial Display 1.
		LCD_WriteReg(0x82,0x0000);	//RAM Address End-Partial Display 1.
		LCD_WriteReg(0x83,0x0000);	//Displsy Position? Partial Display 2.
		LCD_WriteReg(0x84,0x0000);	//RAM Address Start? Partial Display 2.
		LCD_WriteReg(0x85,0x0000);	//RAM Address End? Partial Display 2.
	
		LCD_WriteReg(0x90,(0<<7)|(16<<0));	//Frame Cycle Contral.(0x0013)
		LCD_WriteReg(0x92,0x0000);	//Panel Interface Contral 2.(0x0000)
		LCD_WriteReg(0x93,0x0001);	//Panel Interface Contral 3.
		LCD_WriteReg(0x95,0x0110);	//Frame Cycle Contral.(0x0110)
		LCD_WriteReg(0x97,(0<<8));	//
		LCD_WriteReg(0x98,0x0000);	//Frame Cycle Contral.	   
		LCD_WriteReg(0x07,0x0173);	//(0x0173)
	}else if(lcddev.id==0X9331)//OK |/|/|			 
	{
		LCD_WriteReg(0x00E7, 0x1014);
		LCD_WriteReg(0x0001, 0x0100); // set SS and SM bit
		LCD_WriteReg(0x0002, 0x0200); // set 1 line inversion
        LCD_WriteReg(0x0003,(1<<12)|(3<<4)|(1<<3));//65K    
		//LCD_WriteReg(0x0003, 0x1030); // set GRAM write direction and BGR=1.
		LCD_WriteReg(0x0008, 0x0202); // set the back porch and front porch
		LCD_WriteReg(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
		LCD_WriteReg(0x000A, 0x0000); // FMARK function
		LCD_WriteReg(0x000C, 0x0000); // RGB interface setting
		LCD_WriteReg(0x000D, 0x0000); // Frame marker Position
		LCD_WriteReg(0x000F, 0x0000); // RGB interface polarity
		//*************Power On sequence ****************//
		LCD_WriteReg(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
		LCD_WriteReg(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
		LCD_WriteReg(0x0012, 0x0000); // VREG1OUT voltage
		LCD_WriteReg(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
		delay_ms(200); // Dis-charge capacitor power voltage
		LCD_WriteReg(0x0010, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB
		LCD_WriteReg(0x0011, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
		delay_ms(50); // Delay 50ms
		LCD_WriteReg(0x0012, 0x000C); // Internal reference voltage= Vci;
		delay_ms(50); // Delay 50ms
		LCD_WriteReg(0x0013, 0x0800); // Set VDV[4:0] for VCOM amplitude
		LCD_WriteReg(0x0029, 0x0011); // Set VCM[5:0] for VCOMH
		LCD_WriteReg(0x002B, 0x000B); // Set Frame Rate
		delay_ms(50); // Delay 50ms
		LCD_WriteReg(0x0020, 0x0000); // GRAM horizontal Address
		LCD_WriteReg(0x0021, 0x013f); // GRAM Vertical Address
		// ----------- Adjust the Gamma Curve ----------//
		LCD_WriteReg(0x0030, 0x0000);
		LCD_WriteReg(0x0031, 0x0106);
		LCD_WriteReg(0x0032, 0x0000);
		LCD_WriteReg(0x0035, 0x0204);
		LCD_WriteReg(0x0036, 0x160A);
		LCD_WriteReg(0x0037, 0x0707);
		LCD_WriteReg(0x0038, 0x0106);
		LCD_WriteReg(0x0039, 0x0707);
		LCD_WriteReg(0x003C, 0x0402);
		LCD_WriteReg(0x003D, 0x0C0F);
		//------------------ Set GRAM area ---------------//
		LCD_WriteReg(0x0050, 0x0000); // Horizontal GRAM Start Address
		LCD_WriteReg(0x0051, 0x00EF); // Horizontal GRAM End Address
		LCD_WriteReg(0x0052, 0x0000); // Vertical GRAM Start Address
		LCD_WriteReg(0x0053, 0x013F); // Vertical GRAM Start Address
		LCD_WriteReg(0x0060, 0x2700); // Gate Scan Line
		LCD_WriteReg(0x0061, 0x0001); // NDL,VLE, REV 
		LCD_WriteReg(0x006A, 0x0000); // set scrolling line
		//-------------- Partial Display Control ---------//
		LCD_WriteReg(0x0080, 0x0000);
		LCD_WriteReg(0x0081, 0x0000);
		LCD_WriteReg(0x0082, 0x0000);
		LCD_WriteReg(0x0083, 0x0000);
		LCD_WriteReg(0x0084, 0x0000);
		LCD_WriteReg(0x0085, 0x0000);
		//-------------- Panel Control -------------------//
		LCD_WriteReg(0x0090, 0x0010);
		LCD_WriteReg(0x0092, 0x0600);
		LCD_WriteReg(0x0007, 0x0133); // 262K color and display ON
	}else if(lcddev.id==0x5408)
	{
		LCD_WriteReg(0x01,0x0100);								  
		LCD_WriteReg(0x02,0x0700);//LCD Driving Waveform Contral 
		LCD_WriteReg(0x03,0x1030);//Entry Mode…Ë÷√ 	   
		//÷∏’Î¥”◊Û÷¡”“◊‘…œ∂¯œ¬µƒ◊‘∂Ø‘ˆƒ£ Ω
		//Normal Mode(Window Mode disable)
		//RGB∏Ò Ω
		//16Œª ˝æ›2¥Œ¥´ ‰µƒ8◊‹œﬂ…Ë÷√
		LCD_WriteReg(0x04,0x0000); //Scalling Control register     
		LCD_WriteReg(0x08,0x0207); //Display Control 2 
		LCD_WriteReg(0x09,0x0000); //Display Control 3	 
		LCD_WriteReg(0x0A,0x0000); //Frame Cycle Control	 
		LCD_WriteReg(0x0C,0x0000); //External Display Interface Control 1 
		LCD_WriteReg(0x0D,0x0000); //Frame Maker Position		 
		LCD_WriteReg(0x0F,0x0000); //External Display Interface Control 2 
 		delay_ms(20);
		//TFT “∫æß≤ …´ÕºœÒœ‘ æ∑Ω∑®14
		LCD_WriteReg(0x10,0x16B0); //0x14B0 //Power Control 1
		LCD_WriteReg(0x11,0x0001); //0x0007 //Power Control 2
		LCD_WriteReg(0x17,0x0001); //0x0000 //Power Control 3
		LCD_WriteReg(0x12,0x0138); //0x013B //Power Control 4
		LCD_WriteReg(0x13,0x0800); //0x0800 //Power Control 5
		LCD_WriteReg(0x29,0x0009); //NVM read data 2
		LCD_WriteReg(0x2a,0x0009); //NVM read data 3
		LCD_WriteReg(0xa4,0x0000);	 
		LCD_WriteReg(0x50,0x0000); //…Ë÷√≤Ÿ◊˜¥∞ø⁄µƒX÷·ø™ º¡–
		LCD_WriteReg(0x51,0x00EF); //…Ë÷√≤Ÿ◊˜¥∞ø⁄µƒX÷·Ω· ¯¡–
		LCD_WriteReg(0x52,0x0000); //…Ë÷√≤Ÿ◊˜¥∞ø⁄µƒY÷·ø™ º––
		LCD_WriteReg(0x53,0x013F); //…Ë÷√≤Ÿ◊˜¥∞ø⁄µƒY÷·Ω· ¯––
		LCD_WriteReg(0x60,0x2700); //Driver Output Control
		//…Ë÷√∆¡ƒªµƒµ„ ˝“‘º∞…®√Ëµƒ∆ º––
		LCD_WriteReg(0x61,0x0001); //Driver Output Control
		LCD_WriteReg(0x6A,0x0000); //Vertical Scroll Control
		LCD_WriteReg(0x80,0x0000); //Display Position ®C Partial Display 1
		LCD_WriteReg(0x81,0x0000); //RAM Address Start ®C Partial Display 1
		LCD_WriteReg(0x82,0x0000); //RAM address End - Partial Display 1
		LCD_WriteReg(0x83,0x0000); //Display Position ®C Partial Display 2
		LCD_WriteReg(0x84,0x0000); //RAM Address Start ®C Partial Display 2
		LCD_WriteReg(0x85,0x0000); //RAM address End ®C Partail Display2
		LCD_WriteReg(0x90,0x0013); //Frame Cycle Control
		LCD_WriteReg(0x92,0x0000);  //Panel Interface Control 2
		LCD_WriteReg(0x93,0x0003); //Panel Interface control 3
		LCD_WriteReg(0x95,0x0110);  //Frame Cycle Control
		LCD_WriteReg(0x07,0x0173);		 
		delay_ms(50);
	}	
	else if(lcddev.id==0x1505)//OK
	{
		// second release on 3/5  ,luminance is acceptable,water wave appear during camera preview
        LCD_WriteReg(0x0007,0x0000);
        delay_ms(50); 
        LCD_WriteReg(0x0012,0x011C);//0x011A   why need to set several times?
        LCD_WriteReg(0x00A4,0x0001);//NVM	 
        LCD_WriteReg(0x0008,0x000F);
        LCD_WriteReg(0x000A,0x0008);
        LCD_WriteReg(0x000D,0x0008);	    
  		//Ÿ§¬Ì–£’˝
        LCD_WriteReg(0x0030,0x0707);
        LCD_WriteReg(0x0031,0x0007); //0x0707
        LCD_WriteReg(0x0032,0x0603); 
        LCD_WriteReg(0x0033,0x0700); 
        LCD_WriteReg(0x0034,0x0202); 
        LCD_WriteReg(0x0035,0x0002); //?0x0606
        LCD_WriteReg(0x0036,0x1F0F);
        LCD_WriteReg(0x0037,0x0707); //0x0f0f  0x0105
        LCD_WriteReg(0x0038,0x0000); 
        LCD_WriteReg(0x0039,0x0000); 
        LCD_WriteReg(0x003A,0x0707); 
        LCD_WriteReg(0x003B,0x0000); //0x0303
        LCD_WriteReg(0x003C,0x0007); //?0x0707
        LCD_WriteReg(0x003D,0x0000); //0x1313//0x1f08
        delay_ms(50); 
        LCD_WriteReg(0x0007,0x0001);
        LCD_WriteReg(0x0017,0x0001);//ø™∆ÙµÁ‘¥
        delay_ms(50); 
  		//µÁ‘¥≈‰÷√
        LCD_WriteReg(0x0010,0x17A0); 
        LCD_WriteReg(0x0011,0x0217);//reference voltage VC[2:0]   Vciout = 1.00*Vcivl
        LCD_WriteReg(0x0012,0x011E);//0x011c  //Vreg1out = Vcilvl*1.80   is it the same as Vgama1out ?
        LCD_WriteReg(0x0013,0x0F00);//VDV[4:0]-->VCOM Amplitude VcomL = VcomH - Vcom Ampl
        LCD_WriteReg(0x002A,0x0000);  
        LCD_WriteReg(0x0029,0x000A);//0x0001F  Vcomh = VCM1[4:0]*Vreg1out    gate source voltage??
        LCD_WriteReg(0x0012,0x013E);// 0x013C  power supply on
        //Coordinates Control//
        LCD_WriteReg(0x0050,0x0000);//0x0e00
        LCD_WriteReg(0x0051,0x00EF); 
        LCD_WriteReg(0x0052,0x0000); 
        LCD_WriteReg(0x0053,0x013F); 
    	//Pannel Image Control//
        LCD_WriteReg(0x0060,0x2700); 
        LCD_WriteReg(0x0061,0x0001); 
        LCD_WriteReg(0x006A,0x0000); 
        LCD_WriteReg(0x0080,0x0000); 
    	//Partial Image Control//
        LCD_WriteReg(0x0081,0x0000); 
        LCD_WriteReg(0x0082,0x0000); 
        LCD_WriteReg(0x0083,0x0000); 
        LCD_WriteReg(0x0084,0x0000); 
        LCD_WriteReg(0x0085,0x0000); 
  		//Panel Interface Control//
        LCD_WriteReg(0x0090,0x0013);//0x0010 frenqucy
        LCD_WriteReg(0x0092,0x0300); 
        LCD_WriteReg(0x0093,0x0005); 
        LCD_WriteReg(0x0095,0x0000); 
        LCD_WriteReg(0x0097,0x0000); 
        LCD_WriteReg(0x0098,0x0000); 
  
        LCD_WriteReg(0x0001,0x0100); 
        LCD_WriteReg(0x0002,0x0700); 
        LCD_WriteReg(0x0003,0x1038);//…®√Ë∑ΩœÚ …œ->œ¬  ◊Û->”“ 
        LCD_WriteReg(0x0004,0x0000); 
        LCD_WriteReg(0x000C,0x0000); 
        LCD_WriteReg(0x000F,0x0000); 
        LCD_WriteReg(0x0020,0x0000); 
        LCD_WriteReg(0x0021,0x0000); 
        LCD_WriteReg(0x0007,0x0021); 
        delay_ms(20);
        LCD_WriteReg(0x0007,0x0061); 
        delay_ms(20);
        LCD_WriteReg(0x0007,0x0173); 
        delay_ms(20);
	}else if(lcddev.id==0xB505)
	{
		LCD_WriteReg(0x0000,0x0000);
		LCD_WriteReg(0x0000,0x0000);
		LCD_WriteReg(0x0000,0x0000);
		LCD_WriteReg(0x0000,0x0000);
		
		LCD_WriteReg(0x00a4,0x0001);
		delay_ms(20);		  
		LCD_WriteReg(0x0060,0x2700);
		LCD_WriteReg(0x0008,0x0202);
		
		LCD_WriteReg(0x0030,0x0214);
		LCD_WriteReg(0x0031,0x3715);
		LCD_WriteReg(0x0032,0x0604);
		LCD_WriteReg(0x0033,0x0e16);
		LCD_WriteReg(0x0034,0x2211);
		LCD_WriteReg(0x0035,0x1500);
		LCD_WriteReg(0x0036,0x8507);
		LCD_WriteReg(0x0037,0x1407);
		LCD_WriteReg(0x0038,0x1403);
		LCD_WriteReg(0x0039,0x0020);
		
		LCD_WriteReg(0x0090,0x001a);
		LCD_WriteReg(0x0010,0x0000);
		LCD_WriteReg(0x0011,0x0007);
		LCD_WriteReg(0x0012,0x0000);
		LCD_WriteReg(0x0013,0x0000);
		delay_ms(20);
		
		LCD_WriteReg(0x0010,0x0730);
		LCD_WriteReg(0x0011,0x0137);
		delay_ms(20);
		
		LCD_WriteReg(0x0012,0x01b8);
		delay_ms(20);
		
		LCD_WriteReg(0x0013,0x0f00);
		LCD_WriteReg(0x002a,0x0080);
		LCD_WriteReg(0x0029,0x0048);
		delay_ms(20);
		
		LCD_WriteReg(0x0001,0x0100);
		LCD_WriteReg(0x0002,0x0700);
        LCD_WriteReg(0x0003,0x1038);//…®√Ë∑ΩœÚ …œ->œ¬  ◊Û->”“ 
		LCD_WriteReg(0x0008,0x0202);
		LCD_WriteReg(0x000a,0x0000);
		LCD_WriteReg(0x000c,0x0000);
		LCD_WriteReg(0x000d,0x0000);
		LCD_WriteReg(0x000e,0x0030);
		LCD_WriteReg(0x0050,0x0000);
		LCD_WriteReg(0x0051,0x00ef);
		LCD_WriteReg(0x0052,0x0000);
		LCD_WriteReg(0x0053,0x013f);
		LCD_WriteReg(0x0060,0x2700);
		LCD_WriteReg(0x0061,0x0001);
		LCD_WriteReg(0x006a,0x0000);
		//LCD_WriteReg(0x0080,0x0000);
		//LCD_WriteReg(0x0081,0x0000);
		LCD_WriteReg(0x0090,0X0011);
		LCD_WriteReg(0x0092,0x0600);
		LCD_WriteReg(0x0093,0x0402);
		LCD_WriteReg(0x0094,0x0002);
		delay_ms(20);
		
		LCD_WriteReg(0x0007,0x0001);
		delay_ms(20);
		LCD_WriteReg(0x0007,0x0061);
		LCD_WriteReg(0x0007,0x0173);
		
		LCD_WriteReg(0x0020,0x0000);
		LCD_WriteReg(0x0021,0x0000);	  
		LCD_WriteReg(0x00,0x22);  
	}else if(lcddev.id==0xC505)
	{
		LCD_WriteReg(0x0000,0x0000);
		LCD_WriteReg(0x0000,0x0000);
		delay_ms(20);		  
		LCD_WriteReg(0x0000,0x0000);
		LCD_WriteReg(0x0000,0x0000);
		LCD_WriteReg(0x0000,0x0000);
		LCD_WriteReg(0x0000,0x0000);
 		LCD_WriteReg(0x00a4,0x0001);
		delay_ms(20);		  
		LCD_WriteReg(0x0060,0x2700);
		LCD_WriteReg(0x0008,0x0806);
		
		LCD_WriteReg(0x0030,0x0703);//gamma setting
		LCD_WriteReg(0x0031,0x0001);
		LCD_WriteReg(0x0032,0x0004);
		LCD_WriteReg(0x0033,0x0102);
		LCD_WriteReg(0x0034,0x0300);
		LCD_WriteReg(0x0035,0x0103);
		LCD_WriteReg(0x0036,0x001F);
		LCD_WriteReg(0x0037,0x0703);
		LCD_WriteReg(0x0038,0x0001);
		LCD_WriteReg(0x0039,0x0004);
		
		
		
		LCD_WriteReg(0x0090, 0x0015);	//80Hz
		LCD_WriteReg(0x0010, 0X0410);	//BT,AP
		LCD_WriteReg(0x0011,0x0247);	//DC1,DC0,VC
		LCD_WriteReg(0x0012, 0x01BC);
		LCD_WriteReg(0x0013, 0x0e00);
		delay_ms(120);
		LCD_WriteReg(0x0001, 0x0100);
		LCD_WriteReg(0x0002, 0x0200);
		LCD_WriteReg(0x0003, 0x1030);
		
		LCD_WriteReg(0x000A, 0x0008);
		LCD_WriteReg(0x000C, 0x0000);
		
		LCD_WriteReg(0x000E, 0x0020);
		LCD_WriteReg(0x000F, 0x0000);
		LCD_WriteReg(0x0020, 0x0000);	//H Start
		LCD_WriteReg(0x0021, 0x0000);	//V Start
		LCD_WriteReg(0x002A,0x003D);	//vcom2
		delay_ms(20);
		LCD_WriteReg(0x0029, 0x002d);
		LCD_WriteReg(0x0050, 0x0000);
		LCD_WriteReg(0x0051, 0xD0EF);
		LCD_WriteReg(0x0052, 0x0000);
		LCD_WriteReg(0x0053, 0x013F);
		LCD_WriteReg(0x0061, 0x0000);
		LCD_WriteReg(0x006A, 0x0000);
		LCD_WriteReg(0x0092,0x0300); 
 
 		LCD_WriteReg(0x0093, 0x0005);
		LCD_WriteReg(0x0007, 0x0100);
	}else if(lcddev.id==0x4531)//OK |/|/|
	{
		LCD_WriteReg(0X00,0X0001);   
		delay_ms(10);   
		LCD_WriteReg(0X10,0X1628);   
		LCD_WriteReg(0X12,0X000e);//0x0006    
		LCD_WriteReg(0X13,0X0A39);   
		delay_ms(10);   
		LCD_WriteReg(0X11,0X0040);   
		LCD_WriteReg(0X15,0X0050);   
		delay_ms(10);   
		LCD_WriteReg(0X12,0X001e);//16    
		delay_ms(10);   
		LCD_WriteReg(0X10,0X1620);   
		LCD_WriteReg(0X13,0X2A39);   
		delay_ms(10);   
		LCD_WriteReg(0X01,0X0100);   
		LCD_WriteReg(0X02,0X0300);   
		LCD_WriteReg(0X03,0X1038);//∏ƒ±‰∑ΩœÚµƒ   
		LCD_WriteReg(0X08,0X0202);   
		LCD_WriteReg(0X0A,0X0008);   
		LCD_WriteReg(0X30,0X0000);   
		LCD_WriteReg(0X31,0X0402);   
		LCD_WriteReg(0X32,0X0106);   
		LCD_WriteReg(0X33,0X0503);   
		LCD_WriteReg(0X34,0X0104);   
		LCD_WriteReg(0X35,0X0301);   
		LCD_WriteReg(0X36,0X0707);   
		LCD_WriteReg(0X37,0X0305);   
		LCD_WriteReg(0X38,0X0208);   
		LCD_WriteReg(0X39,0X0F0B);   
		LCD_WriteReg(0X41,0X0002);   
		LCD_WriteReg(0X60,0X2700);   
		LCD_WriteReg(0X61,0X0001);   
		LCD_WriteReg(0X90,0X0210);   
		LCD_WriteReg(0X92,0X010A);   
		LCD_WriteReg(0X93,0X0004);   
		LCD_WriteReg(0XA0,0X0100);   
		LCD_WriteReg(0X07,0X0001);   
		LCD_WriteReg(0X07,0X0021);   
		LCD_WriteReg(0X07,0X0023);   
		LCD_WriteReg(0X07,0X0033);   
		LCD_WriteReg(0X07,0X0133);   
		LCD_WriteReg(0XA0,0X0000); 
	}else if(lcddev.id==0x4535)
	{			      
		LCD_WriteReg(0X15,0X0030);   
		LCD_WriteReg(0X9A,0X0010);   
 		LCD_WriteReg(0X11,0X0020);   
 		LCD_WriteReg(0X10,0X3428);   
		LCD_WriteReg(0X12,0X0002);//16    
 		LCD_WriteReg(0X13,0X1038);   
		delay_ms(40);   
		LCD_WriteReg(0X12,0X0012);//16    
		delay_ms(40);   
  		LCD_WriteReg(0X10,0X3420);   
 		LCD_WriteReg(0X13,0X3038);   
		delay_ms(70);   
		LCD_WriteReg(0X30,0X0000);   
		LCD_WriteReg(0X31,0X0402);   
		LCD_WriteReg(0X32,0X0307);   
		LCD_WriteReg(0X33,0X0304);   
		LCD_WriteReg(0X34,0X0004);   
		LCD_WriteReg(0X35,0X0401);   
		LCD_WriteReg(0X36,0X0707);   
		LCD_WriteReg(0X37,0X0305);   
		LCD_WriteReg(0X38,0X0610);   
		LCD_WriteReg(0X39,0X0610); 
		  
		LCD_WriteReg(0X01,0X0100);   
		LCD_WriteReg(0X02,0X0300);   
		LCD_WriteReg(0X03,0X1030);//∏ƒ±‰∑ΩœÚµƒ   
		LCD_WriteReg(0X08,0X0808);   
		LCD_WriteReg(0X0A,0X0008);   
 		LCD_WriteReg(0X60,0X2700);   
		LCD_WriteReg(0X61,0X0001);   
		LCD_WriteReg(0X90,0X013E);   
		LCD_WriteReg(0X92,0X0100);   
		LCD_WriteReg(0X93,0X0100);   
 		LCD_WriteReg(0XA0,0X3000);   
 		LCD_WriteReg(0XA3,0X0010);   
		LCD_WriteReg(0X07,0X0001);   
		LCD_WriteReg(0X07,0X0021);   
		LCD_WriteReg(0X07,0X0023);   
		LCD_WriteReg(0X07,0X0033);   
		LCD_WriteReg(0X07,0X0133);   
	}else if(lcddev.id==0X1963)
	{
		LCD_WR_REG(0xE2);		//Set PLL with OSC = 10MHz (hardware),	Multiplier N = 35, 250MHz < VCO < 800MHz = OSC*(N+1), VCO = 300MHz
		LCD_WR_DATA(0x1D);		//≤Œ ˝1 
		LCD_WR_DATA(0x02);		//≤Œ ˝2 Divider M = 2, PLL = 300/(M+1) = 100MHz
		LCD_WR_DATA(0x04);		//≤Œ ˝3 Validate M and N values   
		delay_us(100);
		LCD_WR_REG(0xE0);		// Start PLL command
		LCD_WR_DATA(0x01);		// enable PLL
		delay_ms(10);
		LCD_WR_REG(0xE0);		// Start PLL command again
		LCD_WR_DATA(0x03);		// now, use PLL output as system clock	
		delay_ms(12);  
		LCD_WR_REG(0x01);		//»Ì∏¥Œª
		delay_ms(10);
		
		LCD_WR_REG(0xE6);		//…Ë÷√œÒÀÿ∆µ¬ ,33Mhz
		LCD_WR_DATA(0x2F);
		LCD_WR_DATA(0xFF);
		LCD_WR_DATA(0xFF);
		
		LCD_WR_REG(0xB0);		//…Ë÷√LCDƒ£ Ω
		LCD_WR_DATA(0x20);		//24Œªƒ£ Ω
		LCD_WR_DATA(0x00);		//TFT ƒ£ Ω 
	
		LCD_WR_DATA((SSD_HOR_RESOLUTION-1)>>8);//…Ë÷√LCDÀÆ∆ΩœÒÀÿ
		LCD_WR_DATA(SSD_HOR_RESOLUTION-1);		 
		LCD_WR_DATA((SSD_VER_RESOLUTION-1)>>8);//…Ë÷√LCD¥π÷±œÒÀÿ
		LCD_WR_DATA(SSD_VER_RESOLUTION-1);		 
		LCD_WR_DATA(0x00);		//RGB–Ú¡– 
		
		LCD_WR_REG(0xB4);		//Set horizontal period
		LCD_WR_DATA((SSD_HT-1)>>8);
		LCD_WR_DATA(SSD_HT-1);
		LCD_WR_DATA(SSD_HPS>>8);
		LCD_WR_DATA(SSD_HPS);
		LCD_WR_DATA(SSD_HOR_PULSE_WIDTH-1);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_REG(0xB6);		//Set vertical period
		LCD_WR_DATA((SSD_VT-1)>>8);
		LCD_WR_DATA(SSD_VT-1);
		LCD_WR_DATA(SSD_VPS>>8);
		LCD_WR_DATA(SSD_VPS);
		LCD_WR_DATA(SSD_VER_FRONT_PORCH-1);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		
		LCD_WR_REG(0xF0);	//…Ë÷√SSD1963”ÎCPUΩ”ø⁄Œ™16bit  
		LCD_WR_DATA(0x03);	//16-bit(565 format) data for 16bpp 

		LCD_WR_REG(0x29);	//ø™∆Ùœ‘ æ
		//…Ë÷√PWM ‰≥ˆ  ±≥π‚Õ®π˝’ºø’±»ø…µ˜ 
		LCD_WR_REG(0xD0);	//…Ë÷√◊‘∂Ø∞◊∆Ω∫‚DBC
		LCD_WR_DATA(0x00);	//disable
	
		LCD_WR_REG(0xBE);	//≈‰÷√PWM ‰≥ˆ
		LCD_WR_DATA(0x05);	//1…Ë÷√PWM∆µ¬ 
		LCD_WR_DATA(0xFE);	//2…Ë÷√PWM’ºø’±»
		LCD_WR_DATA(0x01);	//3…Ë÷√C
		LCD_WR_DATA(0x00);	//4…Ë÷√D
		LCD_WR_DATA(0x00);	//5…Ë÷√E 
		LCD_WR_DATA(0x00);	//6…Ë÷√F 
		
		LCD_WR_REG(0xB8);	//…Ë÷√GPIO≈‰÷√
		LCD_WR_DATA(0x03);	//2∏ˆIOø⁄…Ë÷√≥… ‰≥ˆ
		LCD_WR_DATA(0x01);	//GPIO π”√’˝≥£µƒIOπ¶ƒ‹ 
		LCD_WR_REG(0xBA);
		LCD_WR_DATA(0X01);	//GPIO[1:0]=01,øÿ÷∆LCD∑ΩœÚ
		
		LCD_SSD_BackLightSet(100);//±≥π‚…Ë÷√Œ™◊Ó¡¡
	}		 
	LCD_Display_Dir(0);		//ƒ¨»œŒ™ ˙∆¡
	LCD_LED=1;				//µ„¡¡±≥π‚
	LCD_Clear(WHITE);
}  
//«Â∆¡∫Ø ˝
//color:“™«Â∆¡µƒÃÓ≥‰…´
void LCD_Clear(u16 color)
{
	u32 index=0;      
	u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			//µ√µΩ◊‹µ„ ˝
	if((lcddev.id==0X6804)&&(lcddev.dir==1))//6804∫·∆¡µƒ ±∫ÚÃÿ ‚¥¶¿Ì  
	{						    
 		lcddev.dir=0;	 
 		lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  	 			
		LCD_SetCursor(0x00,0x0000);		//…Ë÷√π‚±ÍŒª÷√  
 		lcddev.dir=1;	 
  		lcddev.setxcmd=0X2B;
		lcddev.setycmd=0X2A;  	 
 	}else LCD_SetCursor(0x00,0x0000);	//…Ë÷√π‚±ÍŒª÷√ 
	LCD_WriteRAM_Prepare();     		//ø™ º–¥»ÎGRAM	 	  
	for(index=0;index<totalpoint;index++)
	{
		LCD->LCD_RAM=color;	
	}
}  
//‘⁄÷∏∂®«¯”Úƒ⁄ÃÓ≥‰µ•∏ˆ—’…´
//(sx,sy),(ex,ey):ÃÓ≥‰æÿ–Œ∂‘Ω«◊¯±Í,«¯”Ú¥Û–°Œ™:(ex-sx+1)*(ey-sy+1)   
//color:“™ÃÓ≥‰µƒ—’…´
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
	u16 temp;
	if((lcddev.id==0X6804)&&(lcddev.dir==1))	//6804∫·∆¡µƒ ±∫ÚÃÿ ‚¥¶¿Ì  
	{
		temp=sx;
		sx=sy;
		sy=lcddev.width-ex-1;	  
		ex=ey;
		ey=lcddev.width-temp-1;
 		lcddev.dir=0;	 
 		lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  	 			
		LCD_Fill(sx,sy,ex,ey,color);  
 		lcddev.dir=1;	 
  		lcddev.setxcmd=0X2B;
		lcddev.setycmd=0X2A;  	 
 	}else
	{
		xlen=ex-sx+1;	 
		for(i=sy;i<=ey;i++)
		{
		 	LCD_SetCursor(sx,i);      				//…Ë÷√π‚±ÍŒª÷√ 
			LCD_WriteRAM_Prepare();     			//ø™ º–¥»ÎGRAM	  
			for(j=0;j<xlen;j++)LCD->LCD_RAM=color;	//œ‘ æ—’…´ 	    
		}
	}	 
}  
//‘⁄÷∏∂®«¯”Úƒ⁄ÃÓ≥‰÷∏∂®—’…´øÈ			 
//(sx,sy),(ex,ey):ÃÓ≥‰æÿ–Œ∂‘Ω«◊¯±Í,«¯”Ú¥Û–°Œ™:(ex-sx+1)*(ey-sy+1)   
//color:“™ÃÓ≥‰µƒ—’…´
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 			//µ√µΩÃÓ≥‰µƒøÌ∂»
	height=ey-sy+1;			//∏ﬂ∂»
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//…Ë÷√π‚±ÍŒª÷√ 
		LCD_WriteRAM_Prepare();     //ø™ º–¥»ÎGRAM
		for(j=0;j<width;j++)LCD->LCD_RAM=color[i*width+j];//–¥»Î ˝æ› 
	}		  
}  
//ª≠œﬂ
//x1,y1:∆µ„◊¯±Í
//x2,y2:÷’µ„◊¯±Í  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //º∆À„◊¯±Í‘ˆ¡ø 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //…Ë÷√µ•≤Ω∑ΩœÚ 
	else if(delta_x==0)incx=0;//¥π÷±œﬂ 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ÀÆ∆Ωœﬂ 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //—°»°ª˘±æ‘ˆ¡ø◊¯±Í÷· 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//ª≠œﬂ ‰≥ˆ 
	{  
		LCD_DrawPoint(uRow,uCol);//ª≠µ„ 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//ª≠æÿ–Œ	  
//(x1,y1),(x2,y2):æÿ–Œµƒ∂‘Ω«◊¯±Í
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//‘⁄÷∏∂®Œª÷√ª≠“ª∏ˆ÷∏∂®¥Û–°µƒ‘≤
//(x,y):÷––ƒµ„
//r    :∞Îæ∂
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //≈–∂œœ¬∏ˆµ„Œª÷√µƒ±Í÷æ
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		// π”√BresenhamÀ„∑®ª≠‘≤     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  
//‘⁄÷∏∂®Œª÷√œ‘ æ“ª∏ˆ◊÷∑˚
//x,y:∆ º◊¯±Í
//num:“™œ‘ æµƒ◊÷∑˚:" "--->"~"
//size:◊÷ÃÂ¥Û–° 12/16/24
//mode:µ˛º”∑Ω Ω(1)ªπ «∑«µ˛º”∑Ω Ω(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//µ√µΩ◊÷ÃÂ“ª∏ˆ◊÷∑˚∂‘”¶µ„’ÛºØÀ˘’ºµƒ◊÷Ω⁄ ˝	
 	num=num-' ';//µ√µΩ∆´“∆∫Ûµƒ÷µ£®ASCII◊÷ø‚ «¥”ø’∏Òø™ º»°ƒ££¨À˘“‘-' 'æÕ «∂‘”¶◊÷∑˚µƒ◊÷ø‚£©
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//µ˜”√1206◊÷ÃÂ
		else if(size==16)temp=asc2_1608[num][t];	//µ˜”√1608◊÷ÃÂ
		else if(size==24)temp=asc2_2412[num][t];	//µ˜”√2412◊÷ÃÂ
		else return;								//√ª”–µƒ◊÷ø‚
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//≥¨«¯”Ú¡À
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//≥¨«¯”Ú¡À
				break;
			}
		}  	 
	}  	    	   	 	  
}   
//m^n∫Ø ˝
//∑µªÿ÷µ:m^n¥Œ∑Ω.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//œ‘ æ ˝◊÷,∏ﬂŒªŒ™0,‘Ú≤ªœ‘ æ
//x,y :∆µ„◊¯±Í	 
//len : ˝◊÷µƒŒª ˝
//size:◊÷ÃÂ¥Û–°
//color:—’…´ 
//num: ˝÷µ(0~4294967295);	 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//œ‘ æ ˝◊÷,∏ﬂŒªŒ™0,ªπ «œ‘ æ
//x,y:∆µ„◊¯±Í
//num: ˝÷µ(0~999999999);	 
//len:≥§∂»(º¥“™œ‘ æµƒŒª ˝)
//size:◊÷ÃÂ¥Û–°
//mode:
//[7]:0,≤ªÃÓ≥‰;1,ÃÓ≥‰0.
//[6:1]:±£¡Ù
//[0]:0,∑«µ˛º”œ‘ æ;1,µ˛º”œ‘ æ.
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//œ‘ æ◊÷∑˚¥Æ
//x,y:∆µ„◊¯±Í
//width,height:«¯”Ú¥Û–°  
//size:◊÷ÃÂ¥Û–°
//*p:◊÷∑˚¥Æ∆ ºµÿ÷∑		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//≈–∂œ «≤ª «∑«∑®◊÷∑˚!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//ÕÀ≥ˆ
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}



























