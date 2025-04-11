/**
  ******************************************************************************
  * @file    analog.c (STM32 ADC related functions, internal and external channel)
  * @author  S.Ma
  * @brief   ADC management, Rock Switch Analog Keyboard management (convert voltage to key index), external and internal channels, mV conversion, loop channels continuous conversion mode.
  * 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018-2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "main.h"

//#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
// STUPID NAMING CONF CONFIG!

ADC_ChannelConfTypeDef sConfig[2]; // scratch structure (RAM)
ADC_InjectionConfTypeDef sConfigInjected[2]; // scratch structure (RAM)

ADC_HandleTypeDef hadc[2] = { { .Instance = ADC1 }, { .Instance = ADC2 } }; // can be const or shared
DMA_HandleTypeDef hdma_adc[2]; // can be const or sh

DAC_HandleTypeDef hdac[2] = { { .Instance = DAC1 }, };
DAC_ChannelConfTypeDef sConf[2];

//__ALIGN_BEGIN
Analog_t Analog;
//__ALIGN_END IAR compile fail on Cube 4.0


void ADCs_Init(void);
void ADCs_Init(void)
{
  HAL_StatusTypeDef hal_status = HAL_OK;
  
  /* Configure PWR for ADC */
  LL_AHB1_GRP2_EnableClock(LL_AHB1_GRP2_PERIPH_PWR);
  LL_PWR_EnableVDDA();
  
  __HAL_RCC_VREF_CLK_ENABLE();

#if 0 // Use this code if ADC Vdda does not come from external 3.3V source and is routed to VRef+ as supply provided by STM32 internal programmable voltage generator
  HAL_SYSCFG_VREFBUF_VoltageScalingConfig(SYSCFG_VREFBUF_VOLTAGE_SCALE3);
  HAL_SYSCFG_EnableVREFBUF();
#endif

  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC12);  
  
  for(uint8_t i=0;i<=1;i++)
  { // manual single channel basic mode here. 
    hadc[i].State = HAL_ADC_STATE_RESET;
    hadc[i].Init.Resolution     = ADC_RESOLUTION_12B;
    hadc[i].Init.ScanConvMode   = ADC_SCAN_DISABLE;
    hadc[i].Init.EOCSelection   = ADC_EOC_SINGLE_CONV;
    hadc[i].Init.LowPowerAutoWait      = DISABLE;
    hadc[i].Init.ContinuousConvMode    = DISABLE; //ENABLE;
    hadc[i].Init.NbrOfConversion       = 1;
    hadc[i].Init.DiscontinuousConvMode = DISABLE;
    hadc[i].Init.NbrOfDiscConversion   = 1;
    hadc[i].Init.ExternalTrigConv      = ADC_SOFTWARE_START;
    hadc[i].Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc[i].Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
    hadc[i].Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
    hadc[i].Init.Overrun                  = ADC_OVR_DATA_OVERWRITTEN;
    hadc[i].Init.OversamplingMode         = DISABLE;
    hadc[i].Init.Oversampling.Ratio                 = 1;
    hadc[i].Init.Oversampling.RightBitShift         = ADC_RIGHTBITSHIFT_NONE;
    hadc[i].Init.Oversampling.TriggeredMode         = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
    hadc[i].Init.Oversampling.OversamplingStopReset = ADC_REGOVERSAMPLING_CONTINUED_MODE;
    hadc[i].Init.LowPowerAutoWait = DISABLE;
    hadc[i].Init.GainCompensation = 0;

    /* Init ADC */
    hal_status |= HAL_ADC_Init(&hadc[i]);
    // Calibrate ADC
    hal_status |= HAL_ADCEx_Calibration_Start(&hadc[i], ADC_SINGLE_ENDED);  
    
    // Prepare the channel configuration structure with its fixed members
    sConfig[i].SingleDiff   = ADC_SINGLE_ENDED;
    sConfig[i].OffsetNumber = ADC_OFFSET_NONE;
    sConfig[i].Offset       = 0;
    sConfig[i].OffsetSignedSaturation = DISABLE;  
    sConfig[i].Channel       = ADC_CHANNEL_11;// to be fixed later
    sConfig[i].Rank          = ADC_REGULAR_RANK_1;    
    sConfig[i].SamplingTime     = ADC_SAMPLETIME_246CYCLES_5;// ADC_SAMPLETIME_1499CYCLES_5;
    // channels should be configured before starting the ADC
    // hal_status |= HAL_ADC_Start(phadc);
    
    // Initialize the application item
    Analog.ADCs[i].phadc = &hadc[i];
    Analog.ADCs[i].psConfig = &sConfig[i];
  }
  
}

uint32_t ADC_DoSingleConversion(ADC_t *pA, uint32_t HAL_ch, uint32_t SampleTime);
uint32_t ADC_DoSingleConversion(ADC_t *pA, uint32_t HAL_ch, uint32_t SampleTime)
{
  HAL_StatusTypeDef hal_status = HAL_OK;
  // works only on ADC2?

  pA->psConfig->Channel       = HAL_ch;
  pA->psConfig->SamplingTime  = SampleTime;// ADC_SAMPLETIME_1499CYCLES_5;
  
  hal_status |= HAL_ADC_ConfigChannel(pA->phadc, pA->psConfig);
  
  hal_status |= HAL_ADC_Start(pA->phadc);
  
  hal_status |= HAL_ADC_PollForConversion(pA->phadc, 5);
  
  uint16_t lsb = HAL_ADC_GetValue(pA->phadc);

  return lsb;
}




