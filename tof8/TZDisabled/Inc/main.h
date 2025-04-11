/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//DISABLED V0.4 #include "stm32u3xx_nucleo.h"
//===== SEB START
#ifndef countof
#define countof(a) (sizeof(a) / sizeof(*(a)))
#endif

  
#define SYS_CLK_MHZ 96 // editable to use : uint32_t SystemCoreClock = 48000000UL;
#define SYS_CLK_HZ (SYS_CLK_MHZ * 1000000)
#define BASE_TIMER_CLOCK_MHZ SYS_CLK_MHZ // (PWM LEDs.c) we use MSI 48MHz here to tune the timers for PWM dimming with 1 us period  
  
#include "commons.h"  
#include "u8fifo.h"
#include "HabuLQFP100_SMPS.h"
#include "io_drive.h"

#include "MB2095_NET.h"
  
#include "LEDs.h"
#include "I2C_MasterIO.h"
#include "SPI_MasterIO.h"
//#define SYS_CLK_HZ 96000000
//#define SYS_CLK_MHZ (SYS_CLK_HZ / 1000000)
#include "stm32u3xx_ll_bus.h"
#include "stm32u3xx_ll_rcc.h"  
#include "stm32u3xx_ll_pwr.h"    
#include "stm32u3xx_ll_adc.h"
#include "stm32u3xx_ll_usart.h"
#include "stm32u3xx_ll_lpuart.h"
#include "stm32u3xx_ll_gpio.h"  
#include "adc_polling.h"
#include "analog.h"
#include "octospi 1.h"
#include "m95p32_hal_xspi.h"  
#include "sensors.h"
#include "SerialComPort.h"
  

  
#include "stdlib.h"
#include "vl53l8cx_api.h"
  #include "LCD_16x2.h"
 #include "AE_vl53l8.h"
//#include "Joystick.h"
//#include "B_HMI_LCD16x2.h"

  #define Absolute_Difference(x,y) (((x) < (y)) ? ((y) - (x)) : ((x) - (y)));
  
int32_t app_init();
int32_t app_polling();

void ADC_Init(void);  
int32_t ADC_Scan(void); 


//===== SEB END  
  
  
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */
// Simple timebase flags, set by ISR, cleared by main loop.
extern uint8_t Brisk_5ms_Flag; // set by ISR, cleared by main
extern uint8_t Brisk_10ms_Flag; 
extern uint8_t Brisk_50ms_Flag;
extern uint8_t Brisk_100ms_Flag;
extern uint8_t Brisk_1sec_Flag;

typedef struct {
  IO_Pad_t* pPin;
  rock_keys_t codebefore;
  rock_keys_t codenow;
  char /*int32_t*/ Pressed;
  char /*int32_t*/ Pressed_5sec;
  int32_t PressedCount_50ms;
} UserKey_t;
/* USER CODE END Private defines */


typedef enum { // Brisk LEDs example (static)
  NO_DEMO = 0,
  
  JACQUES_LED_DEMO, // 1
  PWM_LED_DEMO, // 2
  ADC_TEST_DEMO, // 3

  DEMO_COUNT,
} Demo_Names_t;

extern int32_t DemoMode;
#include "app.h"  

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
