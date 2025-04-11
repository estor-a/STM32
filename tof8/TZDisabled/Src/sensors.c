
#include "main.h"

//uint8_t STHS_Write[64], STHS_Read[64];

/** Please note that is MANDATORY: return 0 -> no Error.**/
int32_t platform_write(void *handle, uint8_t Reg, const uint8_t *Bufp, uint16_t len);
int32_t platform_read(void *handle, uint8_t Reg, uint8_t *Bufp, uint16_t len);

stmdev_ctx_t dev_ctx;

/** Please note that is MANDATORY: return 0 -> no Error.**/
int32_t platform_write(void *handle, uint8_t Reg, const uint8_t *Bufp, uint16_t len) 
{
  I2C_SlaveDevice_t* pS = handle;
  int32_t result = 0;
  // we use global for now
  pS->SubAdrBytes[0] = Reg;
  pS->pWriteByte = (uint8_t*) Bufp;
  pS->WriteByteCount = len;
  pS->pReadByte = 0;
  pS->ReadByteCount = 0;
  result = I2C_MasterIO_AccessSlave(pS);
  return result;
}

int32_t platform_read(void *handle, uint8_t Reg, uint8_t *Bufp, uint16_t len)
{
  I2C_SlaveDevice_t* pS = handle;
  int32_t result = 0;
  // we use global for now
  pS->SubAdrBytes[0] = Reg;
  pS->pWriteByte = 0;
  pS->WriteByteCount = 0;
  pS->pReadByte = Bufp;
  pS->ReadByteCount = len;
  result = I2C_MasterIO_AccessSlave(pS);
  return result;
}

I2C_SlaveDevice_t STHS_Slave = {   &gI2C_Bus_Sensor, STHS34PF80_I2C_ADD, 1 };   // how many bytes for the sub address? 0,1,2,4...
int32_t cnt = 0;
sths34pf80_lpf_bandwidth_t lpf_m, lpf_p, lpf_p_m, lpf_a_t;
sths34pf80_drdy_status_t STHS_status;
sths34pf80_func_status_t STHS_func_status; // This is the global sensor reporting info as 3 flags

STHS_sensor_t STHS_sensor = { &STHS_Slave, &STHS_func_status, 0x00000002 };
/*
  sths34pf80_func_status_t
  uint8_t pres_flag  : 1;
  uint8_t mot_flag  : 1;
  uint8_t tamb_shock_flag  : 1;*/
#if 0
static void platform_delay(uint32_t ms);
static void platform_delay(uint32_t ms)
{
  HAL_Delay(ms);
}
#endif

int32_t STHS_Init(void)
{
  uint8_t whoami;

  /* Initialize mems driver interface */
  //dev_ctx.write_reg = platform_write;
  //dev_ctx.read_reg = platform_read;
  // change the handle when changing sensor (non ideal)
  dev_ctx.handle = &STHS_Slave;

  /* Initialize platform specific hardware */
  //platform_init();

  /* Wait sensor boot time */

#define    BOOT_TIME         10 //ms  
//DONE BY CALLER  platform_delay(BOOT_TIME);

  /* Check device ID */
  sths34pf80_device_id_get(&dev_ctx, &whoami);
  if (whoami != STHS34PF80_ID)
    while(1);

  /* Set averages (AVG_TAMB = 8, AVG_TMOS = 32) */
  sths34pf80_avg_tobject_num_set(&dev_ctx, STHS34PF80_AVG_TMOS_32);
  sths34pf80_avg_tambient_num_set(&dev_ctx, STHS34PF80_AVG_T_8);

  /* read filters */
  sths34pf80_lpf_m_bandwidth_get(&dev_ctx, &lpf_m);
  sths34pf80_lpf_p_bandwidth_get(&dev_ctx, &lpf_p);
  sths34pf80_lpf_p_m_bandwidth_get(&dev_ctx, &lpf_p_m);
  sths34pf80_lpf_a_t_bandwidth_get(&dev_ctx, &lpf_a_t);

//  sprintf((char *)tx_buffer,
//         "lpf_m: %02d, lpf_p: %02d, lpf_p_m: %02d, lpf_a_t: %02d\r\n", lpf_m, lpf_p, lpf_p_m, lpf_a_t);
//  tx_com(tx_buffer, strlen((char const *)tx_buffer));

  /* Set BDU */
  sths34pf80_block_data_update_set(&dev_ctx, 1);

  /* Set ODR */
  sths34pf80_odr_set(&dev_ctx, STHS34PF80_ODR_AT_30Hz);
  
  return 0;
}


