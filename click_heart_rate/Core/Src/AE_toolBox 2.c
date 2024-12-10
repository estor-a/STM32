/*
 * AE_toolBox.c
 *
 *  Created on: Oct 27, 2023
 *      Author: alexandre estor
 */
#include "main.h"
//commentaire
//#include "AE_toolBox.h"

#define CAL_US 500
//uint32_t gresult, cyclePer_calUs, calNop = 0;
//volatile uint32_t *DWT_CYCCNT = (uint32_t *) 0xE0001004;
/*
void initCYCCNT(void)
{
	volatile uint32_t *DWT_CONTROL = (uint32_t *) 0xE0001000;
	volatile uint32_t *DEMCR = (uint32_t *) 0xE000EDFC;
	volatile uint32_t *LAR  = (uint32_t *) 0xE0001FB0;   // <-- added lock access register
	*DEMCR = *DEMCR | 0x01000000;     // enable trace
	*LAR = 0xC5ACCE55;                // <-- added unlock access to DWT (ITM, etc.)registers
	*DWT_CONTROL = *DWT_CONTROL | 1;  // enable DWT cycle counter
}

void AE_Delay_us(uint32_t usec, int32_t cyclePer_us)
{
	uint32_t start = *DWT_CYCCNT;
	uint32_t lenght = usec*cyclePer_us;
	while((*DWT_CYCCNT - start) < lenght){}
}
*/
uint32_t get_available_bytes(circularBuff_t *fifo)
{
	if(fifo->indexR > fifo->indexW)
		return((fifo->length - fifo->indexR) + fifo->indexW);
	return(fifo->indexW - fifo->indexR);
}

uint32_t get_free_space(circularBuff_t *fifo)
{
	if(fifo->indexR > fifo->indexW)
		return(fifo->indexR - fifo->indexW)-1;
	return(((fifo->length - fifo->indexW)+ fifo->indexR)-1);
}

uint32_t returnNextIndex(uint32_t index, uint32_t number, uint32_t length)
{
	return (index + number) % length;
}



/*
void initCst()
{
	 int32_t cyclePer_us= SystemCoreClock/1000000;
	 if(cyclePer_us < 2)
		 while(1);//your core frequency is too low
	 uint32_t minCycleCal = 0;
	 uint32_t maxCycleCal = cyclePer_us*500;
	 calNop = retMean(minCycleCal, maxCycleCal);
	 calibreNOP(minCycleCal, maxCycleCal, cyclePer_us);
}

uint32_t retMean(uint32_t a, uint32_t b)
{
	return((a+b)/2);
}



uint32_t getTimeStamp_cy(void)
{
	return(*DWT_CYCCNT);
}

uint32_t getDuration_us(uint32_t start, uint32_t stop, int32_t cyclePer_us)
{
	return((stop-start)/cyclePer_us);
}

void NOPs(uint32_t loop) { // global
  while(loop--) asm("nop\n");
}

void NOPs_us(uint32_t usec, int32_t cyclePer_us)
{
	uint32_t loop = (cyclePer_us * usec) / calNop;
	NOPs(loop);
}

void calibreNOP(uint32_t min, uint32_t max, int32_t cyclePer_us)
{
	uint32_t start, stop, result, previous = 1000000;
	uint32_t interrupts_enabled = (__get_PRIMASK() == 0);


	while(1)
	{
		__disable_irq();

		start = getTimeStamp_cy();
		NOPs_us(CAL_US, cyclePer_us);
		stop = getTimeStamp_cy();
		if (interrupts_enabled)
			__enable_irq();
		gresult = getDuration_us(start, stop, cyclePer_us);

		if (gresult < CAL_US)
			max = calNop;
		else if (gresult > (CAL_US+(CAL_US/10)) || previous > gresult)
			min = calNop;
		else
			break;
		calNop = retMean(max, min);
		previous = gresult;
	}
}


void BlinkFast(void)
{
	GPIOC->BSRR = 1 << 3;
	AE_Delay_us(1000000,48);
	GPIOC->BSRR = 1 << (3+16);
	GPIOC->BSRR = 1 << 2;
	AE_Delay_us(1000000,48);
	GPIOC->BSRR = 1 << (2+16);
		GPIOC->BSRR = 1 ;
		AE_Delay_us(1000000,48);
		GPIOC->BSRR = 1 << 16;

}
*/
uint32_t AddTou8fifo(circularBuff_t* pu8fifo, uint32_t In) {

	char txt[2];
	txt[0] = (char)In;
	txt[1] = 0;
	writeIntoBufferUSART1(pu8fifo, txt);
	return 0;
}

//void BTEL_putint_to_RAM(circularBuff_t* pL, uint32_t i);
void BTEL_putint_to_RAM(circularBuff_t* pL, uint32_t i)
{
	i= '0' + (i%10);	// get only the digit '0' + (byte modulo 10)
	AddTou8fifo(pL, i);
}/* end putint() */

