#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l4xx_hal.h"
#include "stm32l4xx_it.h"
#include "filter_utils.h"
#include "adc.h"
#include "DAC.h"
#include "timer.h"
#include <math.h>
#include "filters.h"
// PC
#define POTS_PORT GPIOF
  // PF{3,4,5,6,7,8,9,10,14,15} 
#define AUDIO_DIGITAL_MODER (GPIO_MODER_MODE0 | GPIO_MODER_MODE1 | GPIO_MODER_MODE2)
#define AUDIO_DIGITAL_OTYPER (GPIO_OTYPER_OT0 | GPIO_OTYPER_OT1 | GPIO_OTYPER_OT2)
#define AUDIO_DIGITAL_PUPDR (GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1 | GPIO_PUPDR_PUPD2)
#define AUDIO_DIGITAL_OSPEEDR ((3 << GPIO_OSPEEDR_OSPEED0_Pos) | (3 << GPIO_OSPEEDR_OSPEED1_Pos) | (3 << GPIO_OSPEEDR_OSPEED2_Pos))
#define POTS_ANALOG_MODER (GPIO_MODER_MODE3 |GPIO_MODER_MODE4 | GPIO_MODER_MODE5 |GPIO_MODER_MODE6 | GPIO_MODER_MODE7 | GPIO_MODER_MODE8 | GPIO_MODER_MODE9 | GPIO_MODER_MODE10 | GPIO_MODER_MODE14 | GPIO_MODER_MODE15)
#define ON_1 GPIO_PIN_0
#define CF_1 GPIO_PIN_3
#define BW_1 GPIO_PIN_4
#define ON_2 GPIO_PIN_1
#define CF_2 GPIO_PIN_5
#define BW_2 GPIO_PIN_6
#define ON_3 GPIO_PIN_2
#define CF_3 GPIO_PIN_7
#define BW_3 GPIO_PIN_8
#define SF_3 GPIO_PIN_9
#define ATTACK GPIO_PIN_10
#define AUDIO_ANALOG_MODER ( GPIO_MODER_MODE5 | GPIO_MODER_MODE6)
#define AUDIO_PORT GPIOC
#define DECAY GPIO_PIN_14
#define SUSTAIN GPIO_PIN_15
#define RELEASE GPIO_PIN_5
#define VOLUME GPIO_PIN_6

#define SAMPLES (256)
// Sample snapshot length (microseconds)
#define SAMPLE_TIME (33333)
// Single sample period (microseconds)
#define SAMPLE_INTVL (SAMPLE_TIME / SAMPLES)


void GPIO_init_pins( void );
float voltsToHz( void );
void Error_Handler(void);
#ifdef __cplusplus
}
#endif

#endif
