#ifndef __LCD_H
#define __LCD_H		
#include "sys.h"	 
#include "stdlib.h" 
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

//LCD÷ÿ“™≤Œ ˝ºØ
typedef struct  
{										    
	u16 width;			//LCD øÌ∂»
	u16 height;			//LCD ∏ﬂ∂»
	u16 id;				//LCD ID
	u8  dir;			//∫·∆¡ªπ « ˙∆¡øÿ÷∆£∫0£¨ ˙∆¡£ª1£¨∫·∆¡°£	
	u16	wramcmd;		//ø™ º–¥gram÷∏¡Ó
	u16  setxcmd;		//…Ë÷√x◊¯±Í÷∏¡Ó
	u16  setycmd;		//…Ë÷√y◊¯±Í÷∏¡Ó 
}_lcd_dev; 	  

//LCD≤Œ ˝
extern _lcd_dev lcddev;	//π‹¿ÌLCD÷ÿ“™≤Œ ˝
//LCDµƒª≠± —’…´∫Õ±≥æ∞…´	   
extern u16  POINT_COLOR;//ƒ¨»œ∫Ï…´    
extern u16  BACK_COLOR; //±≥æ∞—’…´.ƒ¨»œŒ™∞◊…´


//////////////////////////////////////////////////////////////////////////////////	 
//-----------------LCD∂Àø⁄∂®“Â---------------- 
#define	LCD_LED PBout(15)  		//LCD±≥π‚    		 PB15 	    
//LCDµÿ÷∑Ω·ππÃÂ
typedef struct
{
	vu16 LCD_REG;
	vu16 LCD_RAM;
} LCD_TypeDef;
// π”√NOR/SRAMµƒ Bank1.sector4,µÿ÷∑ŒªHADDR[27,26]=11 A6◊˜Œ™ ˝æ›√¸¡Ó«¯∑÷œﬂ 
//◊¢“‚…Ë÷√ ±STM32ƒ⁄≤øª·”““∆“ªŒª∂‘∆‰! 111 1110=0X7E			    
#define LCD_BASE        ((u32)(0x6C000000 | 0x0000007E))
#define LCD             ((LCD_TypeDef *) LCD_BASE)
//////////////////////////////////////////////////////////////////////////////////
	 
//…®√Ë∑ΩœÚ∂®“Â
#define L2R_U2D  0 //¥”◊ÛµΩ”“,¥”…œµΩœ¬
#define L2R_D2U  1 //¥”◊ÛµΩ”“,¥”œ¬µΩ…œ
#define R2L_U2D  2 //¥””“µΩ◊Û,¥”…œµΩœ¬
#define R2L_D2U  3 //¥””“µΩ◊Û,¥”œ¬µΩ…œ

#define U2D_L2R  4 //¥”…œµΩœ¬,¥”◊ÛµΩ”“
#define U2D_R2L  5 //¥”…œµΩœ¬,¥””“µΩ◊Û
#define D2U_L2R  6 //¥”œ¬µΩ…œ,¥”◊ÛµΩ”“
#define D2U_R2L  7 //¥”œ¬µΩ…œ,¥””“µΩ◊Û	 

#define DFT_SCAN_DIR  L2R_U2D  //ƒ¨»œµƒ…®√Ë∑ΩœÚ

//ª≠± —’…´
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //◊ÿ…´
#define BRRED 			 0XFC07 //◊ÿ∫Ï…´
#define GRAY  			 0X8430 //ª“…´
//GUI—’…´

#define DARKBLUE      	 0X01CF	//…Ó¿∂…´
#define LIGHTBLUE      	 0X7D7C	//«≥¿∂…´  
#define GRAYBLUE       	 0X5458 //ª“¿∂…´
//“‘…œ»˝…´Œ™PANELµƒ—’…´ 
 
#define LIGHTGREEN     	 0X841F //«≥¬Ã…´
//#define LIGHTGRAY        0XEF5B //«≥ª“…´(PANNEL)
#define LGRAY 			 0XC618 //«≥ª“…´(PANNEL),¥∞ÃÂ±≥æ∞…´

#define LGRAYBLUE        0XA651 //«≥ª“¿∂…´(÷–º‰≤„—’…´)
#define LBBLUE           0X2B12 //«≥◊ÿ¿∂…´(—°‘ÒÃıƒøµƒ∑¥…´)
	    															  
void LCD_Init(void);													   	//≥ı ºªØ
void LCD_DisplayOn(void);													//ø™œ‘ æ
void LCD_DisplayOff(void);													//πÿœ‘ æ
void LCD_Clear(u16 Color);	 												//«Â∆¡
void LCD_SetCursor(u16 Xpos, u16 Ypos);										//…Ë÷√π‚±Í
void LCD_DrawPoint(u16 x,u16 y);											//ª≠µ„
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);								//øÏÀŸª≠µ„
u16  LCD_ReadPoint(u16 x,u16 y); 											//∂¡µ„ 
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);						 			//ª≠‘≤
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);							//ª≠œﬂ
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   				//ª≠æÿ–Œ
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);		   				//ÃÓ≥‰µ•…´
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);				//ÃÓ≥‰÷∏∂®—’…´
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);						//œ‘ æ“ª∏ˆ◊÷∑˚
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  						//œ‘ æ“ª∏ˆ ˝◊÷
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				//œ‘ æ  ˝◊÷
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);		//œ‘ æ“ª∏ˆ◊÷∑˚¥Æ,12/16◊÷ÃÂ

void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);
u16 LCD_ReadReg(u16 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);
void LCD_SSD_BackLightSet(u8 pwm);							//SSD1963 ±≥π‚øÿ÷∆
void LCD_Scan_Dir(u8 dir);									//…Ë÷√∆¡…®√Ë∑ΩœÚ
void LCD_Display_Dir(u8 dir);								//…Ë÷√∆¡ƒªœ‘ æ∑ΩœÚ
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);	//…Ë÷√¥∞ø⁄					   						   																			 
//LCD∑÷±Ê¬ …Ë÷√
#define SSD_HOR_RESOLUTION		800		//LCDÀÆ∆Ω∑÷±Ê¬ 
#define SSD_VER_RESOLUTION		480		//LCD¥π÷±∑÷±Ê¬ 
//LCD«˝∂Ø≤Œ ˝…Ë÷√
#define SSD_HOR_PULSE_WIDTH		1		//ÀÆ∆Ω¬ˆøÌ
#define SSD_HOR_BACK_PORCH		46		//ÀÆ∆Ω«∞¿»
#define SSD_HOR_FRONT_PORCH		210		//ÀÆ∆Ω∫Û¿»

#define SSD_VER_PULSE_WIDTH		1		//¥π÷±¬ˆøÌ
#define SSD_VER_BACK_PORCH		23		//¥π÷±«∞¿»
#define SSD_VER_FRONT_PORCH		22		//¥π÷±«∞¿»
//»Áœ¬º∏∏ˆ≤Œ ˝£¨◊‘∂Øº∆À„
#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)

#endif  
	 
	 