#if 0
void Seb_Printf(circularBuff_t* pL, const char *str,...)
{
  uint32_t arg, arg1;
  int k;
  uint32_t pow;
  va_list ap;
// u8  count_char =0;
//  if(BTEL.Enabled==0) return;

  va_start(ap, str);

  while (*str)
  {
    if(*str==0x0A)
    {
//		Virtual_Com_Write_Char(0x0A); // rename the /n translated by the compiler string encoder into the 0x0D ...
      //LCD_16x2_Writedata(0x0D);
      AddTou8fifo(pL, *str);
    }
    else
    if(*str==0x09)
    {
      // tabulation, goes to middle of the screen
      AddTou8fifo(pL, *str);
    }
    else

    if (*str == 92)    // backslash (\)
    {
      str++;
      if (*str == 'n')
      {
//LCD_16x2_Writedata('\n'); // does not go to the next line in hyper terminal. Adding 0x0A/0D or 0D0A will not help.
        AddTou8fifo(pL, 0x0A);
      }
      else
      if(*str == 't')
      {
        // tabulation, goes to middle of the screen
        AddTou8fifo(pL, 0x0D); // tochange
      }
      else
      {
        AddTou8fifo(pL, 92); // backslash (\)
        AddTou8fifo(pL, *str);
      }
    }
    else { /* else 1 */
     if (*str != '%')
        AddTou8fifo(pL, *str);
     else /* else 2 */
     {
        pow = 1;
        str++;
        switch (*str)
        {
          case '%':
                  AddTou8fifo(pL, '%');
                  break; // '%%' = '%' !

           case 'd': /* decimal */
              arg = va_arg(ap, uint32_t);

              if (arg & 0x80000000) /* negative */
              {
                 AddTou8fifo(pL, '-');
                 arg = (uint32_t) (- ((int32_t) arg));
              }
              while ((uint32_t)(arg/(pow*10)))
              pow*=10;
              do{
                 BTEL_putint_to_RAM(pL, arg/pow);
                 pow/=10;
              } while (pow >= 1);
           break;
// special for 0.1 (x10) and 0.01 (x100) resolution on integers (most common for sensors)
           case 'D': /* decimal, DECI 0.1 (/10) */
              arg = va_arg(ap, uint32_t);

              if (arg & 0x80000000) /* negative */
              {
                 AddTou8fifo(pL, '-');
                 arg = (uint32_t) (- ((int32_t) arg));
              }
               while ((uint32_t)(arg/(pow*10)))
                pow*=10;

              pow = max2(10,pow);
              do{
                 BTEL_putint_to_RAM(pL, arg/pow);
                 if(pow==10)
                   AddTou8fifo(pL, '.');
                 pow/=10;
              } while (pow >= 1);
           break;
            case 'C': /* decimal, DECI 0.01 (/100) */
              arg = va_arg(ap, uint32_t);

              if (arg & 0x80000000) /* negative */
              {
                AddTou8fifo(pL, '-');
                arg = (uint32_t) (- ((int32_t) arg));
              }
              while ((uint32_t)(arg/(pow*10)))
                pow*=10;

              pow = max2(100,pow);
              do{
                BTEL_putint_to_RAM(pL, arg/pow);
                if(pow==100)
                  AddTou8fifo(pL, '.');
                pow/=10;
              } while (pow >= 1);
           break;

          case 'M': /* decimal, MILI 0.001 (/1000) */
               arg = va_arg(ap, uint32_t);

               if (arg & 0x80000000) /* negative */
               {
                  AddTou8fifo(pL, '-');
                  arg = (uint32_t) (- ((int32_t) arg));
               }

               while ((uint32_t)(arg/(pow*10)))
                 pow*=10;

               pow = max2(1000,pow);
               do{
                  BTEL_putint_to_RAM(pL, arg/pow);
                  if(pow==1000)
                    AddTou8fifo(pL, '.');
                  pow/=10;
               } while (pow >= 1);
            break;
// end

           case 'c': /* char */
              arg = va_arg(ap, uint32_t);
              AddTou8fifo(pL, arg);
           break;
           case 'x': /* Hexadecimal */
           case 'X':
              arg = va_arg(ap, uint32_t);
              arg1 = arg;
              arg1 = ((arg >> 4) & 0x0000000F);
              if (arg1 < 10)
                BTEL_putint_to_RAM(pL, arg1);
              else
                AddTou8fifo(pL, arg1 - 10 + 65 );
              arg1 = (arg & 0x0000000F);
              if (arg1 < 10)
                BTEL_putint_to_RAM(pL, arg1);
              else
                AddTou8fifo(pL, arg1 - 10 + 65 );
           break;
           case '2':
              str++;
              if ((*str == 'x') || (*str == 'X'))
              {
                arg = va_arg(ap, uint32_t);
                arg1 = arg;
		for (k=12; k>=0 ;k-=4)
                {
                  arg1 = ((arg >> k) & 0x0000000F);
                  if (arg1 < 10)
                      BTEL_putint_to_RAM(pL, arg1);
                  else
                      AddTou8fifo(pL, arg1 - 10 + 65 );
                }
              }
            break;
           case '4':
              str++;
              if ((*str == 'x') || (*str == 'X'))
              {
                arg = va_arg(ap, uint32_t); // u32 original
                arg1 = arg;

                for (k=28; k>=0 ;k-=4)
								{
                  arg1 = ((arg >> k) & 0x0000000F);
                  if (arg1 < 10)
                    BTEL_putint_to_RAM(pL, arg1);
                  else
			AddTou8fifo(pL, arg1 - 10 + 65 );
                }
              }
          break;
          default:
          break ;
        }/* end switch */
      }/* end else 2*/
     }/* end else 1 */
    str++;
   }/* end while() */

   /* Write the USB serial buffer if USB is used */
//   Virtual_Com_Write_Buffer(&serial_usb_buffer_in[0], count_char);

   va_end(ap);
}


void BTEL_TestPrintf(void) {

  int v1,v2;
  v1 = 123456;
  v2 = 234567;

  while(1) {

	  Seb_Printf(&bufferTUSART1, "*TBriskV2*U%i*V%D*W%C*%M*", v1, v1, v1, v1);
    HAL_Delay(1000);
    v1++;
    Seb_Printf(&bufferTUSART1, "*TAdd-on*U%i*V%D*W%C*%M*", v2, v2, v2, v2);
    HAL_Delay(1000);
    v1++;
  }

}
#endif