int32_t STHS_Polling(void)
{
  dev_ctx.handle = &STHS_Slave; // sensor selection
  /* Read samples in polling mode (no int) */
  sths34pf80_drdy_status_get(&dev_ctx, &STHS_status);
  if (STHS_status.drdy)
    sths34pf80_func_status_get(&dev_ctx, &STHS_func_status); // BTEL variable
  
  return 0;
}

//====================== 8>< ---------------------------

lis2du12_pin_int_route_t int1_route;
lis2du12_status_t lis_status;
//stmdev_ctx_t dev_ctx;
lis2du12_id_t lis_id;
lis2du12_md_t lis_md;
lis2du12_data_t lis_data;

I2C_SlaveDevice_t LIS_Slave = {   &gI2C_Bus_Sensor, LIS2DU12_I2C_ADD_H, 1 };   // how many bytes for the sub address? 0,1,2,4...

LIS_sensor_t LIS_sensor = { &LIS_Slave, &lis_data, 0x00000004 };

int32_t LIS_Init(void);
int32_t LIS_Init(void)
{
  /* Initialize mems driver interface */
//  dev_ctx.write_reg = platform_write;
//  dev_ctx.read_reg = platform_read;
  dev_ctx.handle = &LIS_Slave;

  /* Initialize platform specific hardware */
//  platform_init();

  /* Wait sensor boot time */
  // DONE BY CALLER platform_delay(BOOT_TIME);

  /* Check device ID */
  lis2du12_id_get(&dev_ctx, &lis_id);
  if (lis_id.whoami != LIS2DU12_ID)
    while(1);

  /* Restore default configuration */
  lis2du12_init_set(&dev_ctx, LIS2DU12_RESET);
  do {
    lis2du12_status_get(&dev_ctx, &lis_status);
  } while (lis_status.sw_reset);

  /* Set bdu and if_inc recommended for driver usage */
  lis2du12_init_set(&dev_ctx, LIS2DU12_DRV_RDY);

  /* Select bus interface */
  lis2du12_bus_mode_set(&dev_ctx, LIS2DU12_I3C_DISABLE);

  /* Set Output Data Rate */
  lis_md.fs =  LIS2DU12_4g;
  lis_md.odr = LIS2DU12_25Hz;
  lis2du12_mode_set(&dev_ctx, &lis_md);

  /* Configure inerrupt pins */
  lis2du12_pin_int1_route_get(&dev_ctx, &int1_route);
  int1_route.drdy_xl   = PROPERTY_DISABLE;
  lis2du12_pin_int1_route_set(&dev_ctx, &int1_route);
  
  return 0;
}

    float gravity_x100;
    float roll_deg; // mems placement on PCB is different
    float pitch_deg; // pitch 90 = stand USB up, -90 upside down, usb on the bottom side


