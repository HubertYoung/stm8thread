

#ifndef __DELAY_H
#define __DELAY_H

#include "stm8s.h"

void delay_us(uint16_t n_1us);
void delay_ms(uint16_t n_ms);
void time_loop(uint16_t ms);

void delay_isr(void);
#endif