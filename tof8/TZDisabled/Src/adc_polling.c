
#include "main.h"


const uint32_t VREF_PLUS_VOLT_MV = 3300UL;

/*
#define TEMp_Sensor_TYP_CAL1_V          (( int32_t)  760)        // Internal temperature sensor, parameter V30 typical value (unit: mV). Refer to device datasheet for min/typ/max values. 
#define TEMp_Sensor_TYP_AVGSLOPE        (( int32_t) 2500)        // Internal temperature sensor, parameter Avg_Slope typical value (unit: uV/DegCelsius). Refer to device datasheet for min/typ/max values. 
    temperature_degC = __LL_ADC_CALC_TEMPERATURE_TYP_PARAMS(TEMp_Sensor_TYP_AVGSLOPE,
                                                            TEMp_Sensor_TYP_CAL1_V,
                                                            TEMp_Sensor_CAL1_TEMP, // Defined in LL Driver 
                                                            VDDA_APPLI,           // Value of VDDA in (unit: mV)(depends on board, typical value : 3300mV) 
                                                            temperature_adc_raw_data,
                                                            LL_ADC_RESOLUTION_12B);
 
La valeur typique de calibration pour VREF c'est 1504 à 3.3V (0x5E0)
*/


uint32_t adc_data_raw;
uint32_t adc_data_mV;

ADC_HandleTypeDef     hadc1 = {0,};
ADC_HandleTypeDef     hadc2 = {0,};

uint32_t ADC1_DoSingleConversion(uint32_t ADC_Channel, uint32_t SampleTime);
uint32_t ADC2_DoSingleConversion(uint32_t ADC_Channel, uint32_t SampleTime);

HAL_StatusTypeDef ADC_Polling_Init(ADC_HandleTypeDef* phadc, ADC_TypeDef * ADCx);

HAL_StatusTypeDef ADC_Polling_Init(ADC_HandleTypeDef* phadc, ADC_TypeDef * ADCx) {

  /* Configure PWR for ADC */
  LL_AHB1_GRP2_EnableClock(LL_AHB1_GRP2_PERIPH_PWR);
  LL_PWR_EnableVDDA();
  
  
  __HAL_RCC_VREF_CLK_ENABLE();
#if 1 // Vref is not HW tied to Vdda, so we need to activate it
//  HAL_SYSCFG_VREFBUF_VoltageScalingConfig(SYSCFG_VREFBUF_VOLTAGE_SCALE3);
//  HAL_SYSCFG_EnableVREFBUF();
#endif  
  
  
  /* Configure RCC for ADC kernel clock */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC12);
  /* Clock for sleep mode */

  HAL_StatusTypeDef hal_status = HAL_OK;
  phadc->Instance = ADCx;
  phadc->State = HAL_ADC_STATE_RESET;
  
  phadc->Init.Resolution     = ADC_RESOLUTION_12B;
  phadc->Init.ScanConvMode   = ADC_SCAN_DISABLE;
  phadc->Init.EOCSelection   = ADC_EOC_SINGLE_CONV;
  phadc->Init.LowPowerAutoWait      = DISABLE;
  phadc->Init.ContinuousConvMode    = DISABLE; //ENABLE;
  phadc->Init.NbrOfConversion       = 1;
  phadc->Init.DiscontinuousConvMode = DISABLE;
  phadc->Init.NbrOfDiscConversion   = 1;
  phadc->Init.ExternalTrigConv      = ADC_SOFTWARE_START;
  phadc->Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
  phadc->Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  phadc->Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
  phadc->Init.Overrun                  = ADC_OVR_DATA_OVERWRITTEN;
  phadc->Init.OversamplingMode         = DISABLE;
  phadc->Init.Oversampling.Ratio                 = 1;
  phadc->Init.Oversampling.RightBitShift         = ADC_RIGHTBITSHIFT_NONE;
  phadc->Init.Oversampling.TriggeredMode         = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
  phadc->Init.Oversampling.OversamplingStopReset = ADC_REGOVERSAMPLING_CONTINUED_MODE;
  phadc->Init.LowPowerAutoWait = DISABLE;
  phadc->Init.GainCompensation = 0;

