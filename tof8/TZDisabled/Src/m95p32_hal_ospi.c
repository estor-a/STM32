#include "main.h"
#include "m95p32_hal_ospi.h"

void WREN(OSPI_HandleTypeDef *local_hospi)
{
	OSPI_RegularCmdTypeDef     		sCommand;
	sCommand.OperationType			= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId				= HAL_OSPI_FLASH_ID_1;
	sCommand.Instruction			= CMD_WREN;
	sCommand.InstructionMode		= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize		= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
	sCommand.Address				= 0;
	sCommand.AddressMode			= HAL_OSPI_ADDRESS_NONE;
	sCommand.AddressSize			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
	sCommand.AlternateBytes			= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
	sCommand.DataMode				= HAL_OSPI_DATA_NONE;
	sCommand.NbData					= 0;
	sCommand.DataDtrMode			=	HAL_OSPI_DATA_DTR_DISABLE;
	sCommand.DummyCycles			= 0;
	sCommand.DQSMode				= HAL_OSPI_DQS_DISABLE;
	sCommand.SIOOMode				= HAL_OSPI_SIOO_INST_EVERY_CMD;
  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void WRDI(OSPI_HandleTypeDef *local_hospi)
{
	OSPI_RegularCmdTypeDef			sCommand;
	sCommand.OperationType			= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId				= HAL_OSPI_FLASH_ID_1;
	sCommand.Instruction			= CMD_WRDI;
	sCommand.InstructionMode		= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize		= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;

	sCommand.Address				= 0;
	sCommand.AddressMode			= HAL_OSPI_ADDRESS_NONE;
	sCommand.AddressSize			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
	sCommand.AlternateBytes			= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
	sCommand.DataMode				= HAL_OSPI_DATA_NONE;
	sCommand.NbData					= 0;
	sCommand.DataDtrMode			=	HAL_OSPI_DATA_DTR_DISABLE;
	sCommand.DummyCycles			= 0;
	sCommand.DQSMode				= HAL_OSPI_DQS_DISABLE;
	sCommand.SIOOMode				= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
uint8_t RDSR(OSPI_HandleTypeDef *local_hospi)
{
	uint8_t							statusValue;
	OSPI_RegularCmdTypeDef			sCommand;
	sCommand.OperationType			= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId				= HAL_OSPI_FLASH_ID_1;
	sCommand.Instruction			= CMD_READ_STATUS_REG;
	sCommand.InstructionMode		= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize		= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
	sCommand.Address				= 0;
	sCommand.AddressMode			= HAL_OSPI_ADDRESS_NONE;
	sCommand.AddressSize			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
	sCommand.AlternateBytes			= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
	sCommand.DataMode				= HAL_OSPI_DATA_1_LINE;
	sCommand.NbData					= 1;
	sCommand.DataDtrMode			=	HAL_OSPI_DATA_DTR_DISABLE;
	sCommand.DummyCycles			= 0;
	sCommand.DQSMode				= HAL_OSPI_DQS_DISABLE;
	sCommand.SIOOMode				= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_OSPI_Receive(local_hospi, &statusValue, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	return statusValue;
}
void AutoPolling(OSPI_HandleTypeDef *local_hospi)
{
  OSPI_RegularCmdTypeDef  sCommand;
  OSPI_AutoPollingTypeDef sConfig;


  /* Configure automatic polling mode to wait for memory ready ------ */
  sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;
  sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction        = CMD_READ_STATUS_REG;
  sCommand.InstructionMode    = HAL_OSPI_INSTRUCTION_1_LINE;
  sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
  sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.Address            = 0x0;
  sCommand.AddressMode        = HAL_OSPI_ADDRESS_NONE;
  sCommand.AddressSize        = HAL_OSPI_ADDRESS_24_BITS;
  sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;
  sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode           = HAL_OSPI_DATA_1_LINE;
  sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.NbData             = 1;
  sCommand.DummyCycles        = 0;
  sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }

  sConfig.Match         = 0x00;
  sConfig.Mask          = 0x03;
  sConfig.MatchMode     = HAL_OSPI_MATCH_MODE_AND;
  sConfig.Interval      = 0x0a;
  sConfig.AutomaticStop = HAL_OSPI_AUTOMATIC_STOP_ENABLE;

  if (HAL_OSPI_AutoPolling(local_hospi, &sConfig, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	
}
void WRSR(OSPI_HandleTypeDef *local_hospi,uint32_t nb_data,uint8_t *pData)
{
	OSPI_RegularCmdTypeDef			sCommand;
//	OSPI_AutoPollingTypeDef			sConfig;
 	sCommand.OperationType			= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId				= HAL_OSPI_FLASH_ID_1;
	sCommand.Instruction			= CMD_WRITE_SATUTS_CONF_REG;
	sCommand.InstructionMode		= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize	 	= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
	sCommand.Address				= 0;
	sCommand.AddressMode			= HAL_OSPI_ADDRESS_NONE;
	sCommand.AddressSize			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
	sCommand.AlternateBytes			= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
	sCommand.DataMode				= HAL_OSPI_DATA_1_LINE;
	sCommand.NbData					= nb_data;
	sCommand.DataDtrMode			=	HAL_OSPI_DATA_DTR_DISABLE;
	sCommand.DummyCycles			= 0;
	sCommand.DQSMode				= HAL_OSPI_DQS_DISABLE;
	sCommand.SIOOMode				= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	if (HAL_OSPI_Transmit(local_hospi, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	AutoPolling(local_hospi);
}
void Single_Read(OSPI_HandleTypeDef *local_hospi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
	OSPI_RegularCmdTypeDef			sCommand;
	sCommand.OperationType			= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId				= HAL_OSPI_FLASH_ID_1;
	sCommand.Instruction			= CMD_READ_DATA;
	sCommand.InstructionMode		= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize		= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
	sCommand.Address				= add;
	sCommand.AddressMode			= HAL_OSPI_ADDRESS_1_LINE;
	sCommand.AddressSize			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
	sCommand.AlternateBytes			= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
	sCommand.DataMode				= HAL_OSPI_DATA_1_LINE;
	sCommand.NbData					= nb_data;
	sCommand.DataDtrMode			=	HAL_OSPI_DATA_DTR_DISABLE;
	sCommand.DummyCycles			= 0;
	sCommand.DQSMode				= HAL_OSPI_DQS_DISABLE;
	sCommand.SIOOMode				= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_OSPI_Receive(local_hospi, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void FAST_Read(OSPI_HandleTypeDef *local_hospi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
  OSPI_RegularCmdTypeDef			sCommand;
	sCommand.OperationType			= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId				= HAL_OSPI_FLASH_ID_1;
	sCommand.Instruction			= CMD_FAST_READ_SINGLE;
	sCommand.InstructionMode		= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize		= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
	sCommand.Address				= add;
	sCommand.AddressMode			= HAL_OSPI_ADDRESS_1_LINE;
	sCommand.AddressSize			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
	sCommand.AlternateBytes			= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
	sCommand.DataMode				= HAL_OSPI_DATA_1_LINE;
	sCommand.NbData					= nb_data;
	sCommand.DataDtrMode			=	HAL_OSPI_DATA_DTR_DISABLE;
	sCommand.DummyCycles			= 8;
	sCommand.DQSMode				= HAL_OSPI_DQS_DISABLE;
	sCommand.SIOOMode				= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_OSPI_Receive(local_hospi, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void FAST_DRead(OSPI_HandleTypeDef *local_hospi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
	OSPI_RegularCmdTypeDef			sCommand;

	sCommand.OperationType			= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId				= HAL_OSPI_FLASH_ID_1;
	sCommand.Instruction			= CMD_FAST_READ_DUAL;
	sCommand.InstructionMode		= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize		= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
	sCommand.Address				= add;
	sCommand.AddressMode			= HAL_OSPI_ADDRESS_1_LINE;
	sCommand.AddressSize			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
	sCommand.AlternateBytes			= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
	sCommand.DataMode				= HAL_OSPI_DATA_2_LINES;
	sCommand.NbData					= nb_data;
	sCommand.DataDtrMode			=	HAL_OSPI_DATA_DTR_DISABLE;
	sCommand.DummyCycles			= 8;
	sCommand.DQSMode				= HAL_OSPI_DQS_DISABLE;
	sCommand.SIOOMode				= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_OSPI_Receive(local_hospi, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void FAST_QRead(OSPI_HandleTypeDef *local_hospi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
	OSPI_RegularCmdTypeDef			sCommand;
 	sCommand.OperationType			= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId				= HAL_OSPI_FLASH_ID_1;
	sCommand.Instruction			= CMD_FAST_READ_QUAD;
	sCommand.InstructionMode		= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize		= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
	sCommand.Address				= add;
	sCommand.AddressMode			= HAL_OSPI_ADDRESS_1_LINE;
	sCommand.AddressSize			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
	sCommand.AlternateBytes			= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
	sCommand.DataMode				= HAL_OSPI_DATA_4_LINES;
	sCommand.NbData					= nb_data;
	sCommand.DataDtrMode			=	HAL_OSPI_DATA_DTR_DISABLE;
	sCommand.DummyCycles			= 8;
	sCommand.DQSMode				= HAL_OSPI_DQS_DISABLE;
	sCommand.SIOOMode				= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_OSPI_Receive(local_hospi, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void Page_Write(OSPI_HandleTypeDef *local_hospi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
	OSPI_RegularCmdTypeDef     sCommand;
//	OSPI_AutoPollingTypeDef		 sConfig;
 
	sCommand.OperationType					= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId								= HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction       			= CMD_WRITE_PAGE;
  sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize	 			= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
  sCommand.Address       					= add;
  sCommand.AddressMode       			= HAL_OSPI_ADDRESS_1_LINE;
  sCommand.AddressSize       			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode		 			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
  sCommand.AlternateBytes					= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize 		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
	
  sCommand.DataMode          			= HAL_OSPI_DATA_1_LINE;
	sCommand.NbData 								= nb_data;
	sCommand.DataDtrMode						=	HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles       			= 0;
  sCommand.DQSMode       					= HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode         			= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	if (HAL_OSPI_Transmit(local_hospi, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	AutoPolling(local_hospi);
}
void Page_Prog(OSPI_HandleTypeDef *local_hospi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
	OSPI_RegularCmdTypeDef     sCommand;
//	OSPI_AutoPollingTypeDef		 sConfig;

	sCommand.OperationType					= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId								= HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction       			= CMD_PROG_PAGE;
  sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize	 			= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
  sCommand.Address       					= add;
  sCommand.AddressMode       			= HAL_OSPI_ADDRESS_1_LINE;
  sCommand.AddressSize       			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode		 			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
  sCommand.AlternateBytes					= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize 		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
	
  sCommand.DataMode          			= HAL_OSPI_DATA_1_LINE;
	sCommand.NbData 								= nb_data;
	sCommand.DataDtrMode						=	HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles       			= 0;
  sCommand.DQSMode       					= HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode         			= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	if (HAL_OSPI_Transmit(local_hospi, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	AutoPolling(local_hospi);
}
void Page_Erase(OSPI_HandleTypeDef *local_hospi,uint32_t Add)
{
	OSPI_RegularCmdTypeDef     sCommand;
//	OSPI_AutoPollingTypeDef		 sConfig;

	sCommand.OperationType					= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId								= HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction       			= CMD_ERASE_PAGE;
  sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize	 			= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
  sCommand.Address       					= Add;
  sCommand.AddressMode       			= HAL_OSPI_ADDRESS_1_LINE;
  sCommand.AddressSize       			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode		 			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
  sCommand.AlternateBytes					= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize 		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
	
  sCommand.DataMode          			= HAL_OSPI_DATA_NONE;
	sCommand.NbData 								= 0;
	sCommand.DataDtrMode						=	HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles       			= 0;
  sCommand.DQSMode       					= HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode         			= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  AutoPolling(local_hospi);
}
void Sector_Erase(OSPI_HandleTypeDef *local_hospi,uint32_t Add)
{
	OSPI_RegularCmdTypeDef     sCommand;
//	OSPI_AutoPollingTypeDef		 sConfig;

	sCommand.OperationType					= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId								= HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction       			= CMD_ERASE_SECTOR;
  sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize	 			= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
  sCommand.Address       					= Add;
  sCommand.AddressMode       			= HAL_OSPI_ADDRESS_1_LINE;
  sCommand.AddressSize       			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode		 			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
  sCommand.AlternateBytes					= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize 		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
	
  sCommand.DataMode          			= HAL_OSPI_DATA_NONE;
	sCommand.NbData 								= 0;
	sCommand.DataDtrMode						=	HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles       			= 0;
  sCommand.DQSMode       					= HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode         			= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  AutoPolling(local_hospi);
}
void Block_Erase(OSPI_HandleTypeDef *local_hospi,uint32_t Add)
{
	OSPI_RegularCmdTypeDef     sCommand;
//	OSPI_AutoPollingTypeDef		 sConfig;

	sCommand.OperationType					= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId								= HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction       			= CMD_ERASE_BLOCK;
  sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize	 			= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
  sCommand.Address       					= Add;
  sCommand.AddressMode       			= HAL_OSPI_ADDRESS_1_LINE;
  sCommand.AddressSize       			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode		 			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
  sCommand.AlternateBytes					= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize 		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
	
  sCommand.DataMode          			= HAL_OSPI_DATA_NONE;
	sCommand.NbData 								= 0;
	sCommand.DataDtrMode						=	HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles       			= 0;
  sCommand.DQSMode       					= HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode         			= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  AutoPolling(local_hospi);
}
void Chip_Erase(OSPI_HandleTypeDef *local_hospi)
{
	OSPI_RegularCmdTypeDef     sCommand;
//	OSPI_AutoPollingTypeDef		 sConfig;

	sCommand.OperationType					= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId								= HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction       			= CMD_ERASE_CHIP;
  sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize	 			= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
  sCommand.Address       					= 0;
  sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;
  sCommand.AddressSize       			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode		 			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
  sCommand.AlternateBytes					= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize 		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
	
  sCommand.DataMode          			= HAL_OSPI_DATA_NONE;
	sCommand.NbData 								= 0;
	sCommand.DataDtrMode						=	HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles       			= 0;
  sCommand.DQSMode       					= HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode         			= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  AutoPolling(local_hospi);
}
void Read_ID(OSPI_HandleTypeDef *local_hospi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
  OSPI_RegularCmdTypeDef     sCommand;

	sCommand.OperationType					= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId								= HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction       			= CMD_READ_ID_PAGE;
  sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize	 			= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
  sCommand.Address       					= add;
  sCommand.AddressMode       			= HAL_OSPI_ADDRESS_1_LINE;
  sCommand.AddressSize       			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode		 			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
  sCommand.AlternateBytes					= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize 		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
  sCommand.DataMode          			= HAL_OSPI_DATA_1_LINE;
	sCommand.NbData 								= nb_data;
	sCommand.DataDtrMode						=	HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles       			= 0;
  sCommand.DQSMode       					= HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode         			= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_OSPI_Receive(local_hospi, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void FAST_Read_ID(OSPI_HandleTypeDef *local_hospi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
  OSPI_RegularCmdTypeDef     sCommand;

	sCommand.OperationType					= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId								= HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction       			= CMD_FAST_READ_ID_PAGE;
  sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize	 			= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
  sCommand.Address       					= add;
  sCommand.AddressMode       			= HAL_OSPI_ADDRESS_1_LINE;
  sCommand.AddressSize       			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode		 			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
  sCommand.AlternateBytes					= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize 		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
  sCommand.DataMode          			= HAL_OSPI_DATA_1_LINE;
	sCommand.NbData 								= nb_data;
	sCommand.DataDtrMode						=	HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles       			= 8;
  sCommand.DQSMode       					= HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode         			= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_OSPI_Receive(local_hospi, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void Write_ID(OSPI_HandleTypeDef *local_hospi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
	OSPI_RegularCmdTypeDef     sCommand;
//	OSPI_AutoPollingTypeDef		 sConfig;

	sCommand.OperationType					= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId								= HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction       			= CMD_WRITE_ID_PAGE;
  sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize	 			= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
  sCommand.Address       					= add;
  sCommand.AddressMode       			= HAL_OSPI_ADDRESS_1_LINE;
  sCommand.AddressSize       			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode		 			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
  sCommand.AlternateBytes					= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize 		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
	
  sCommand.DataMode          			= HAL_OSPI_DATA_1_LINE;
	sCommand.NbData 								= nb_data;
	sCommand.DataDtrMode						=	HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles       			= 0;
  sCommand.DQSMode       					= HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode         			= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	if (HAL_OSPI_Transmit(local_hospi, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	AutoPolling(local_hospi);
}
void Deep_Power_Down(OSPI_HandleTypeDef *local_hospi)
{
  OSPI_RegularCmdTypeDef     sCommand;

	sCommand.OperationType					= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId								= HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction       			= CMD_DEEP_POWER_DOWN;
  sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize	 			= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
  sCommand.Address       					= 0;
  sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;
  sCommand.AddressSize       			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode		 			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
  sCommand.AlternateBytes					= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize 		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
  sCommand.DataMode          			= HAL_OSPI_DATA_NONE;
	sCommand.NbData 								= 0;
	sCommand.DataDtrMode						=	HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles       			= 0;
  sCommand.DQSMode       					= HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode         			= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void Deep_Power_Down_Release(OSPI_HandleTypeDef *local_hospi)
{
  OSPI_RegularCmdTypeDef     sCommand;
	sCommand.OperationType					= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId								= HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction       			= CMD_DEEP_POWER_DOWN_RELEASE;
  sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize	 			= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
  sCommand.Address       					= 0;
  sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;
  sCommand.AddressSize       			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode		 			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
  sCommand.AlternateBytes					= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize 		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
  sCommand.DataMode          			= HAL_OSPI_DATA_NONE;
	sCommand.NbData 								= 0;
	sCommand.DataDtrMode						=	HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles       			= 0;
  sCommand.DQSMode       					= HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode         			= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void Read_JEDEC(OSPI_HandleTypeDef *local_hospi,uint8_t nb_data,uint8_t *pData)
{
  OSPI_RegularCmdTypeDef     sCommand;

	sCommand.OperationType					= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId								= HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction       			= CMD_READ_JEDEC;
  sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize	 			= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
  sCommand.Address       					= 0;
  sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;
  sCommand.AddressSize       			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode		 			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
  sCommand.AlternateBytes					= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize 		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
  sCommand.DataMode          			= HAL_OSPI_DATA_1_LINE;
	sCommand.NbData 								= nb_data;
	sCommand.DataDtrMode						=	HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles       			= 0;
  sCommand.DQSMode       					= HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode         			= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_OSPI_Receive(local_hospi, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void ReadConfigReg(OSPI_HandleTypeDef *local_hospi,uint8_t nb_reg,uint8_t *pData)
{
  OSPI_RegularCmdTypeDef     sCommand;

	sCommand.OperationType					= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId								= HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction       			= CMD_READ_CONF_SAFETY_REG;
  sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize	 			= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
  sCommand.Address       					= 0;
  sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;
  sCommand.AddressSize       			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode		 			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
  sCommand.AlternateBytes					= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize 		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
  sCommand.DataMode          			= HAL_OSPI_DATA_1_LINE;
	sCommand.NbData 								= nb_reg;
	sCommand.DataDtrMode						=	HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles       			= 0;
  sCommand.DQSMode       					= HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode         			= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_OSPI_Receive(local_hospi, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }	
}
uint8_t ReadVolatileReg(OSPI_HandleTypeDef *local_hospi)
{
	uint8_t value;
  OSPI_RegularCmdTypeDef     sCommand;

	sCommand.OperationType					= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId								= HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction       			= CMD_READ_VOLATILE_REG;
  sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize	 			= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
  sCommand.Address       					= 0;
  sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;
  sCommand.AddressSize       			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode		 			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
  sCommand.AlternateBytes					= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize 		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
  sCommand.DataMode          			= HAL_OSPI_DATA_1_LINE;
	sCommand.NbData 								= 1;
	sCommand.DataDtrMode						=	HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles       			= 0;
  sCommand.DQSMode       					= HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode         			= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_OSPI_Receive(local_hospi, &value, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }	
	return value;
}
void WriteVolatileRegister(OSPI_HandleTypeDef *local_hospi,uint8_t regVal)
{
	OSPI_RegularCmdTypeDef     sCommand;
//	OSPI_AutoPollingTypeDef		 sConfig;

	sCommand.OperationType					= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId								= HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction       			= CMD_WRITE_VOLATILE_REG;
  sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize	 			= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
  sCommand.Address       					= 0;
  sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;
  sCommand.AddressSize       			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode		 			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
  sCommand.AlternateBytes					= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize 		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
	
  sCommand.DataMode          			= HAL_OSPI_DATA_1_LINE;
	sCommand.NbData 								= 1;
	sCommand.DataDtrMode						=	HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles       			= 0;
  sCommand.DQSMode       					= HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode         			= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	if (HAL_OSPI_Transmit(local_hospi, &regVal, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
	AutoPolling(local_hospi);
}
void ClearSafetyFlag(OSPI_HandleTypeDef *local_hospi)
{
  OSPI_RegularCmdTypeDef     sCommand;
	sCommand.OperationType					= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId								= HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction       			= CMD_CLEAR_SAFETY_REG;
  sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize	 			= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
  sCommand.Address       					= 0;
  sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;
  sCommand.AddressSize       			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode		 			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
  sCommand.AlternateBytes					= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize 		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
  sCommand.DataMode          			= HAL_OSPI_DATA_NONE;
	sCommand.NbData 								= 0;
	sCommand.DataDtrMode						=	HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles       			= 0;
  sCommand.DQSMode       					= HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode         			= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void Read_SFDP(OSPI_HandleTypeDef *local_hospi,uint32_t add,uint32_t nb_data,uint8_t *pData)
{
  OSPI_RegularCmdTypeDef     sCommand;
	sCommand.OperationType					= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId								= HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction       			= CMD_READ_SFDP;
  sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize	 			= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
  sCommand.Address       					= add;
  sCommand.AddressMode       			= HAL_OSPI_ADDRESS_1_LINE;
  sCommand.AddressSize       			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode		 			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
  sCommand.AlternateBytes					= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize 		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
  sCommand.DataMode          			= HAL_OSPI_DATA_1_LINE;
	sCommand.NbData 								= nb_data;
	sCommand.DataDtrMode						=	HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles       			= 8;
  sCommand.DQSMode       					= HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode         			= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_OSPI_Receive(local_hospi, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void Reset_Enable(OSPI_HandleTypeDef *local_hospi)
{
  OSPI_RegularCmdTypeDef     sCommand;
	sCommand.OperationType					= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId								= HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction       			= CMD_ENABLE_RESET;
  sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize	 			= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
  sCommand.Address       					= 0;
  sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;
  sCommand.AddressSize       			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode		 			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
  sCommand.AlternateBytes					= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize 		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
  sCommand.DataMode          			= HAL_OSPI_DATA_NONE;
	sCommand.NbData 								= 0;
	sCommand.DataDtrMode						=	HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles       			= 0;
  sCommand.DQSMode       					= HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode         			= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
void Soft_Reset(OSPI_HandleTypeDef *local_hospi)
{
  OSPI_RegularCmdTypeDef     sCommand;
	sCommand.OperationType					= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId								= HAL_OSPI_FLASH_ID_1;
  sCommand.Instruction       			= CMD_SOFT_RESET;
  sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize	 			= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	
  sCommand.Address       					= 0;
  sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;
  sCommand.AddressSize       			= HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode		 			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	
  sCommand.AlternateBytes					= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.AlternateBytesSize 		= HAL_OSPI_ALTERNATE_BYTES_8_BITS;
	sCommand.AlternateBytesDtrMode 	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
  sCommand.DataMode          			= HAL_OSPI_DATA_NONE;
	sCommand.NbData 								= 0;
	sCommand.DataDtrMode						=	HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DummyCycles       			= 0;
  sCommand.DQSMode       					= HAL_OSPI_DQS_DISABLE;
  sCommand.SIOOMode         			= HAL_OSPI_SIOO_INST_EVERY_CMD;

  if (HAL_OSPI_Command(local_hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
}
