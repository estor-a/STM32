#ifndef _SENSORS_H_
#define _SENSORS_H_

#include "sths34pf80_reg.h"  // comment it to remove it to the project
#include "lis2du12_reg.h"
#include "vl53l4cd_api.h"

int32_t SensorsInit(void);
int32_t SensorsPolling(void);

int32_t STHS_Init(void);
int32_t STHS_Polling(void);

int32_t LIS_Init(void);
int32_t LIS_Polling(void);

typedef struct {
  int32_t gravity_x100;
  int32_t roll_deg;
  int32_t pitch_deg;
} Motion3D_t; // gravity vector from Accelerometer (hand tilt the board)


typedef struct
{
  I2C_SlaveDevice_t* pSTHS_Slave;
  sths34pf80_func_status_t* pSTHS_func_status;
  uint32_t UserSpare;
} STHS_sensor_t;

typedef struct
{
  I2C_SlaveDevice_t* pLIS_Slave;
  lis2du12_data_t* plis_data;
  uint32_t UserSpare;
  Motion3D_t* pMotion3D;
} LIS_sensor_t;

typedef struct
{
  I2C_SlaveDevice_t* pVL53_Slave;
  VL53L4CD_ResultsData_t* 	pVL_results;
  uint32_t UserSpare;
} VL_sensor_t;

typedef struct
{
  STHS_sensor_t* pSTHS;
  LIS_sensor_t* pLIS;
  VL_sensor_t* pVL;
  
} sensors_t;

#endif