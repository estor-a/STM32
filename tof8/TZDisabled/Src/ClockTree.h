
#ifndef _CLOCKTREE_H_
#define _CLOCKTREE_H_

#define MHzToHz(a) (((uint32_t)a)*1000000)

void TestClockStrategy(void);

typedef struct {
  uint32_t Min;
  uint32_t Max;
  uint32_t Value;
} RangedValue_t;

typedef struct {

  uint32_t* pInHz; // pointing to another's node frequency
  uint32_t InHz; // the incoming frequency 
  // coefficient related (the variable knob)
  int32_t Min; 
  int32_t Coef;  
  int32_t Max;
  int32_t Step;  
  int32_t ClosestCoef;
  int32_t InitCoef;
  
  // frequency related (the resulting freq)
  uint32_t MinHz;
  uint32_t Hz;
  uint32_t MaxHz;
  
  uint32_t TargetHz;
  uint32_t ClosestHz;
  uint32_t ErrorHz;
  
  uint8_t Done;
  
} PLL_Node_t;

typedef struct {
  // the frequency domains...
  PLL_Node_t* M; // FreqM = In / M [2..63] => Freq [1..2] MHz 2MHz preffered for lower jitter
  PLL_Node_t* N; // VCO = FreqM * N [2..511] => Freq [192..432] MHz
  PLL_Node_t* P; // FreqP = VCO / P [2,4,6,8] => Freq < 180 MHz
  PLL_Node_t* Q; // FreqQ = VCO / Q [2..15] => Freq ... 
  PLL_Node_t* R; // FreqR = VCO / R [2..7] => Freq <192 MHz max error of 0.3%
  
} PLL_Clocks_t;

void Init_PLL_Node(PLL_Node_t* X, int32_t Step, uint32_t InHz, int32_t Min, int32_t Max, uint32_t MinHz, uint32_t MaxHz, uint32_t TargetHz);
void PLL_Init(PLL_Clocks_t* PLL, PLL_Node_t* M, PLL_Node_t* N, PLL_Node_t* P, PLL_Node_t* Q, PLL_Node_t* R);
//uint32_t SolvePLL(PLL_Clocks_t* PLL, uint8_t Skip, uint8_t SweepAll);
void TestPLL(void);

//=========================================== 

typedef struct {
  
  uint32_t Gear;
  uint32_t FlashWaitState;
  uint32_t Core_Hz;
  uint32_t OverDrive;
  uint32_t DataCacheEnable;
  
} Gear_t;
/*
typedef struct {
  uint32_t Min;
  uint32_t Max;
  uint32_t Value;
} RangedValue_t;
*/
typedef struct {

Gear_t Gears[5]; // all 5 gears must exist, even if it's the same cloned 5 times.
uint32_t CurrentGear;
uint32_t MaxGears; // how many real gears we can use.

PLL_Clocks_t* MainPLL; // NULL = PLL not to be used
PLL_Clocks_t* I2C_PLL; // NULL = PLL not to be used
PLL_Clocks_t* SAI_PLL; // NULL = PLL not to be used

RangedValue_t Vdd_mV; // if Vdd, no voltage information is available, we will take the most conservative option then. To override it, put 3.3V
RangedValue_t MCU_mA; // the current core estimated consumption in mA
RangedValue_t MCU_mW;

uint32_t InHSE_Hz; // if zero means no clock feed from external oscin/oscout
uint32_t InHSI_Hz; // internal oscillator, if put zero here means it is not allowed to use it (ETH/USB/...) (16MHz)(16000000)

uint32_t InLSE_Hz; // same here
uint32_t InLSI_Hz; // same (32kHz)(32000)

uint32_t InI2SClk_Hz; // external clock feed, 0 if not available
uint32_t InETH_Clk_Hz; // external feed, 0 if not available
uint32_t InOTG_Clk_Hz; // external feed, 0 if not available

uint32_t ADC_Max_Hz; // Single ADC Max conversion rate (Vdd dependent)
uint32_t DAC_Max_Hz; // Single DAC Max conversion rate (?)

RangedValue_t OutSysClk_Hz;
RangedValue_t OutCoreClk_Hz;
RangedValue_t OutAPB1Clk_Hz;
RangedValue_t OutAPB2Clk_Hz;
RangedValue_t OutSAIClk_Hz;
RangedValue_t OutI2SClk_Hz; // to cook
RangedValue_t OutMainClk_Hz; // to cook
RangedValue_t Out48MHzClk_Hz;

RangedValue_t OutMCO1_Hz;
RangedValue_t OutMCO2_Hz;
RangedValue_t OutRTC_Hz;

//  IO_Pin_t* SDA; // we need the pointer to the pin
//  uint32_t (*fnWaitMethod)(uint32_t);
//  uint32_t ctWaitMethod;
  
} MCU_Clocks_t; // there should be only one globally for this one

extern MCU_Clocks_t MCU_Clocks; // this is the global clock tree

void SetMCU_Gear(MCU_Clocks_t* C, uint32_t Gear);
void MCU_GearUp(MCU_Clocks_t* C);
void MCU_GearDown(MCU_Clocks_t* C);


void FindMaxSysClkFromMax_mA(MCU_Clocks_t* C, uint32_t Max_mA);
void EstimatePower(MCU_Clocks_t* C);
void SetMCU_Gears(MCU_Clocks_t* C);
void SetMinSysClkAPB1APB2_Hz(MCU_Clocks_t* C, uint32_t SysClkMin_Hz, uint32_t APB1Min_Hz, uint32_t APB2Min_Hz);
void SetMaxSysClkFromVdd_mV(MCU_Clocks_t* C, uint32_t Vdd_mV);
void ClockTreeInit(MCU_Clocks_t* C, uint32_t InHSE_Hz, uint32_t InLSE_Hz, uint32_t I2S_CKIN_Hz, uint32_t InETH_Clk_Hz, uint32_t InOTG_Clk_Hz );



#endif
