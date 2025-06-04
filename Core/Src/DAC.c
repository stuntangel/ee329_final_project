/* USER CODE BEGIN Header */
/*******************************************************************************
 * EE 329 A5 SPI DAC
 *******************************************************************************
 * @file           : DAC.c
 * @brief          : DAC module source file
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
 * WIRING DIAGRAM:
 * STM32 - NUCLEO L4A6ZG - MCP4821 DAC
 * PA4	- CN7-9  (D20)	 - NCS (2)
 * PA5	- CN7-10 (D13)	 - SCK (3)
 * PA7	- CN7-14 (D11)	 - SDI (4)
 *******************************************************************************
 USER CODE END Header */


#include "DAC.h"
#include "main.h"

/* -----------------------------------------------------------------------------
 * function : DAC_Write_Volt()
 * action   : Full function to request an analogue voltage from the DAC
 * args		: voltage - the decimal ("analogue") voltage
 * 				to output from DAC, in mV
 * authors  : AJ Gregory
 * version  : 0.0
 * date     : 05.14.25
 * -------------------------------------------------------------------------- */
void DAC_Write_Volt(int voltage)
{
	// Calculate need for output gain on DAC
	int gain;
	if(voltage >= MAX_VOLT)
	{
		voltage = 3300 / 2;
		voltage = (int) (((float) voltage) - 0.5793);
		voltage = (int) (((float) voltage) / 0.4025);

		gain = 2;
	}
	else if((voltage >= 2048) & (voltage <= MAX_VOLT))
	{
		// y = 0.402536x + 0.5793
		// Fix upper level (with gain)
		voltage = (int) (((float) voltage) - 0.5793);
		voltage = (int) (((float) voltage) / 0.4025);

		voltage /= 2;
		gain = 2;
	}
	else
	{
		// y = 0.403318x - 1.30559
		voltage = (int) (((float) voltage) + 1.30559);
		voltage = (int) (((float) voltage) / 0.40331);

		gain = 1;
	}

	uint16_t data = (uint16_t) ((MAX_VOLT * voltage) / MAX_DIG);
	DAC_Write(data, gain);
}

/* -----------------------------------------------------------------------------
 * function : DAC_Write()
 * action   : Writes the data specified to the DAC via SPI
 * args		: data - 12 bit data to write to DAC
 * authors  : AJ Gregory
 * version  : 0.0
 * date     : 05.14.25
 * -------------------------------------------------------------------------- */
void DAC_Write(uint16_t data, int gain)
{
	uint16_t txData = 0;
	// Build transmission packet
	if(gain == 2)
		txData = (0b0001 << 12) | data;
	else
		txData = (0b0011 << 12) | data;

	// Wait for transmit buffer to clear
	while (!(SPI1->SR & SPI_SR_TXE));

	// Place packet in TX buffer
	SPI1->DR |= txData;
}


/* -----------------------------------------------------------------------------
 * function : DAC_Init()
 * action   : Initializes MCP4821 DAC
 * authors  : AJ Gregory
 * version  : 0.0
 * date     : 05.14.25
 * -------------------------------------------------------------------------- */
void DAC_Init(void)
{
	// enable clock for GPIOA & SPI1
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN);		// GPIOA: DAC NSS/SCK/SDO
	RCC->APB2ENR |= (RCC_APB2ENR_SPI1EN);		// SPI1 port

	// Clear MODER
	DAC_PORT->MODER &= ~(GPIO_MODER_MODE4 |
								GPIO_MODER_MODE5 |
								GPIO_MODER_MODE7);
	// Set Pins to mode 2 (alt. func.)
	DAC_PORT->MODER |=  (GPIO_MODER_MODE4_1 |
								GPIO_MODER_MODE5_1 |
								GPIO_MODER_MODE7_1);

	// Clear PUPDR
	DAC_PORT->PUPDR &= ~(GPIO_PUPDR_PUPD4 |
								GPIO_PUPDR_PUPD5 |
								GPIO_PUPDR_PUPD7);

	// Clear OTYPER
	DAC_PORT->OTYPER &= ~(DAC_PINS);

	// Clear OSPEEDR
	DAC_PORT->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED4 |
								  GPIO_OSPEEDR_OSPEED5 |
								  GPIO_OSPEEDR_OSPEED7);
	// Set each pin to high speed
	DAC_PORT->OSPEEDR |=  (GPIO_OSPEEDR_OSPEED4 |
								  GPIO_OSPEEDR_OSPEED5 |
								  GPIO_OSPEEDR_OSPEED7);

	// clear nibbles for bits 4, 5, 7 AFR
	DAC_PORT->AFR[0] &= ~(GPIO_AFRL_AFSEL4 |
								 GPIO_AFRL_AFSEL5 |
								 GPIO_AFRL_AFSEL7);
	// set bits 4, 5, 7 AF to SPI1 (fcn 5)
	DAC_PORT->AFR[0] |=  ((0x5 << GPIO_AFRL_AFSEL4_Pos) |
								 (0x5 << GPIO_AFRL_AFSEL5_Pos) |
								 (0x5 << GPIO_AFRL_AFSEL7_Pos));

	// Initialize SPI for DAC
	SPI_Init();
}

/* -----------------------------------------------------------------------------
 * function : SPI_Init()
 * action   : Initializes SPI communications on STM32
 * authors  : AJ Gregory
 * version  : 0.0
 * date     : 05.14.25
 * -------------------------------------------------------------------------- */
void SPI_Init(void)
{
	// SPI config as specified @ STM32L4 RM0351 rev.9 p.1459
	// called by or with DAC_init()
	// build control registers CR1 & CR2 for SPI control of peripheral DAC
	// assumes no active SPI xmits & no recv data in process (BSY=0)
	// CR1 (reset value = 0x0000)
	SPI1->CR1 &= ~(SPI_CR1_SPE);			// disable SPI for config
	SPI1->CR1 &= ~(SPI_CR1_RXONLY);		// recv-only OFF
	SPI1->CR1 &= ~(SPI_CR1_LSBFIRST);	// data bit order MSb:LSb
	SPI1->CR1 &= ~(SPI_CR1_CPOL |
						SPI_CR1_CPHA);			// SCLK polarity:phase = 0:0
	SPI1->CR1 |=	SPI_CR1_MSTR;			// MCU is SPI controller
	// CR2 (reset value = 0x0700 : 8b data)
	SPI1->CR2 &= ~(SPI_CR2_TXEIE |
						SPI_CR2_RXNEIE);		// disable FIFO intrpts
	SPI1->CR2 &= ~(SPI_CR2_FRF);			// Moto frame format
	SPI1->CR2 |=	SPI_CR2_NSSP;			// auto-generate NSS pulse
	SPI1->CR2 |=	SPI_CR2_DS;				// 16-bit data
	SPI1->CR2 |=	SPI_CR2_SSOE;			// enable SS output
	// CR1
	SPI1->CR1 |=	SPI_CR1_SPE;			// re-enable SPI for ops
}
