/**
 ****************************************************************************
 * @file           : hw_setup.c
 * @brief          : This file contains the hardware setup routines.
 **************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "hw_setup.h"

/* Private includes ----------------------------------------------------------*/
#include <stdint.h>
#include <stdlib.h>

/* Privatefunctions prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void GPIO_Config(void);

/* Private user code ---------------------------------------------------------*/
/**
 * @brief  System Clock Configuration
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void)
{
  CLK_DeInit();

  /* Select the clock source */
  CLK_HSECmd(DISABLE);
  CLK_LSICmd(DISABLE);
  CLK_HSICmd(ENABLE);
  while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == FALSE)
  {
  };

  /* Set the core and peripherial clocks */
  CLK_ClockSwitchCmd(ENABLE);
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);

  CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI,
                        DISABLE, CLK_CURRENTCLOCKSTATE_ENABLE);

  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE);

  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, DISABLE);

// #ifdef DISABLE_OTHER_PERIPHERIAL_CLOCKS
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, DISABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_AWU, DISABLE);
  // CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, DISABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, DISABLE);
// #endif
}

/**
 * @brief Used GPIO Configuration
 * @retval None
 */
static void GPIO_Config(void)
{
  GPIO_DeInit(GPIOE);
  GPIO_DeInit(GPIOA);
  GPIO_DeInit(GPIOC);
  GPIO_DeInit(GPIOB);

  // GPIO_Init(SSD1306_SCL_Port, SSD1306_SCL_Pin, GPIO_MODE_OUT_OD_HIZ_FAST);
  // GPIO_Init(SSD1306_SDA_Port, SSD1306_SDA_Pin, GPIO_MODE_OUT_OD_HIZ_FAST);

  GPIO_Init(TEST_Port, TEST_Pin, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(GPIOA, GPIO_PIN_1, GPIO_MODE_OUT_PP_LOW_FAST);

}

/* Exported user code --------------------------------------------------------*/
/**
 * @brief  Microcontroller main configuration
 * @param  None
 * @retval None
 */
void MCU_Config(void)
{
  /* Configure the system clock */
  SystemClock_Config();

  GPIO_Config();
}
/* *END OF FILE* */
