#include "stm8s.h"
#include "key.h"
#include "eeprom.h"

//按键初始化函数
void KEY_Init(void)
{
     // key
  GPIO_Init(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT); // KEY_UP
}

//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，WKUP按下 KEY_DOWN
//注意此函数有响应优先级,KEY_LEFT>KEKEY_LEFTEY2>KEY_DOWN!!
u8 KEY_Scan(u8 mode)
{
    static u8 key_up = 1; //按键松开标志
    if (mode == 1)
        key_up = 1; //支持连按
    if (key_up && (KEY_LEFT == 1 || KEY_UP == 1 || KEY_RIGHT == 1 || KEY_DOWN == 1))
    {
        delay_ms(10);
        key_up = 0;
        if (KEY_LEFT == 1)
            return KEY_LEFT_PRES;
        else if (KEY_UP == 1)
            return KEY_UP_PRES;
        else if (KEY_RIGHT == 1)
            return KEY_RIGHT_PRES;
        else if (KEY_DOWN == 1)
            return KEY_DOWN_PRES;
    }
    else if (KEY_LEFT == 0 && KEY_UP == 0 && KEY_RIGHT == 0 && KEY_DOWN == 0)
        key_up = 1;
    return 0; //无按键按下
}