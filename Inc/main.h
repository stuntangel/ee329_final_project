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
#define SAMPLE_TIME (33333)
// Single sample period (microseconds)
#define SAMPLE_INTVL (SAMPLE_TIME / SAMPLES)

void Error_Handler(void);
#ifdef __cplusplus
}
#endif

#endif
