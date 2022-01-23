#include "stm8s.h"
#include "gui.h"
#include "pwm.h"

static u8 func_index = _Main_UI; //当前页面索引值
static u8 last_index = _Main_UI; //上一个界面索引值

static void (*current_operation_func)(u8, u8); //定义一个函数指针
//索引表
Main_Menu table[2] =
    {
        // Cur_Index,previous,next,enter,back,(*current_operation)(u8,u8)
        //主界面 展示 当前实时速度 FG频率，期望频率
        {_Main_UI, _Main_UI, _Main_UI, _PWM_Option, _OLED_Lock, Main_UI},
        //主菜单
        {_PWM_Option, _Info_Option, _Bluetooth_Option, _Wifi_Child, _Main_UI, Main_Menu_Func}, // Wifi
                                                                                               //  {_Bluetooth_Option, _Wifi_Option, _Setting_Option, _Bluetooth_Child, _Main_UI, Main_Menu_Func}, //蓝牙
                                                                                               //  {_Setting_Option, _Bluetooth_Option, _Info_Option, _Setting_Child, _Main_UI, Main_Menu_Func},   //设置
                                                                                               //  {_Info_Option, _Setting_Option, _Wifi_Option, _Info_Child, _Main_UI, Main_Menu_Func},           //关于信息

        // //子菜单
        // {_Wifi_Child, _Wifi_Child, _Wifi_Child, _Wifi_Child, _Wifi_Option, Wifi_Child},                               //Wifi子菜单
        // {_Bluetooth_Child, _Bluetooth_Child, _Bluetooth_Child, _Bluetooth_Child, _Bluetooth_Option, Bluetooth_Child}, //蓝牙子菜单
        // {_Setting_Child, _Setting_Child, _Setting_Child, _Setting_Child, _Setting_Option, Setting_Child},             //设置子菜单
        // {_Info_Child, _Info_Child, _Info_Child, _Info_Child, _Info_Option, Info_Child},                               //关于信息子菜单
        // {_OLED_Lock, _Main_UI, _Main_UI, _Main_UI, _Main_UI, OLED_Lock},                                              //OLED熄屏
};
u16 duty_cycle = 0;
u16 pwm1_period = 0;

/* Benchmark variables */
uint16_t frames = 0;
uint16_t fps = 0;
uint16_t loops = 0;
uint16_t loops_overal = 0;

char fps_c[15];

/* SSD1306 demo code */

/*   SSD1306_Bitmap((uint8_t*)picture);
   *   Delay_ms(5000);
   */

void GUI_Init(void)
{
    GFX_SetFont(font_6x4);
    GFX_SetFontSize(1);

    duty_cycle = eeprom_read_data16(PWM_CYCLE_ADD);
    pwm1_period = eeprom_read_data16(PWM_PERIOD_ADD);
}

void GUI_Refresh(void)
{
    u8 key_val = keyRead();
    if (key_val != 0) //只有按键按下才刷屏
    {
        last_index = func_index; //更新上一界面索引值
        switch (key_val)
        {
        case KEY_PREVIOUS:
            func_index = table[func_index].previous; //更新索引值
            break;
        case KEY_ENTER:
            func_index = table[func_index].enter; //更新索引值
            break;
        case KEY_NEXT:
            func_index = table[func_index].next; //更新索引值
            break;
        case KEY_BACK:
            func_index = table[func_index].back; //更新索引值
            break;
        default:
            break;
        }
        LED1_REVERSE;
        // delay_ms(500);
        // OLED_Clear(0); //清屏
    }
    // LED_SYS_REVERSE;
    // delay_ms(500);
    current_operation_func = table[func_index].current_operation;
    (*current_operation_func)(func_index, key_val); //执行当前索引对应的函数
}

