#ifndef __DRV_GPIO_H
#define __DRV_GPIO_H

#include <stm8s.h>
#include <delay.h>

#include "atomport-private.h"
#include "atomport.h"

#define KEY_GPIO GPIOB
#define KEY1_GPIO_PIN GPIO_PIN_0
#define KEY2_GPIO_PIN GPIO_PIN_1

//Maps of KEYs
#define KEY(n) ((uint8_t)(0x01 << (uint8_t)(n - 1)))

#define KEY_UP KEY(1)
#define KEY_DOWN KEY(2)

void keyInit(void);
uint8_t keyRead(uint8_t key_map);

INTERRUPT void EXTI_PORTB_IRQHandler(void);

#endif