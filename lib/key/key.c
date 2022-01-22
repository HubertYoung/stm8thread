#include "stm8s.h"
#include "key.h"
#include "eeprom.h"

void keyInit(void)
{
    GPIO_DeInit(KEY_GPIO);
    GPIO_Init(KEY_GPIO, KEY1_GPIO_PIN, GPIO_MODE_IN_PU_IT); //KEY_UP
    GPIO_Init(KEY_GPIO, KEY2_GPIO_PIN, GPIO_MODE_IN_PU_IT); //KEY_DOWN
    // GPIO_Init(KEY_GPIO, KEY1_GPIO_PIN, GPIO_MODE_IN_PU_NO_IT); //KEY_UP
    // GPIO_Init(KEY_GPIO, KEY2_GPIO_PIN, GPIO_MODE_IN_PU_NO_IT); //KEY_DOWN
    EXTI_DeInit();
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB, EXTI_SENSITIVITY_FALL_ONLY);
}

uint8_t keyRead(uint8_t key_map)
{
    uint8_t result = 0;
    if (key_map & KEY_UP != RESET && GPIO_ReadInputPin(KEY_GPIO, KEY1_GPIO_PIN) == RESET)
    {
        delay_ms(20);
        if (key_map & KEY_UP != RESET && GPIO_ReadInputPin(KEY_GPIO, KEY1_GPIO_PIN) == RESET)
        {
            result |= KEY_UP;
        }
    }
    if (key_map & KEY_DOWN != RESET && GPIO_ReadInputPin(KEY_GPIO, KEY2_GPIO_PIN) == RESET)
    {
        delay_ms(20);
        if (key_map & KEY_DOWN != RESET && GPIO_ReadInputPin(KEY_GPIO, KEY2_GPIO_PIN) == RESET)
        {
            result |= KEY_DOWN;
        }
    }
    return result;
}

#if defined(__IAR_SYSTEMS_ICC__)
#pragma vector = 6
#endif
INTERRUPT void EXTI_PORTB_IRQHandler(void)
#if defined(__RCSTM8__)
    interrupt 4
#endif
{

    uint8_t keyPassValue = keyRead(KEY_UP | KEY_DOWN);

    uint8_t duty_cycle = eeprom_read_data8(PWM_CYCLE_ADD);
    uint16_t pwm1_period = eeprom_read_data16(PWM_PERIOD_ADD);

    switch (keyPassValue)
    {
    case KEY_UP:
        GPIO_WriteReverse(GPIOA, GPIO_PIN_1);

        duty_cycle++;
        eeprom_write_data8(PWM_CYCLE_ADD, duty_cycle >= 99 ? 99 : duty_cycle);
        TIM1_SetCompare2((pwm1_period * duty_cycle / 100) - 1); //脉冲宽度值
        break;
    case KEY_DOWN:
        GPIO_WriteReverse(GPIOA, GPIO_PIN_2);
        duty_cycle--;
        eeprom_write_data8(PWM_CYCLE_ADD, duty_cycle <= 1 ? 1 : duty_cycle);
        TIM1_SetCompare2((pwm1_period * duty_cycle / 100) - 1); //脉冲宽度值
        break;
    default:
        break;
    }
    // printf("KEY_UP uint8_t{%d}\n", (KEY_UP != RESET && GPIO_ReadInputPin(KEY_GPIO, KEY1_GPIO_PIN) == RESET));
}