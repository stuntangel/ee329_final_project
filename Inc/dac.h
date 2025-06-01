#include "stm32l4xx_hal.h"

#define DAC_PORT GPIOA // use GPIOA for DAC
#define PICO GPIO_PIN_7
#define SCLK GPIO_PIN_5
#define NCS GPIO_PIN_4 // NCS = ~CS

// these were not used but still useful to reference
#define WRITE_TO_DAC 0 // bit 16
#define BUFFERED 0 // bit 15
#define ENABLE_DOUBLE_GAIN 0 // bit 14
#define ACTIVE_MODE 1 // bit 13

void GPIO_Init_DAC( void );
void DAC_write( uint16_t milliVolts );
uint16_t DAC_volt_conv(uint16_t milliVolts);
