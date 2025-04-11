/**
  ******************************************************************************
  * @file    I2C_MasterIO.c (I2C Bus Master using only GPIO and SW, bit bang quick bringup method)
  * @author  A.Es + S.Ma
  * @brief   Robust, could be improved with bitrate calibration implementation method. (to become compiler and core speed independent)
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
//ae peut etre mettre toutes les var globales en _g et tous les pointeurs en p_
#include "main.h"
//#include "brisk.h" //ae a supprimer

//void NOPs(uint32_t loop); // defined in IO_Drive (low level)

// Unfinished, here we assume the core speed is 48Mhz here (search for it to modify)
// I2C single master with pure GPIO, 8 bit address format, no clock stretch support for speed, bus recovery implemented.
// blocking for main loop
//#include "I2C_MasterIO.h" 

/* These are the local functions to build an I2C transaction */
static int32_t ReceiveByte (I2C_MasterIO_t* pM, uint32_t DoAck); // local, reads 8 data bit then send the ack/nack bit
static int32_t TransmitByte (I2C_MasterIO_t* pM, uint8_t bValue); // local, write 8 data bit and read back the ack/nack
static int32_t GenerateStart (I2C_MasterIO_t* pM, uint8_t SlaveAdr); // local, generate start bit followed by the slave address (bit 0 is Read(1) or Write(0) bit), returns if slave Ack/NAck'ed. (present or busy/missing)
static int32_t ErrorRecovery (I2C_MasterIO_t* pM); // local, if SDA and SCL are not high before generating a Start bit, flush the bus and its out of sync slaves by sending 9 stop bits
static int32_t GenerateStop (I2C_MasterIO_t* pM); // generate the stop bit
static int32_t NopsWait(uint32_t u); // this is a SW delay dependent of SYSCLK MHz and compiler optimisation and provide the I2C bus SCL frequency and bit rate.
static int32_t I2C_MasterIO_CNT_AutoTune(I2C_MasterIO_t* pM, int32_t MinBps, int32_t MaxBps);
//ae peut etre mettre les declarations dans le .h


int32_t I2C_MasterIO_Init( I2C_MasterIO_t* pM ) { // except asserting here to avoid doing it later, nothing special here.
  // check the structure and the pads seems valid
  if(pM==NULL)
    while(1) {};
  if(pM->SCL>=DIE_PAD_MAX)
    while(1) {};
  if(pM->SDA>=DIE_PAD_MAX)
    while(1) {};
  
  return 0;  
}

int32_t I2C_MasterIO_ConfigHW( I2C_MasterIO_t* pM, IO_Pad_t* pSDA, IO_Pad_t* pSCL ) {

//  IO_Pin_t MIO_SDA_STMod = {  GPIOB,          {    GPIO_PIN_11, GPIO_MODE_OUTPUT_OD, GPIO_PULLUP, GPIO_SPEED_FREQ_LOW, 0,   } };
//  IO_Pad_t SDA = { U8_SDA, {.Mode = IO_OUTPUT, .Pull = IO_PULLUP, .Drive = IO_OPENDRAIN, .Speed = IO_SPEED_01, .Odr = IO_ODR_HIGH} };
//  IO_Pin_t MIO_SCL_STMod = {  GPIOB,          {    GPIO_PIN_10, GPIO_MODE_OUTPUT_OD, GPIO_PULLUP, GPIO_SPEED_FREQ_LOW, 0,   } };
//  IO_Pad_t SCL = { U8_SCL, {.Mode = IO_OUTPUT, .Pull = IO_PULLUP, .Drive = IO_OPENDRAIN, .Speed = IO_SPEED_01, .Odr = IO_ODR_HIGH} };
  
  pM->SDA = pSDA-> PadName;
  pM->SCL = pSCL-> PadName;
  
  IO_AddPadToRawPortPads(pSDA-> PadName, &pM->RawPad_SDA );
  IO_AddPadToRawPortPads(pSCL-> PadName, &pM->RawPad_SCL );
  
  IO_PadInit(pSDA); // configure the pins
  IO_PadInit(pSCL); // configure the pins
  
  return 0;
}

