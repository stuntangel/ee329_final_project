/* USER CODE BEGIN Header */
/*******************************************************************************
 * EE 329 A5 SPI DAC
 *******************************************************************************
 * @file           : DAC.h
 * @brief          : DAC module header file
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

 #ifndef DAC_H_
 #define DAC_H_
 
 //#include "main.h"
 #include <stdint.h>
 
 // Prototypes
 void DAC_Init(void);
 void SPI_Init(void);
 void DAC_Write(uint16_t data, int gain);
 void DAC_Write_Volt(int voltage);
 
 // Ports and Pins
 #define DAC_PORT (GPIOA)
 #define DAC_PINS (GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7)
 
 // MCP4821 Specs
 #define MAX_VOLT (3300)
 #define MIN_VOLT (0)
 #define MAX_DIG  (0xFFF)
 #define MIN_DIG  (0x0)
 
 #endif /* INC_DAC_H_ */
 
