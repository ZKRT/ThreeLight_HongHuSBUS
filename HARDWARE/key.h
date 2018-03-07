#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 

#define KEY_DEFAULT 0
#define KEY1_PRES  1
#define KEY2_PRES	 2
#define KEY3_PRES	 3

//add by yanly
//#define KEY_TEST_FUN		1   //按键测试吊舱控制接口

#ifdef KEY_TEST_FUN
void KEY_Init(void);
uint8_t KEY_Scan(void);
void KEY_Rock(void);
#endif
#endif