//  RCC_PeriphCLKInitTypeDef PeriphClkInit;
//  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADCDAC;          /*!< The Extended Clock to be configured. This parameter can be a value of @ref RCCEx_Periph_Clock_Selection */
//  PeriphClkInit.AdcDacClockSelection = RCC_ADCDACCLKSOURCE_HCLK;      /*!< Specifies ADCDAC clock source. This parameter can be a value of @ref RCCEx_ADCDAC_Clock_Source */
//  PeriphClkInit.AdcDacClockDivider   = RCC_ADCDACCLK_DIV4;            /*!< Specifies ADCDAC clock divider. This parameter can be a value of @ref RCCEx_ADCDACDIV_Clock_Divider */
//  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
  
  /* Init ADC */
  hal_status |= HAL_ADC_Init(phadc);
#if 0  
  ADC_ChannelConfTypeDef sConfig;
  sConfig.SingleDiff   = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset       = 0;
  sConfig.OffsetSignedSaturation = DISABLE;

  if(ADCx==ADC1) {
    sConfig.Channel = ADC_CHANNEL_VREFINT;
    sConfig.Rank    = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_1499CYCLES_5;
    hal_status |= HAL_ADC_ConfigChannel(phadc, &sConfig);
  }

  if(ADCx==ADC2) {
    sConfig.Channel = ADC_CHANNEL_11;
    sConfig.Rank    = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_246CYCLES_5;// ADC_SAMPLETIME_1499CYCLES_5;
    hal_status |= HAL_ADC_ConfigChannel(phadc, &sConfig);
  }
#endif
  
  hal_status |= HAL_ADCEx_Calibration_Start(phadc, ADC_SINGLE_ENDED);
  
  /* Start ADC group regular conversion */
  hal_status |= HAL_ADC_Start(phadc);
  return hal_status;
}  
  
// can we convert from a pin and signal to the channel and ADCn ?

uint32_t ADC1_DoSingleConversion(uint32_t ADC_Channel, uint32_t SampleTime) // Brute force to bringup the peripheral on ADC2 only
{
  HAL_StatusTypeDef hal_status = HAL_OK;
  // works only on ADC2?

  ADC_ChannelConfTypeDef sConfig;
  sConfig.SingleDiff   = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset       = 0;
  sConfig.OffsetSignedSaturation = DISABLE;  
  sConfig.Channel       = ADC_Channel;
  sConfig.Rank          = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime  = SampleTime;// ADC_SAMPLETIME_1499CYCLES_5;
  
  hal_status |= HAL_ADC_ConfigChannel(&hadc1, &sConfig);
  
  hal_status |= HAL_ADC_Start(&hadc1);
  
  hal_status |= HAL_ADC_PollForConversion(&hadc1, 5);
  
  uint16_t lsb = HAL_ADC_GetValue(&hadc1);

  return lsb;
}

uint32_t ADC2_DoSingleConversion(uint32_t ADC_Channel, uint32_t SampleTime) // Brute force to bringup the peripheral on ADC2 only
{
  HAL_StatusTypeDef hal_status = HAL_OK;
  // works only on ADC2?

  ADC_ChannelConfTypeDef sConfig;
  sConfig.SingleDiff   = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset       = 0;
  sConfig.OffsetSignedSaturation = DISABLE;  
  sConfig.Channel       = ADC_Channel;
  sConfig.Rank          = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime  = SampleTime;// ADC_SAMPLETIME_1499CYCLES_5;
  
  hal_status |= HAL_ADC_ConfigChannel(&hadc2, &sConfig);
  
  hal_status |= HAL_ADC_Start(&hadc2);
  
  hal_status |= HAL_ADC_PollForConversion(&hadc2, 5);
  
  uint16_t lsb = HAL_ADC_GetValue(&hadc2);

  return lsb;
}

  
void ADC_Polling_Test(void);

