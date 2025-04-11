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
#include "main.h"

 AE_vl53l8_t g_Sensor = {.position = -1, .click = 0, .flatSide = 0, .flatForward = 0, .aveFront = 0, .aveBack = 0, .aveRight = 0, .aveLeft = 0, .aveHand = 0}; //declare the struct wich contains every usefull data from the sensor 

void sortAscendingArray(VL53L8CX_ResultsData *data, AE_vl53l8_t *p_Sensor)
{
    uint32_t i = 0; //current index
    uint32_t j = 0; //index for the min value in data tab
    uint32_t k = 0; //index for the sorted array
    uint32_t min;// lower value
    
    for(i = 0; i < modeTof ; i++) //copy data array into temp array
      p_Sensor->rawArray[i] = (uint16_t)data->distance_mm[i];
    
    while(k < modeTof) // while the destination array is not fully sorted
    {
      i = 0;
      j = 0;
      min = 4000;
      while(i < modeTof)
      {
          if((p_Sensor->rawArray[i] <= min )&&( p_Sensor->rawArray[i] > 0))
          {
            min = p_Sensor->rawArray[i];
            j = i;
          }
          i++;
      }
      p_Sensor->sortedArray[k] = p_Sensor->rawArray[j]; // fill the sorted Array
      p_Sensor->rawArray[j] = 0;
      k++;
    }
    return;
}

uint16_t returnCenterValue_mm(AE_vl53l8_t *p_Sensor)// this function will return the value with the most value close to him(used to delete unwanted value and keep only value from our hand)
{
  uint32_t i = 0;       //index for the sorted Array
  uint32_t j = 0;       // index use to find friend 
  uint32_t k= -1;   //index of the point with the most friend
  uint32_t currentFriend;       // number of value near the current value +- 75
  uint32_t maxFriend = 0;      // number of friend  max for a value got 
  
  while(i < modeTof)
  {
    j = 0;
    currentFriend = 0;
    while(j < modeTof)
    {
      if(abs(p_Sensor->sortedArray[i] - p_Sensor->sortedArray[j]) <= 50 && p_Sensor->sortedArray[i] < 1000) // limit the maximum measuring distance of the sensor to 1m
        currentFriend++;
      j++;
    }
    if(currentFriend > maxFriend)
    {
        maxFriend = currentFriend;
        k = i;
    }
    i++;
  }
  if(k == -1)
    return(0);
  return(p_Sensor->sortedArray[k]); // return the value wich have the most value near it(+-50mm)  ex : (100, 125, 3, 3324, 49, 150, 160) output : 125 
  
}

uint16_t Tof_inRange_u16(uint16_t aimValue, uint16_t currentValue) // this function return 0 if the currentValue is 50 mm away form aimValue or if current value > 1900mm wich mean this value is not wanted
{
    if(abs(aimValue - currentValue) > 50 || currentValue > 1900)
      return(0);
    return(1); // currentValue is ok
}

void parseDataTof8(AE_vl53l8_t *p_Sensor, uint16_t value, VL53L8CX_ResultsData *data) // this function will fill an array with the value from the tof8 without the false value
{
    for(uint16_t i = 0; i < modeTof; i++)  // this for loop get the result from the sensor and put the value into another array while suppressing bad value ( when nothing is detected by the sensor)
    {
      if(Tof_inRange_u16(value, data->distance_mm[i]))
         p_Sensor->parsedArray[i] = data->distance_mm[i];// we write good value into parsedArray
      else
        p_Sensor->parsedArray[i] = 0;   // we write that this value is not good by writing 0
    } 
    return;
}
    
    
void findCenterHand(AE_vl53l8_t *p_Sensor) // this function will return the position in X and Y of the hand relative to the Tof
 {     
    uint32_t i = 0;
    uint32_t tempx = 0, tempy = 0;
    uint32_t times = 0;
   
    while(i < modeTof)
    {
      if(p_Sensor->parsedArray[i])
      {
         tempx += (i / nbPadPerLine);//we want the value from 0 to 3 equal 0, value from 4 to 7 equal 1, value from 8 to 11 equal 2 
         tempy += (i % nbPadPerLine);// we want the value 0, 4, 8, 12 equal0, 1, 5, 9, 13 equal 1 ...
         times++;
      }
      i++;
    }
    p_Sensor->centerValueX = ((float)tempx) /times;
    p_Sensor->centerValueY = ((float)tempy) /times;
    return;
}

