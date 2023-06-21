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
/*     FILE NAME   :  clk.c                                                      */
/*                                                                                */
/*     DESCRIPTION :  This is the code file for clk routine.                      */
/*                                                                                */
/* +--------------+---------+---------------------------------------------------+ */
/* |Rev. HISTORY  |  DATE   |           MODIFIED POINT                          | */
/* +--------------+---------+---------------------------------------------------+ */
/* |    0.1       |28-MAR-22| Initial Version                                   | */
/* +--------------+---------+---------------------------------------------------+ */
/**********************************************************************************/

/**********************************************************************************/
/*Modifications                                                                   */
/**********************************************************************************/

/**********************************************************************************/
/*Header File Inclusion                                                           */
/**********************************************************************************/

#include "clk.h"
/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :   SOSC_init_40MHz                             */
/*                                                                 */
/*   FUNCTION BRIEF :   									       */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :   None                                       */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/
void SOSC_init_40MHz(void) {
	/*! SOSC Initialization (40 MHz):
	 * =============================
	 * */
	SCG->SOSCDIV = SCG_SOSCDIV_SOSCDIV1(1) | SCG_SOSCDIV_SOSCDIV2(1); /* SOSCDIV1 & SOSCDIV2 =1: divide by 1 		*/
	SCG->SOSCCFG = SCG_SOSCCFG_RANGE(3)|SCG_SOSCCFG_EREFS(1); /* Range=3: High frequency range selected	*/

    //	    SCG->SOSCCFG  =	SCG_SOSCCFG_RANGE(3)|		/* Range=3: High frequency range selected	*/
	//		SCG_SOSCCFG_EREFS_MASK;		/* HGO=0:   Config xtal osc for low power 	*/
	//  	  	  	  	  	  	  	  	  	/* EREFS=1: Input is external XTAL 			*/

	while (SCG->SOSCCSR & SCG_SOSCCSR_LK_MASK)
		; /* Ensure SOSCCSR unlocked 							*/
	SCG->SOSCCSR = SCG_SOSCCSR_SOSCEN_MASK; /* LK=0:          SOSCCSR can be written 				*/
	/* SOSCCMRE=0:    OSC CLK monitor IRQ if enabled 		*/
	/* SOSCCM=0:      OSC CLK monitor disabled 			*/
	/* SOSCERCLKEN=0: Sys OSC 3V ERCLK output clk disabled */
	/* SOSCLPEN=0:    Sys OSC disabled in VLP modes 		*/
	/* SOSCSTEN=0:    Sys OSC disabled in Stop modes 		*/
	/* SOSCEN=1:      Enable oscillator 					*/

	while (!(SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK))
		; /* Wait for sys OSC clk valid */
}

/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :   RUN_mode_48MHz                             */
/*                                                                 */
/*   FUNCTION BRIEF :   									       */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :   None                                       */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/
void RUN_mode_48MHz(void) {
	/*! Fast IRC is enabled and trimmed to 48 MHz in reset (default).
	 * Set FIRCDIV1 & FIRCDIV2 = 1: divide by 1 = 48 MHz.
	 * */
	SCG->FIRCDIV = SCG_FIRCDIV_FIRCDIV1(1) | SCG_FIRCDIV_FIRCDIV2(1);

	/*! Fast IRC is enabled in high range (8MHz) in reset (default).
	 * Set FIRCDIV1 & FIRCDIV2 = 1: divide by 1 = 8 MHz.
	 * */
	SCG->SIRCDIV = SCG_SIRCDIV_SIRCDIV1(1) | SCG_SIRCDIV_SIRCDIV2(1);

	/*!
	 *  Select FIRC for RUN mode.
	 *  ===========================
	 */
	SCG->RCCR = SCG_RCCR_SCS(3) /* Select FIRC as clock source 								*/
	| SCG_RCCR_DIVCORE(0b00) /* DIVCORE=0, div. by 1: Core clock = 48 MHz 					*/
	| SCG_RCCR_DIVBUS(0b00) /* DIVBUS=0, div. by 1: bus clock = 48 MHz 					*/
	| SCG_RCCR_DIVSLOW(0b01); /* DIVSLOW=1, div. by 2: SCG slow, flash clock= 24 MHz			*/

	while (((SCG->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT) != 3) {
	} /* Wait for sys clk src = SPLL */
}
