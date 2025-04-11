
#include "main.h"
#include "m95p32_hal_ospi.h"
#include "octospi.h"

	uint8_t SR_val=5;

	uint8_t Rcv[1024]={0};

	uint32_t index;

	uint32_t index_2;

	uint32_t add = 0x00;

SR_val =  RDSR(&hospi1);
	  WREN(&hospi1);
	  SR_val =  RDSR(&hospi1);
	  WRDI(&hospi1);
	  SR_val =  RDSR(&hospi1);
	  Single_Read(&hospi1,0,20,Rcv);
	  for(index=0; index<512; index ++){
	  	  Rcv[index]= index;
	  }
	  WREN(&hospi1);
	  Page_Write(&hospi1,0,512,Rcv);
	  for(index=0; index<512; index ++){
	  	  Rcv[index]= 0;
	  }
	  Single_Read(&hospi1,0,20,Rcv);
	  WREN(&hospi1);
	  Page_Erase(&hospi1,0);
	  Single_Read(&hospi1,0,20,Rcv);
	  	  for(index=0; index<512; index ++){
	  	  	  Rcv[index]= 0;
	  	  }
	  	  Read_SFDP(&hospi1,0,20,Rcv);
	  	  for(index=0; index<512; index ++){
	  	  	  Rcv[index]= 0;
	  	  }