/* USER CODE BEGIN Header */
/*******************************************************************************
 * EE 329 A5 SPI DAC
 *******************************************************************************
 * @file           : delay.c
 * @brief          : delay module source file
 * project         : EE 329 S'25 Assignment 5
 * authors         : AJ Gregory
 * 						Soumil Joshi
 * version         : 0.0
 * date            : 05.14.25
 * compiler        : STM32CubeIDE v.1.18.0 Build: 14980_20230301_1550 (UTC)
 * target          : NUCLEO-L4A6ZG
 * clocks          : 4 MHz MSI to AHB2
 * @attention      : (c) 2023 STMicroelectronics.  All rights reserved.
 *******************************************************************************
 USER CODE END Header */

 #include "delay.h"
 #include "main.h"
 //#include "stm32l4xx.h"     // ← Must be first for STM32 registers (RCC, GPIOx, etc.)
 //#include <stdint.h>        // ← Needed for uint32_t, int8_t, etc.
 
 void SysTick_Init(void)
 {
     SysTick->CTRL |= (SysTick_CTRL_ENABLE_Msk |     	// enable SysTick Timer
                       SysTick_CTRL_CLKSOURCE_Msk); 	// select CPU clock
     SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk);  	// disable interrupt
 }
 
 // delay in microseconds using SysTick timer to count CPU clock cycles
 // do not call with 0 : error, maximum delay.
 // careful calling with small nums : results in longer delays than specified:
 //	   e.g. @4MHz, delay_us(1) = 10-15 us delay.
 void delay_us(const uint32_t time_us)
 {
     // set the counts for the specified delay
     SysTick->LOAD = (uint32_t)((time_us * (SystemCoreClock / 1000000)) - 1);
     SysTick->VAL = 0;                                  	 // clear timer count
     SysTick->CTRL &= ~(SysTick_CTRL_COUNTFLAG_Msk);    	 // clear count flag
     while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)); // wait for flag
 }
 