/*******************************************************************************
 * EE 329 A5 DAC INTERFACE
 *******************************************************************************
 * @file           : dac.c
 * @brief          : A5
 * project         : EE 329 S'25 Assignment 7
 * author1         : Rodrigo
 * author2         : Ryan Niemi reniemi@calpoly.edu
 *
 * version         : 0.1
 *
 * date            : 5/15/25
 * compiler        : STM32CubeIDE v.1.12.0 Build: 14980_20230301_1550 (UTC)
 * target          : NUCLEO-L4A6ZG
 * clocks          : 4 MHz MSI to AHB2
 * @attention      : (c) 2023 STMicroelectronics.  All rights reserved.
 *******************************************************************************
 *******************************************************************************
 * REVISION HISTORY
 * 0.1 - 15/5/25 wrote DAC interface
 *******************************************************************************
 * TODO: NA
 *******************************************************************************
*/
#include "dac.h"
// GPIO A PA7 PA5 PA4
void GPIO_Init_DAC( void ) {
	// enable clock for GPIOA & SPI1
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN);                // GPIOA: DAC NSS/SCK/SDO
	RCC->APB2ENR |= (RCC_APB2ENR_SPI1EN);                 // SPI1 port
	// configure PICO:
	DAC_PORT->MODER   &= ~(GPIO_MODER_MODE7); // clear bits mode
	DAC_PORT->MODER   |= (GPIO_MODER_MODE7_1); // specify AF mode
	DAC_PORT->OTYPER  &= ~(GPIO_OTYPER_OT7); // specify push-pull
	DAC_PORT->PUPDR   &= ~(GPIO_PUPDR_PUPD7); // clears bits
	DAC_PORT->OSPEEDR |=  (3 << GPIO_OSPEEDR_OSPEED7_Pos); // high speed
	DAC_PORT->AFR[0] &= ~((0x000F << GPIO_AFRL_AFSEL7_Pos)); // clear nibble for bit 7 AF
	DAC_PORT->AFR[0] |=  ((0x0005 << GPIO_AFRL_AFSEL7_Pos)); // set b7 AF to SPI1 (fcn 5)

    // configure SCLK:
	DAC_PORT->MODER   &= ~(GPIO_MODER_MODE5); // clear bits mode
	DAC_PORT->MODER   |= (GPIO_MODER_MODE5_1); // specify AF mode
	DAC_PORT->OTYPER  &= ~(GPIO_OTYPER_OT5); // specify push-pull
	DAC_PORT->PUPDR   &= ~(GPIO_PUPDR_PUPD5); // clears bits
	DAC_PORT->OSPEEDR |=  (3 << GPIO_OSPEEDR_OSPEED5_Pos); // high speed
	DAC_PORT->AFR[0] &= ~((0x000F << GPIO_AFRL_AFSEL5_Pos)); // clear nibble for bit 5 AF
	DAC_PORT->AFR[0] |=  ((0x0005 << GPIO_AFRL_AFSEL5_Pos)); // set b5 AF to SPI1 (fcn 5)
	// configure NCS
	DAC_PORT->MODER   &= ~(GPIO_MODER_MODE4); // clear bits mode
	DAC_PORT->MODER   |= (GPIO_MODER_MODE4_1); // specify AF mode
	DAC_PORT->OTYPER  &= ~(GPIO_OTYPER_OT4); // specify push-pull
	DAC_PORT->PUPDR   &= ~(GPIO_PUPDR_PUPD4); // clears bits
	DAC_PORT->OSPEEDR |=  (3 << GPIO_OSPEEDR_OSPEED4_Pos); // high speed
	DAC_PORT->AFR[0] &= ~((0x000F << GPIO_AFRL_AFSEL4_Pos)); // clear nibble for bit 4 AF
	DAC_PORT->AFR[0] |=  ((0x0005 << GPIO_AFRL_AFSEL4_Pos)); // set b4 AF to SPI1 (fcn 5)
}

uint16_t DAC_volt_conv(uint16_t milliVolts) {
	//determines if mvolts requested is above/below 1/2 threshold
	uint16_t data;
	milliVolts = (10000/9635)*(milliVolts + 150); //trendline from calibration and manual tweaking

	if(milliVolts < 2048){ // if value is less than 2048, which is less than Vref, gain is assumed 1
		data = (milliVolts * 4095)/2048;
		return ((0x3 << 12) | (data & 0xFFF)); // gain is 1 (so write (0b0011)
	}
	else if(milliVolts <= 3300 ){ // if value is greater than Vref but less than VDD, assume gain is 2
		data = (milliVolts * 4095)/(2*2048);
		return ((0x1 << 12) | (data & 0xFFF)); // gain is 2 (so write 0b0001)
	}
	else{
		return ((0x1 << 12) | 0xDAC); // where DAC is 3500
                // issue encounted where 3300 would write out less than 3300mV onto D.A.C.
                // funny coincidence that 3500 = 0xDAC is value that makes the D.A.C. work
	}
}

void DAC_write( uint16_t milliVolts ) {
	SPI1->DR = DAC_volt_conv(milliVolts); //  write to SPI Tx

	while (!(SPI1->SR & SPI_SR_TXE)); // wait for Tx to be half empty before writing
	while (SPI1->SR & SPI_SR_BSY);

}
