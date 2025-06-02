/* USER CODE BEGIN Header */
/*******************************************************************************
 * EE 329 A8 ADC
 *******************************************************************************
 * @file           : ADC.c
 * @brief          : Useful functions for Analog to Digital Converter (ADC)
 * project         : EE 329 S'25 Assignment 8
 * authors         : AJ Gregory
 * 						Chase Kadooka
 * version         : 0.0
 * date            : 05.27.25
 * compiler        : STM32CubeIDE v.1.18.0 Build: 14980_20230301_1550 (UTC)
 * target          : NUCLEO-L4A6ZG
 * clocks          : 4 MHz MSI to AHB2
 * @attention      : (c) 2023 STMicroelectronics.  All rights reserved.
 *******************************************************************************
 * WIRING DIAGRAM:
 *******************************************************************************
 USER CODE END Header */

 #include "ADC.h"

 // Conversion variable (module globality)
 static uint32_t ADC_convert = 0;
 uint8_t  ADC_CONV_RDY = 0;
 
 /* -----------------------------------------------------------------------------
  * function : ADC_Init()
  * action   : Initialize ADC for Port A Pin 0
  * authors  : AJ Gregory
  * version  : 0.0
  * date     : 05.27.25
  * -------------------------------------------------------------------------- */
 void ADC_Init()
 {
     // turn on clock for ADC
     RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
 //	RCC->CFGR |= (0x0 << RCC_CFGR_HPRE_Pos);		// Maybe needed according to RM
 
     // power up & calibrate ADC - see RM 18.4.6
     ADC123_COMMON->CCR |= (1 << ADC_CCR_CKMODE_Pos);	// clock source = HCLK/1
     ADC1->CR &= ~(ADC_CR_DEEPPWD);						// disable deep-power-down
     ADC1->CR |= (ADC_CR_ADVREGEN);						// enable V regulator
     delay_us(20);											// wait 20us for ADC to power up
     ADC1->DIFSEL &= ~(ADC_DIFSEL_DIFSEL_5);		// PA0=ADC1_IN5, single-ended
     ADC1->CR &= ~(ADC_CR_ADEN | ADC_CR_ADCALDIF);// disable ADC, single-end calib
     ADC1->CR |= ADC_CR_ADCAL;							// start calibration
     while (ADC1->CR & ADC_CR_ADCAL);					// wait for calib to finish
 
     // enable ADC
     ADC1->ISR |= (ADC_ISR_ADRDY);					// set to clr ADC Ready flag
     ADC1->CR  |= ADC_CR_ADEN;						// enable ADC
     while(!(ADC1->ISR & ADC_ISR_ADRDY));		// wait for ADC Ready flag
     ADC1->ISR |= (ADC_ISR_ADRDY);					// set to clr ADC Ready flag
 
     // configure ADC sampling & sequencing
     ADC1->SQR1  |= (5 << ADC_SQR1_SQ1_Pos);	// sequence = 1 conv., ch 5
     ADC1->SMPR1 |= (7 << ADC_SMPR1_SMP5_Pos);	// ch 5 sample time = 47.5 clocks
     ADC1->CFGR  &= ~(ADC_CFGR_CONT  |			// single conversion mode
                           ADC_CFGR_EXTEN |			// h/w trig disabled for s/w trig
                           ADC_CFGR_RES);				// 12-bit resolution
 
     // configure & enable ADC interrupt
     ADC1->IER |= ADC_IER_EOCIE;					// enable end-of-conv interrupt
     ADC1->ISR |= ADC_ISR_EOC;						// set to clear EOC flag
     NVIC->ISER[0] = (1<<(ADC1_2_IRQn & 0x1F));// enable ADC interrupt service
     __enable_irq();									// enable global interrupts
 
     // configure GPIO pin PA0
     RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN);		// connect clock to GPIOA
     GPIOA->MODER |= (GPIO_MODER_MODE0);			// analog mode for PA0
 
     ADC1->CR |= ADC_CR_ADSTART;					// start 1st conversion
 }
 
 /* -----------------------------------------------------------------------------
  * function : ADC_Read()
  * action   : Read last value retrieved from ADC
  * ret		: Last converted value from ADC
  * authors  : AJ Gregory
  * version  : 0.0
  * date     : 05.27.25
  * -------------------------------------------------------------------------- */
 uint32_t ADC_Read(void)
 {
     return ADC_convert;
 }
 
 /* -----------------------------------------------------------------------------
  * function : ADC1_2_IRQHandler()
  * action   : Interrupt for end of conversion of ADC
  * authors  : AJ Gregory
  * version  : 0.0
  * date     : 05.27.25
  * -------------------------------------------------------------------------- */
 void ADC1_2_IRQHandler(void)
 {
     // End of conversion flag
     if(ADC1->ISR & ADC_ISR_EOC)
     {
         // Global conversion result
         ADC_convert = ADC1->DR;
 
         // Global flag for conversion ready
         ADC_CONV_RDY = 1;
 
         // Reset EOC Flag
         ADC1->ISR &= ~(ADC_ISR_EOC);
     }
 
     //while(!(ADC1->ISR & ADC_ISR_ADRDY));		// wait for ADC Ready flag
     ADC1->CR |= ADC_CR_ADSTART;					// start 1st conversion
 }
 