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
#define POTS_PORT GPIOC

#define POTS_MODER (GPIO_MODER_MODE0 | GPIO_MODER_MODE1 |GPIO_MODER_MODE2 |GPIO_MODER_MODE3 | GPIO_MODER_MODE4 |GPIO_MODER_MODE5 |GPIO_MODER_MODE6 | GPIO_MODER_MODE8 | GPIO_MODER_MODE9 | GPIO_MODER_MODE10 | GPIO_MODER_MODE11 | GPIO_MODER_MODE12)
#define ON_1 GPIO_PIN_0
#define CF_1 GPIO_PIN_1
#define BW_1 GPIO_PIN_2
#define ON_2 GPIO_PIN_3
#define CF_2 GPIO_PIN_4
#define BW_2 GPIO_PIN_5
#define ON_3 GPIO_PIN_6
#define CF_3 GPIO_PIN_8
#define BW_3 GPIO_PIN_9
#define SF_3 GPIO_PIN_10
#define SF_3 GPIO_PIN_11
#define ATTACK GPIO_PIN_12
//PF
#define AUDIO_MODER (GPIO_MODER_MODE0 | GPIO_MODER_MODE1 |GPIO_MODER_MODE2 |GPIO_MODER_MODE3 | GPIO_MODER_MODE5 |GPIO_MODER_MODE6 |GPIO_MODER_MODE7 | GPIO_MODER_MODE8 | GPIO_MODER_MODE9 | GPIO_MODER_MODE12 | GPIO_MODER_MODE13)
#define AUDIO_PORT GPIOF
#define DECAY GPIO_PIN_0
#define SUSTAIN GPIO_PIN_1
#define RELEASE GPIO_PIN_2
#define VOLUME GPIO_PIN_3
#define IN_1 GPIO_PIN_5
#define IN_2 GPIO_PIN_6
#define IN_3 GPIO_PIN_7
#define OUT_1 GPIO_PIN_8
#define OUT_2 GPIO_PIN_9
#define OUT_3 GPIO_PIN_12
#define AUDIO_IN GPIO_PIN_13

void Error_Handler(void);
#ifdef __cplusplus
}
#endif

#endif
