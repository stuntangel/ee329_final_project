/* USER CODE BEGIN Header */
/*******************************************************************************
 * EE 329 A5 SPI DAC
 *******************************************************************************
 * @file           : delay.h
 * @brief          : delay module header file
 * project         : EE 329 S'25 Assignment 5
 * authors         : AJ Gregory
 * 						Soumil Joshi
 * version         : 0.0
 * date            : 05.12.25
 * compiler        : STM32CubeIDE v.1.18.0 Build: 14980_20230301_1550 (UTC)
 * target          : NUCLEO-L4A6ZG
 * clocks          : 4 MHz MSI to AHB2
 * @attention      : (c) 2023 STMicroelectronics.  All rights reserved.
 *******************************************************************************
 USER CODE END Header */

 #ifndef DELAY_H_
 #define DELAY_H_
 
 //#include "main.h"
 #include <stdint.h>        // ← Needed for uint32_t, int8_t, etc.
 
 void SysTick_Init(void);
 void delay_us(const uint32_t time_us);
 
 #endif /* INC_DELAY_H_ */
 