/*
函数功能：显示主界面
参数：u8 page_index,u8 key_val
返回值：无
*/
void Main_UI(u8 page_index, u8 key_val)
{
    // if (!timer_1s)
    // {
    //     timer_1s = 1000;
    fps = frames;
    frames = 0;
    loops_overal = loops;
    loops = 0;
    // }

    // sprintf(fps_c, "LOOP: %04d", loops_overal);
    SSD1306_Clear(WHITE);
    GFX_DrawString(0, 0, fps_c, WHITE, BLACK);
    // sprintf(fps_c, " FPS: %02d", fps);
    GFX_DrawString(0, 8, fps_c, WHITE, BLACK);
    frames++;

    GPIO_WriteHigh(TEST_Port, TEST_Pin);
    SSD1306_Display();
    GPIO_WriteLow(TEST_Port, TEST_Pin);
    loops++;
    // uint8_t duty_cycle = eeprom_read_data8(PWM_CYCLE_ADD);
    // uint16_t pwm1_period = eeprom_read_data16(PWM_PERIOD_ADD);
    // printf("duty_cycle${%d}\n", duty_cycle);
    // printf("pwm1_period${%d}\n", pwm1_period);
    // OLED_ShowString(0, 0, "0.91OLEDTEST", 8);
    // atomTimerDelay(500);
    // char buff[20];
    // OLED_Display_Image(0,0,11,7,(char*)Signal_11x7,NO_ALGIN);//显示信号塔
    // OLED_Display_Image(14,0,9,9,(char*)Bluetooth_9x9,NO_ALGIN);//显示蓝牙
    // snprintf(buff,sizeof(buff),"%02d-%02d-%02d",Current_Time.year%100,Current_Time.month,Current_Time.day);
    // OLED_Display_String(0,1,8,8,buff,NORMAL,CEN_ALIGN);//显示日期
    // snprintf(buff,sizeof(buff),"%02d:%02d",Current_Time.hour,Current_Time.min);
    // OLED_Display_String(0,2,16,32,buff,NORMAL,CEN_ALIGN);//显示时间
    // OLED_Display_Image(112,0,16,8,(char*)Bat_16x8,NO_ALGIN);//显示电量
    // OLED_Display_Chinese(40,6,16,16,(char *)(Week_Font_16x16+7*32),NO_ALGIN);//星
    // OLED_Display_Chinese(56,6,16,16,(char *)(Week_Font_16x16+8*32),NO_ALGIN);//期
    // OLED_Display_Chinese(72,6,16,16,(char *)(Week_Font_16x16+(Week_Get()-1)*32),NO_ALGIN);//一~七
    // OLED_Display_Image(0,6,16,16,(char*)Menu_16x16,NO_ALGIN);//显示菜单
    // OLED_Display_Image(112,6,16,16,(char*)Lock_16x16,NO_ALGIN);//显示锁屏
}

void Main_Menu_Func(u8 page_index, u8 key_val)
{
    // 	char buff[20];
    // 	OLED_Display_Image(0, 0, 11, 7, (char *)Signal_11x7, NO_ALGIN);	  //显示信号塔
    // 	OLED_Display_Image(14, 0, 9, 9, (char *)Bluetooth_9x9, NO_ALGIN); //显示蓝牙
    // 	snprintf(buff, sizeof(buff), "%02d:%02d:%02d", Current_Time.hour, Current_Time.min, Current_Time.sec);
    // 	OLED_Display_String(0, 0, 8, 8, buff, NORMAL, CEN_ALIGN);		  //显示时间
    // 	OLED_Display_Image(112, 0, 16, 8, (char *)Bat_16x8, NO_ALGIN);	  //显示电量
    // 	OLED_Display_Image(15, 4, 16, 16, (char *)Left_16x16, NO_ALGIN);  //显示左箭头
    // 	OLED_Display_Image(97, 4, 16, 16, (char *)Right_16x16, NO_ALGIN); //显示右箭头
    // 	OLED_Display_Image(0, 6, 16, 16, (char *)Enter_16x16, NO_ALGIN);  //显示确定
    // 	OLED_Display_Image(112, 6, 16, 16, (char *)Back_16x16, NO_ALGIN); //显示返回箭头
    // 	switch (page_index)
    // 	{
    // 	case _Wifi_Option:
    // 		OLED_Display_Image(0, 2, 48, 48, (char *)Wifi_48x48, CEN_ALIGN); //显示Wifi图标
    // 		break;
    // 	case _Bluetooth_Option:
    // 		OLED_Display_Image(0, 2, 48, 48, (char *)Bluetooth_48x48, CEN_ALIGN); //显示蓝牙图标
    // 		break;
    // 	case _Setting_Option:
    // 		OLED_Display_Image(0, 2, 48, 48, (char *)Setting_48x48, CEN_ALIGN); //显示设置图标
    // 		break;
    // 	case _Info_Option:
    // 		OLED_Display_Image(0, 2, 48, 48, (char *)Info_48x48, CEN_ALIGN); //显示关于信息图标
    // 		break;
    // 	default:
    // 		break;
    // 	}
}