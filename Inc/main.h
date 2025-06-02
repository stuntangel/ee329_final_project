#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l4xx_hal.h"
#include "stm32l4xx_it.h"

#define FALSE (0)
#define TRUE !FALSE
#define SAMPLES (256)
// Sample snapshot length (microseconds)
#define SAMPLE_TIME (250)
// Single sample period (nanoseconds)
#define SAMPLE_INTVL (SAMPLES / (SAMPLE_TIME * 1000))

void Error_Handler(void);
#ifdef __cplusplus
}
#endif

#endif
