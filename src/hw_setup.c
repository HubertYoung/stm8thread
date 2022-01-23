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
static void TIM4_Config(void);
static void I2C_Config(void);

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

  /* Enable the particular preipherial clocks */
#ifdef SSD1306_SPI_CONTROL
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, ENABLE);
#else
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, DISABLE);
#endif
#ifdef SSD1306_I2C_CONTROL
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
#else
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, DISABLE);
#endif

  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE);

  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, DISABLE);

#ifdef DISABLE_OTHER_PERIPHERIAL_CLOCKS
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, DISABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_AWU, DISABLE);
  // CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, DISABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, DISABLE);
#endif
}

/**
  * @brief Used GPIO Configuration
  * @retval None
  */
static void GPIO_Config(void)
{
  // GPIO_DeInit(GPIOE);
  // GPIO_DeInit(GPIOA);
  // GPIO_DeInit(GPIOC);
  // GPIO_DeInit(GPIOB);

  /* Configure GPIO pins */
#ifdef SSD1306_RESET_USE
  GPIO_Init(SSD1306_RESET_Port, SSD1306_RESET_Pin, GPIO_MODE_OUT_PP_HIGH_SLOW);
#endif

#ifdef SSD1306_SPI_CONTROL
  /* Configure the SCK, MOSI, MISO */
  GPIO_Init(SSD1306_SCK_Port, SSD1306_SCK_Pin, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(SSD1306_MOSI_Port, SSD1306_MOSI_Pin, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(SSD1306_MISO_Port, SSD1306_MISO_Pin, GPIO_MODE_IN_PU_NO_IT);
  /* Configure the SS pin */
  GPIO_Init(SSD1306_CS_Port, SSD1306_CS_Pin, GPIO_MODE_OUT_PP_HIGH_FAST);
  /* Configure the D/C pin */
  GPIO_Init(SSD1306_DC_Port, SSD1306_DC_Pin, GPIO_MODE_OUT_PP_LOW_SLOW);
#endif

#ifdef SSD1306_I2C_CONTROL
  /* Configure the SCL and SDA pins */
  GPIO_Init(SSD1306_SCL_Port, SSD1306_SCL_Pin, GPIO_MODE_OUT_OD_HIZ_FAST);
  GPIO_Init(SSD1306_SDA_Port, SSD1306_SDA_Pin, GPIO_MODE_OUT_OD_HIZ_FAST);
#endif
  /* Configure USART Tx as alternate function push-pull  (software pull up)*/
  GPIO_ExternalPullUpConfig(GPIOC, GPIO_PIN_3, ENABLE);
  /* Configure USART Rx as alternate function push-pull  (software pull up)*/
  GPIO_ExternalPullUpConfig(GPIOC, GPIO_PIN_2, ENABLE);

  GPIO_Init(TEST_Port, TEST_Pin, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(GPIOA, GPIO_PIN_1, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(GPIOA, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST);
  // key
  GPIO_Init(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_MODE_IN_PU_NO_IT); //KEY_UP
}

#ifdef SSD1306_I2C_CONTROL
/**
  * @brief I2C Peripherial Configuration
  * @retval None
  */
static void I2C_Config(void)
{
  I2C_DeInit();

  /* Set the I2C transmit parameters */
  I2C_Init(SSD1306_I2C_SPEED,
           1,
           I2C_DUTYCYCLE_2,
           I2C_ACK_CURR,
           I2C_ADDMODE_7BIT,
           I2C_MAX_INPUT_FREQ);
  I2C_Cmd(ENABLE);
}
#endif

#ifdef SSD1306_SPI_CONTROL
/**
  * @brief SPI Peripherial Configuration
  * @retval None
  */
static void SPI_Config(void)
{
  SPI_DeInit();

  /* Select the method how NSS is handled */
#ifndef SPI_CS_HARDWARE_CONTROL
  SPI_Init(SPI_FIRSTBIT_MSB,
           SPI_BAUDRATEPRESCALER_2,
           SPI_MODE_MASTER,
           SPI_CLOCKPOLARITY_LOW,
           SPI_CLOCKPHASE_1EDGE,
           SPI_DATADIRECTION_1LINE_TX,
           SPI_NSS_SOFT,
           7);
#else
  SPI_Init(SPI_FIRSTBIT_MSB,
           SPI_BAUDRATEPRESCALER_2,
           SPI_MODE_MASTER,
           SPI_CLOCKPOLARITY_LOW,
           SPI_CLOCKPHASE_1EDGE,
           SPI_DATADIRECTION_1LINE_TX,
           SPI_NSS_HARD,
           7);
#endif

  /* Set the SPI transmit parameters */
  SPI_Cmd(ENABLE);
}
#endif
/**
  * @brief TIM2 Configuration for system tick
  * @retval None
  */
static void TIM4_Config(void)
{
  // TIM4_DeInit();

  // /* Time period is 1ms, 1/((16MHz/16) * 1000) */
  // TIM4_TimeBaseInit(TIM4_PRESCALER_64, 3);
  // /* Enable the OVF interrupts from TIM2 */
  // TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  // /* Start Timer */
  // TIM4_Cmd(ENABLE);

  // TIM4_DeInit();

  // /* Time period is 1ms, 1/((16MHz/16) * 1000) */
  // TIM4_TimeBaseInit(TIM4_PRESCALER_64, 3);
  // /* Enable the OVF interrupts from TIM2 */
  // TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  // /* Start Timer */
  // TIM4_Cmd(ENABLE);
  // enableInterrupts();
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
  // TIM4_Config();
#ifdef SSD1306_I2C_CONTROL
  I2C_Config();
#endif
#ifdef SSD1306_SPI_CONTROL
  SPI_Config();
#endif

  /* Initialize interrupts */
  // enableInterrupts();
}

#ifdef SSD1306_I2C_CONTROL
/**
  * @brief  I2C with Control Byte Transaction
  * @param    Address:      Slave address
  * @param    ControllByte: Controll byte before data
  * @param    pData:        Pointer to block of data
  * @param    DataSize:     How many bytes of data to send
  * @retval None
  */
void I2C_CWrite(uint8_t Address, uint8_t ControlByte, uint8_t *pData, uint16_t DataSize)
{
  /* Check the params */
  // if ((pData == NULL) || (DataSize == 0U))
  //   return;

  // /* Check the busy flag */
  // while (I2C_GetFlagStatus(I2C_FLAG_BUSBUSY))
  // {
  // };

  // /* Send START condition and check if uC act as a Master */
  // I2C_GenerateSTART(ENABLE);
  // while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
  // {
  // };

  // /* Send slave address and wait for Slave ACK */
  // I2C_Send7bitAddress(Address, I2C_DIRECTION_TX);
  // while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  // {
  // };

  // /* If TX buffer is empty, send a control byte to the Slave
  //  * and wait for end of transmission
  //  */
  // while ((I2C_GetFlagStatus(I2C_FLAG_TXEMPTY) == RESET))
  // {
  // };
  // I2C_SendData(ControlByte);
  // while ((I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED) == RESET))
  // {
  // };

  // /* Start sending data stream */
  // while (DataSize--)
  // {
  //   /* In case of last byte send NACK */
  //   if (!DataSize)
  //   {
  //     I2C_AcknowledgeConfig(I2C_ACK_NONE);
  //   }
  //   I2C_SendData(*pData++);
  //   while ((I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED) == RESET))
  //   {
  //   };
  // }
  // /* End of transaction, put STOP condition */
  // I2C_GenerateSTOP(ENABLE);
}
#endif

#ifdef SSD1306_SPI_CONTROL
/**
  * @brief  SPI Bulk Transaction
  * @param    pData:    Pointer to block of data
  * @param    DataSize: How many bytes of data to send
  * @retval None
  */
void SPI_Transmit(uint8_t *pData, uint16_t DataSize)
{
  /* Check the params */
  if ((pData == NULL) || (DataSize == 0U))
    return;
  /* Do the full transaction */
  while (DataSize--)
  {
    while (SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET)
    {
    };
    SPI_SendData(*pData++);
  }
}
#endif

/* *END OF FILE* */
