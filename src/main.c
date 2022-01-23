
#include <uart.h>

#include <stm8s.h>
#include <stm8s_gpio.h>
#include <pwm.h>
#include <key.h>
#include <eeprom.h>

void system_status(uint32_t param);

int main(void)
{

    /* STM8 configuration */
    MCU_Config();

    eeprom_init();

    keyInit();
    // IIC_Init();
    /* SSD1306 configuration */
#ifdef SSD1306_I2C_CONTROL
    SSD1306_I2cInit();
#endif
#ifdef SSD1306_SPI_CONTROL
    SSD1306_SpiInit();
#endif

    PWM_Init();

    // OLED_Init(); //初始化OLED
    // OLED_Clear();

    GUI_Init();
    /**
     * Note: to protect OS structures and data during initialisation,
     * interrupts must remain disabled until the first thread
     * has been restored. They are reenabled at the very end of
     * the first thread restore, at which point it is safe for a
     * reschedule to take place.
     */

    system_status(0);
    /* There was an error starting the OS if we reach here */
    // while (1)
    //     ;
    return 0;
}

/**
 * \b main_thread_func
 *
 * Entry point for main application thread.
 *
 * This is the first thread that will be executed when the OS is started.
 *
 * @param[in] param Unused (optional thread entry parameter)
 *
 * @return None
 */
void system_status(uint32_t param)
{
    /* Compiler warnings */
    param = param;
    /* Initialise UART (115200bps) */
    if (uart_init(115200) != 0)
    {
        /* Error initialising UART */
    }
    /* Put a message out on the UART */
    printf("Sys Init Over\n");
    /* Test finished, flash slowly for pass, fast for fail */
    while (1)
    {
        time_loop(1);
        GUI_Refresh();
        // atomTimerDelay(1);

        /* Sleep then toggle LED again */
        // atomTimerDelay(500);
        // LED_SYS_REVERSE;
        // uint8_t duty_cycle = eeprom_read_data8(PWM_CYCLE_ADD);
        // uint16_t pwm1_period = eeprom_read_data16(PWM_PERIOD_ADD);
        // printf("duty_cycle${%d}\n", duty_cycle);
        // printf("pwm1_period${%d}\n", pwm1_period);
    }
}

// static void at_thread_func(uint32_t param)
// {
//     while (1)
//     {
//         atomTimerDelay(1);
//         GUI_Refresh();
//     }
// }