/* Set the SW delay based on I2C desired speed range and SYSCLK and max compiler optimisation, should be tuned when optimising performance */
int32_t I2C_MasterIO_ConfigTimings(I2C_MasterIO_t* pM, uint32_t MinBps, uint32_t MaxBps, uint32_t ClockStretch_ms) { // to explain more

  if(ClockStretch_ms) 
    while(1) {};
  
  // basic implementation
#if 0 // using NOPs method (cortex_M0)  
  pM->fnWaitMethod = NopsWait; // use nops SW loop especially for Cortex M0+, for others, you might use DBG Cycle counter which is more accurate.
  
  uint32_t HalfClockPeriod_us = (SYS_CLK_HZ)/(MaxBps*SYS_CLK_MHZ /*48*/); // ADAPT_HERE :Here the core is assumed running at 48 MHz with 1 cycle per NOP. Crude formula. Maybe core type dependent.
  // ideally run here a calibration to become compiler and optimisation independent.... GCC in 2020 was the worst case to consider in zero optimisation configuration. Alternatively, use of library removes this issue.
  // it should not be zero or the delays won't scale for the communication
  pM->WaitParam = HalfClockPeriod_us;
  
  pM->bitrate_kHz = MinBps / 1000;
  
#else // using debug cycle counter (better)
  pM->fnWaitMethod = CNT_Delay_cy; // this will be the wait method (you could dynamically change on the fly using a breakpoint...)
  
  if (MinBps<1000) MinBps = 1000; // for a minimum to avoid slow or divide by zero //ae peut etre rajouter un retour a la ligne apres le if

    //  bit per cycle
  // 1000 bps = 1000 bit per 48000000 cycles
  // 1 bit per 48000000 / 1000
  uint32_t WaitParam = (SYS_CLK_HZ)/(MinBps); // 1 bit duration (conservative, goal is to have a higher value before calibrating
  // ideally run here a calibration to become compiler and optimisation independent.... GCC in 2020 was the worst case to consider in zero optimisation configuration. Alternatively, use of library removes this issue.
  // it should not be zero or the delays won't scale for the communication
  pM->WaitParam = WaitParam / 2; // (/2) to avoid having too much search
  
  I2C_MasterIO_CNT_AutoTune(pM, MinBps, MaxBps);
  //pM->bitrate_kHz = MinBps / 1000;
  
#endif
  
  return 0;
}

int32_t I2C_MasterIO_Enable(I2C_MasterIO_t* pM) { //ae c'est quoi le but de cette fonction
  
  return 0;
}
//==============================================
static int32_t NopsWait(uint32_t m) { // this affect the bit rate and clock period, better way would be to use Timer or DNG Cyclecounter when available. TODO Time Calibration would improve this implementation to achieve desired baud rate.
  // run a message, check the duration vs target, adjust the SW delay using binary search algo. Once known, could be hard coded later on.
  I2C_MasterIO_t* pM = (I2C_MasterIO_t*) m;
  uint32_t n = pM->ctWaitMethod * pM->WaitParam; //ae n ?
  while(n--) asm("nop\n");
  return 0;
}

static uint32_t WaitHere(I2C_MasterIO_t* pM, uint32_t delay) { // here as starter, the delay function is always blocking

  pM->ctWaitMethod = delay * pM->WaitParam; // CNT only
  if(pM->fnWaitMethod) pM->fnWaitMethod((uint32_t)pM->ctWaitMethod); // bugged
  return 0;
}
 
// This function can be called upon any I2C bus error condition.
static int32_t ErrorRecovery (I2C_MasterIO_t* pM) 
{
  // blindly generate 9 stop bits to flush any stuck situation
  // non-invasive, no side effects.
  GenerateStop(pM);
  GenerateStop(pM);
  GenerateStop(pM);
  GenerateStop(pM);
  GenerateStop(pM);
  GenerateStop(pM);
  GenerateStop(pM);
  GenerateStop(pM);	// flush the bus if it is stuck
  return 0;
}

static int32_t GenerateStart (I2C_MasterIO_t* pM, uint8_t SlaveAdr)
{
  IO_RawPadSetHigh(&pM->RawPad_SDA);//dir_I2C_SDA_IN;	// to check if I2C is idle... or stuck
  WaitHere(pM,1);
  if(IO_RawPadGet(&pM->RawPad_SDA)==0) {
    ErrorRecovery(pM);
    if(IO_RawPadGet(&pM->RawPad_SDA)==0) { // to debug with hot plug if glitch could code (try twice?)
      /*MinDelay_ms(10);*/HAL_Delay(10);//ae Sebastien Marsanne l'utilisateur numero 1 de la lib HAL
      ErrorRecovery(pM); // can't recover (or try again with delay?)
    }
  };

  if((SlaveAdr & 0x01) == 0) // if it is a write address, we start a transaction, hence we clear ackfail.
    pM->AckFail = 0;
  // this is bugged, it's not a start bit if SCL is low...  it's too short.
  IO_RawPadSetHigh(&pM->RawPad_SCL);//bit_I2C_SCL_HIGH;
  WaitHere(pM,1);					

  // Fixed violation on Start hold time
  IO_RawPadSetLow(&pM->RawPad_SDA);//bit_I2C_SDA_LOW;
  WaitHere(pM,1);

  IO_RawPadSetLow(&pM->RawPad_SCL);//bit_I2C_SCL_LOW;
  WaitHere(pM,1);

  return TransmitByte (pM,SlaveAdr);				// Send the slave address
}


