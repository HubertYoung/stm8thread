

#ifndef __DELAY_H
#define __DELAY_H

#include <atomtimer.h>
#include <atomport.h>
#include "stm8s_tim4.h"

void delay_us(uint16_t n_1us);
void delay_ms(uint16_t n_ms);

#endif