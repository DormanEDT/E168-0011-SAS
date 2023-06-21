
/**********************************************************************************/
/*Modifications                                                                   */
/**********************************************************************************/
/****************************Header File Inclusion*********************************/

#include "Flash.h"

/**********************************************************************************/

/************************Variable Declarations ************************************/
uint8_t loop = 0U;

uint32_t start_address=EERAMBASE;  //EEPROM start address
uint32_t temp_addr=0;

/**********************************************************************************/

/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :FLASH_DRV_EWrite                              */
/*                                                                 */
/*   FUNCTION BRIEF :Write to EEPROM  					           */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :uint32_t(Destination address of EEPROM),      */
/*                   uint32_t(Number of data to write) ,           */
/*                   uint8_t*(Array address of data to be write)   */
/*   RETURN VALUES  :void                                          */
/*                                                                 */
/*******************************************************************/
void FLASH_DRV_EWrite(uint32_t dest, uint32_t size, const uint8_t * pData)
{

    if (FTFC->FSTAT & FTFC_FSTAT_CCIF_MASK)
    {
	 while(size > 0U)
     {
      if((0U == (dest & 3U)) && (size >= 4U))
	  {
	   loop = 4U;
	  }
	  else if((0U == (dest & 1U)) && (size >= 2U))
	  {
	   loop = 2U;
	  }
	  else
	  {
	   loop = 1U;
	  }
	  if(0x04U == loop)
	  {
		 temp_addr =  (uint32_t)(pData[3]) << 24U;
		 temp_addr |= (uint32_t)(pData[2]) << 16U;
		 temp_addr |= (uint32_t)(pData[1]) << 8U;
		 temp_addr |= (uint32_t)(pData[0]);
		 *(volatile uint32_t *)dest = temp_addr;
	  }
	  else
	  {
		  //Do nothing
	  }
	  if(0x01U == loop)
	  {
	   *(uint8_t *)dest = *pData;
	  }
	  else
	  {
		  //Do nothing
	  }
	  if(0x02U == loop)
	  {
		temp_addr = (uint32_t)(pData[1]) << 8U;
		temp_addr |= (uint32_t)(pData[0]);
		*(volatile uint16_t *)dest = (uint16_t)temp_addr;
	  }
	  else
	  {
		  //Do nothing
	  }
      while(0U == (FTFC->FCNFG & FTFC_FCNFG_EEERDY_MASK))
	  {
	   /* Check for protection violation error */
	   if((FTFC->FSTAT & (FTFC_FSTAT_MGSTAT0_MASK | FTFC_FSTAT_FPVIOL_MASK |
		   FTFC_FSTAT_ACCERR_MASK | FTFC_FSTAT_RDCOLERR_MASK)) != 0U)
	   {
		break;
	   }
      }
			/* Update destination address for next iteration */
			dest += loop;
			/* Update size for next iteration */
			size -= loop;
			/* Update data for next iteration */
			pData += loop;
      }
    }
}
/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :FLASH_DRV_EEERead                             */
/*                                                                 */
/*   FUNCTION BRIEF :Read  EEPROM  					               */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :uint32_t(Destination address of EEPROM),      */
/*                   uint32_t(Number of data to Read) ,            */
/*                   uint8_t*(Array address of data to be read)    */
/*   RETURN VALUES  :void                                          */
/*                                                                 */
/*******************************************************************/
void FLASH_DRV_EEERead(uint32_t dest, uint16_t DataSize, uint8_t * pDataArray)
{

  while(DataSize > 0U)
	   {
		*pDataArray = *(volatile uint8_t *)dest;

		/* Update destination address for next iteration */
		dest += 1;
		/* Update size for next iteration */
		DataSize -= 1;
		/* Update data for next iteration */
		pDataArray += 1;

	   }

}
/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :Flash_ProgramPartitioning                     */
/*                                                                 */
/*   FUNCTION BRIEF :Partition the EEPROM   					   */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :void                                          */
/*                                                                 */
/*                                                                 */
/*   RETURN VALUES  :void                                          */
/*                                                                 */
/*******************************************************************/
void Flash_ProgramPartitioning(void)
{

    /* Check CCIF to verify the previous command is completed */
	if (0U == (FTFC->FSTAT & FTFC_FSTAT_CCIF_MASK))
    {
        ;//ret = STATUS_BUSY;
    }
    else
    {
		/* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1 to clear */
		FTFC->FSTAT = (uint8_t)(FTFC_FSTAT_FPVIOL_MASK | FTFC_FSTAT_ACCERR_MASK | FTFC_FSTAT_RDCOLERR_MASK);
		FTFC->FCNFG = 0x01;
		FTFC->FCCOB [3] = 0x80;
		FTFC->FCCOB [2] = 0;
		FTFC->FCCOB [1] = 0;
		FTFC->FCCOB [0] = 0;
		FTFC->FCCOB[7] = 0x03;
		FTFC->FCCOB[6] = 0x03;

		/* Clear CCIF to launch command */
		FTFC->FSTAT |= FTFC_FSTAT_CCIF_MASK;

		while (0U == (FTFC->FSTAT & FTFC_FSTAT_CCIF_MASK))
		{
			;
		}
    }
}
/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :Flash_SetFlexRAM                              */
/*                                                                 */
/*   FUNCTION BRIEF :SetflexRAM    					               */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :void                                          */
/*                                                                 */
/*                                                                 */
/*   RETURN VALUES  :void                                          */
/*                                                                 */
/*******************************************************************/
void Flash_SetFlexRAM(void)
{

    /* Check CCIF to verify the previous command is completed */
	if (0U == (FTFC->FSTAT & FTFC_FSTAT_CCIF_MASK))
    {
        ;//ret = STATUS_BUSY;
    }
    else
    {
    	/* Clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1 to clear */
		FTFC->FSTAT = (uint8_t)(FTFC_FSTAT_FPVIOL_MASK | FTFC_FSTAT_ACCERR_MASK | FTFC_FSTAT_RDCOLERR_MASK);

		FTFC-> FCCOB [3] = 0x81; /* FCCOB0: Selects the SETRAM command */
		FTFC-> FCCOB [2] = 0x00; /* FCCOB1: Make FlexRAM available for emulated EEPROM */
		FTFC->FSTAT |= FTFC_FSTAT_CCIF_MASK;

		while (0U == (FTFC->FSTAT & FTFC_FSTAT_CCIF_MASK))
		{
			;
		}
    }
}
/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :Flash_Init                                    */
/*                                                                 */
/*   FUNCTION BRIEF :Initializing the EEPROM    				   */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :void                                          */
/*                                                                 */
/*                                                                 */
/*   RETURN VALUES  :void                                          */
/*                                                                 */
/*******************************************************************/
void Flash_Init(void)
{
    Flash_ProgramPartitioning();
    Flash_SetFlexRAM();
}
/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :Write_DFlash32                                */
/*                                                                 */
/*   FUNCTION BRIEF :Writing 4 byte to EEPROM 				       */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :uint8_t                                       */
/*                                                                 */
/*                                                                 */
/*   RETURN VALUES  :uint8_t*                                      */
/*                                                                 */
/*******************************************************************/
uint8_t Write_DFlash32(uint8_t *Data,uint8_t size)
{
	uint8_t RData[4]={0},check_space=1;
	if(WritingDFlash)
	{
		return 1;
	}
	else
	{
	WritingDFlash = 1;
	while(check_space)
	{
	FLASH_DRV_EEERead(start_address,4,RData);
	if(RData[0]==0xFF)
	{
		FLASH_DRV_EWrite(start_address,size,Data);
		start_address +=4;
		check_space=0;
	}
	else
	{
	start_address +=size;
	}
	WritingDFlash=0;
	}
	return 0;
	}
}
