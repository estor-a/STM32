#include "AE_Max30100.h"
#include "AE_toolBox.h"

uint16_t str[lengthBuff];
circularBuff_t FIFO_Samples = { .indexW = 0, .indexR = 0, .length = lengthBuff,
		.pBuffer = str };


void resetFIFO(circularBuff_t *FIFO) {
	uint16_t i = 0;
	while (i < FIFO->length) {
		FIFO->pBuffer[i] ^= FIFO->pBuffer[i];
		i++;
	}
	FIFO->indexR = 0;
	FIFO->indexW = 0;
}


uint8_t numberSamplesReady(circularBuff_t *FIFO, uint8_t WR_PTR, uint8_t RD_PTR, uint8_t OF_PTR) {
	if (WR_PTR == RD_PTR && OF_PTR != 0) {
		resetFIFO(FIFO);
		return (16);
	}
	if (WR_PTR == RD_PTR)
		return (0);
	int8_t diff = abs(RD_PTR - WR_PTR);
	if (RD_PTR < WR_PTR)
		return (diff);
	else
		return (16 - diff);
}

uint8_t enoughDataForHR(circularBuff_t *FIFO) {
	uint16_t dataReady = 0;
	if (FIFO->indexR == FIFO->indexW)
		return (0);
	int16_t diff = abs(FIFO->indexR - FIFO->indexW);
	if (FIFO->indexR < FIFO->indexW)
		dataReady = diff;
	else
		dataReady = (FIFO->length - diff);
	if (dataReady > 160)
		return 1;
	return 0;

}


uint16_t HeartRateSensor_Read(I2C_HandleTypeDef *hi2c1, uint8_t numberSamples) {
	uint8_t fifoData[4];
	uint8_t i = 0;
	while (i < numberSamples) {
		HAL_I2C_Mem_Read(hi2c1, MAX30100_I2C_ADDR << 1, MAX30100_REG_FIFO_DATA,
		I2C_MEMADD_SIZE_8BIT, fifoData, 4, HAL_MAX_DELAY);
		FIFO_Samples.pBuffer[FIFO_Samples.indexW] = ((fifoData[0] << 8) | fifoData[1]);
		FIFO_Samples.indexW = returnNextIndex(FIFO_Samples.indexW, 1,lengthBuff);

		i++;
	}
	return 1;
}


uint8_t HeartRateSensor_Read_Reg(I2C_HandleTypeDef *hi2c1, uint8_t MAC30100_REG_ADDR) {

	uint8_t Data = 0;

	// Lire 4 octets de données à partir du registre FIFO
	HAL_I2C_Mem_Read(hi2c1, MAX30100_I2C_ADDR << 1, MAC30100_REG_ADDR,
	I2C_MEMADD_SIZE_8BIT, &Data, 1, HAL_MAX_DELAY);
	return Data;
}

int16_t linearRegressionX(circularBuff_t *FIFO) // this function calcul the X angle of the Hand. 0 is flat, 45° is when hand face forward, -45° is when hand face backward
{
    float slope = 0, intercept = 0;
    uint16_t count = 0;
    uint16_t val = 0;
    uint16_t x =0;
    float sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    for (int i = 0; i < numberSamplesLinear; i++)
    {
      val = FIFO->pBuffer[returnNextIndex(FIFO->indexR, i, FIFO->length)]; // sry for this horrible line
      x = 1 * i;
      if(val == 0)
        continue;

        sumX += x;
        sumY += val;
        sumXY += x * val;
        sumX2 += x * x;
        count++;
    }
    FIFO->indexR = returnNextIndex(FIFO->indexR,  1,  FIFO->length);
    if(count == 0)
      return 200;
    slope = ((float)count * sumXY - sumX * sumY) / ((float)count * sumX2 - sumX * sumX);
    intercept = ((float)sumY - slope * sumX) / count;
    int32_t slopeInt = (int32_t)slope;
    //printf("%d\n\r", slopeInt);
    if(slopeInt < -200 || slopeInt > 200)
    {
    	resetFIFO(FIFO);
    	printf("slope too big, reset FIFO\n\r");
    	return(200);
    }

    return (slopeInt);
}

uint16_t diffFIFO(uint16_t value, uint16_t target, uint16_t length)
{
	uint16_t i = 0;
	while(value != target)
	{
		value = returnNextIndex(value, 1, length);
		i++;
	}
	return(i);
}

void AE_Max30100Init(I2C_HandleTypeDef *hi2c1)
{
		MAX30100_Init(hi2c1, 0); //&huart4);
		MAX30100_SetSpO2SampleRate(MAX30100_SPO2SR_DEFAULT);
		MAX30100_SetLEDPulseWidth(MAX30100_LEDPW_DEFAULT);
		MAX30100_SetLEDCurrent(MAX30100_LEDCURRENT_DEFAULT,MAX30100_LEDCURRENT_DEFAULT);
		MAX30100_SetMode(MAX30100_HRONLY_MODE);
}

void getHR(I2C_HandleTypeDef *hi2c1)
{
uint8_t sampleReady = 0;
int16_t slope = 0;
int16_t indexLastPeak = -1;
int16_t indexPeak = -1;
uint16_t endPeak = 1;
uint16_t periodHeart = 0;
uint16_t heartRate = 0;
float frequency_heart;
while(!enoughDataForHR(&FIFO_Samples)){
	if(FIFO_Samples.indexW == 0 && FIFO_Samples.indexR == 0)
				printf("start sampling, do not move\n\r");
			sampleReady = numberSamplesReady(&FIFO_Samples, HeartRateSensor_Read_Reg(hi2c1, MAX30100_REG_FIFO_WR_PTR), HeartRateSensor_Read_Reg(hi2c1, MAX30100_REG_FIFO_RD_PTR), HeartRateSensor_Read_Reg(hi2c1, MAX30100_REG_FIFO_OF_PTR));
			if(sampleReady)
				HeartRateSensor_Read(hi2c1, sampleReady); //read the amount of data ready
}
			if(enoughDataForHR(&FIFO_Samples))
			{
				indexLastPeak = -1;
				indexPeak = -1;
				while(FIFO_Samples.indexR != (FIFO_Samples.indexW - numberSamplesLinear))
				{
					slope = linearRegressionX(&FIFO_Samples);
					if(slope == 200)
						break;
					if(slope < -20 && endPeak)
					{
						indexLastPeak = indexPeak;
						indexPeak = (FIFO_Samples.indexR - 1);
						endPeak = 0;
					}
					if(slope > 0 && endPeak == 0 )
					{
						endPeak = 1;
					}
					if(indexLastPeak != -1 && indexPeak != -1)
					{
						periodHeart = diffFIFO(indexLastPeak, indexPeak, FIFO_Samples.length) * 10; //period en ms
						frequency_heart = (1 /((float)periodHeart/1000));
						heartRate = frequency_heart * 60;
						if(heartRate > 30 && heartRate < 150)
						{
							printf("HR = %d\n\r", heartRate);
							break;
						}

						else
						{
							printf("error while computing, reset FIFO\n\r");
							resetFIFO(&FIFO_Samples);
							break;
						}

						indexLastPeak = indexPeak;
						indexPeak = -1;

					}
				}
			}

}

