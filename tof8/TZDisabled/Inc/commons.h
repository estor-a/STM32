/**
  ******************************************************************************
  * @file    commons.h
  * @author  S.Ma
  * @brief   My macros...
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

#ifndef _COMMONS_H_
#define _COMMONS_H_

//u32 u32_fn_u32(u32 u); // for typeless function pointers.
#include <stdarg.h>
 // to be able to use printf(...) it's not MISRA compliant, however, we need it here.



#define gap2(x,y) (((x) < (y)) ? ((y) - (x)) : ((x) - (y)))
#define min2(x,y)		(((x) < (y)) ? (x) : (y)) 
#define max2(x,y)		(((x) > (y)) ? (x) : (y)) 
#define min3(x,y,z)	((((x) < (y)) && ((x) < (z))) ? (x) : (((y) < (z)) ? (y) : (z)))
#define max3(x,y,z)	((((x) > (y)) && ((x) > (z))) ? (x) : (((y) > (z)) ? (y) : (z)))

//#ifndef LOBYTE
//#define LOBYTE(w)		((u8)(w))
//#endif
//#ifndef HIBYTE
//#define HIBYTE(w)		((u8)(((u16)(w) >> 8) & 0xFF))
//#endif
#define HIWORD(w)		((u16)(w>>16))  
#define LOWORD(w)		((u16)(w))
#define MAKEWORD_HI_LO(b, a)	((uint16_t)(((uint8_t)(a)) | ((uint16_t)((uint8_t)(b))) << 8))
#define MAKEDWORD_HI_LO(d,c,b,a)    ((uint32_t)((uint8_t)(a))) | (((uint32_t)((uint8_t)(b))) << 8) | (((uint32_t)((uint8_t)(c))) << 16) | (((uint32_t)((uint8_t)(d))) << 24)
#define ABS(x)                 ((x) < 0 ? -(x) : (x))
#define LIMIT(x,min,max)       {if ((x) <= (min)) (x) = (min); else if ((x) >= (max)) (x) = (max);}

int32_t Interpolate_i32 (int32_t x0, int32_t x1, int32_t y0, int32_t y1, int32_t x);

#define countof(a) (sizeof(a) / sizeof(*(a)))

#ifndef DIV_ROUND_CLOSEST
#define DIV_ROUND_CLOSEST(x, d)  (((x) + ((d) / 2U)) / (d))
#endif

// Delay methods
void NOP_Init(void);
void NOPs(uint32_t loop);

// read about debug cycle counter for Cortex M (except M0) MCU core
// https://embeddedcomputing.com/technology/processing/measuring-code-execution-time-on-arm-cortex-m-mcus
void CNT_Init(void);
uint32_t CNT_GetTimeCy(void);
int32_t CNT_Delay_cy(uint32_t cy);
int32_t CNT_Delay_us(uint32_t us);
int32_t CNT_cy2us_x10(uint32_t cy);

void MinDelay_us(uint32_t us);
void MinDelay_ms(uint32_t ms);

#define TrapError() while(1)
#define TrapWarning() while(1)

void CopyFromToByteSize (void* src, const void* dst, uint32_t cnt);
void FillByteToByteSize ( uint8_t ByteValue, const void* dst, uint32_t cnt);
void StringCopy (void* dst, const void* src, uint32_t cnt);
uint32_t BytesIdenticals (const void* dst, const void* src, uint32_t cnt);
uint32_t StringsIdenticals (const void* dst, const void* src, uint32_t cnt);

#include "main.h" // where all the LL_*.h files are includes

// one point where HAL_Delay can be hacked in one point
void MinDelay_ms(uint32_t ms); // this can be SW delay with non exact delay.
void MinDelay_us(uint32_t us);



#endif