/*
 * AE_toolBox.h
 *
 *  Created on: Oct 27, 2023
 *      Author: alexandre estor
 */


#ifndef CRITICAL_HDR_H
#define CRITICAL_HDR_H

/* Critical section macros */
#define CRITICAL_SECT_DEFINE                 uint32_t primask = __get_PRIMASK();
#define CRITICAL_SECT_LOCK()                 __disable_irq()
#define CRITICAL_SECT_UNLOCK()               __set_PRIMASK(primask)

/* Assing result of cmd to var */
#define CRITICAL_SECT_EXECUTE_READ(var, cmd) CRITICAL_SECT_EXECUTE((var) = (cmd))

/* New defines to enter/exit critical section with interrupts */
#define CRITICAL_ENTER()                                                                                               \
    do {                                                                                                               \
        CRITICAL_SECT_DEFINE;                                                                                          \
        CRITICAL_SECT_LOCK();
#define CRITICAL_EXIT()                                                                                                \
    CRITICAL_SECT_UNLOCK();                                                                                            \
    }                                                                                                                  \
    while (0)
#define CRITICAL_SECT_EXECUTE(cmd)                                                                                     \
    do {                                                                                                               \
        CRITICAL_ENTER();                                                                                              \
        cmd;                                                                                                           \
        CRITICAL_EXIT();                                                                                               \
    } while (0)

// TODO: Have a conditional code to choose between 3 modes
// Critical section code removed
// Normal implementation
// Add Statistics to use debug cycle counter and keep the largest found disabled interrupt time interval over time.

#endif


#ifndef INC_AE_TOOLBOX_H_
#define INC_AE_TOOLBOX_H_

typedef struct{
	uint32_t indexW;
	uint32_t indexR;
	uint32_t length;
	uint16_t *pBuffer;
}circularBuff_t;

extern circularBuff_t FIFO_Samples;


uint32_t isPresent(char *str, uint32_t indexMin, uint32_t indexMax, char *needle);
uint32_t returnNextIndex(uint32_t index, uint32_t number, uint32_t length);
void AE_printfUSART1(circularBuff_t *fifo, char *strsrc);
void AE_printfLPUART1(circularBuff_t *fifo, char *strsrc);
void writeIntoBufferLPUART1(circularBuff_t *fifo, char *strsrc);
void writeIntoBufferUSART1(circularBuff_t *fifo, char *strsrc);
uint32_t retMean(uint32_t a, uint32_t b);
uint32_t returnNextIndex(uint32_t index, uint32_t number, uint32_t lenght);
void initCst(void);
void processReceiveDataUSART1(circularBuff_t* fifoR, circularBuff_t* fifoT);
uint32_t get_free_space(circularBuff_t *fifo);
uint32_t get_available_bytes(circularBuff_t *fifo);
uint32_t getTimeStamp_cy(void);
void initCYCCNT(void);
uint32_t getDuration_us(uint32_t start, uint32_t stop, int32_t cyclePer_us);
//void NOPs(uint32_t loop);
void calibreNOP(uint32_t min, uint32_t max, int32_t cyclePer_us);
//uint32_t AE_Delay_us(uint32_t usec, int32_t cyclePer_us);
uint32_t AE_Delay_us(uint32_t m);
void NOPs(uint32_t loop);
void NOPs_us(uint32_t usec, int32_t cyclePer_us);
void AE_choixPWM(char PWM);
void BlinkFast(void);


#endif /* INC_AE_TOOLBOX_H_ */
