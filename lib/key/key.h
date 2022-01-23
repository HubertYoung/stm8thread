#ifndef __DRV_GPIO_H
#define __DRV_GPIO_H

#include <stm8s.h>
#include <delay.h>
#include <sys.h>

#include "atomport-private.h"
#include "atomport.h"

#define KEY_UP 		PBin(0)
#define KEY_DOWN 	PBin(1)
#define KEY_LEFT 	PBin(2)
#define KEY_RIGHT 	PBin(3)

#define LED1_REVERSE  GPIO_WriteReverse(GPIOA, GPIO_PIN_1);
#define LED_SYS_REVERSE  GPIO_WriteReverse(GPIOE, GPIO_PIN_5);

void keyInit(void);
uint8_t keyRead(void);
void keyLoadRun(void);

// INTERRUPT void EXTI_PORTB_IRQHandler(void);

#endif