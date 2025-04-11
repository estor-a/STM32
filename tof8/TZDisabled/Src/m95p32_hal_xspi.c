#include "main.h"



void WREN(XSPI_HandleTypeDef *local_ospi)
{
  XSPI_RegularCmdTypeDef     sCommand;
	
	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_WREN;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= 0;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_NONE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_NONE;
	sCommand.DataLength							= 1;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;

  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void WRDI(XSPI_HandleTypeDef *local_ospi)
{
  XSPI_RegularCmdTypeDef     sCommand;

	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_WRDI;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= 0;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_NONE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_NONE;
	sCommand.DataLength							= 1;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;

  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
uint8_t RDSR(XSPI_HandleTypeDef *local_ospi)
{
  XSPI_RegularCmdTypeDef     sCommand;
	uint8_t									statusValue;

	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_READ_STATUS_REG;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= 0;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_NONE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength							= 1;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	
	
  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_XSPI_Receive(local_ospi, &statusValue, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	return statusValue;
}
void Read_JEDEC(XSPI_HandleTypeDef *local_ospi,uint8_t nb_data,uint8_t *pData)
{

  XSPI_RegularCmdTypeDef     sCommand;

	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_READ_JEDEC;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= 0;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_NONE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength							= nb_data;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	
	
  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	  if (HAL_XSPI_Receive(local_ospi, pData, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void Read_SFDP(XSPI_HandleTypeDef *local_ospi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
  XSPI_RegularCmdTypeDef     sCommand;

	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_READ_SFDP;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= add;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength							= nb_data;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 8;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	
	
  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	  if (HAL_XSPI_Receive(local_ospi, pData, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void WRSR(XSPI_HandleTypeDef *local_ospi,uint32_t nb_data,uint8_t *pData)
{
  XSPI_RegularCmdTypeDef     sCommand;
	XSPI_AutoPollingTypeDef 	sConfig;

	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_WRITE_SATUTS_CONF_REG;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= 0;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_NONE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength							= nb_data;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	
	
  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	  if (HAL_XSPI_Transmit(local_ospi, pData, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  /* Configure automatic polling mode to wait for write enabling ---- */
	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_READ_STATUS_REG;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= 0;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_NONE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength							= 1;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	

  sConfig.MatchValue        	= 0x00;
  sConfig.MatchMask           = 0x03;
  sConfig.MatchMode       		= HAL_XSPI_MATCH_MODE_AND;
  sConfig.IntervalTime        = 0x10;
  sConfig.AutomaticStop   		= HAL_XSPI_AUTOMATIC_STOP_ENABLE;

	if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }	
  if (HAL_XSPI_AutoPolling(local_ospi, &sConfig, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void Single_Read(XSPI_HandleTypeDef *local_ospi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
  XSPI_RegularCmdTypeDef     sCommand;

	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_READ_DATA;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= add;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength							= nb_data;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	
	
  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	  if (HAL_XSPI_Receive(local_ospi, pData, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void FAST_Read(XSPI_HandleTypeDef *local_ospi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
  XSPI_RegularCmdTypeDef     sCommand;

	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_FAST_READ_SINGLE;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= add;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength							= nb_data;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 8;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	
	
  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	  if (HAL_XSPI_Receive(local_ospi, pData, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void FAST_DRead(XSPI_HandleTypeDef *local_ospi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
  XSPI_RegularCmdTypeDef     sCommand;

	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_FAST_READ_DUAL;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= add;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_2_LINES;
	sCommand.DataLength							= nb_data;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 8;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	
	
  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	  if (HAL_XSPI_Receive(local_ospi, pData, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void FAST_QRead(XSPI_HandleTypeDef *local_ospi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
  XSPI_RegularCmdTypeDef     sCommand;

	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_FAST_READ_QUAD;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= add;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_4_LINES;
	sCommand.DataLength							= nb_data;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 8;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	
	
  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	  if (HAL_XSPI_Receive(local_ospi, pData, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void Page_Write(XSPI_HandleTypeDef *local_ospi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
  XSPI_RegularCmdTypeDef     sCommand;
	XSPI_AutoPollingTypeDef 	sConfig;

	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_WRITE_PAGE;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= add;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength							= nb_data;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	
	
  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	  if (HAL_XSPI_Transmit(local_ospi, pData, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  /* Configure automatic polling mode to wait for write enabling ---- */
	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_READ_STATUS_REG;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= 0;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_NONE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength							= 1;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	

  sConfig.MatchValue        	= 0x00;
  sConfig.MatchMask           = 0x03;
  sConfig.MatchMode       		= HAL_XSPI_MATCH_MODE_AND;
  sConfig.IntervalTime        = 0x10;
  sConfig.AutomaticStop   		= HAL_XSPI_AUTOMATIC_STOP_ENABLE;

	if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }	
  if (HAL_XSPI_AutoPolling(local_ospi, &sConfig, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void Page_Prog(XSPI_HandleTypeDef *local_ospi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
  XSPI_RegularCmdTypeDef     sCommand;
	XSPI_AutoPollingTypeDef 	sConfig;

	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_PROG_PAGE;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= add;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength							= nb_data;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	
	
  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	  if (HAL_XSPI_Transmit(local_ospi, pData, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  /* Configure automatic polling mode to wait for write enabling ---- */
	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_READ_STATUS_REG;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= 0;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_NONE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength							= 1;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	

  sConfig.MatchValue        	= 0x00;
  sConfig.MatchMask           = 0x03;
  sConfig.MatchMode       		= HAL_XSPI_MATCH_MODE_AND;
  sConfig.IntervalTime        = 0x10;
  sConfig.AutomaticStop   		= HAL_XSPI_AUTOMATIC_STOP_ENABLE;

	if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }	
  if (HAL_XSPI_AutoPolling(local_ospi, &sConfig, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void Page_Erase(XSPI_HandleTypeDef *local_ospi,uint32_t Add)
{
  XSPI_RegularCmdTypeDef     sCommand;
	XSPI_AutoPollingTypeDef 	sConfig;

	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_ERASE_PAGE;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= Add;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_NONE;
	sCommand.DataLength							= 0;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	
	
  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  /* Configure automatic polling mode to wait for write enabling ---- */
	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_READ_STATUS_REG;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= 0;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_NONE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength							= 1;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	

  sConfig.MatchValue        	= 0x00;
  sConfig.MatchMask           = 0x03;
  sConfig.MatchMode       		= HAL_XSPI_MATCH_MODE_AND;
  sConfig.IntervalTime        = 0x10;
  sConfig.AutomaticStop   		= HAL_XSPI_AUTOMATIC_STOP_ENABLE;

	if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }	
  if (HAL_XSPI_AutoPolling(local_ospi, &sConfig, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void Sector_Erase(XSPI_HandleTypeDef *local_ospi,uint32_t Add)
{
  XSPI_RegularCmdTypeDef     sCommand;
	XSPI_AutoPollingTypeDef 	sConfig;

	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_ERASE_SECTOR;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= Add;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_NONE;
	sCommand.DataLength							= 0;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	
	
  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  /* Configure automatic polling mode to wait for write enabling ---- */
	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_READ_STATUS_REG;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= 0;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_NONE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength							= 1;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	

  sConfig.MatchValue        	= 0x00;
  sConfig.MatchMask           = 0x03;
  sConfig.MatchMode       		= HAL_XSPI_MATCH_MODE_AND;
  sConfig.IntervalTime        = 0x10;
  sConfig.AutomaticStop   		= HAL_XSPI_AUTOMATIC_STOP_ENABLE;

	if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }	
  if (HAL_XSPI_AutoPolling(local_ospi, &sConfig, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void Block_Erase(XSPI_HandleTypeDef *local_ospi,uint32_t Add)
{
  XSPI_RegularCmdTypeDef     sCommand;
	XSPI_AutoPollingTypeDef 	sConfig;

	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_ERASE_BLOCK;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= Add;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_NONE;
	sCommand.DataLength							= 0;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	
	
  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  /* Configure automatic polling mode to wait for write enabling ---- */
	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_READ_STATUS_REG;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= 0;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_NONE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength							= 1;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	

  sConfig.MatchValue        	= 0x00;
  sConfig.MatchMask           = 0x03;
  sConfig.MatchMode       		= HAL_XSPI_MATCH_MODE_AND;
  sConfig.IntervalTime        = 0x10;
  sConfig.AutomaticStop   		= HAL_XSPI_AUTOMATIC_STOP_ENABLE;

	if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }	
  if (HAL_XSPI_AutoPolling(local_ospi, &sConfig, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void Chip_Erase(XSPI_HandleTypeDef *local_ospi)
{
  XSPI_RegularCmdTypeDef     sCommand;
	XSPI_AutoPollingTypeDef 	sConfig;

	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_ERASE_CHIP;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= 0;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_NONE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_NONE;
	sCommand.DataLength							= 0;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	
	
  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  /* Configure automatic polling mode to wait for write enabling ---- */
	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_READ_STATUS_REG;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= 0;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_NONE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength							= 1;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	

  sConfig.MatchValue        	= 0x00;
  sConfig.MatchMask           = 0x03;
  sConfig.MatchMode       		= HAL_XSPI_MATCH_MODE_AND;
  sConfig.IntervalTime        = 0x10;
  sConfig.AutomaticStop   		= HAL_XSPI_AUTOMATIC_STOP_ENABLE;

	if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }	
  if (HAL_XSPI_AutoPolling(local_ospi, &sConfig, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void Read_ID(XSPI_HandleTypeDef *local_ospi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
  XSPI_RegularCmdTypeDef     sCommand;

	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_READ_ID_PAGE;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= add;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength							= nb_data;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	
	
  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	  if (HAL_XSPI_Receive(local_ospi, pData, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void FAST_Read_ID(XSPI_HandleTypeDef *local_ospi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
  XSPI_RegularCmdTypeDef     sCommand;

	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_FAST_READ_ID_PAGE;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= add;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength							= nb_data;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 8;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	
	
  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	  if (HAL_XSPI_Receive(local_ospi, pData, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void Write_ID(XSPI_HandleTypeDef *local_ospi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
  XSPI_RegularCmdTypeDef     sCommand;
	XSPI_AutoPollingTypeDef 	sConfig;

	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_WRITE_ID_PAGE;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= add;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength							= nb_data;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	
	
  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	  if (HAL_XSPI_Transmit(local_ospi, pData, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  /* Configure automatic polling mode to wait for write enabling ---- */
	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_READ_STATUS_REG;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= 0;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_NONE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength							= 1;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	

  sConfig.MatchValue        	= 0x00;
  sConfig.MatchMask           = 0x03;
  sConfig.MatchMode       		= HAL_XSPI_MATCH_MODE_AND;
  sConfig.IntervalTime        = 0x10;
  sConfig.AutomaticStop   		= HAL_XSPI_AUTOMATIC_STOP_ENABLE;

	if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }	
  if (HAL_XSPI_AutoPolling(local_ospi, &sConfig, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void Deep_Power_Down(XSPI_HandleTypeDef *local_ospi)
{
  XSPI_RegularCmdTypeDef     sCommand;
	
	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_DEEP_POWER_DOWN;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= 0;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_NONE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_NONE;
	sCommand.DataLength							= 1;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;

  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void Deep_Power_Down_Release(XSPI_HandleTypeDef *local_ospi)
{
  XSPI_RegularCmdTypeDef     sCommand;
	
	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_DEEP_POWER_DOWN_RELEASE;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= 0;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_NONE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_NONE;
	sCommand.DataLength							= 1;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;

  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void WriteVolatileRegister(XSPI_HandleTypeDef *local_ospi,uint8_t regVal)
{
  XSPI_RegularCmdTypeDef     sCommand;
	XSPI_AutoPollingTypeDef 	sConfig;

	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_WRITE_VOLATILE_REG;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= 0;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_NONE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength							= 1;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	
	
  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	  if (HAL_XSPI_Transmit(local_ospi, &regVal, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  /* Configure automatic polling mode to wait for write enabling ---- */
	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_READ_STATUS_REG;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= 0;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_NONE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength							= 1;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;	

  sConfig.MatchValue        	= 0x00;
  sConfig.MatchMask           = 0x03;
  sConfig.MatchMode       		= HAL_XSPI_MATCH_MODE_AND;
  sConfig.IntervalTime        = 0x10;
  sConfig.AutomaticStop   		= HAL_XSPI_AUTOMATIC_STOP_ENABLE;

	if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }	
  if (HAL_XSPI_AutoPolling(local_ospi, &sConfig, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void ClearSafetyFlag(XSPI_HandleTypeDef *local_ospi)
{
  XSPI_RegularCmdTypeDef     sCommand;
	
	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_CLEAR_SAFETY_REG;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= 0;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_NONE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_NONE;
	sCommand.DataLength							= 1;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;

  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void Reset_Enable(XSPI_HandleTypeDef *local_ospi)
{
  XSPI_RegularCmdTypeDef     sCommand;
	
	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_ENABLE_RESET;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= 0;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_NONE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_NONE;
	sCommand.DataLength							= 1;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;

  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void Soft_Reset(XSPI_HandleTypeDef *local_ospi)
{
  XSPI_RegularCmdTypeDef     sCommand;
	
	sCommand.OperationType					= HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect 							= HAL_XSPI_SELECT_IO_3_0;
  sCommand.Instruction       			= CMD_SOFT_RESET;
  sCommand.InstructionMode   			= HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth	 			= HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode			= HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address								= 0;
  sCommand.AddressMode       			= HAL_XSPI_ADDRESS_NONE;
	sCommand.AddressWidth						= HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode					= HAL_XSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytes					= 0;
	sCommand.AlternateBytesMode			= HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth		=	HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode	=	HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode								=	HAL_XSPI_DATA_NONE;
	sCommand.DataLength							= 1;
  sCommand.DataDTRMode						= HAL_XSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles						= 0;
  sCommand.DQSMode								= HAL_XSPI_DQS_DISABLE;
  sCommand.SIOOMode								= HAL_XSPI_SIOO_INST_EVERY_CMD;

  if (HAL_XSPI_Command(local_ospi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
