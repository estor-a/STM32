/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "max30100_for_stm32_hal.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

// Adresse I2C du capteur MAX30100
#define MAX30100_I2C_ADDR 0x57

// Registres du MAX30100
#define MAX30100_REG_FIFO_DATA 0x05
#define MAX30100_REG_FIFO_WR_PTR 0x02
#define MAX30100_REG_FIFO_OF_PTR 0x03
#define MAX30100_REG_FIFO_RD_PTR 0x04

#define lengthBuff 400
#define numberSamplesLinear 10
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

COM_InitTypeDef BspCOMInit;

DAC_HandleTypeDef hdac1;

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart4;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void SystemPower_Config(void);
static void MX_GPIO_Init(void);
static void MX_ICACHE_Init(void);
static void MX_I2C1_Init(void);
static void MX_UART4_Init(void);
static void MX_DAC1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int32_t Interpolate_i32 (int32_t x0, int32_t x1, int32_t y0, int32_t y1, int32_t x) {
  int32_t dwQ;
  dwQ = ((y1-y0))*x+(x1*y0)-(x0*y1);	// overflow not checked yet
  dwQ = dwQ / (x1-x0);// we can also do roundings here
  return dwQ;
}




void resetFIFO(circularBuff_t *FIFO) {
	uint16_t i = 0;
	while (i < FIFO->length) {
		FIFO->pBuffer[i] ^= FIFO->pBuffer[i];
		i++;
	}
	FIFO->indexR = 0;
	FIFO->indexW = 0;
}

uint8_t numberSamplesReady(circularBuff_t *FIFO, uint8_t WR_PTR, uint8_t RD_PTR, uint8_t OF_PTR) {
	if (WR_PTR == RD_PTR && OF_PTR != 0) {
		resetFIFO(FIFO);
		return (16);
	}
	if (WR_PTR == RD_PTR)
		return (0);
	int8_t diff = abs(RD_PTR - WR_PTR);
	if (RD_PTR < WR_PTR)
		return (diff);
	else
		return (16 - diff);
}

uint16_t HeartRateSensor_Read(uint8_t numberSamples) {
	uint8_t fifoData[4];
	uint8_t i = 0;
	while (i < numberSamples) {
		HAL_I2C_Mem_Read(&hi2c1, MAX30100_I2C_ADDR << 1, MAX30100_REG_FIFO_DATA,
		I2C_MEMADD_SIZE_8BIT, fifoData, 4, HAL_MAX_DELAY);
		FIFO_Samples.pBuffer[FIFO_Samples.indexW] = ((fifoData[0] << 8) | fifoData[1]);
		//printf("%d\n\r", FIFO_Samples.pBuffer[FIFO_Samples.indexW]);
		FIFO_Samples.indexW = returnNextIndex(FIFO_Samples.indexW, 1,
				lengthBuff);

		i++;
	}
	return 1;
}

uint8_t HeartRateSensor_Read_Reg(uint8_t MAC30100_REG_ADDR) {

	uint8_t Data = 0;

	// Lire 4 octets de données à partir du registre FIFO
	HAL_I2C_Mem_Read(&hi2c1, MAX30100_I2C_ADDR << 1, MAC30100_REG_ADDR,
	I2C_MEMADD_SIZE_8BIT, &Data, 1, HAL_MAX_DELAY);
	return Data;
}

uint8_t enoughDataForHR(circularBuff_t *FIFO) {
	uint16_t dataReady = 0;
	if (FIFO->indexR == FIFO->indexW)
		return (0);
	int16_t diff = abs(FIFO->indexR - FIFO->indexW);
	if (FIFO->indexR < FIFO->indexW)
		dataReady = diff;
	else
		dataReady = (FIFO->length - diff);
	if (dataReady > 260)
		return 1;
	return 0;

}