static int32_t TransmitByte(I2C_MasterIO_t* pM, uint8_t bValue) 
{
  uint8_t loop;

  for (loop = 0; loop < 8; loop++) 
  {
      if (bValue & 0x80) {
        IO_RawPadSetHigh(&pM->RawPad_SDA);//bit_I2C_SDA_HIGH;
      }else{ 
        IO_RawPadSetLow(&pM->RawPad_SDA);//bit_I2C_SDA_LOW;
      }

      WaitHere(pM,1);// Sept 17
//		dir_I2C_SDA_OUT;				// make sure SDA is configured as output (once DR initialised)

      IO_RawPadSetHigh(&pM->RawPad_SCL);//bit_I2C_SCL_HIGH;
      WaitHere(pM,1);//1028
      bValue <<= 1;
      IO_RawPadSetLow(&pM->RawPad_SCL);//bit_I2C_SCL_LOW;
      WaitHere(pM,1);
  }

  // Acknowledge Write
  // bit_I2C_SCL = HIGH;
  // ack is READ to check if Slave is responding

  IO_RawPadSetHigh(&pM->RawPad_SDA);//dir_I2C_SDA_IN;
  WaitHere(pM,1);
  IO_RawPadSetHigh(&pM->RawPad_SCL);//bit_I2C_SCL_HIGH;	// SCL = 1
  WaitHere(pM,1);					

  // Here we could sense NACK and manage error info to calling function
  // for debug to find ACK bit as long scl pulse...
  // Error = bit_I2C_SDA; // 1 = Error, 0 = Ok
  IO_RawPadSetHigh(&pM->RawPad_SCL);//bit_I2C_SCL_HIGH;	// SCL = 1
  WaitHere(pM,2);//	NOP;					
  pM->AckFail |= IO_RawPadGet(&pM->RawPad_SDA);	// Acknowledge bit
  if(pM->AckFail) 
    NOPs(1); // breakpoint hook
  IO_RawPadSetLow(&pM->RawPad_SCL);//bit_I2C_SCL_LOW;	// SCL = 0

  WaitHere(pM,1);// Sept 17

//-	SetSDAOutput();//dir_I2C_SDA_OUT;

//-	WaitHere(u,1);//	NOP;	add sept 17

  return pM->AckFail;
}


static int32_t ReceiveByte(I2C_MasterIO_t* pM, uint32_t DoAck) 
{ 
  uint8_t bValue, loop;

  bValue = 0;
  IO_RawPadSetHigh(&pM->RawPad_SDA);//dir_I2C_SDA_IN; // make SDA as input before reading pin level

  for (loop = 0; loop < 8; loop ++) 
  {
      WaitHere(pM,1);// NOP; NOP;	// 1 us delay

      IO_RawPadSetHigh(&pM->RawPad_SCL);//bit_I2C_SCL_HIGH;	// SCL = 1
      WaitHere(pM,2);					
      bValue <<= 1;

      if(IO_RawPadGet(&pM->RawPad_SDA)) bValue++;
      IO_RawPadSetLow(&pM->RawPad_SCL);//bit_I2C_SCL_LOW;	// SCL = 0
      WaitHere(pM,1);//1028
  }

// Manage the ackknowledge bit
  if(DoAck) {
    IO_RawPadSetLow(&pM->RawPad_SDA);//bit_I2C_SDA_LOW;
  }else{
    IO_RawPadSetHigh(&pM->RawPad_SDA);//bit_I2C_SDA_HIGH;
  }

//  SetSDAOutput();//dir_I2C_SDA_OUT; // make sure SDA is configured as output (once DR initialised)
  WaitHere(pM,1);	// enlarge the pulse to see it on the scope
  IO_RawPadSetHigh(&pM->RawPad_SCL);//bit_I2C_SCL_HIGH;	// SCL = 1
  WaitHere(pM,1);
  IO_RawPadSetLow(&pM->RawPad_SCL);//bit_I2C_SCL_LOW;	// SCL = 0

  WaitHere(pM,1);//	NOP;	add sept 17

  return bValue;
}


