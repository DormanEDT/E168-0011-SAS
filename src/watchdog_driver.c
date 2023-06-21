//**********************************************************************************/
/*                                                                                */
/*                     CONFIDENTIAL DORMAN PRODUCTS                               */
/*                                                                                */
/*      This is an unpublished work of authorship, which contains trade           */
/*      secrets, created in 2022.  Dorman Products owns all rights                */
/*      to this work and intends to maintain it in confidence to preserve         */
/*      its trade secret status.  Dorman Products reserves the right,             */
/*      under the copyright laws of the United States or those of any other       */
/*      country that may have jurisdiction, to protect this work as an            */
/*      unpublished work, in the event of an inadvertent or deliberate            */
/*      unauthorized publication.  Dorman Products also reserves its              */
/*      rights under all copyright laws to protect this work as a published       */
/*      work, when appropriate.  Those having access to this work may not         */
/*      copy it, use it, modify it, or disclose the information contained         */
/*      in it without the written authorization of Dorman Products.               */
/*                                                                                */
/**********************************************************************************/
/**********************************************************************************/
/*                                                                                */
/*     FILE NAME   :  watchdog_driver.c                                           */
/*                                                                                */
/*     DESCRIPTION :  This is the code file for watchdog_driver routine.          */
/*                                                                                */
/* +--------------+---------+---------------------------------------------------+ */
/* |Rev. HISTORY  |  DATE   |           MODIFIED POINT                          | */
/* +--------------+---------+---------------------------------------------------+ */
/* |    0.1       |13-OCT-22| 			Initial Version                         | */
/* +--------------+---------+---------------------------------------------------+ */
/**********************************************************************************/

/**********************************************************************************/
/*Modifications                                                                   */
/**********************************************************************************/

/**********************************************************************************/
/*Header File Inclusion                                                           */
/**********************************************************************************/
#include <watchdog_driver.h>

/**********************************************************************************/
/*Constant and Variable Declaration                                               */
/**********************************************************************************/

/* Variable Declarations **********************************************/

uint32_t Freq=0;
double Temp = 0;
uint32_t Tot_Count=0;

/* Variable Declarations **********************************************/
/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :   WDOG_init                                  */
/*                                                                 */
/*   FUNCTION BRIEF :   Time taken to reset						   */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :  uint32                                      */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/
void WDOG_init(uint32_t time)
{
	Freq=128000;
	Temp=time * 0.001;
	Tot_Count=(uint32_t)(Freq * Temp);
	WDOG_enable(Tot_Count);
}
/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :   WDOG_refresh                               */
/*                                                                 */
/*   FUNCTION BRIEF :   refreshing the count value				   */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :  None                                        */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/
void WDOG_refresh(void)
{
	WDOG->CNT = REFRESH_WATCHDOG;
}
/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :   WDOG_enable                                */
/*                                                                 */
/*   FUNCTION BRIEF :   Enable WDOG				                   */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :   None                                       */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/
void WDOG_enable(uint32_t Tot_Count)
{
	WDOG->CNT = UNLOCK_WATCHDOG; //unlock watchdog


	while(((WDOG->CS &  WDOG_CS_ULK_MASK)>>WDOG_CS_ULK_SHIFT)==0);////wait until registers are unlocked

	WDOG->CS = WDOG_CS_EN(1)| WDOG_CS_CLK(1) |  WDOG_CS_INT(0) |
			 WDOG_CS_WIN(0) | WDOG_CS_UPDATE(1)|WDOG_CS_CMD32EN(1);
	WDOG->TOVAL = (uint32_t)Tot_Count;
	while(WDOG->CS == WDOG_CS_RCS(0));
}
/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :   WDOG_disable                               */
/*                                                                 */
/*   FUNCTION BRIEF :   Disable WDOG				               */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :   None                                       */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/

void WDOG_disable(void)
{
	WDOG->CNT = 0xD928C520; //unlock watchdog
	WDOG->CS &= ~WDOG_CS_EN_MASK; // disable watchdog
	WDOG->TOVAL= 0xFFFF;
	while(WDOG->CS == WDOG_CS_ULK(0));   // waiting for lock
}