int16_t linearRegressionX(circularBuff_t *FIFO) // this function calcul the X angle of the Hand. 0 is flat, 45° is when hand face forward, -45° is when hand face backward
{
    float slope = 0, intercept = 0;
    uint16_t count = 0;
    uint16_t val = 0;
    uint16_t x =0;
    float sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    for (int i = 0; i < numberSamplesLinear; i++)
    {
      val = FIFO->pBuffer[returnNextIndex(FIFO->indexR, i, FIFO->length)]; // sry for this horrible line
      x = 1 * i;
      if(val == 0)
        continue;

        sumX += x;
        sumY += val;
        sumXY += x * val;
        sumX2 += x * x;
        count++;
    }
    FIFO->indexR = returnNextIndex(FIFO->indexR,  1,  FIFO->length);
    if(count == 0)
      return 200;
    slope = ((float)count * sumXY - sumX * sumY) / ((float)count * sumX2 - sumX * sumX);
    intercept = ((float)sumY - slope * sumX) / count;
    int32_t slopeInt = (int32_t)slope;
    //printf("%d\n\r", slopeInt);
    if(slopeInt < -200 || slopeInt > 200)
    {
    	resetFIFO(FIFO);
    	printf("slope too big, reset FIFO\n\r");
    	return(200);
    }

    return (slopeInt);
}

uint16_t diffFIFO(uint16_t value, uint16_t target, uint16_t length)
{
	uint16_t i = 0;
	while(value != target)
	{
		value = returnNextIndex(value, 1, length);
		i++;
	}
	return(i);
}

#define SAMPLE_RATE 100 // Taux d'échantillonnage en Hz