static int32_t GenerateStop (I2C_MasterIO_t* pM) {
  
  IO_RawPadSetLow(&pM->RawPad_SCL);//bit_I2C_SCL_LOW;
  WaitHere(pM,1);
  IO_RawPadSetLow(&pM->RawPad_SDA);//bit_I2C_SDA_LOW;
  WaitHere(pM,1);							// Extra to make sure delay is ok
  
  IO_RawPadSetHigh(&pM->RawPad_SCL);//bit_I2C_SCL_HIGH;
  WaitHere(pM,1);

  IO_RawPadSetHigh(&pM->RawPad_SDA);//bit_I2C_SDA_HIGH;
  WaitHere(pM,1);  
  return 0;
}

//====---------------------------> 8>< <----------------===========================
// THE ONLY FUNCTION NEEDED FOR MOST SLAVE ACCESS, ONE API ONLY. Grep on it to see how it is used....
// simplified I2C Master write/read command, monoblock
int32_t I2C_MasterIO_AccessSlave(I2C_SlaveDevice_t* pD) { // Param1: Block adr, Param2: Block size byte, Param3: Ack when read or not
  
  I2C_MasterIO_t* pM = (I2C_MasterIO_t*) pD->M;
  uint8_t* pu8; //ae pas tres clair comme nom de var
  uint16_t bCount; //ae same
  
  // if the TX adr is null, no bytes to transmit, disable MOSI, and transmit dummy things instead
  if((pD->SubAdrByteCount!=0)||(pD->WriteByteCount!=0)) { // skip if no sub address nor any byte to write // 09/05/2019
    
    pD->SlaveAdrWriteNack = GenerateStart (pM, pD->SlaveAdr & 0xFE); // AckFail = 0 done inside

    // transmit prefix  
    pu8 = (uint8_t*) pD->SubAdrBytes;
    bCount = (uint16_t) pD->SubAdrByteCount;
    
    if((pu8)&&(bCount)) {
      while((pM->AckFail==0)&& (bCount--)) { // for all bytes... if ack fail, skip the transmit part
         pM->AckFail |= TransmitByte (pM, *pu8++);
      }
    }

    // transmit
    pu8 = (uint8_t*) pD->pWriteByte;
    bCount = (uint16_t) pD->WriteByteCount;
    
    if((pu8)&&(bCount)) // if there is something to send
      while((pM->AckFail==0)&& (bCount--)) { // for all bytes... if ack fail, skip the transmit part
         pM->AckFail |= TransmitByte (pM, *pu8++);
      }
  }
  // receive
  pu8 = (uint8_t*) pD->pReadByte;
  bCount = (uint16_t) pD->ReadByteCount;
  
  // if no error and something to read
  if((pM->AckFail==0)&&(pu8)&&(bCount))
    pD->SlaveAdrReadNack = GenerateStart(pM, pD->SlaveAdr | 0x01); // do the restart
  
  while((pM->AckFail==0)&&(pu8)&&(bCount--)) {
    *pu8++ = (uint8_t) ReceiveByte (pM,bCount);
  }

  GenerateStop(pM);
  
  if(pM->AckFail || pD->SlaveAdrReadNack || pD->SlaveAdrWriteNack)
    return -1; // error
  
  return 0; // no interrupt call back setup here
}


//==== higher level abstraction functions

uint8_t I2C_MasterIO_IsSlavePresentByAddress(I2C_MasterIO_t* pM, uint8_t SlaveAdr) {

  uint8_t SlavePresent;  
  
  if(GenerateStart (pM, SlaveAdr & 0xFE)==0)
    SlavePresent = 1;
  else
    SlavePresent = 0;
  
  GenerateStop(pM);
  
  return SlavePresent;  
}

uint8_t I2C_MasterIO_IsSlavePresent(I2C_SlaveDevice_t* pD) {
  
  return I2C_MasterIO_IsSlavePresentByAddress(pD->M, pD->SlaveAdr);
}

uint8_t I2C_MasterIO_FindNextSlaveByAddress(I2C_MasterIO_t* pM, uint8_t StartAdr) {

  uint8_t SlaveAdr;
  SlaveAdr = (StartAdr & 0xFE) + 2; // start sweeping from next address, when you find one, store the address and continue with it. You can start with 0 which is general call unused address)
  
  for( ; SlaveAdr < 0xF0; SlaveAdr+=2)
    if(I2C_MasterIO_IsSlavePresentByAddress(pM, SlaveAdr))
      return SlaveAdr;
  
  return 0; // nothing found till the end of the sweep spectrum
}

