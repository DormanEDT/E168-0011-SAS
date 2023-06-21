/**********************************************************************************/
/*                                                                                */
/*                     CONFIDENTIAL DORMAN PRODUCTS                               */
/*                                                                                */
/*      This is an unpublished work of authorship, which contains trade           */
/*      secrets, created in 2022.Dorman Products owns all rights                  */
/*      to this work and intends to maintain it in confidence to preserve         */
/*      its trade secret status.Dorman Products reserves the right,             */
/*      under the copyright laws of the United States or those of any other       */
/*      country that may have jurisdiction, to protect this work as an            */
/*      unpublished work, in the event of an inadvertent or deliberate            */
/*      unauthorized publication.Dorman Products also reserves its                */
/*      rights under all copyright laws to protect this work as a published       */
/*      work, when appropriate.  Those having access to this work may not         */
/*      copy it, use it, modify it, or disclose the information contained         */
/*      in it without the written authorization of Dorman Products.               */
/*                                                                                */
/**********************************************************************************/
/**********************************************************************************/
/*                                                                                */
/*     FILE NAME   :  lptmr.c                                                     */
/*                                                                                */
/*     DESCRIPTION :  This is the code file for lowpass timer routine.            */
/*                                                                                */
/* +--------------+----------+---------------------------------------------------+*/
/* |Rev. HISTORY  |  DATE    |           MODIFIED POINT                          |*/
/* +--------------+----------+---------------------------------------------------+*/
/* |    1.1       |22-JULY-22| Initial Version                                   |*/
/* +--------------+----------+---------------------------------------------------+*/
/**********************************************************************************/

/**********************************************************************************/
/*Modifications                                                                   */
/**********************************************************************************/

/****************************Header File Inclusion*********************************/

#include "lptmr.h"

/**********************************************************************************/

/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :LPTMR_Init                                    */
/*                                                                 */
/*   FUNCTION BRIEF :LPTMR initialization    	                   */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :void                                          */
/*                                                                 */
/*                                                                 */
/*   RETURN VALUES  :void                                          */
/*                                                                 */
/*******************************************************************/
void LPTMR_Init(void)
{
 PCC->PCCn[PCC_LPTMR0_INDEX] = PCC_PCCn_CGC_MASK;	/* Enable LPTMR Clock 		*/

 LPTMR0->PSR |= LPTMR_PSR_PCS(0b01)                 /* LPTMR clk src: lpo 1khz  */
                |LPTMR_PSR_PBYP_MASK;
}
/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :LPTMR_Enable                                  */
/*                                                                 */
/*   FUNCTION BRIEF :LPTMR enable(350ms overflow interrupt)    	   */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :void                                          */
/*                                                                 */
/*                                                                 */
/*   RETURN VALUES  :void                                          */
/*                                                                 */
/*******************************************************************/
void LPTMR_Enable(uint32_t delay)
{
  LPTMR0->CMR =delay;
  LPTMR0->CSR |= LPTMR_CSR_TIE_MASK; 				 /* Timer interrupt enabled */
  LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;                 /* Enable Timer 			*/

}
/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :LPTMR_Disable                                 */
/*                                                                 */
/*   FUNCTION BRIEF :LPTMR Disable   	                           */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :void                                          */
/*                                                                 */
/*                                                                 */
/*   RETURN VALUES  :void                                          */
/*                                                                 */
/*******************************************************************/
void LPTMR_Disable(void)
{
 LPTMR0->CSR =LPTMR_CSR_TIE(0);					/* Timer interrupt disabled */
 LPTMR0->CSR = LPTMR_CSR_TEN(0);                /* Disable Timer 			*/
}
/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :LPTMR0_IRQHandler                             */
/*                                                                 */
/*   FUNCTION BRIEF :LPTMR Interrupt handler  	                   */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :void                                          */
/*                                                                 */
/*                                                                 */
/*   RETURN VALUES  :void                                          */
/*                                                                 */
/*******************************************************************/
void LPTMR0_IRQHandler (void)
{

	 /* Check if TCF flag is set */
	 LPTMR0->CSR |= LPTMR_CSR_TCF_MASK;	/*	Clear TCF flag by writting a logic one */
	 PIN_Toggle(PTD,0);


}

