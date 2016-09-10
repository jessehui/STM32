#ifndef 	__WKUP_H__
#define 	__WKUP_H__

#include "sys.h"

#define WKUP_KD		PAin(0)		//位带操作. PA0检测是否有外部按键被按下

u8 Check_WKUP(void);
void WKUP_Init(void);
void Sys_Enter_Standby(void);



#endif