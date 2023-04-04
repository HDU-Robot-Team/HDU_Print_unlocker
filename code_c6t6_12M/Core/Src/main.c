/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "print_mode.h"
#include "motor_control.h"
#include "standby_mode.h"
#include "oled4p.h"
#include "keys_4x4.h"
#include "flash.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t Unlocked = 0;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim==&htim1)
	{
		if(Compare_expected>Compare_now)
			Compare_now++;
		else if(Compare_expected<Compare_now)
			Compare_now--;
		__HAL_TIM_SET_COMPARE(&motor_tim,motor_channel,Compare_now);
	}
	if(htim==&htim3)
	{
		keys_scan();
	}
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	int i;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	print_mode_init();
	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_ADC_Start_DMA(&hadc1,electric_quantity,10);
	Compare_now=Compare_expected=lock_compare;
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim1);
		power_on();
	
	OLED_Init();
	OLED_Init();
	count_down=5;
	uint32_t data[4];
	if(ReadFlash(SN)==0x66669999)
	{
		uint16_t compare1,compare2;
		data[0]=ReadFlash(SN+4);
		data[1]=ReadFlash(SN+8);
		data[2]=ReadFlash(SN+12);
		data[3]=ReadFlash(SN+16);
		if(data[3]==0x11111111)
			Unlocked=1;
		for(i=0;i<8;i++)
			Save[i+4]=password[i]=data[i/4]>>(24-i%4*8);
		for(i=0;i<4;i++)
		{
			if(password[i*2]=='F'&& password[i*2+1]=='F')
			{
				Print_ID_[i]=0xFF;
			}
			else if(password[i*2]=='F')
				Print_ID_[i]=0xF0+(password[i*2+1]-'0');
			else if(password[i*2+1]=='F')
				Print_ID_[i]=(password[i*2]-'0')*16+0x0F;
			else
				Print_ID_[i]=(password[i*2]-'0')*16+(password[i*2+1]-'0');
		}
		for(i=0;i<4;i++)
			Save[i+12]=data[2]>>(24-i%4*8);
		compare1=((uint16_t)Save[12]<<8)+(uint16_t)Save[13];
		compare2=((uint16_t)Save[14]<<8)+(uint16_t)Save[15];
		if(compare1>35&&compare1<181)
			Compare_now=lock_compare=compare1;
		if(compare2>35&&compare2<181)
			unlock_compare=compare2;
	}
	i=0;

	while(electric_quantity[9]==0)
	{
		HAL_Delay(1);
	}
	HAL_ADC_Stop_DMA(&hadc1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
  while (1)
  {

//		SET_ID(0xff,0xff,0xff,0xff);
//		HAL_Delay(1000);
//		reply_buf.pkg_size=0; 
//						Send_CMD(&print_uart,CMD_ValidTempleteNum,0x00);
//						HAL_Delay(100);
//						if(reply_buf.pkg_size==0x0005&&reply_buf.cmd==0x00)
//						{
//							print_max_id=((uint16_t)reply_buf.parameter[0]<<8)+reply_buf.parameter[1];
//							OLED_ShowCHinese(8,2,5);
//							OLED_ShowCHinese(24,2,33);
//							OLED_ShowCHinese(40,2,66);
//							OLED_ShowCHinese(56,2,42);
//							OLED_ShowNum(9,1,print_max_id,3);
//						}
//						else
//						{
//							OLED_ShowCHinese(32,3,64);
//							OLED_ShowCHinese(48,3,65);
//							OLED_ShowCHinese(64,3,10);
//							OLED_ShowCHinese(80,3,11);
//						}
//						HAL_Delay(2000);
//						while(1);//	
		OLED_ShowBattery();
		print_fun();
		keys_start();
		count_down--;
		if(count_down==0)
		{
			count_down=5;
			oled_standby_enter();
			HAL_Delay(500);
			power_off();
		}
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
