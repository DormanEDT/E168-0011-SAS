/**********************************************************************************/
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
/*     FILE NAME   :  PWRMode_Driver.c                                            */
/*                                                                                */
/*     DESCRIPTION :  This is the code file for GPIO routine.                     */
/*                                                                                */
/* +--------------+---------+----------------------------------------------------+*/
/* |Rev. HISTORY  |  DATE   |           MODIFIED POINT                           |*/
/* +--------------+---------+----------------------------------------------------+*/
/* |    0.1       |08-AUG-22|          Initial Version                           |*/
/* +--------------+---------+----------------------------------------------------+*/
/**********************************************************************************/

/**********************************************************************************/
/*Modifications                                                                   */
/**********************************************************************************/
/**********************************************************************************/
/*Header File Inclusion                                                           */
/**********************************************************************************/
#include "PWRMode_Driver.h"

/**********************************************************************************/
/*Constant and Variable Declaration                                               */
/**********************************************************************************/

/* Variable Declarations **********************************************/

/* Variable Declarations **********************************************/

/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :   Halt                                       */
/*                                                                 */
/*   FUNCTION BRIEF :   wait for interrupt						   */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :   None                                       */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/
void Halt(void)
{
	 asm("WFI");

}


/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :   SIRC_Init                                  */
/*                                                                 */
/*   FUNCTION BRIEF : SIRC 8MHz, SIRCDIV1 1MHz, SIRCDIV2 1MHz      */
/*                    SIRC disabled in VLPS	                       */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :   None                                       */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/

void SIRC_Init(void)
{
    SCG->SIRCCSR &= ~ (1 << 23);
    // [23] LK = 0 Unlock Control Status Register

    SCG->SIRCCSR |= 0x01;
    // [2] SIRCLPEN = 0 Slow IRC is disabled in VLP modes
    // [1] SIRCSTEN = 0 Slow IRC is disabled in Stop modes
    // [0] SIRCEN = 1 Slow IRC is enabled

    SCG->SIRCDIV |= 0x0404;
    // [10-8] SIRCDIV2 0b100 Divide by 8 (1MHz)
    // [2-0]  SIRCDIV1 0b100 Divide by 8 (1MHz)

    while((SCG->SIRCCSR & (1 << 24)) == 0); // wait until clock is valid
    // [24] SIRCVLD = 1 Slow IRC is enabled and output clock is valid

    SCG->SIRCCSR |= (1 << 23);
    // [23] LK = 1 lock Control Status Register
}

/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :  VLPR_Init                                   */
/*                                                                 */
/*   FUNCTION BRIEF : Max frequencies in VLPR: Core/System 4MHz,
 *                    Bus: 2MHz, Flash: 1MHz
                      See e11063 mask 0N57U                        */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :   None                                       */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/



void VLPR_Init(void)
{
    SCG->VCCR = 0x02010013;
    // [27-24] SCS = 2     Slow IRC (SIRC_CLK 8MHZ)
    // [19-16] DIVCORE = 1 Divide by 2  (4 MHz)
    // [7-4] DIVBUS = 1    Divide core by 2  (2 MHz)
    // [3-1] DIVSLOW = 3   Divide core by 4  (1 MHz)
}

/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :   SIRC_RUNState                              */
/*                                                                 */
/*   FUNCTION BRIEF :   switch to SIRC in RUN                      */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :   None                                       */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/


void SIRC_RUNState(void)
{
    uint32_t srie = RCM->SRIE;
    RCM->SRIE = 0x0000; // configure all reset sources to be ‘Reset' (not as Interrupt)
    RCM->SRIE = 0xFFFF; // Program each reset source as Interrupt via RCM_SRIE
    // for a minimum delay time of 10 LPO.

    SCG->RCCR = 0x02010013;
    // [27-24] SCS = 2  Slow IRC (SIRC_CLK 8MHZ)
    // [19-16] DIVCORE = 1 Divide by 2  (4 MHz)
    // [7-4] DIVBUS = 1     Divide core by 2  (2 MHz)
    // [3-1] DIVSLOW = 3   Divide core by 4  (1 MHz)

    while(!((SCG->CSR & (0x0F000000)) & 0x02000000));
    // [27-24] SCS = 0b0010 Slow IRC (SIRC_CLK)
    // or
    while((SCG->SIRCCSR & (1 << 25)) == 0);
    // [25] SIRCCSR = 1 Until SIRC is the system clock source

    RCM->SRIE = srie;
}

/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :   FIRC_Disable                               */
/*                                                                 */
/*   FUNCTION BRIEF :   Disable FIRC in RUN Mode                   */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :   None                                       */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/

