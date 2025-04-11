/**
  ******************************************************************************
  * @file    analog.h
  * @author  S.Ma
  * @brief   ...
  * 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018-2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef _ANALOG_H_
#define _ANALOG_H_

// note: This code come from ADC IP V1 which did not support differential signed ADC result values.
// to support differential, modifications will be needed, test will be needed too....
// This module includes all analog resources (ADC/DAC/OpAmp) of the MCU
//extern uint16_t Wave_In[1024];
//extern uint16_t Wave_Out[1024];
//#define SHARED 0x12345678


// Here is the reasoning for the implementation:
// Normal channels have only one single data register.
// We have a RAM info for the buffer (Adr,Size)
// 1 or more channel, use DMA if calling the Waveform function
// Adr = 0: Use ADC internal buffer
// 2+ normal channel: For this one, DMA will be used and the DMA will loop through the size. The size will be in sample count?

// Injected channels have their own individual registers, so these ones will be used without DMA. As interrupting the normal channels, they are irregular coming events...

#if 0
  { ADC_CHANNEL_VREFINT,    ADC_REGULAR_RANK_1,    ADC_SAMPLINGTIME_COMMON_1, ADC_OFFSET_NONE, 0      }, // 1 Internal
  { ADC_CHANNEL_TEMp_Sensor, ADC_REGULAR_RANK_2,    ADC_SAMPLINGTIME_COMMON_1, ADC_OFFSET_NONE, 0       }, // 2 Internal
  { ADC_CHANNEL_4,          ADC_REGULAR_RANK_3,    ADC_SAMPLINGTIME_COMMON_1, ADC_OFFSET_NONE, 0       }, // 3 Analog Joystick
  { ADC_CHANNEL_5,          ADC_REGULAR_RANK_4,    ADC_SAMPLINGTIME_COMMON_1, ADC_OFFSET_NONE, 0       }, // 4 Analog PA4
  { ADC_CHANNEL_1,          ADC_REGULAR_RANK_5,    ADC_SAMPLINGTIME_COMMON_1, ADC_OFFSET_NONE, 0       }, // 3 Analog PA5
  { ADC_CHANNEL_2,          ADC_REGULAR_RANK_6,    ADC_SAMPLINGTIME_COMMON_1, ADC_OFFSET_NONE, 0       }, // 4 Analog PC0
  { ADC_CHANNEL_11,         ADC_REGULAR_RANK_7,    ADC_SAMPLINGTIME_COMMON_1, ADC_OFFSET_NONE, 0       }, // 5 Analog PC1
#endif

void ADC_Config(void);
void ADC_Streams_Init(void);

void ADC_Init(void);
void DAC_Init(void);
void ADCs_Sweep(void);


typedef struct {
  
  ADC_HandleTypeDef* phadc; // can be const or shared
  DMA_HandleTypeDef* phdma_adc; // can be const or shared
  ADC_ChannelConfTypeDef* psConfig; // scratch structure (RAM)
  ADC_InjectionConfTypeDef* psConfigInjected; // scratch structure (RAM)
  
  IO_Pad_t *pPads;
  
  uint16_t      Normal_Lsb[16]; // if no RAM buffer allocated for more than a single normal channel, this one will be used instead. It will also have its conversion in mV for debugging and bringup
  
  uint32_t      VRef_mV; // The ADC supply voltage in mV
  uint32_t      Clock_Hz;
  
  // this is for debug only
  uint16_t      Normal_mV[20]; // this can be updated by interrupts or manually
  int16_t       Injected_Lsb[4];
  int16_t       Injected_mV[4];
  
  int16_t       Temp_degC_x10;
  int16_t       MeasuredVdd_mV; // estimation of Vdd from Vref
  int16_t       MeasuredVBat_mV; // estimation of Vbat from readings
//  int16_t       MeasuredDAC_1_mV;
//  int16_t       MeasuredDAC_2_mV;
  
  uint8_t       NormalDone : 1;
  uint8_t       InjectedDone : 1;
  uint8_t       Overflow : 1;
  uint8_t       Calibration :1;
  
} ADC_t;

typedef struct {

  DAC_HandleTypeDef* phdac;
  DAC_ChannelConfTypeDef* psConfig; // use same name instead of the idiotic sConf
  
  IO_Pad_t* pPad[2];
  
  uint16_t Normal_Lsb[2];
  uint16_t Normal_mV[2];
  
  uint8_t  Use_mV[2];

} DAC_t;

typedef struct {
  
  ADC_t ADCs[2];
  
  DAC_t DACs[2];
  
//  uint8_t Checksum;
  
} Analog_t; // this is the structure which will be exchanged remotely (no pointers)

//uint32_t ADC2_DoSingleConversion(uint32_t ADC_Channel, uint32_t SampleTime);

uint32_t ADC_BackupInjected(ADC_t* A);
uint32_t ADC_LsbTo_mV(ADC_t* A, uint32_t Lsb);
uint32_t ADC_ConvertNormalTo_mV(ADC_t* A);
uint32_t ADC_ConvertInjectedTo_mV(ADC_t* A);

int32_t ADC_Convert_VRefByLsb(ADC_t* A, uint32_t Lsb);
int32_t ADC_FeedbackVdd(ADC_t* A, uint32_t Vdd_mV);
int32_t ADC_Convert_mV_to_DegC_x10(ADC_t* A, uint32_t mV);

void ADC_UpdateConveredValues(ADC_t* A);

void DAC_UpdateConvertedValues(DAC_t* D, ADC_t* A);

extern Analog_t Analog;

typedef enum { ROCK_SEL, ROCK_LEFT, ROCK_UP, ROCK_DOWN, ROCK_RIGHT, ROCK_UNPRESSED } rock_keys_t;
uint8_t RockGetKey(void);
void ActivateRockSwitch(void);

uint32_t ADC_Normal_LsbTo_mV(ADC_t* A, uint32_t Lsb);
uint32_t ADC_Injected_LsbTo_mV(ADC_t* A, uint32_t Lsb);


void AnalogInit(void);
void AnalogTest(void);

#endif
