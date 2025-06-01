#include "main.h"
static float fft_in_buf[FFTLEN];

void SystemClock_Config(void);
int main(void) {
  HAL_Init();
  SystemClock_Config();
  GPIO_Init_DAC();
  init_FFT();
  uint16_t center_freq = 400;
  float32_t* lowpassCoeffs = computeLowpassCoeffs(center_freq, bandwidth);
  load_IIR(lowpassCoeffs);
  for (int idx = 0; idx < FFTLEN; idx++) {
	  fft_in_buf[idx] = sin(2*PI*center_freq/FFTLEN);
  }
  float32_t* Y = perform_IIR(X);
  //Timer_setup_TIM2((int)ONESEC/(center_freq*FFT_LENGTH)); // initialize Tim2 at sampling rate


  //float* Y = perform_IIR(X);
  //TIM2->CR1 |= TIM_CR1_CEN; // start TIM2 CR1
//  DAC_write(2000);
  while (1) {
//	  for (int idx = 0; idx < FFT_LENGTH; idx++) {
//		  for(int i=0; i <20000; i++);
//		  DAC_write(idx*10);
//	  }
  }
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
