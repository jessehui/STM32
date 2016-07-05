#ifndef __LED_H
#define __LED_H

#include "sys.h"

//led端口定义
#define LED0 PFout(9)		//位带操作
#define LED1 PFout(10)

void LED_Init(void);
#endif