int32_t LIS_Polling(void)
{
  dev_ctx.handle = &LIS_Slave;
  
  /* Read samples in polling mode (no int). */
  /* Read output only if new values are available */
  lis2du12_status_get(&dev_ctx, &lis_status);
  if ( lis_status.drdy_xl ) {

    lis2du12_data_get(&dev_ctx, &lis_md, &lis_data);
    float local_acceleration_mg[3];
  
      local_acceleration_mg[0] =  lis_data.xl.mg[0];
      local_acceleration_mg[1] =  lis_data.xl.mg[1];
      local_acceleration_mg[2] =  lis_data.xl.mg[2];
     
      // we convert to spherical coordinates
     
// conversion to spherical coordinates and vector
    float Xg = local_acceleration_mg[0]/1000;//gx100 unit ((float) sample_xyzt.x) / 4096;
    float Yg = local_acceleration_mg[1]/1000;//gx100 unit ((float) sample_xyzt.y) / 4096;
    float Zg = local_acceleration_mg[2]/1000;//gx100 unit ((float) sample_xyzt.z) / 4096;
   
    float alpha = 1;
    //Low Pass Filter
    float fXg = (float)Xg * alpha + (fXg * (1.0 - alpha));
    float fYg = (float)Yg * alpha + (fYg * (1.0 - alpha));
    float fZg = (float)Zg * alpha + (fZg * (1.0 - alpha));
 
    //Roll & Pitch Equations
    float seb = fXg*fXg + fYg*fYg + fZg*fZg;
    float gravity = pow( seb, 0.5 );
    float roll  = (atan2(-fYg, fZg)*180.0)/3.14159265359;
   // float pitch = (atan2(fXg, sqrt(fYg*fYg + fZg*fZg))*180.0)/3.14159265359;
   float pitch = (asin(fXg / sqrt(fYg*fYg + fZg*fZg + fXg*fXg))*180.0)/3.14159265359;
   
    gravity_x100 = (int32_t) (gravity * 100);
    roll_deg = (int32_t) (roll); // mems placement on PCB is different
    pitch_deg = (int32_t) (-pitch); // pitch 90 = stand USB up, -90 upside down, usb on the bottom side

    /* print sensor data  */
    //sprintf((char*)tx_buffer, "Acceleration [mg]:%4.2f\t%4.2f\t%4.2f\tTemp[degC]:%3.2f\r\n",
    //        data.xl.mg[0], data.xl.mg[1], data.xl.mg[2], data.heat.deg_c);
    //tx_com(tx_buffer, strlen((char const*)tx_buffer));
  }
  return 0;
}


//====================== 8>< -----------------------------

#include "vl53l8cx_api.h"
#include "vl53l8cx_platform.h"

#include "VL53L4CD_api.h"
#include "platform.h"
#if 1
I2C_SlaveDevice_t VL53_Slave = {   &gI2C_Bus_Sensor, 0x52, 2 };   // how many bytes for the sub address? 0,1,2,4...

Dev_t 			dev;
uint8_t 		status, loop, isReady;
uint16_t 		sensor_id;
VL53L4CD_ResultsData_t 	VL_results;		/* results data from VL53L4CD */
Motion3D_t LIS_Motion3D; // spherical angular and gravity coordinate

VL_sensor_t VL_sensor = { &VL53_Slave, &VL_results, 0x00000001 };

// optional 8x8 TOF plugged by Digital connector plugged onto bread board.
I2C_SlaveDevice_t VL53L8_Slave = {   &gI2C_Bus_DigitalHeader, 0x52, 2 };   // how many bytes for the sub address? 0,1,2,4...

//int status;

int32_t VL53_Init(void);
int32_t VL53_Init(void)
{
  uint8_t status = VL53L4CD_GetSensorId(dev, &sensor_id);
  if(status || (sensor_id != 0xEBAA))
  {
//          printf("VL53L4CD not detected at requested address\n");
          return status;
  }

  /* (Mandatory) Init VL53L4CD sensor */
  status = VL53L4CD_SensorInit(dev);
  if(status)
  {
//          printf("VL53L4CD ULD Loading failed\n");
          return status;
  }

//  printf("VL53L4CD ULD ready !\n");

  /*********************************/
  /*         Ranging loop          */
  /*********************************/

  status = VL53L4CD_StartRanging(dev);
  return 0;
}

int32_t VL53_Polling(void);
int32_t VL53_Polling(void)
{
  uint8_t status = VL53L4CD_CheckForDataReady(dev, &isReady);

  if(isReady)
  {
    /* (Mandatory) Clear HW interrupt to restart measurements */
    VL53L4CD_ClearInterrupt(dev);

    /* Read measured distance. RangeStatus = 0 means valid data */
    VL53L4CD_GetResult(dev, &VL_results);
//    printf("Status = %6u, Distance = %6u, Signal = %6u\n",
//             results.range_status,
//             results.distance_mm,
//             results.signal_per_spad_kcps);
  }

  /* Wait a few ms to avoid too high polling (function in platform
   * file, not in API) */
  // DONE BY CALLER WaitMs(dev, 5);
  
  // status = VL53L4CD_StopRanging(dev);
  return 0;
}

#endif

//====================== 8>< ----------------------------


VL53L8CX_Configuration 	DevL8;			/* Sensor configuration */
VL53L8CX_ResultsData 	Results;		/* Results data from VL53L8CX */


