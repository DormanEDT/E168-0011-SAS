
/**********************************************************************************/
/*Modifications                                                                   */
/**********************************************************************************/

/*****************Define to prevent recursive inclusion****************************/

#ifndef FLASH_H_
#define FLASH_H_

/**********************************************************************************/

/****************************Header File Inclusion*********************************/

#include "main.h"

/**********************************************************************************/

/********************************Macro Definitions ********************************/

#define EERAMBASE					0x14000000U
#define FLASHBASE					0x0U
#define FLASHSIZE					0x20000U
#define DFLASHBASE					0x10000000U
#define EERAMSIZE					0x800U
#define EEPROM_DATA					0x14000100U
#define BYTE_4                      4
/**********************************************************************************/
/****************************Extern Variable Declarations**************************/

extern uint8_t WritingDFlash;
extern uint32_t start_address;  //EEPROM start address
/**********************************************************************************/

/*******************************Function Declarations******************************/

void FLASH_DRV_EWrite(uint32_t dest, uint32_t size, const uint8_t * pData);
void FLASH_DRV_EEERead(uint32_t dest, uint16_t DataSize, uint8_t * pDataArray);
void Flash_ProgramPartitioning(void);
void Flash_SetFlexRAM(void);
void Flash_Init(void);
uint8_t Write_DFlash32(uint8_t *,uint8_t);
/**********************************************************************************/

#endif /* FLASH_H_ */