void ADCs_Sweep(void)
{
//    Analog.ADCs[0].Normal_Lsb[1] = ADC_DoSingleConversion(&Analog.ADCs[0], ADC_CHANNEL_1, ADC_SAMPLETIME_246CYCLES_5);
//    Analog.ADCs[0].Normal_Lsb[2] = ADC_DoSingleConversion(&Analog.ADCs[0], ADC_CHANNEL_2, ADC_SAMPLETIME_246CYCLES_5);
    Analog.ADCs[1].Normal_Lsb[4] = ADC_DoSingleConversion(&Analog.ADCs[1], ADC_CHANNEL_4, ADC_SAMPLETIME_246CYCLES_5);
    Analog.ADCs[1].Normal_Lsb[5] = ADC_DoSingleConversion(&Analog.ADCs[1], ADC_CHANNEL_5, ADC_SAMPLETIME_246CYCLES_5);
    Analog.ADCs[1].Normal_Lsb[11] = ADC_DoSingleConversion(&Analog.ADCs[1], ADC_CHANNEL_11, ADC_SAMPLETIME_246CYCLES_5);

    Analog.ADCs[0].Normal_Lsb[4] = ADC_DoSingleConversion(&Analog.ADCs[0], ADC_CHANNEL_4, ADC_SAMPLETIME_246CYCLES_5);
    Analog.ADCs[0].Normal_Lsb[5] = ADC_DoSingleConversion(&Analog.ADCs[0], ADC_CHANNEL_5, ADC_SAMPLETIME_246CYCLES_5);
    Analog.ADCs[0].Normal_Lsb[11] = ADC_DoSingleConversion(&Analog.ADCs[0], ADC_CHANNEL_11, ADC_SAMPLETIME_246CYCLES_5);
    
    //Analog.ADCs[0].Normal_Lsb[5] = ADC_DoSingleConversion(&Analog.ADCs[0], ADC_CHANNEL_VREFINT, ADC_SAMPLETIME_246CYCLES_5);
    //Analog.ADCs[0].Normal_Lsb[6] = ADC_DoSingleConversion(&Analog.ADCs[0], ADC_CHANNEL_TEMp_Sensor, ADC_SAMPLETIME_246CYCLES_5);
} 

void DACs_Init(void);

void DACs_Init(void)
{
  LL_AHB2_GRP1_EnableClock(RCC_AHB2ENR1_DAC1EN);
    
  if (HAL_DAC_Init(&hdac[0]) != HAL_OK)   {
	  Error_Handler();
  }

    /**DAC channel OUT1 config */
  sConf[0].DAC_SignedFormat = DISABLE;
  sConf[0].DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConf[0].DAC_Trigger = DAC_TRIGGER_NONE;
  sConf[0].DAC_HighFrequency = DAC_HIGH_FREQUENCY_INTERFACE_MODE_DISABLE;
  sConf[0].DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConf[0].DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_BOTH;//ENABLE; Another non-portable change.;
  sConf[0].DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac[0], &sConf[0], DAC_CHANNEL_1) != HAL_OK) {
	  Error_Handler();
  }
    /**DAC channel OUT2 config  */
  sConf[0].DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_BOTH;//ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac[0], &sConf[0], DAC_CHANNEL_2) != HAL_OK) {
	  Error_Handler();
  }

// turn on the DACs with default values for now  
  HAL_StatusTypeDef result;
  result = HAL_DAC_SetValue(&hdac[0], DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0x0C00);
  result = HAL_DAC_SetValue(&hdac[0], DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0x0400);
  result = HAL_DAC_Start(&hdac[0], DAC_CHANNEL_1);
  result = HAL_DAC_Start(&hdac[0], DAC_CHANNEL_2);

  // configure DACs
  Analog.DACs[0].Normal_Lsb[0] = 0x0C00;
  Analog.DACs[0].Normal_Lsb[1] = 0x0400;
  Analog.DACs[0].Use_mV[0] = 0;
  Analog.DACs[0].Use_mV[1] = 0;
  
  
}




void AnalogInit(void)
{
  HAL_StatusTypeDef result;
  ADCs_Init();
  DACs_Init();
}


uint32_t AnalogDebug;
void AnalogTest(void)
{
  AnalogInit();
  
  do
  {
    ADCs_Sweep();
    AnalogDebug = HAL_DAC_SetValue(&hdac[0], DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0x0C00);
    AnalogDebug = HAL_DAC_SetValue(&hdac[0], DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0x0400);
    HAL_Delay(1);
    AnalogDebug = HAL_DAC_SetValue(&hdac[0], DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0x0400);
    AnalogDebug = HAL_DAC_SetValue(&hdac[0], DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0x0C00);
    HAL_Delay(1);
    
  }while(0);
}


