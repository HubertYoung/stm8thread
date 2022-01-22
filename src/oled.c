/**
 * **************************************************************
 * 
 * 0.96 inch OLED demo for stm8s chips
 * 
 * CPU: stm8s103/003
 * 
 * FREQ: 16MHz
 * 
 * PIN: 
 *      SCL: PB4
 *      SDA: PB5 
 * 
 * **************************************************************
*/

#include "stm8s.h"
#include "oled.h"
#include <stdio.h>

#define LED_GPIO_PORT (GPIOB)
#define LED_GPIO_PINS (GPIO_PIN_5)

void DelayInit(void);
void DelayMs(uint16_t ms);
void Driver_OLED_Init(void);

void main(void)
{
    // set system clock to 16Mhz
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

    // init delay
    DelayInit();

    // init LED pin
    GPIO_Init(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);

    // init OLED
    Driver_OLED_Init();

    while (1)
    {
        printf("Hello World !\n"); // print 'hello world' to OLED
        GPIO_WriteReverse(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
        DelayMs(500);
        OLED_ClearAll();
    }
}

/**
 * redirect 'int putchar(int);' method
*/
int putchar(int c)
{
    OLED_putchar((char)c);
    return c;
}

void DelayInit(void)
{
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE);
    TIM4_TimeBaseInit(TIM4_PRESCALER_64, 249); // 1ms
    TIM4_SelectOnePulseMode(TIM4_OPMODE_SINGLE);
}

void DelayMs(uint16_t ms)
{
    while (ms--)
    {
        TIM4->SR1 = (uint8_t)(~TIM4_FLAG_UPDATE);
        TIM4->CR1 |= TIM4_CR1_CEN;
        while ((TIM4->SR1 & (uint8_t)TIM4_FLAG_UPDATE) == 0)
            ;
    }
}

/**
 * =========================================================
 *                     interrupt functions
 * =========================================================
*/

/** @addtogroup GPIO_Toggle
  * @{
  */
#ifdef _COSMIC_
/**
  * @brief  Dummy interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(NonHandledInterrupt, 25)
{
    /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*_COSMIC_*/

/**
  * @brief  TRAP interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
    /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  This is a example for interrupt function define
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(example, EXTI_PORTA_IRQn)
{
    // TODO
}
