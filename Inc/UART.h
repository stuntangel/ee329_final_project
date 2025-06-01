#include <stdio.h>
#include "stm32l4xx_hal.h"
// note: 256 char message
// 4MHz clock
// 115.2 Kb baud rate
// LPUARTDIV=256*4M/115.2K=approx 8889 = 0x22B9
#define LPUARTDIV 0x22B9
// ESC[ValueA moves cursor up value lines
// ESC[ValueB moves cursor down value lines
// ESC[ValueC moves cursor right value lines
// ESC[ValueD moves cursor left value lines
// ESC[3Valuem changes text color
// ESC[5m turns on blinking mode 
// ESC[Line;ColumnH moves to location Line Column 
// ESC[H moves to top left position

// ESC ascii is 0x1B
// [ is 0x5B
// ; is 0x3B

#define ESC_ASCII 0x1B
#define LEFT_BRAKET_ASCII 0x5B
#define COLON_ASCII 0x3B
#define UP_ESC_CODE "A" // A
#define DOWN_ESC_CODE "B" // B
#define RIGHT_ESC_CODE "C" // C
#define LEFT_ESC_CODE "D" // D
#define DISP_ESC_CODE "m" // m for blinking and changing color 
#define MOVE_ESC_CODE "H" // H
#define ASCII_START 0x30
#define DONT_CARE 0x0 // shows that this value will not be used
#define WHITE "7" 
#define RED "1" 
#define GREEN "2"
#define BLUE "4"
#define ENTER_ASCII 0x0D
#define DEL_ASCII 0x7F
typedef enum {
	RIGHT,
	CLEAR,
	DOWN,
	UP,
	LEFT,
	RECOLOR,
	BLINK,
	DONT_BLINK,
	TOPLEFT,
	BOLD
} ACTIONS;

void LPUART_Intro( void );
void UART_init( void );
char* UART_num_to_counts(const uint16_t value );
char* UART_num_to_volts(const uint16_t value );
void UART_Print( const char* message );
void LPUART1_IRQHandler( void  );
void LPUART_ESC_Print(ACTIONS action, char* value);
