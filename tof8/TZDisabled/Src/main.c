/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/** @addtogroup STM32U3xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
  

//int32_t DemoMode = JACQUES_LED_DEMO;
int32_t DemoMode = PWM_LED_DEMO;
//int32_t DemoMode = ADC_TEST_DEMO;

  
int32_t percent, adc_joy_lsb;



/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
  int8_t count =100;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
void Brisk_Polling(void);
void Brisk_1ms_ISR(void);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void SystemPower_Config(void);	

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */


void ospi_init(void);
void ospi_test(void);


int main(void)
{
  // ATOMIC_SET_BIT()
  // ATOMIC_CLEAR_BIT()
  // ATOMIC_MODIFY_REG
  /* STM32U3xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user
             can eventually implement his proper time base source (a general purpose
             timer for example or other time source), keeping in mind that Time base
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
             handled in milliseconds basis.
       - Set NVIC Group Priority to 3
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the System clock to have a frequency of 96 MHz */
  SystemClock_Config();
  
  // real bad to do this.
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;  //;
  PeriphClkInitStruct.Octospi1ClockSelection = RCC_USART1CLKSOURCE_HSI;//; HSI16
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    Error_Handler();
  
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;  //;
  PeriphClkInitStruct.Octospi1ClockSelection = RCC_USART3CLKSOURCE_HSI;//; HSI16
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    Error_Handler();
  
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPUART1;  //;
  PeriphClkInitStruct.Octospi1ClockSelection = RCC_LPUART1CLKSOURCE_HSI;//; HSI16
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    Error_Handler();
  __HAL_RCC_CRC_CLK_ENABLE();
  
    /* Configure the System Power */
  SystemPower_Config();

  /* For better performances, enable the instruction cache in 1-way direct mapped mode */
  HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY);
  if (HAL_ICACHE_Enable() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

//==============================  
  
  NOP_Init(); // configure NOP based delays
  CNT_Init(); // configure DBG_CYCCNT core function (not available on Cortex M0)
  
void ADC_Polling_Test(void); // local declaration

  // There is no name on the joystick side
  const IO_Pad_t JOYSTICK = {  PD_11, { .Mode = IO_ANALOG, .Signal = ADC2_IN11, /*.Drive = IO_PUSHPULL,*/ .Pull = IO_NOPULL, /*.Speed = IO_SPEED_03,*/ /*.Odr = IO_ODR_HIGH*/ } };   
  IO_PadInit((IO_Pad_t*)&JOYSTICK);

  
//disabled ADC_Polling_Test(); //wait for Vdda = Vref+ HW fix to use ADC.
  AnalogTest();
  
#if 1 // Initializing XSPI pins for U3  (to move out of main.c later)
  ospi_init();
#if 1
  ospi_test();
#endif
#endif

  if((DemoMode == ADC_TEST_DEMO)||(DemoMode == PWM_LED_DEMO))
  {
    TimedLEDsInit(); // to debug  
  }
  
  const IO_Pad_t LED1_AsOutput = {  LD1_ /*PB_15*/, { .Mode = IO_OUTPUT, .Drive = IO_PUSHPULL, .Pull = IO_NOPULL, .Speed = IO_SPEED_01, .Odr = IO_ODR_HIGH } }; 
  const IO_Pad_t LED2_AsOutput = {  LD2_ /*PB_14*/, { .Mode = IO_OUTPUT, .Drive = IO_PUSHPULL, .Pull = IO_NOPULL, .Speed = IO_SPEED_01, .Odr = IO_ODR_HIGH } }; 
  const IO_Pad_t LED3_AsOutput = {  LD3_ /*PA_8*/,  { .Mode = IO_OUTPUT, .Drive = IO_PUSHPULL, .Pull = IO_NOPULL, .Speed = IO_SPEED_01, .Odr = IO_ODR_HIGH } }; 
  
  if(DemoMode == JACQUES_LED_DEMO)
  {
    IO_PadInit((IO_Pad_t*)&LED1_AsOutput);
    IO_PadInit((IO_Pad_t*)&LED2_AsOutput);
    IO_PadInit((IO_Pad_t*)&LED3_AsOutput);
  }
  
  Brisk_I2C_MasterIO_Init();

  
  
  /* Add your application code here */
  BTEL_Init(); // no HW involved.
  Serials_Init();
  AnalogInit();
  
  SensorsInit();
/* Infinite loop */
  LCD_16x2_Init(&LCD_16x2);
  while (1)
  {
    
    Brisk_Polling();
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = MSIS
  *            SYSCLK(Hz)                     = 96000000
  *            HCLK(Hz)                       = 96000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            APB3 Prescaler                 = 1
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  uint32_t latency;

  /* Enable Epod Booster */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_RCCEx_EpodBoosterClkConfig(RCC_EPODBOOSTER_SOURCE_MSIS, RCC_EPODBOOSTER_DIV1);
  HAL_PWREx_EnableEpodBooster();

  /* At reset, the regulator is the LDO, in range 2 : Need to move to range 1 to reach 96 MHz */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Set Flash latency before increasing MSIS */
#if !defined(USE_FPGA)
  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_2);
#endif

  /* Activate MSIS as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSIS;
  RCC_OscInitStruct.MSISState = RCC_MSI_ON;
  RCC_OscInitStruct.MSISSource = RCC_MSI_RC0;
#if !defined(USE_FPGA)
  RCC_OscInitStruct.MSISDiv = RCC_MSI_DIV1;
#else
  RCC_OscInitStruct.MSISDiv = RCC_MSI_DIV2;
#endif
  
  RCC_OscInitStruct.OscillatorType |= RCC_OSCILLATORTYPE_HSI48; // added
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  
  RCC_OscInitStruct.OscillatorType |= RCC_OSCILLATORTYPE_LSI; // added
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  
  RCC_OscInitStruct.OscillatorType |= RCC_OSCILLATORTYPE_HSI; // added
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Select MSIS as system clock source and configure the HCLK, PCLK1, PCLK2 and PCLK3
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK3);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSIS;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;
#if !defined (USE_FPGA)
  latency = FLASH_LATENCY_2;
#else
  latency = FLASH_LATENCY_1;
#endif
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, latency) != HAL_OK)
  {
    /* Initialization Error */
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
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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

  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

void ospi_init(void) {
__HAL_RCC_OCTOSPI1_CLK_ENABLE();

  const IO_Pad_t QSPI_IO1 = {  U11_Q   /*PE12*/, { .Mode = IO_SIGNAL, .Signal = OCTOSPI1_IO1, .Drive = IO_PUSHPULL, .Pull = IO_NOPULL, .Speed = IO_SPEED_03, .Odr = IO_ODR_HIGH } };   
  const IO_Pad_t QSPI_IO2 = {  U11_Wn  /*PA7*/,  { .Mode = IO_SIGNAL, .Signal = OCTOSPI1_IO2, .Drive = IO_PUSHPULL, .Pull = IO_NOPULL, .Speed = IO_SPEED_03, .Odr = IO_ODR_HIGH } };   
  const IO_Pad_t QSPI_IO3 = {  U11_Hn  /*PC6*/,  { .Mode = IO_SIGNAL, .Signal = OCTOSPI1_IO3, .Drive = IO_PUSHPULL, .Pull = IO_NOPULL, .Speed = IO_SPEED_03, .Odr = IO_ODR_HIGH } };   
  const IO_Pad_t QSPI_NCS = {  U11_Sn  /*PE11*/, { .Mode = IO_SIGNAL, .Signal = OCTOSPI1_NCS, .Drive = IO_PUSHPULL, .Pull = IO_NOPULL, .Speed = IO_SPEED_03, .Odr = IO_ODR_HIGH } };   
  const IO_Pad_t QSPI_IO0 = {  U11_D   /*PE12*/, { .Mode = IO_SIGNAL, .Signal = OCTOSPI1_IO0, .Drive = IO_PUSHPULL, .Pull = IO_NOPULL, .Speed = IO_SPEED_03, .Odr = IO_ODR_HIGH } };   
  const IO_Pad_t QSPI_CLK = {  U11_CLK /*PE10*/, { .Mode = IO_SIGNAL, .Signal = OCTOSPI1_CLK, .Drive = IO_PUSHPULL, .Pull = IO_NOPULL, .Speed = IO_SPEED_03, .Odr = IO_ODR_HIGH } };   
  IO_PadInit((IO_Pad_t*)&QSPI_IO0);
  IO_PadInit((IO_Pad_t*)&QSPI_IO1);
  IO_PadInit((IO_Pad_t*)&QSPI_IO2);
  IO_PadInit((IO_Pad_t*)&QSPI_IO3);
  IO_PadInit((IO_Pad_t*)&QSPI_NCS);
  IO_PadInit((IO_Pad_t*)&QSPI_CLK);

  MX_OCTOSPI1_Init();

}



uint8_t Rcv[512] ={0};
uint8_t SR;
void ospi_test(void) {

  //uint8_t SR;
  uint32_t index;
//  uint8_t Rcv[512] ={0};
  SR = RDSR(&hospi1);
  WREN(&hospi1);
  SR = RDSR(&hospi1);
  WRDI(&hospi1);
  SR = RDSR(&hospi1);

  Single_Read(&hospi1,0,20,Rcv);

  for(index=0; index<512; index++)
    Rcv[index]= index;
  WREN(&hospi1);
  Page_Write(&hospi1,0,512,Rcv);

  for(index=0; index<512; index++)
    Rcv[index]= 0;
  Single_Read(&hospi1,0,20,Rcv);

  WREN(&hospi1);
 // Page_Erase(&hospi1,0);
  Single_Read(&hospi1,0,20,Rcv);

  for(index=0; index<512; index++)
    Rcv[index]= 0;
  Read_SFDP(&hospi1,0,20,Rcv);
  return;

  for(index=0; index<512; index++)
    Rcv[index]= 0;
  for(index=0; index<512; index++)
    Rcv[index]= index;
  WREN(&hospi1);
  Page_Write(&hospi1,0,512,Rcv);

  for(index=0; index<512; index++)
    Rcv[index]= 0;
  FAST_QRead(&hospi1,0,512,Rcv);
}


// 1 msec Systick interrupt handler for Brisk application purpose
uint32_t Brisk_1ms_Counter;
uint8_t Brisk_5ms_Flag, Brisk_10ms_Flag, Brisk_50ms_Flag, Brisk_100ms_Flag, Brisk_1sec_Flag;
uint32_t Brisk_1sec_Counter;


void Brisk_1ms_ISR(void) {

  Brisk_1ms_Counter++;
  if((Brisk_1ms_Counter % 8/*5*/)==0) { // optimisation for speed and size avoiding division on CortexM0
    Brisk_5ms_Flag = 1;
    if((Brisk_1ms_Counter % 16 /*10*/)==0) {
      Brisk_10ms_Flag = 1;
      if((Brisk_1ms_Counter % 64 /*50*/)==0) {
        Brisk_50ms_Flag = 1;
        if((Brisk_1ms_Counter % 128 /*100*/)==0) {
          Brisk_100ms_Flag = 1;
          if((Brisk_1ms_Counter % 1024 /*1000*/)==0) {
            Brisk_1sec_Flag = 1;
            Brisk_1sec_Counter++; 
          }
        }
      }
    }
  }
}



int32_t SleepingLED = 0;

void Brisk_Polling(void) { // hook from main() loop

  if(Brisk_5ms_Flag) { // 8 ms
    Brisk_5ms_Flag = 0;
  }
  if(Brisk_10ms_Flag) { // 16ms things to do each 10+ msec
    TimedLED_10msecTick(); // manage LED Blink function
    SensorsPolling();  // testing the sensor // NO NEED TO BE FASTER
//>    BriskI2C_10msecTick(); // eeprom, BMC and ST Mems //TODO TO WORK ON IT SEB THIS FUNCTION IS VERY SLOW, WHY?
    if(DemoMode == PWM_LED_DEMO)
    {
      SleepingLED += 1;
      if(SleepingLED>200)
        SleepingLED = 0;

      if(SleepingLED>100)
      { // 101..199
        BriskDimTimedLED(1,SleepingLED-100);
        BriskDimTimedLED(2,200-SleepingLED);
        BriskDimTimedLED(3,SleepingLED-100);
      }
      else
      { // 0 1 2 3 ... 100
        BriskDimTimedLED(1,100-SleepingLED);
        BriskDimTimedLED(2,SleepingLED);
        BriskDimTimedLED(3,100-SleepingLED);
      }
    }
    
    //OK AddTou8fifo(&u8fifo_to_TX[BT_SERIAL], 'S');
    //OK AddTou8fifo(&u8fifo_to_TX[BT_SERIAL], 'T');
    ADCs_Sweep(); // ADC channels scan every 10 msec
    Brisk_10ms_Flag = 0; // 10ms++ (relaxed)
  }
  if(Brisk_50ms_Flag) { // 64ms things to do each 50+ msec
    // here we manage the initialization of the hot pluggable LCD 16x2 char with 5 way rock switch
    Joy_50ms();
    
    
    if(   (UserKey.codenow==ROCK_SEL)
       && (UserKey.Pressed_5sec == 1)  ) { // push the BTEL dashboard
            UserKey.Pressed_5sec = 0;
            BTEL.Delayed_PanelCountdown_100ms = 2;
    }

    if(DemoMode == ADC_TEST_DEMO)
    {
      percent = Interpolate_i32(0, 4095, 0, 100, adc_joy_lsb);
      BriskDimTimedLED(LED_1,percent);
      BriskDimTimedLED(LED_2,percent);
      BriskDimTimedLED(LED_3,percent);
    }
    Brisk_50ms_Flag = 0; // 50ms++
  }
  if(Brisk_100ms_Flag) { // things to do each 100 msec
    BTEL_100ms();
    Brisk_100ms_Flag = 0; // 100ms++
  }
  
  if(Brisk_1sec_Flag) { // 1 second
    Brisk_1sec_Flag = 0;
  }

 // BTEL_Polling(); // monitor the FIFO from STmod+ USART and process any incoming/outgoing message.

  
  // DEMO MODES
  if(DemoMode == JACQUES_LED_DEMO)
  {
    // original LED demo by default
    /* USER CODE BEGIN 3 */
    if(count==0)
      count = 100;
    
    IO_PadToggle(LD1_);
    
    /* Insert delay 100 ms */
    HAL_Delay(count); 
    IO_PadToggle(LD2_);
    /* Insert delay 100 ms */
    HAL_Delay(count);
    IO_PadToggle(LD3_);
    /* Insert delay 100 ms */
    HAL_Delay(count);  
    //IO_PadSetHigh(LD3_);
    //IO_PadSetLow(LD3_);
    count--;    
  }
  
  
}

// flawsfix
