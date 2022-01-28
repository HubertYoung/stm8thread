#ifndef __DRV_GPIO_H
#define __DRV_GPIO_H

#include <stm8s.h>
#include <delay.h>
#include <sys.h>

// #include "atomport-private.h"
// #include "atomport.h"

#define KEY_LEFT 		PBin(0)
#define KEY_UP 	        PBin(1)
#define KEY_RIGHT 	    PBin(2)
#define KEY_DOWN 	    PBin(3)

#define LED1_REVERSE  GPIO_WriteReverse(GPIOA, GPIO_PIN_1);
#define LED_SYS_REVERSE  GPIO_WriteReverse(GPIOE, GPIO_PIN_5);

#define KEY_LEFT_PRES 	1
#define KEY_UP_PRES	2
#define KEY_RIGHT_PRES	3
#define KEY_DOWN_PRES   4

void KEY_Init(void);
u8 KEY_Scan(u8 mode);

#endif