uint16_t str[lengthBuff];
circularBuff_t FIFO_Samples = { .indexW = 0, .indexR = 0, .length = lengthBuff,
		.pBuffer = str };

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the System Power */
  SystemPower_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ICACHE_Init();
  MX_I2C1_Init();
  MX_UART4_Init();
  MX_DAC1_Init();
  /* USER CODE BEGIN 2 */
  HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
  HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);

	MAX30100_Init(&hi2c1, 0); //&huart4);
	MAX30100_SetSpO2SampleRate(MAX30100_SPO2SR_DEFAULT);
	MAX30100_SetLEDPulseWidth(MAX30100_LEDPW_DEFAULT);
	MAX30100_SetLEDCurrent(MAX30100_LEDCURRENT_DEFAULT,
			MAX30100_LEDCURRENT_DEFAULT);
	MAX30100_SetMode(MAX30100_HRONLY_MODE);
  /* USER CODE END 2 */

  /* Initialize led */
  BSP_LED_Init(LED_GREEN);

  /* Initialize USER push-button, will be used to trigger an interrupt each time it's pressed.*/
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Initialize COM1 port (115200, 8 bits (7-bit data + 1 stop bit), no parity */
  BspCOMInit.BaudRate   = 115200;
  BspCOMInit.WordLength = COM_WORDLENGTH_8B;
  BspCOMInit.StopBits   = COM_STOPBITS_1;
  BspCOMInit.Parity     = COM_PARITY_NONE;
  BspCOMInit.HwFlowCtl  = COM_HWCONTROL_NONE;
  if (BSP_COM_Init(COM1, &BspCOMInit) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	uint8_t WR_PTR = 0;
	uint8_t RD_PTR = 0;
	uint8_t OF_PTR = 0;
	uint8_t sampleReady = 0;
	resetFIFO(&FIFO_Samples);
	int16_t slope = 0;
	int16_t indexLastPeak = -1;
	int16_t indexPeak = -1;
	uint16_t endPeak = 1;
	uint16_t periodHeart = 0;
	uint16_t heartRate = 0;
	float frequency_heart;
	while (1) {
		if(FIFO_Samples.indexW == 0 && FIFO_Samples.indexR == 0)
			printf("start sampling, do not move\n\r");

		WR_PTR = HeartRateSensor_Read_Reg(MAX30100_REG_FIFO_WR_PTR);
		RD_PTR = HeartRateSensor_Read_Reg(MAX30100_REG_FIFO_RD_PTR);
		OF_PTR = HeartRateSensor_Read_Reg(MAX30100_REG_FIFO_OF_PTR);
		sampleReady = numberSamplesReady(&FIFO_Samples, WR_PTR, RD_PTR, OF_PTR);
		if(sampleReady)
			HeartRateSensor_Read(sampleReady); //read the amount of data ready

		//DAC1->DHR12L1 = FIFO_Samples.pBuffer[FIFO_Samples.indexW - 1];
		//printf("%d, %d, %d, %d\n\r",sampleReady, WR_PTR, RD_PTR, OF_PTR);


		if(enoughDataForHR(&FIFO_Samples))
		{
			indexLastPeak = -1;
			indexPeak = -1;
			while(FIFO_Samples.indexR != (FIFO_Samples.indexW - numberSamplesLinear))
			{
				slope = linearRegressionX(&FIFO_Samples);
				if(slope == 200)
					break;
				if(slope < -20 && endPeak)
				{
					indexLastPeak = indexPeak;
					indexPeak = (FIFO_Samples.indexR - 1);
					endPeak = 0;
				}
				if(slope > 0 && endPeak == 0 )
				{
					endPeak = 1;
				}
				if(indexLastPeak != -1 && indexPeak != -1)
				{
					periodHeart = diffFIFO(indexLastPeak, indexPeak, FIFO_Samples.length) * 10; //period en ms
					frequency_heart = (1 /(  / 1000));
					heartRate = frequency_heart * 60;
					if(heartRate > 30 && heartRate < 175)
						printf("HR = %d\n\r", heartRate);
					//printf("%d, %d, %d, %d, %d\n\r", indexLastPeak, indexPeak, periodHeart, heartRate);
					else
					{
						printf("error while computing, reset FIFO\n\r");
						resetFIFO(&FIFO_Samples);
						break;
					}

					indexLastPeak = indexPeak;
					indexPeak = -1;

				}
			}
		}

	}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI
                              |RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_4;
  RCC_OscInitStruct.LSIDiv = RCC_LSI_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Power Configuration
  * @retval None
  */
static void SystemPower_Config(void)
{

  /*
   * Switch to SMPS regulator instead of LDO
   */
  if (HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK)
  {
    Error_Handler();
  }
/* USER CODE BEGIN PWR */
/* USER CODE END PWR */
}

/**
  * @brief DAC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC1_Init(void)
{

  /* USER CODE BEGIN DAC1_Init 0 */

  /* USER CODE END DAC1_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};
  DAC_AutonomousModeConfTypeDef sAutonomousMode = {0};

  /* USER CODE BEGIN DAC1_Init 1 */

  /* USER CODE END DAC1_Init 1 */

  /** DAC Initialization
  */
  hdac1.Instance = DAC1;
  if (HAL_DAC_Init(&hdac1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_HighFrequency = DAC_HIGH_FREQUENCY_INTERFACE_MODE_AUTOMATIC;
  sConfig.DAC_DMADoubleDataMode = DISABLE;
  sConfig.DAC_SignedFormat = DISABLE;
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_EXTERNAL;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Autonomous Mode
  */
  sAutonomousMode.AutonomousModeState = DAC_AUTONOMOUS_MODE_DISABLE;
  if (HAL_DACEx_SetConfigAutonomousMode(&hdac1, &sAutonomousMode) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT2 config
  */
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC1_Init 2 */

  /* USER CODE END DAC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00000E14;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief ICACHE Initialization Function
  * @param None
  * @retval None
  */
static void MX_ICACHE_Init(void)
{

  /* USER CODE BEGIN ICACHE_Init 0 */

  /* USER CODE END ICACHE_Init 0 */

  /* USER CODE BEGIN ICACHE_Init 1 */

  /* USER CODE END ICACHE_Init 1 */

  /** Enable instruction cache in 1-way (direct mapped cache)
  */
  if (HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_ICACHE_Enable() != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ICACHE_Init 2 */

  /* USER CODE END ICACHE_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart4, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart4, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
	while (1) {
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
