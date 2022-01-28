
#include <stm8s.h>
#include <stm8s_gpio.h>
#include <pwm.h>
#include <key.h>
#include <eeprom.h>
// #include <oled.h>
#include "hw_setup.h"
#include "sys.h"
#include "oled.h"
#include "SOLGUI_Include.h"

u32 switch_reg = 0;
MENU_PAGE wifi_list, wifi_init, wifi_conn, tcp_conn, senddata;

__M_PAGE(wifi_list, "wifi_list", PAGE_NULL,
         {
             SOLGUI_Cursor(6, 0, 4);
             SOLGUI_Widget_GotoPage(0, &wifi_init);
             SOLGUI_Widget_GotoPage(1, &wifi_conn);
             SOLGUI_Widget_GotoPage(2, &tcp_conn);
             SOLGUI_Widget_GotoPage(3, &senddata);
         });
__M_PAGE(wifi_init, "wifi_init", &wifi_list,
         {
             SOLGUI_Cursor(0, 0, 1);
             SOLGUI_Widget_Switch(0, "WIFI init ON/OFF", &switch_reg, 0);
         });
__M_PAGE(wifi_conn, "wifi_conn", &wifi_list,
         {
             SOLGUI_Cursor(0, 0, 1);
             SOLGUI_Widget_Switch(0, "WIFI connection ON/OFF", &switch_reg, 1);
         });
__M_PAGE(tcp_conn, "tcp_conn", &wifi_list,
         {
             SOLGUI_Cursor(0, 0, 1);
             SOLGUI_Widget_Switch(0, "TCP connection ON/OFF", &switch_reg, 2);
         });
__M_PAGE(senddata, "senddata", &wifi_list,
         {
             SOLGUI_Cursor(0, 0, 1);
             SOLGUI_Widget_Switch(0, "send mode ON/OFF", &switch_reg, 3);
         });
u8 key = 0;
void main(void)
{
    /* STM8 configuration */
    MCU_Config();

    eeprom_init();

    KEY_Init();

    SOLGUI_Init(&wifi_list);

    PWM_Init();

    while (1)
    {
        key = KEY_Scan(0);
        SOLGUI_InputKey(key);
        SOLGUI_Menu_PageStage();
        SOLGUI_Refresh();

        delay_ms(10);
        // GUI_Refresh();
    }
}
