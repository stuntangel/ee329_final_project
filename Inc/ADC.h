/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : ADC.h
  * @brief          : Header for ADC.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#ifndef INC_ADC_H_
#define INC_ADC_H_

// Module Includes
#include "main.h"
#include "delay.h"

// Function Prototypes
void ADC_Init(void);
void ADC1_2_IRQHandler(void);
uint32_t ADC_Read(void);

// Global flag
extern uint8_t  ADC_CONV_RDY;

#endif /* INC_ADC_H_ */