//================= 8>< --------------------------------------------------------------------------------------------


// === application board level specifics, here we declare the 3 I2C buses we use on one application board. Should be self explanatory.
// ADAPT_HERE
// let's declare all the I2C bus on Brisk, we have the arduino, the STMod+ and the internal ones.
// these initialisation will later move to the BSP file...

//ae la declaration des bus devrait etre en haut du fichier

I2C_MasterIO_t gI2C_Bus_Sensor; // Same I2C bus for both DIP28 and STMod+ PA11=SCL2, PA12=SDA2
const IO_Pad_t MIO_SDA_Sensor = { U8_SDA, {.Mode = IO_OUTPUT, .Pull = IO_PULLUP, .Drive = IO_OPENDRAIN, .Speed = IO_SPEED_03, .Odr = IO_ODR_HIGH} };
const IO_Pad_t MIO_SCL_Sensor = { U8_SCL, {.Mode = IO_OUTPUT, .Pull = IO_PULLUP, .Drive = IO_OPENDRAIN, .Speed = IO_SPEED_03, .Odr = IO_ODR_HIGH} };

I2C_MasterIO_t gI2C_Bus_DigitalHeader; // Same I2C bus for both DIP28 and STMod+ PA11=SCL2, PA12=SDA2
const IO_Pad_t MIO_SDA_DigitalHeader = { PB_9, {.Mode = IO_OUTPUT, .Pull = IO_PULLUP, .Drive = IO_OPENDRAIN, .Speed = IO_SPEED_03, .Odr = IO_ODR_HIGH} };
const IO_Pad_t MIO_SCL_DigitalHEader = { PB_8, {.Mode = IO_OUTPUT, .Pull = IO_PULLUP, .Drive = IO_OPENDRAIN, .Speed = IO_SPEED_03, .Odr = IO_ODR_HIGH} };

// After this, we declare the I2C slave devices which we are going to play with

//I2C_SlaveDevice_t gBriskLSM303DTR =     { &gI2C_Bus_Sensor, 0x3A, 1 }; // which bus, which slave address, how many bytes to send for the sub address
//I2C_SlaveDevice_t gBriskM24256D =       { &gI2C_Brisk, 0xAC/*0xAE*/, 2 }; // This is the EEPROM which resides in the STMod+ addon board for hot plug and play purpose
//I2C_SlaveDevice_t gBriskM24256D_ID =    { &gI2C_Brisk, 0xBC/*0xBE*/, 2 };

// no slave device on arduino by default
/*
uint8_t IsDevicePresent(I2C_SlaveDevice_t* pD) {
  
  return IsSlavePresent(pD->M, pD->SlaveAdr);
}
*/
uint32_t Minbps = 100000; //ae rajouter le g devant et les declarer en haut du fichier ?
uint32_t Maxbps = 200000;//400000
uint32_t gresult = 0; 
void Brisk_I2C_MasterIO_Init(void) { //ae changer le brisk

  I2C_MasterIO_Init(&gI2C_Bus_Sensor);
  //I2C_MasterIO_ConfigHW(&gI2C_Bus_Sensor, U8_SDA, U8_SCL);
  I2C_MasterIO_ConfigHW(&gI2C_Bus_Sensor, (IO_Pad_t*) &MIO_SDA_Sensor, (IO_Pad_t*) &MIO_SCL_Sensor);
  I2C_MasterIO_ConfigTimings(&gI2C_Bus_Sensor, Minbps, Maxbps, 0);  
  // The brute force calibration with 96MHz core is 0.2 seconds
  // A different algo with binary search would probably work if we don't really toggle the IO (affect slaves with too high speed)
  
  I2C_MasterIO_Enable(&gI2C_Bus_Sensor);
  
  ErrorRecovery(&gI2C_Bus_Sensor);
  
  //while(1) 
    Brisk_I2C_MasterIO_Test();// for bringup //ae changer le brisk
    
    
  // We configure and activate the digital connector for TOF 8x8 on PB9 = SDA and PB8 = SCL, pull ups required
    // regular I2C speed to make sure
    Minbps = 30000;
    Maxbps = 1000000;
  I2C_MasterIO_Init(&gI2C_Bus_DigitalHeader);
  I2C_MasterIO_ConfigHW(&gI2C_Bus_DigitalHeader, (IO_Pad_t*) &MIO_SDA_DigitalHeader, (IO_Pad_t*) &MIO_SCL_DigitalHEader);
  I2C_MasterIO_ConfigTimings(&gI2C_Bus_DigitalHeader, Minbps, Maxbps, 0); 

  I2C_MasterIO_Enable(&gI2C_Bus_DigitalHeader);
  ErrorRecovery(&gI2C_Bus_DigitalHeader);
  
  while(0) { // comment this line if you want to test slave addresses only once and the code to continue running normally //ae while 0 ?
    gresult = I2C_MasterIO_IsSlavePresentByAddress(&gI2C_Bus_DigitalHeader, 0x52);
    gresult = I2C_MasterIO_IsSlavePresentByAddress(&gI2C_Bus_DigitalHeader, 0x53);
    CNT_Delay_us(1000);
  }

}

