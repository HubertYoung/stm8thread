 
/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "delay.h"

#define SYS_CLK_HSI_DIV1

/** @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
void Delay(__IO uint16_t nCount)
{
  for (; nCount != 0; nCount--);
}

//------------------------------------------------------------------------------
// Function Name : delay_ms
// Description   : delay for some time in ms unit(partial accurate)
// Input         : n_ms is how many ms of time to delay
//------------------------------------------------------------------------------
void delay_ms(uint16_t n_ms)
{
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE);
  // Init TIMER 4 //
  // prescaler: / (2^6) = /64
  TIM4->PSCR = 6;
#ifdef SYS_CLK_HSE_DIV1
  // Auto-Reload value: 16M / 64 = 1/4M, 1/4M / 1k = 250
  TIM4->ARR = 250;
#else
#ifdef SYS_CLK_HSE_DIV2
  // Auto-Reload value: 16M/2 / 64 = 1/8M, 1/8M / 1k = 125
  TIM4->ARR = 125;
#else
#ifdef SYS_CLK_HSI_DIV1
  // Auto-Reload value: 16M / 64 = 1/4M, 1/4M / 1k = 250
  TIM4->ARR = 250;
#else
#ifdef SYS_CLK_HSI_DIV2
  // Auto-Reload value: 16M/2 / 64 = 1/8M, 1/8M / 1k = 125
  TIM4->ARR = 125;
#else
#error "either SYS_CLK_HSE_DIV1/DIV2 or SYS_CLK_HSI_DIV1/DIV2 should be defined"
#endif
#endif
#endif
#endif
  // Counter value: 2, to compensate the initialization of TIMER
  TIM4->CNTR = 2;

  // clear update flag
  TIM4->SR1 &= ~TIM4_SR1_UIF;
  // Enable Counter
  TIM4->CR1 |= TIM4_CR1_CEN;

  while (n_ms--)
  {
    while ((TIM4->SR1 & TIM4_SR1_UIF) == 0) ;
    TIM4->SR1 &= ~TIM4_SR1_UIF;
  }

  // Disable Counter
  TIM4->CR1 &= ~TIM4_CR1_CEN;
}


//------------------------------------------------------------------------------
// Function Name : delay_10us
// Description   : delay for some time in 10us unit(partial accurate)
// Input         : n_10us is how many 10us of time to delay
//------------------------------------------------------------------------------
void delay_1us(uint16_t n_1us)
{
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE);
  // Init TIMER 4 //
  // prescaler: / (2^0) = /1
  TIM4->PSCR = 0;
#ifdef SYS_CLK_HSE_DIV1
  // Auto-Reload value: 16M / 1 = 16M, 16M / 100k = 160
  TIM4->ARR = 160;
#else
#ifdef SYS_CLK_HSE_DIV2
  // Auto-Reload value: 16M/2 / 1 = 8M, 8M / 100k = 80
  TIM4->ARR = 80;
#else
#ifdef SYS_CLK_HSI_DIV1
  // Auto-Reload value: 16M / 1 = 16M, 16M / 100k = 160
  TIM4->ARR = 16;
#else
#ifdef SYS_CLK_HSI_DIV2
  // Auto-Reload value: 16M/2 / 1 = 8M, 8M / 100k = 80
  TIM4->ARR = 80;
#else
#error "either SYS_CLK_HSE_DIV1/DIV2 or SYS_CLK_HSI_DIV1/DIV2 should be defined"
#endif
#endif
#endif
#endif
  // Counter value: 10, to compensate the initialization of TIMER
  TIM4->CNTR = 10;

  // clear update flag
  TIM4->SR1 &= ~TIM4_SR1_UIF;
  // Enable Counter
  TIM4->CR1 |= TIM4_CR1_CEN;

  while (n_1us--)
  {
    while ((TIM4->SR1 & TIM4_SR1_UIF) == 0) ;
    TIM4->SR1 &= ~TIM4_SR1_UIF;
  }

  // Disable Counter
  TIM4->CR1 &= ~TIM4_CR1_CEN;
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, DISABLE);

}