void FIRC_Disable(void)
{
	// When entering VLPR/VLPS mode, the system clock should be SIRC. The FIRC, SOSC,
	// and SPLL must be disabled by software in RUN mode before making any mode
	// transition.


	if(!(SCG->FIRCCSR & (1 << 25)))
	{ // [25] FIRCSEL, if FIRC is not the system clock source
	SCG->FIRCCSR &= ~(1 << 0);
	// [0] FIRCEN = 0 FIRC disabled
	while(SCG->FIRCCSR & (1 << 24));
	// [24] FIRCVLD = 0 Fast IRC is not enabled or clock is not valid
	}

	if(!(SCG->SOSCCSR & (1 << 25)))
	{
	SCG->SOSCCSR &= ~(1 << 0);
	while(SCG->SOSCCSR & (1 << 24));
	}
}
/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :   Set_Power_Mode                               */
/*                                                                 */
/*   FUNCTION BRIEF :                                              */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :   None                                       */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/
void Set_Power_Mode(Powermode_Typedef powermode)
{
	switch(powermode)
	{
	case RUN:
		/*! Fast IRC is enabled and trimmed to 48 MHz in reset (default).
		 * Set FIRCDIV1 & FIRCDIV2 = 1: divide by 1 = 48 MHz.
		 * */
		SCG->FIRCDIV = SCG_FIRCDIV_FIRCDIV1(1)|
				   SCG_FIRCDIV_FIRCDIV2(1);

		/*! Fast IRC is enabled in high range (8MHz) in reset (default).
		 * Set FIRCDIV1 & FIRCDIV2 = 1: divide by 1 = 8 MHz.
		 * */
		SCG->SIRCDIV = SCG_SIRCDIV_SIRCDIV1(1)|
				   	   SCG_SIRCDIV_SIRCDIV2(1);

		/*!
		 *  Select FIRC for RUN mode.
		 *  ===========================
		 */
		SCG->RCCR=SCG_RCCR_SCS(3)      /* Select FIRC as clock source 								*/
    		|SCG_RCCR_DIVCORE(0b00)      /* DIVCORE=0, div. by 1: Core clock = 48 MHz 					*/
			|SCG_RCCR_DIVBUS(0b00)       /* DIVBUS=0, div. by 1: bus clock = 48 MHz 					*/
			|SCG_RCCR_DIVSLOW(0b01);     /* DIVSLOW=1, div. by 2: SCG slow, flash clock= 24 MHz			*/

		while (((SCG->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT ) != 3) {}	/* Wait for sys clk src = SPLL */
		break;
	case VLPR:
		 	 SIRC_Init();
		 	 SIRC_RUNState();
		 	 FIRC_Disable();
		 	 VLPR_Init();

		 	 SMC->PMPROT |= (1 << 5);
		 	 // [5] AVLP = 1 VLPS allowed

		 	 PMC->REGSC |= (1 << 0);
		 	 // [0] BIASEN = 1 Biasing enabled

		 	 if(SMC->PMSTAT == 0x01) // [7-0] PMSTAT: 0x01 from RUN
		 	 {
		 		 SMC->PMCTRL |= 0x00000040; // enter VLPR
		        // [6-5] RUNM = 0b10 Very-Low-Power Run mode (VLPR)

		 		 while(!((SMC->PMSTAT & 0xFF) & 0x4))
		 		 {   // [7:0] PMSTAT = 0x04 VLPR
		            // wait until the current power mode is VLPR
		 		 }
		 	 }
	case STOP1:
		 /* Enable SLEEPDEEP bit in the Core
			  * (Allow deep sleep modes) */
			  S32_SCB ->SCR|=S32_SCB_SCR_SLEEPDEEP(1);
			  /* Select Stop Mode */
			  SMC->PMCTRL=SMC_PMCTRL_STOPM(0b00);
			  /* Select which STOP mode (Stop1 or Stop2)
			  * is desired (Stop1 - 0b01, Stop2 - 0b10) */
			  SMC->STOPCTRL=SMC_STOPCTRL_STOPO(0b01);
			  /* Check if current mode is RUN mode */
			  if(SMC->PMSTAT == 0x01)
			  {
			  /* Go to deep sleep mode */
				  Halt();
			  }
			  break;
	case STOP2:
		/* Enable SLEEPDEEP bit in the Core
		 * (Allow deep sleep modes) */
		S32_SCB ->SCR|=S32_SCB_SCR_SLEEPDEEP(1);
		/* Select Stop Mode */
		SMC->PMCTRL=SMC_PMCTRL_STOPM(0b00);
		/* Select which STOP mode (Stop1 or Stop2)
		 * is desired (Stop1 - 0b01, Stop2 - 0b10) */
		SMC->STOPCTRL=SMC_STOPCTRL_STOPO(0b10);
		/* Check if current mode is RUN mode */
		if(SMC->PMSTAT == 0x01)
		{
			/* Go to deep sleep mode */
			Halt();
		}
		break;
	case VLPS:
		 /* Adjust SCG settings to meet maximum
		frequencies value */
		if(SMC->PMSTAT == 0x01 )
		{
			FIRC_Disable();
		}
		 /* Enable SLEEPDEEP bit in the Core
		 * (Allow deep sleep modes) */
		 S32_SCB ->SCR|=S32_SCB_SCR_SLEEPDEEP(1);
		 /* Allow very low power run mode */
		 SMC->PMPROT |= SMC_PMPROT_AVLP_MASK;
		 /* Select VLPS Mode */
		 SMC->PMCTRL=SMC_PMCTRL_STOPM(0b10);
		 PMC->REGSC |= PMC_REGSC_BIASEN(1)|PMC_REGSC_CLKBIASDIS(1);
		 /* Check if current mode is RUN or VLPR mode  */
		 if((SMC->PMSTAT == 0x01 ) || (SMC->PMSTAT == 0x04))
		 {
			 Halt();
		 }
		 break;
	default:
		break;
	}

}

/*******************************************************************/
/*                                                                 */
/*                                                                 */
/*   FUNCTION BRIEF :  clkout_bus                                  */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :   None                                       */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/

void Clkout_Bus(void)
{
    SIM->CHIPCTL &= ~(1 << 11); //Disable CLKOUT
    SIM->CHIPCTL |= (9 << 4);   //BUS_CLK
    SIM->CHIPCTL |= (1 << 11);  //Enable CLKOUT
    // [11] CLKOUTEN = 1 CLKOUT enable
    // [7-4] CLKOUTSEL = 0b1001 BUS_CLK
}

/*****************************************************************************
 *
 *	End of file PWRMode_Driver.c
 *
 *****************************************************************************/

