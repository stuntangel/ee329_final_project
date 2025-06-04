#include "stm32l4xx_hal.h"
#define PERIOD 0xFFFFFFFF
#define ONEMICRO 4
#define FALSE 0
#define TRUE (!FALSE)
void Timer_setup_TIM2( int iDutyCycle );
void TIM2_IRQHandler(void);
uint8_t checkTimerFlag( void );
