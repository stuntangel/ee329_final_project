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
  biq_stage1.a = (float*)malloc(2*sizeof(float));
  biq_stage1.b = (float*)malloc(3*sizeof(float));
  biq_stage1.Xn1 = 0;
  biq_stage1.Xn2 = 0;
  biq_stage1.Yn1 = 0;
  biq_stage1.Yn2 = 0;
  Timer_setup_TIM2(SAMPLE_INTVL);
  int X[SAMPLES] = {0};
  for (int i = 0; i < SAMPLES; i++) {
	  X[i] = 1000*sinf(2*PI*i/SAMPLES)+1000;
//	  if ((i % 2) == 0) {
//		  X[i] = 2000;
//	  }
//	  else {
//		  X[i] = 1000;
//	  }
  }
  int i = 0;
  TIM2->CR1 |= TIM_CR1_CEN; // start timer
  LPF(&biq_stage1, 20, 10); // load LPF
  while (1)
  {
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
