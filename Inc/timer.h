#include "stm32l4xx_hal.h"
#define PERIOD 0xFFFFFFFF
#define ONESEC (1000*4000)
#define FIVEKHZ 800 // 800 is 5kHz or at 200micro seconds
#define MILLI250 (FIVEKHZ*1250) // 200micro * 1250 = 250 milli seconds
#define MILLI20 (FIVEKHZ*100) // 200micro * 100 = 20 milli seconds
#define DUTY25PER (FIVEKHZ/4)
#define DUTY50PER (FIVEKHZ/2)

void Timer_setup_TIM2( int iDutyCycle );
void TIM2_IRQHandler(void);
