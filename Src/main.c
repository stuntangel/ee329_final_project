#include "main.h"
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
  float32_t* lowpassCoeffs = LPF(center_freq, bandwidth);
  load_IIR(lowpassCoeffs);
  for (int idx = 0; idx < FFTLEN; idx++) {
	  fft_in_buf[idx] = sin(2*PI*center_freq/FFTLEN);
  }
  perform_IIR(fft_in_buf); // perform IIR of input buffer
  DAC_Write_Volt(2000);
  while (1) {
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