// remove to save RAM if linker does not do it automatically when not used by the project.
uint8_t gSlaveSweep; // use for test only //ae declaration en haut du fichier ?

void Brisk_I2C_MasterIO_Test(void) { // this function sweeps all slave addresses on one bus.

  uint16_t StartAdr;
  // we sweep the slaves
  StartAdr = 0;
  while(StartAdr = I2C_MasterIO_FindNextSlaveByAddress(&gI2C_Bus_Sensor, StartAdr)) {

      gSlaveSweep = StartAdr; // breakpoint here to stop as soon as one slave is detected
      NOPs(1); // put breakpoint here
  }; // when StartAdr == 0, it means the sweep is over
  
  NOPs(1); // breakpoint hook to view gSlaveSweep
// Test on Nucleo (unused default pins on the DK)
//  StartAdr = 0;
//  while(StartAdr = I2C_MasterIO_FindNextSlaveByAddress(&gI2C_Arduino, StartAdr)) {

//      gSlaveSweep = StartAdr; // breakpoint here to stop as soon as one slave is detected
//      NOPs(1);
//  }; // when StartAdr == 0, it means the sweep is over
  
  //while(1); // you can stop and read the content of gSlaveSweep
}

//#define DEBUG_I2C_TIMINGS // enable this option if you'd like to debug details around I2C bitrate tuning
// at 96MHz core speed on U3, IAR 9 low optimisation = 285 kbps, High opt = 350 kbps (+22%)

#ifdef DEBUG_I2C_TIMINGS
  uint32_t start_cy, stop_cy, duration_cy, bps;
  uint32_t best_WaitParam = 0;
  uint32_t best_bps;
#endif

  // Important note: As long as you keep the same core frequency and the same compiler with same options, you can skip this and force the right values.
static int32_t I2C_MasterIO_CNT_AutoTune(I2C_MasterIO_t* pM, int32_t MinBps, int32_t MaxBps)
{
  // we will run slave address check 
  I2C_SlaveDevice_t TestSlave = { &gI2C_Bus_Sensor, 0xFF, 1 }; // does not exist and will NACK its address
  // S-FFn-P = 11 bit
#if 0 // if you did already the calibration, encode it!
  pM->WaitParam = // put the value you found out !
  pM->bitrate_kHz = // enter the value here
  return 0;
#endif  
  
  uint32_t WaitParam = pM->WaitParam; // cycles for 1 bit, slower side 
#ifndef DEBUG_I2C_TIMINGS
  uint32_t start_cy, stop_cy, duration_cy, bps;
  uint32_t best_WaitParam = 0;
  uint32_t best_bps;
#endif  
  
  for(uint32_t cy = WaitParam; cy > 0; cy--) { // not the fastest way (binary search would be predictable)
    pM->WaitParam = cy;
    start_cy = CNT_GetTimeCy();
    I2C_MasterIO_IsSlavePresent(&TestSlave); // run the bus
    stop_cy = CNT_GetTimeCy();
    duration_cy = stop_cy - start_cy;
    bps = (SYS_CLK_HZ * 11) / duration_cy; // 11 bits per transaction
    if( bps < (MinBps/4)) while(1); // should not happen, waitparam is wrong before this function
    if( bps > MaxBps) { // we went too far previous trial was best one
      if(best_WaitParam == 0) 
        while(1); // couldn't find any valid solution. Asked range too narrow?
      break;
    }
    
    // this is a valid solution, we save it
    best_WaitParam = cy;
    best_bps = bps;
  }

  pM->WaitParam = best_WaitParam;
  pM->bitrate_kHz = (best_bps+500) / 1000; // rounded up

  return 0;
}

