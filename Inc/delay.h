/*
 * delay.h
 *
 *  Created on: Apr 20, 2025
 *      Author: sakik
 */

#ifndef INC_DELAY_H_
#define INC_DELAY_H
#include "stm32l4xx_hal.h"
void SysTick_Init(void);
void delay_us(const uint32_t time_us);


#endif /* INC_DELAY_H_ */
