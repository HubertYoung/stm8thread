#include "pwm.h"

void PWM_Init(void){
     // input clk is 2Mhz -> period = 52 means the pwm freq is 2Mhz / 52 ~= 38.5Khz
  uint16_t pwm1_period = 667;
  uint8_t duty_cycle = 98;
  // time_pulse = (period*duty/100) - 1;
  uint16_t tim_pulse =  (pwm1_period * duty_cycle / 100) - 1; //脉冲宽度值

  TIM1_DeInit();
  TIM1_TimeBaseInit(0, TIM1_COUNTERMODE_UP, pwm1_period, 0);
  TIM1_OC2Init(TIM1_OCMODE_PWM2, 
                TIM1_OUTPUTSTATE_ENABLE,
                TIM1_OUTPUTNSTATE_ENABLE,
                tim_pulse, TIM1_OCPOLARITY_LOW,
                TIM1_OCNPOLARITY_LOW, 
                TIM1_OCIDLESTATE_SET, 
                TIM1_OCNIDLESTATE_RESET);
  TIM1_Cmd(ENABLE);
  TIM1_CtrlPWMOutputs(ENABLE);

/* Time base configuration */
//   TIM2_DeInit();
//   TIM2_TimeBaseInit(TIM2_PRESCALER_32768, 51);

//   /* PWM1 Mode configuration: Channel2 */ 
//   TIM2_OC2Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE,24, TIM2_OCPOLARITY_HIGH);
//   TIM2_OC2PreloadConfig(ENABLE);

//   TIM2_ARRPreloadConfig(ENABLE);

//   /* TIM2 enable counter */
//   TIM2_Cmd(ENABLE);
}