#include "stm32l4xx_hal.h"
#include "stm32l4xx_it.h"
#define FALSE 0
#define TRUE (!FALSE)

uint32_t grabConvertedResult( void );
uint8_t checkFlag( void );
void ADC_init( void );