uint16_t AverageDistanceHand(uint16_t* data)
{
   uint32_t sum = 0;
   uint32_t times = 0;
   uint32_t i = 0;
   
   while(i < modeTof)
   {
      if(data[i])
      {
          times++;
          sum += data[i];
      } 
      i++;
   }
   sum = sum / times;
   return(uint16_t)sum;
}


/*
void averageDistanceFront_mm(AE_vl53l8_t *p_Sensor) // this function return the average distance in front of the tof8
{
    uint32_t times = 0;
    uint32_t average = 0;
    uint32_t i = 0;
    while(i < 8) // the front of the sensor are represented by the 8 first value of the parsedArray
    {
      if(p_Sensor->parsedArray[i])
      {
        times++;
        average += p_Sensor->parsedArray[i];
      }
      i++;
    }
    p_Sensor->aveFront =  (average / times);
}


void averageDistanceBack_mm(AE_vl53l8_t *p_Sensor) // this function return the average distance in the back of the tof8
{
    uint32_t times = 0;
    uint32_t average = 0;
    uint32_t i = 8;
    while(i < modeTof) //the back of the sensor are represented by the 8 last value of the parsedArray
    {
      if(p_Sensor->parsedArray[i])
      {
        times++;
        average += p_Sensor->parsedArray[i];
      }
      i++;
    }
   p_Sensor->aveBack = (average / times);
}

void averageDistanceRight_mm(AE_vl53l8_t *p_Sensor) // this function return the average distance in the right of the tof8
{
    uint32_t times = 0;
    uint32_t average = 0;
    uint32_t i = 0;
    while(i < modeTof)
    {
      if(p_Sensor->parsedArray[i])
      {
        times++;
        average += p_Sensor->parsedArray[i];
      }
      if(i % 2) // this if else is to choose only data from the right side (its not 1, 2, 3, 4) but (0, 1, 4, 5, etc...)
        i+=3;
      else
        i++;
    }
    p_Sensor->aveRight = (average / times);
}

void averageDistanceLeft_mm(AE_vl53l8_t *p_Sensor)// this function return the average distance in the left of the tof8
{
    uint32_t times = 0;
    uint32_t average = 0;
    uint32_t i = 2;
    while(i < modeTof)
    {
      if(p_Sensor->parsedArray[i])
      {
        times++;
        average += p_Sensor->parsedArray[i];
      }
      if(i % 2) // this if else is to choose only data from the left side (its not 1, 2, 3, 4) but (2, 3, 6, 7, etc...)
        i+=3;
      else
        i+=1;
    }
    p_Sensor->aveLeft = (average / times);
}
*/

float returnDistanceBetweenTwoPad_mm(uint16_t distance) // the distance between each pad depend on the distance between hand and sensor cause of the FOV
{
  float DistanceEdgeToEdgeAt1000mm =  707.106781; // sin(FOV) * 1000 FOV = 45deg
  float DistancePadToPadAt1000mm = DistanceEdgeToEdgeAt1000mm / nbPadPerLine; // there is 4 pad in one line
  return(((float)distance * DistancePadToPadAt1000mm) / 1000); 
}



void linearRegressionX(AE_vl53l8_t *p_Sensor) // this function calcul the X angle of the Hand. 0 is flat, 45° is when hand face forward, -45° is when hand face backward
{  
    float slope = 0, intercept = 0;
    uint16_t count = 0;
    uint16_t val = 0;
    uint16_t x =0;
    float sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    for (int i = 0; i < modeTof; i++)
    {
      val = p_Sensor->parsedArray[i];
      x =  (p_Sensor->distanceBetweenPad) * (i/nbPadPerLine); 
      if(val == 0)
        continue;
      
        sumX += x;
        sumY += val;
        sumXY += x * val;
        sumX2 += x * x;
        count++;     
    }
    if(count == 0)
      return;
    slope = ( (float)count * sumXY - sumX * sumY) / ((float)count * sumX2 - sumX * sumX);
    intercept = ((float)sumY - slope * sumX) / count;
    
    p_Sensor->angleX = (float)atan(slope);
    p_Sensor->angleX = (p_Sensor->angleX * 180) / M_PI;
    return;
}// for this function I took a lot of inspiration from here : https://www.codeease.net/programming/c/linear-regression-in-c

void linearRegressionY(AE_vl53l8_t *p_Sensor)  // this function calcul the Y angle of the Hand. 0 is flat, 45° is when hand face Right side, -45° is when hand face Left side
{  
    float slope = 0, intercept = 0;
    uint16_t count = 0;
    uint16_t val = 0;
    uint16_t x =0;
    float sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    for (int i = 0; i < modeTof; i++)
    {
      val = p_Sensor->parsedArray[i];
      x =  p_Sensor->distanceBetweenPad * (i%nbPadPerLine);
      if(val == 0)
        continue;
      
        sumX += x;
        sumY += val;
        sumXY += x * val;
        sumX2 += x * x;
        count++;     
    }
    if(count == 0)
      return;
    slope = ( (float)count * sumXY - sumX * sumY) / ((float)count * sumX2 - sumX * sumX);
    intercept = ((float)sumY - slope * sumX) / count;
    
    p_Sensor->angleY = atan(slope);
    p_Sensor->angleY = (p_Sensor->angleY * 180) / M_PI;
    return;
}//  for this function I took a lot of inspiration from here : https://www.codeease.net/programming/c/linear-regression-in-c


void computePosition(AE_vl53l8_t *p_Sensor)
{
  if(abs(p_Sensor->angleX) < 12) // check if more than 12° tilt for the front. If not it's not significant
      p_Sensor->flatForward = 1;
   else
      p_Sensor->flatForward = 0;
    
   if(abs(p_Sensor->angleY) < 12) // check if more than 12° tilt for the side. If not it's not significant
      p_Sensor->flatSide = 1;
   else
      p_Sensor->flatSide = 0;
      
   if(p_Sensor->flatSide && p_Sensor->flatForward)
      p_Sensor->position = 0; //flat
    
   else if(p_Sensor->flatSide && !p_Sensor->flatForward && p_Sensor->angleX > 12)
      p_Sensor->position = 1; //front
    
   else if(p_Sensor->angleX > 12 && p_Sensor->angleY > 12 && !p_Sensor->flatForward && !p_Sensor->flatSide)
     p_Sensor->position = 2; //front_right
    
   else if(p_Sensor->flatForward && p_Sensor->angleY > 12 && !p_Sensor->flatSide)
      p_Sensor->position = 3; //right
      
   else if(p_Sensor->angleX < -12 && p_Sensor->angleY > 12 && !p_Sensor->flatForward && !p_Sensor->flatSide)
      p_Sensor->position = 4; // back-right
    
   else if(p_Sensor->angleX < -12  && p_Sensor->flatSide && !p_Sensor->flatForward)
      p_Sensor->position = 5; //back
    
   else if(p_Sensor->angleX < -12  && p_Sensor->angleY < -12  && !p_Sensor->flatForward && !p_Sensor->flatSide)
      p_Sensor->position = 6;// back_left
    
   else if(p_Sensor->flatForward && p_Sensor->angleY < -12 && !p_Sensor->flatSide)
      p_Sensor->position = 7; // left
    
   else if(p_Sensor->angleX > 12 && p_Sensor->angleY < -12 && !p_Sensor->flatForward && !p_Sensor->flatSide)
      p_Sensor->position = 8; // front-left
   else
      p_Sensor->position = -1;

}

void isClicked(AE_vl53l8_t *p_Sensor)
{
     if(p_Sensor->aveHand < 150)
      p_Sensor->click = 1;    
    else if (p_Sensor->click && p_Sensor->aveHand > 200)
      p_Sensor->click = 0;
    
}

void printResult(LCD_16x2_t* pL, AE_vl53l8_t *p_Sensor)
{
    if(p_Sensor->position == -1)
      return;
        
    if(p_Sensor->position == 0)
    {
      LCD_16x2_ClearScreen_RAM(pL);
      LCD_16x2_Printf(pL, "flat");
    }
    if(p_Sensor->position == 1)
    {
      LCD_16x2_ClearScreen_RAM(pL);
      LCD_16x2_Printf(pL, "front");
    }
    if(p_Sensor->position == 2)
    {
      LCD_16x2_ClearScreen_RAM(pL);
      LCD_16x2_Printf(pL, "front-right");
    }
    if(p_Sensor->position == 3)
    {
      LCD_16x2_ClearScreen_RAM(pL);
      LCD_16x2_Printf(pL, "right");
    }
    if(p_Sensor->position == 4)
    {
      LCD_16x2_ClearScreen_RAM(pL);
      LCD_16x2_Printf(pL, "back-right");
    }
    if(p_Sensor->position == 5)
    {
      LCD_16x2_ClearScreen_RAM(pL);
      LCD_16x2_Printf(pL, "back");
    }
    if(p_Sensor->position == 6)
    {
      LCD_16x2_ClearScreen_RAM(pL);
      LCD_16x2_Printf(pL, "back-left");
    }
    if(p_Sensor->position == 7)
    {
      LCD_16x2_ClearScreen_RAM(pL);
      LCD_16x2_Printf(pL, "left");
    }
    if(p_Sensor->position == 8)
    {
      LCD_16x2_ClearScreen_RAM(pL);
      LCD_16x2_Printf(pL, "front-left");
    }
        
        LCD_16x2_RAM_GOTO_X_Y(pL, 0,1);
	LCD_16x2_Printf(pL, "X = %d, Y = %d", (int)p_Sensor->angleX, (int)p_Sensor->angleY);
	LCD_16x2_RAM_to_Display(pL);
}

void processDataTof8(AE_vl53l8_t *p_Sensor, VL53L8CX_ResultsData *data)
{
  
     
    sortAscendingArray(data, p_Sensor);
    parseDataTof8(p_Sensor, returnCenterValue_mm(p_Sensor), data); 
    findCenterHand(p_Sensor);
    p_Sensor->aveHand = AverageDistanceHand(p_Sensor->parsedArray);
    p_Sensor->distanceBetweenPad = returnDistanceBetweenTwoPad_mm(p_Sensor->aveHand);
    linearRegressionX(p_Sensor);
    linearRegressionY(p_Sensor);
    computePosition(p_Sensor);
    isClicked(p_Sensor);
    printResult(&LCD_16x2, p_Sensor);
    return; 
/*    
   averageDistanceFront_mm(p_Sensor);
   averageDistanceBack_mm(p_Sensor);
   averageDistanceRight_mm(p_Sensor);
   averageDistanceLeft_mm(p_Sensor);
   p_Sensor->aveHand = (p_Sensor->aveFront + p_Sensor->aveBack + p_Sensor->aveRight + p_Sensor->aveLeft)/4; //get the distance between the sensor and the hand in mm
    
    if(!p_Sensor->aveFront || !p_Sensor->aveBack || !p_Sensor->aveRight || !p_Sensor->aveLeft)
    {
        p_Sensor->position = -1;
        p_Sensor->angleX = -1;
        p_Sensor->angleY = -1;
        return;
   }
*/
    
   
    
  
}


