#ifndef __KEY_H
#define __KEY_H

#include "sys.h"
#include "delay.h"

#define  KEY0  PEin(4)
#define  KEY1	 PEin(3)
#define  KEY2  PEin(2)
#define  WK_UP  PAin(0)

#define		KEY0_PRES 1	//KEY0±»°´ÏÂ
#define   KEY1_PRES 2
#define   KEY2_PRES 3
#define   WKUP_PRES 4

void KEY_Init(void);
u8 KEY_Scan(u8);
u8 KEY_Scan_Custom(u8);

#endif



