/*
 * AE_Max30100.h
 *
 *  Created on: Oct 18, 2024
 *      Author: alexandre estor
 */

#ifndef INC_AE_MAX30100_H_
#define INC_AE_MAX30100_H_
#include "AE_toolBox.h"

#define SAMPLE_RATE 100
// Adresse I2C du capteur MAX30100
#define MAX30100_I2C_ADDR 0x57

// Registres du MAX30100
#define MAX30100_REG_FIFO_DATA 0x05
#define MAX30100_REG_FIFO_WR_PTR 0x02
#define MAX30100_REG_FIFO_OF_PTR 0x03
#define MAX30100_REG_FIFO_RD_PTR 0x04

#define lengthBuff 400
#define numberSamplesLinear 10
void AE_Max30100Init(I2C_HandleTypeDef *hi2c1);
void resetFIFO(circularBuff_t *FIFO);
uint8_t numberSamplesReady(circularBuff_t *FIFO, uint8_t WR_PTR, uint8_t RD_PTR, uint8_t OF_PTR);
uint8_t enoughDataForHR(circularBuff_t *FIFO);
uint16_t HeartRateSensor_Read(I2C_HandleTypeDef *hi2c1, uint8_t numberSamples);
uint8_t HeartRateSensor_Read_Reg(I2C_HandleTypeDef *hi2c1, uint8_t MAC30100_REG_ADDR);
#endif /* INC_AE_MAX30100_H_ */
