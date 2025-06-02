#include "main.h"
#include "delay.h"
#include "DAC.h"
#include "filter_utils.h"
#include <math.h>

static float fft_in_buf[FFTLEN];

void SystemClock_Config(void);
int main(void) {
  HAL_Init();
  SystemClock_Config();
  DAC_Init(); //initialize DAC
  ADC_init(); // initialize ADC
  init_FFT(); // initialize FFT
  GPIO_init_pins(); // initialize POTS and AUDIO ports
  grabConvertedResult()
  uint16_t center_freq = 400;
  uint16_t bandwidth = 200;
  load_IIR(LPF(center_freq, bandwidth));
  int X[256];
  for (int i = 0; i < SAMPLES; i++) {
    // basic construction of a test 100 Hz tone
    // Amplitude in millivolts
    // Time in nanoseconds
	  X[i] = (int) (1000 * sin((2 * PI * 100) * (i * SAMPLE_INTVL)));
  }
  //float Y[] = perform_IIR(X);
  while (1) {
    // Playback signal for SAMPLE_TIME at SAMPLE_INTVL speed
    for (int i = 0; i < SAMPLES; i++)
    {
      DAC_Write_Volt(X[i]);
      delay_us(SAMPLE_INTVL);
    }
  }
}

float voltsToHz(float32_t volts) {
	return 2000*volts/3.3+60;
}

void GPIO_init_pins( void ) {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOFEN; // enable clock for GPIOF=POTS_PORT
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; // enable clock for GPIOC=AUDIO_PORT
	POTS_PORT->MODER |= POTS_ANALOG_MODER; // put in analog mode
	AUDIO_PORT->MODER |= AUDIO_ANALOG_MODER; // put in analog mode
	AUDIO_PORT->MODER &= ~AUDIO_DIGITAL_MODER; // put in digital input mode
	AUDIO_PORT->OTYPER  &= ~AUDIO_DIGITAL_OTYPER; // put in digital output mode
	AUDIO_PORT->PUPDR  &= ~AUDIO_DIGITAL_PUPDR; // put in digital output mode
	AUDIO_PORT->OSPEEDR |= AUDIO_DIGITAL_OSPEEDR; // put in digital output mode
}

void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif
