/**
  ******************************************************************************
  * @file           : main.h
    @author         : Alexandre ESTOR
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
    @date           : 4 july 2024
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

#ifndef __AE_VL53L8_H
#define __AE_VL53L8_H
    

#define nbPadPerLine4x4 4
#define nbPadPerLine8x8 8
#define nbPadPerLine nbPadPerLine8x8 // User selection done HERE
#define modeTof (nbPadPerLine * nbPadPerLine)


#define M_PI 3.14159265358979323846
    
#include "vl53l8cx_api.h"

typedef struct{
  
  uint16_t rawArray[modeTof];        // this tab will get the raw value from the TOF
  uint16_t sortedArray[modeTof];     // this tab will get the raw value sorted ascending
      
  union {
    uint16_t parsedArray[modeTof]; // // this bab wil have only the good value
    uint16_t parsedArrayXY[nbPadPerLine][nbPadPerLine]; // this bab wil have only the good value with 2 dimensions
  };
   
  uint16_t aveFront; // this var will get the average distance from the 8 value in front of the Tof
  uint16_t aveBack; // this var will get the average distance from the 8 value in back of the Tof
  uint16_t aveRight; // this var will get the average distance from the 8 value in the right of the Tof
  uint16_t aveLeft; // this var will get the average distance from the 8 value in the Left of the Tof
  uint16_t aveHand; // this var will get the average distance of the hand
  
  uint8_t flatSide; //this flag is high when the distance difference between the right side and the left is < 15mm
  uint8_t flatForward; //this flag is high when the distance difference between the front and the back is < 15mm
  // with these two var we can know where is the hand relative to the Tof
  float centerValueX; // this var will have the X center of the hand
  float centerValueY; // this var will have the Y center of the hand
  
  float distanceBetweenPad; //This var represent the real distance between two case of the tab. It changes because it depend of the FOV and the distance between hand and censor 
  float angleX; // this var will have the angle in degres of the hand relative to the Tof. 0 degres is flat, 45 degres is when the hand is forward, -45 when back
  float angleY; // this var will have the angle in degres of the hand relative to the Tof. 0 degres is flat, 45 degres is when the hand is right, -45 when left
   
  int8_t position;// this var represent the position of the object in front of the tof8
                  // 0 = flat, 1 = front, 2 = front-right, 3 = right, 4 = back-right, 5 = back, 6 = back-left, 7 = left, 8 = front-left
   uint8_t click; //this var represent the click in the joystick. If the average distance of the hand is < 15 cm then there is a click     
   
} AE_vl53l8_t;

extern AE_vl53l8_t g_Sensor;

void sortAscendingArray(VL53L8CX_ResultsData *data, AE_vl53l8_t *p_Sensor);
uint16_t returnCenterValue_mm(AE_vl53l8_t *p_Sensor);
void parseDataTof8(AE_vl53l8_t *p_Sensor, uint16_t value, VL53L8CX_ResultsData *data);
uint16_t Tof_inRange_u16(uint16_t aimValue, uint16_t currentValue);
uint16_t AverageDistanceHand(uint16_t* data);
void findCenterHand(AE_vl53l8_t *p_Sensor);
void processDataTof8(AE_vl53l8_t *p_Sensor, VL53L8CX_ResultsData *data);
void averageDistanceFront_mm(AE_vl53l8_t *p_Sensor);
void averageDistanceBack_mm(AE_vl53l8_t *p_Sensor);
void averageDistanceRight_mm(AE_vl53l8_t *p_Sensor);
void averageDistanceLeft_mm(AE_vl53l8_t *p_Sensor);
float returnDistanceBetweenTwoPad_mm(uint16_t distance);
void linearRegressionX(AE_vl53l8_t *p_Sensor);
void linearRegressionY(AE_vl53l8_t *p_Sensor);
void isClicked(AE_vl53l8_t *p_Sensor);
void computePosition(AE_vl53l8_t *p_Sensor);

void printResult(LCD_16x2_t* pL, AE_vl53l8_t *p_Sensor);
#endif