uint32_t adc_vals_lsb[10];
uint32_t adc_vals_mV[10];

void ADC_Init(void);
int32_t ADC_Scan(void);

void ADC_Polling_Test(void) {

  
  HAL_StatusTypeDef hal_status = HAL_OK;
  ADC_Init();
 /* Infinite loop */
  do
  {
#if 1
    adc_vals_lsb[0] = ADC1_DoSingleConversion(ADC_CHANNEL_1, ADC_SAMPLETIME_246CYCLES_5);
    adc_vals_lsb[1] = ADC1_DoSingleConversion(ADC_CHANNEL_2, ADC_SAMPLETIME_246CYCLES_5);
    adc_vals_lsb[2] = ADC1_DoSingleConversion(ADC_CHANNEL_4, ADC_SAMPLETIME_246CYCLES_5);
    adc_vals_lsb[3] = ADC1_DoSingleConversion(ADC_CHANNEL_5, ADC_SAMPLETIME_246CYCLES_5);
    adc_vals_lsb[4] = ADC1_DoSingleConversion(ADC_CHANNEL_11, ADC_SAMPLETIME_246CYCLES_5);
    adc_vals_lsb[5] = ADC1_DoSingleConversion(ADC_CHANNEL_VREFINT, ADC_SAMPLETIME_246CYCLES_5);
    adc_vals_lsb[6] = ADC1_DoSingleConversion(ADC_CHANNEL_TEMp_Sensor, ADC_SAMPLETIME_246CYCLES_5);
    NOPs(1);
#else
    ADC_Scan();
#endif    
  }while(0);
}


void ADC_Init(void)
{
  ADC_Polling_Init(&hadc1, ADC1);
  ADC_Polling_Init(&hadc2, ADC2);
}


int32_t ADC_Scan(void)
{
  HAL_StatusTypeDef hal_status = HAL_OK;  
  
  hal_status |= HAL_ADC_Start(&hadc1);
  hal_status |= HAL_ADC_Start(&hadc2);
  
  hal_status |= HAL_ADC_PollForConversion(&hadc1, 5);
  hal_status |= HAL_ADC_PollForConversion(&hadc2, 5);

  adc_vals_lsb[1] = HAL_ADC_GetValue(&hadc1);
  adc_vals_lsb[2] = HAL_ADC_GetValue(&hadc2);
  
  return adc_vals_lsb[2];
   
}

void DAC_Init(void);
#if 0
void DAC_Init(void)
{
//==== DAC initialization

  DAC_ChannelConfTypeDef sConf;

  /**DAC Initialization */
  hdac1.Instance = DAC1;
  if (HAL_DAC_Init(&hdac1) != HAL_OK)   {
	  Error_Handler();
  }

    /**DAC channel OUT1 config */
  sConf.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConf.DAC_Trigger = DAC_TRIGGER_NONE;
  sConf.DAC_HighFrequency = DAC_HIGH_FREQUENCY_INTERFACE_MODE_DISABLE;
  sConf.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConf.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_BOTH;//ENABLE; Another non-portable change.
  sConf.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConf, DAC_CHANNEL_1) != HAL_OK) {
	  Error_Handler();
  }
    /**DAC channel OUT2 config  */
  sConf.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConf, DAC_CHANNEL_2) != HAL_OK) {
	  Error_Handler();
  }

// turn on the DACs with default values for now  
  HAL_StatusTypeDef result;
  result = HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0x0C00);
  result = HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0x0400);
  result = HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
  result = HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);

  // configure DACs
  Analog.DACs.Normal_Lsb[0] = 0x0C00;
  Analog.DACs.Normal_Lsb[1] = 0x0400;
  Analog.DACs.Use_mV[0] = 0;
  Analog.DACs.Use_mV[1] = 0;
  
}
#endif