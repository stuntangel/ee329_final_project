#include "main.h"
#include "stdlib.h"
void SystemClock_Config(void);
biquad_t biq_stage1;
static float output_val = 0.0;
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  DAC_Init();
  SysTick_Init();
  ADC_init();
  GPIO_init_pins();
  biq_stage1.a = (float*)malloc(2*sizeof(float));
  biq_stage1.b = (float*)malloc(3*sizeof(float));
  biq_stage1.Xn1 = 0;
  biq_stage1.Xn2 = 0;
  biq_stage1.Yn1 = 0;
  biq_stage1.Yn2 = 0;
  Timer_setup_TIM2(SAMPLE_INTVL);
  int X[SAMPLES] = {0};
//  int j = 0;
  for (int i = 0; i < SAMPLES; i++) {
	  X[i] = 1000*sinf(2*PI*i/SAMPLES)+1000;
//	  if (j > 6) {
//		  X[i] = 2000;
//	  }
//	  else {
//		  X[i] = 1000;
//	  }
//	  j++;
//	  if (j > 12) {
//		  j=0;
//	  }
  }
  int i = 0;
  TIM2->CR1 |= TIM_CR1_CEN; // start timer
  float prev_cutoff_freq = 50;
  float cutoff_freq = 0;
  float prev_bandwidth = 20;
  float bandwidth = 0;
  ADC3->CR |= ADC_CR_ADSTART;                // start 1st conversion
  LPF(&biq_stage1, prev_bandwidth, prev_cutoff_freq); // load LPF
  while (1)
  {
	  if (checkADCFlag()) {
		  cutoff_freq = voltToCutoff(grabConvertedResult());
		  ADC3->CR |= ADC_CR_ADSTART;                // start 1st conversion
		  if(((cutoff_freq - prev_cutoff_freq) > 5) || ((cutoff_freq - prev_cutoff_freq) < 5) ){
			  LPF(&biq_stage1, prev_bandwidth, cutoff_freq); // load LPF
			  prev_cutoff_freq = cutoff_freq;
		  }
	  }
	  if (checkTimerFlag()) {
		  TIM2->CCR1+=(SAMPLE_INTVL);
		  computeBiquad(&biq_stage1, (float)(X[i]), &output_val);
		  DAC_Write_Volt((int)output_val);

		  i++;
	  }
	  if(i>=SAMPLES) {
		  i=0;
	  }
  }
}

// 30 to 80
float voltToCutoff(uint32_t adc_output) {
	return (50*((float)adc_output)/4095)+30;
}
// 1 to 35
float voltToBandwidth(uint32_t adc_output) {
	return (35*((float)adc_output)/4095)+1;
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

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
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

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