uint32_t VL53L8_Init(void);
uint32_t VL53L8_Poll(void);

//VL_sensor_t VL8_sensor = { &VL53_Slave, &VL_results, 0x00000001 };

uint32_t VL53L8_Init(void){
  
  uint8_t status;
  uint8_t 	isAlive, isReady;
  
  DevL8.platform.address = VL53L8CX_DEFAULT_I2C_ADDRESS;
  
  /* (Optional) Reset sensor toggling PINs (see platform, not in API) */
  //Reset_Sensor(&(DevL8.platform));

  /* (Optional) Set a new I2C address if the wanted address is different
  * from the default one (filled with 0x20 for this example).
  */
  //status = vl53l8cx_set_i2c_address(&DevL8, 0x20);  


  /*********************************/
  /*   Power on sensor and init    */
  /*********************************/

  /* (Optional) Check if there is a VL53L8CX sensor connected */
  status = vl53l8cx_is_alive(&DevL8, &isAlive);
  if(!isAlive || status)
  {
          //printf("VL53L8CX not detected at requested address\n");
          return status;
  }

  /* (Mandatory) Init VL53L8CX sensor */
  status = vl53l8cx_init(&DevL8);
  if(status)
  {
          //printf("VL53L8CX ULD Loading failed\n");
          return status;
  }
  if(nbPadPerLine == 8)
          vl53l8cx_set_resolution(&DevL8, VL53L8CX_RESOLUTION_8X8);
        else
          vl53l8cx_set_resolution(&DevL8, VL53L8CX_RESOLUTION_4X4);

  //printf("VL53L8CX ULD ready ! (Version : %s)\n",
  //		VL53L8CX_API_REVISION);


  /*********************************/
  /*         Ranging loop          */
  /*********************************/

  status = vl53l8cx_start_ranging(&DevL8);
  
  return 0;
}

uint32_t VL53L8_Poll(void){

  uint8_t status;
  if(I2C_MasterIO_IsSlavePresentByAddress(&gI2C_Bus_DigitalHeader, 0x52)==0)
    return 0;
  /* Use polling function to know when a new measurement is ready.
   * Another way can be to wait for HW interrupt raised on PIN A1
   * (INT) when a new measurement is ready */

  status = vl53l8cx_check_data_ready(&DevL8, &isReady);

  if(isReady)
  {
    vl53l8cx_get_ranging_data(&DevL8, &Results);

    /* As the sensor is set in 4x4 mode by default, we have a total 
     * of 16 zones to print. For this example, only the data of first zone are 
     * print */
//			printf("Zone : %3d, Status : %3u, Distance : %4d mm\n",
//				i,
//				Results.target_status[VL53L8CX_NB_TARGET_PER_ZONE*i],
//				Results.distance_mm[VL53L8CX_NB_TARGET_PER_ZONE*i]);  
    processDataTof8(&g_Sensor, &Results);//gCensor is extern
    loop++;
    
  }

  /* Wait a few ms to avoid too high polling (function in platform
   * file, not in API) */

//	status = vl53l8cx_stop_ranging(&DevL8);

  return status;
}









const sensors_t Sensors = {
  .pSTHS = &STHS_sensor,
  .pLIS = &LIS_sensor,
  .pVL = &VL_sensor,
};

int32_t SensorsInit(void)
{
   /** xxxxxxx is the used part number **/
  dev_ctx.write_reg = platform_write;
  dev_ctx.read_reg = platform_read;
  
  HAL_Delay(10); // BOOT TIME (so we don't have to delay for each sensor 10 msec x 3
  STHS_Init();
  LIS_Init();
  VL53_Init();
  
  VL53L8_Init(); // optional external sensor on bread board using SATEL accessory boards.

  Sensors.pSTHS->UserSpare++;
  Sensors.pLIS->UserSpare++;
  Sensors.pVL->UserSpare++;
  return 0;
}


int32_t SensorsPolling(void)
{
  STHS_Polling(); // sths34pf80_func_status_t STHS_func_status;
  //LIS_Polling();  // lis2du12_data_t lis_data;
  //VL53_Polling(); // VL53L4CD_ResultsData_t 	VL_results;
  
  VL53L8_Poll(); // external optional sensor.
  return 0;
}

