/**
  ****************************************************************************
  * @file           : hw_setup.h
  * @brief          : Header for hw_setup.c file.
  *                   This file contains the hardware setup.
  *****************************************************************************
  * @attention
  *
  *  This code inspired on:
  *   OLED SSD1306 library for STM32
  *   https://github.com/lamik/OLED_SSD1306_STM32_HAL
  *
  *****************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_SETUP_H
#define __HW_SETUP_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_i2c.h"

/* Exported symbols ----------------------------------------------------------*/
/**
  *  @brief  Hardware settings
  */

/**
  *  @brief  Define this symbol if @ref SystemClock_Config
  *          can disable all other peripherial clocks.
  */
#define DISABLE_OTHER_PERIPHERIAL_CLOCKS

/**
  *  @brief  Debug LED connection
  *  @note   LED is active when display GRAM is
  *          refreshed..
  */
#define TEST_Pin                GPIO_PIN_5
#define TEST_Port               GPIOE


/* Exported functions prototypes ---------------------------------------------*/
void MCU_Config(void);
#endif /* __HW_SETUP_H */

/* *END OF FILE* */