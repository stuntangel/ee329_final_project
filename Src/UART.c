#include "UART.h"
#include "delay.h"
#include "timer.h"

void UART_init( void ) {
	PWR->CR2 |= (PWR_CR2_IOSV);              // power avail on PG[15:2] (LPUART1)
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOGEN);   // enable GPIOG clock
	RCC->APB1ENR2 |= RCC_APB1ENR2_LPUART1EN; // enable LPUART clock bridge
	/* USER: configure GPIOG registers MODER/PUPDR/OTYPER/OSPEEDR then
		 select AF mode and specify which function with AFR[0] and AFR[1] */
	// PG7 --> Tx --> Output
	GPIOG->MODER   &= ~(GPIO_MODER_MODE7); // MAKES BITS ZERO so INPUT MODE DEFAULT
	GPIOG->MODER   |=  (GPIO_MODER_MODE7_1); //specify alternate function mode
	GPIOG->OTYPER  &= ~(GPIO_OTYPER_OT7); //specify push-pull
	GPIOG->PUPDR   &= ~(GPIO_PUPDR_PUPD7); // reset all current pull up pull down choices
	GPIOG->OSPEEDR |= (GPIO_OSPEEDR_OSPEED7); //high speed
	// sets the PG7 which is in AFRL to use alternate function 8
	GPIOG->AFR[0]  |= (0x8 << GPIO_AFRL_AFSEL7_Pos); // select LPUART Tx (AF8) on PG7 (AFRL)
	// PG8 --> Rx --> Input
	GPIOG->MODER   &= ~(GPIO_MODER_MODE8); // MAKE BITS ZERO so INPUT MODE DEFAULT
	GPIOG->MODER   |=  (GPIO_MODER_MODE8_1); //specify alternate function mode
	GPIOG->OTYPER  &= ~(GPIO_OTYPER_OT8); // specify push-pull
	GPIOG->PUPDR   &= ~(GPIO_PUPDR_PUPD8_Pos); // clears bits
	GPIOG->OSPEEDR |= (GPIO_OSPEEDR_OSPEED8); // high speed
	// sets the PG8 which is in AFRH to use alternate function 8
	GPIOG->AFR[1]  |= (0x8 << GPIO_AFRH_AFSEL8_Pos); // select LPUART Rx (AF8) on PG8 (AFRH) 
	// DEBUG: new code above
	LPUART1->CR1 &= ~(USART_CR1_M1 | USART_CR1_M0); // 8-bit data 
	LPUART1->CR1 |= USART_CR1_UE;                   // enable LPUART1
	LPUART1->CR1 |= (USART_CR1_TE | USART_CR1_RE);  // enable xmit & recv
	LPUART1->CR1 |= USART_CR1_RXNEIE;        // enable LPUART1 recv interrupt
	LPUART1->ISR &= ~(USART_ISR_RXNE);       // clear Recv-Not-Empty flag
	/* USER: set baud rate register (LPUART1->BRR) */
	LPUART1->BRR = LPUARTDIV; // set baud rate register 
	// DEBUG: new code above
	NVIC->ISER[2] = (1 << (LPUART1_IRQn & 0x1F));   // enable LPUART1 ISR
	__enable_irq();                          // enable global interrupts
}

void LPUART1_IRQHandler( void  ) {
   uint8_t charRecv;
   if (LPUART1->ISR & USART_ISR_RXNE) {
      charRecv = LPUART1->RDR;
      switch ( charRecv ) {
				case 'R': // change text red
					LPUART_ESC_Print(RECOLOR, RED);
					break;
				default:
					while( !(LPUART1->ISR & USART_ISR_TXE) )
						;    // wait for empty TX buffer
		LPUART1->TDR = charRecv;  // echo char to terminal
	}  // end switch
   }
}

void LPUART_Intro( void ) {
	  // ADC counts volts
	  // MIN  0000  0.000 V
	  // MAX  0000  0.000 V
	  // AVG  0000  0.000 V
	  LPUART_ESC_Print(TOPLEFT, DONT_CARE); // move to topleft
	  UART_Print("ADC counts volts");
	  LPUART_ESC_Print(TOPLEFT, DONT_CARE);
	  LPUART_ESC_Print(DOWN, "1");
	  UART_Print("MIN 0000 0.000 V");
	  LPUART_ESC_Print(TOPLEFT, DONT_CARE);
	  LPUART_ESC_Print(DOWN, "2");
	  UART_Print("MAX 0000 0.000 V");
	  LPUART_ESC_Print(TOPLEFT, DONT_CARE);
	  LPUART_ESC_Print(DOWN, "3");
	  UART_Print("AVG 0000 0.000 V");
	  LPUART_ESC_Print(TOPLEFT, DONT_CARE);
}

void LPUART_ESC_Print(ACTIONS action, char* value) {
	UART_Print("\x1B["); // escape key code
	switch (action) {
	  case RIGHT: // move to the right by value
			UART_Print(value);
			UART_Print(RIGHT_ESC_CODE);
			break;
		case LEFT:
			UART_Print(value); // move to the left by value
			UART_Print(LEFT_ESC_CODE);
			break;
		case CLEAR:
			UART_Print("2J"); // clear the whole screen
			break;
		case DOWN:
			UART_Print(value); // move down by value
			UART_Print(DOWN_ESC_CODE);
			break;
		case UP:
			UART_Print(value); // move up by value
			UART_Print(UP_ESC_CODE);
			break;
		case RECOLOR:
			UART_Print("3"); // recolor to certain color by value
			UART_Print(value);
			UART_Print("m");
			break;
		case BLINK:
			UART_Print("5m"); // appears as highlighted on my terminal
	 		break;
		case DONT_BLINK:
			UART_Print("0m"); // stops blinking the code
			break;
		case TOPLEFT:
			UART_Print(MOVE_ESC_CODE); // move to far topleft of screen
			break;
		case BOLD:
			UART_Print("1m"); // make text bold
	}

}

char* UART_num_to_counts(const uint16_t value ) {
	uint8_t ones = 0x30+(value % 10);
	uint8_t tens = 0x30+((value / 10) % 10);
	uint8_t hundreds = 0x30+((value / 100) % 10);
	uint8_t thousands = 0x30+((value / 1000) % 10);
	char* message = (char*)malloc(5 * sizeof(char));
	message[0] = thousands;
	message[1] = hundreds;
	message[2] = tens;
	message[3] = ones;
	message[4] = '\0';
	return message;
}

char* UART_num_to_volts(const uint16_t value ) {
	uint16_t volts = 1000*3.3*value/4095;
	uint8_t ones = 0x30+(volts % 10);
	uint8_t tens = 0x30+((volts / 10) % 10);
	uint8_t hundreds = 0x30+((volts / 100) % 10);
	uint8_t thousands = 0x30+((volts / 1000) % 10);
	char* message = (char*)malloc(6 * sizeof(char));
	message[0] = thousands;
	message[1] = '.';
	message[2] = hundreds;
	message[3] = tens;
	message[4] = ones;
	message[5] = '\0';
	return message;
}

void UART_Print( const char* message ) {
   uint16_t iStrIdx = 0;
   while ( message[iStrIdx] != 0 ) {
      while(!(LPUART1->ISR & USART_ISR_TXE)) // wait for empty xmit buffer
         ;                          
      LPUART1->TDR = message[iStrIdx];       // send this character
	iStrIdx++;                             // advance index to next char
   }
}

