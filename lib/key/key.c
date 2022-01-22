#include "key.h"

void keyInit(void)
{
    GPIO_DeInit(KEY_GPIO);
    GPIO_Init(KEY_GPIO, KEY1_GPIO_PIN, GPIO_MODE_IN_PU_IT); //KEY_UP
    GPIO_Init(KEY_GPIO, KEY2_GPIO_PIN, GPIO_MODE_IN_PU_IT); //KEY_DOWN
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
    switch (keyPassValue)
    {
    case KEY_UP:
        // printf("KEY_UP keyPassOne\n");
        GPIO_WriteReverse(GPIOA, GPIO_PIN_1);
        break;
    case KEY_DOWN:
        // printf("KEY_UP KeyPassLong\n");
        GPIO_WriteReverse(GPIOA, GPIO_PIN_2);
        break;
    default:
        break;
    }
    // printf("KEY_UP uint8_t{%d}\n", (KEY_UP != RESET && GPIO_ReadInputPin(KEY_GPIO, KEY1_GPIO_PIN) == RESET));
}