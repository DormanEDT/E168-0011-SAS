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
/*     FILE NAME   :  Clock_Driver.c                                              */
/*                                                                                */
/*     DESCRIPTION :  Clock driver .                                              */
/*                                                                                */
/* +--------------+---------+---------------------------------------------------+ */
/* |Rev. HISTORY  |  DATE   |           MODIFIED POINT                          | */
/* +--------------+---------+---------------------------------------------------+ */
/* |    0.1       |15-DEC-22| Initial Version                                   | */
/* +--------------+---------+---------------------------------------------------+ */
/**********************************************************************************/

/**********************************************************************************/
/*Modifications                                                                   */
/**********************************************************************************/

/**********************************************************************************/
/*Header File Inclusion                                                           */
/**********************************************************************************/
#include <Clock_Driver.h>
/**********************************************************************************/
/*Constant and Variable Declaration                                               */
/**********************************************************************************/

/* Variable Declarations **********************************************/
frequency_struct retfreq=
		{
			.busfreq=0,
			.corefreq=0,
			.fircfreq=0,
			.sircfreq=0,
			.slowfreq=0,
			.soscfreq=0
		};
/* Variable Declarations **********************************************/


/*FUNCTION*********************************************************************
 *
 * Function Name : SIM_GetLpo32KStatus
 * Description   : Get SIM LPO 32KHz clock gating status (LPO_32K_CLOCK).
 *END*************************************************************************/
bool SIM_GetLpo32KStatus(const SIM_Type * base)
{
    return (((base->LPOCLKS & SIM_LPOCLKS_LPO32KCLKEN_MASK) >> SIM_LPOCLKS_LPO32KCLKEN_SHIFT) != 0U) ? true : false;
}

/*FUNCTION*********************************************************************
 *
 * Function Name : SIM_GetLpo1KStatus
 * Description   : Get SIM LPO 1KHz clock gating status (LPO_1K_CLOCK).
 *END*************************************************************************/
bool SIM_GetLpo1KStatus(const SIM_Type * base)
{
    return (((base->LPOCLKS & SIM_LPOCLKS_LPO1KCLKEN_MASK) >> SIM_LPOCLKS_LPO1KCLKEN_SHIFT) != 0U) ? true : false;
}
/*****************************************************************************
 *
 * Function Name			: GetCurrentSystemClockSource
 * Function Description		:Clock initialization
 *
 *
 * Parameters				: SCG_Type*
 * Return value				: uint32_t
 *
 *****************************************************************************/
uint32_t GetCurrentSystemClockSource(const SCG_Type * base)
{
    return ((base->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT);
}
/*****************************************************************************
 *
 * Function Name			: PMC_GetLpoMode
 * Function Description		:LPO status
 *
 *
 * Parameters				: PMC_Type*
 * Return value				: bool
 *
 *****************************************************************************/
bool PMC_GetLpoMode(const PMC_Type * const baseAddr)
{
    uint8_t regValue = baseAddr->REGSC;
    regValue = (uint8_t)((regValue & PMC_REGSC_LPODIS_MASK) >> PMC_REGSC_LPODIS_SHIFT);
    return (regValue == 0U) ? true : false;
}
/*****************************************************************************
 *
 * Function Name			: SIM_GetLpoClkSelectorValue
 * Function Description		:LPO clock slect value
 *
 *
 * Parameters				: SIM_Type*
 * Return value				: uint32_t
 *
 *****************************************************************************/
static inline uint32_t SIM_GetLpoClkSelectorValue(const SIM_Type * base)
{
    return ((base->LPOCLKS & SIM_LPOCLKS_LPOCLKSEL_MASK) >> SIM_LPOCLKS_LPOCLKSEL_SHIFT);
}
/*****************************************************************************
 *
 * Function Name			: Clock_Drv_Init
 * Function Description		:Iteration of clock init
 *
 *
 * Parameters				: uint32_t(Maximum ClockCount is 3),Clock_Init_Struct*
 * Return value				: void
 *
 *****************************************************************************/

void Clock_Drv_Init(Clock_DrvInit_Struct* clk_struct,frequency_struct *freq,system_clocktype sys)
{

	Clock_Init(clk_struct,freq,sys);

}

/*****************************************************************************
 *
 * Function Name			: Clock_Init
 * Function Description		:Clock initialization
 *
 *
 * Parameters				: Clock_Init_Struct*
 * Return value				: void
 *
 *****************************************************************************/

void Clock_Init(Clock_DrvInit_Struct* clk_drvstruct,frequency_struct *freq,system_clocktype sys)
{
uint32_t frequency=0;
uint32_t bus_freq=0;
uint32_t core_freq=0;
uint32_t slow_freq=0;
uint32_t clockdiv=0;
uint32_t corediv=0;
uint32_t busdiv=0;
uint32_t slowdiv=0;

Clock_Init_Struct clk_struct={
		.busdiv=0,
		.clockdiv=0,
		.clockselect=0,
		.corediv=0,
		.osctype=0,
		.range=0,
		.slowdiv=0
};

switch(clk_drvstruct->clockselect)
{
	case FIRC_CLK_SELECT:
		/*! Fast IRC is enabled and trimmed to 48 MHz in reset (default).
			 * Set FIRCDIV1 & FIRCDIV2 = .clockdiv: divide by .clockdiv = 48 MHz/.clockdiv .
			 * */
		frequency=freq->fircfreq;
		bus_freq=freq->busfreq;
		core_freq=freq->corefreq;
		slow_freq=freq->slowfreq;
		clockdiv=(48000000/frequency);
		switch(clockdiv)
		{
			case 1:
				clk_struct.clockdiv=Clock_DIV_BY_1;
				break;
			case 2:
				clk_struct.clockdiv=Clock_DIV_BY_2;
				break;
			case 4:
				clk_struct.clockdiv=Clock_DIV_BY_4;
				break;
			case 8:
				clk_struct.clockdiv=Clock_DIV_BY_8;
				break;
			case 16:
				clk_struct.clockdiv=Clock_DIV_BY_16;
				break;
			case 32:
				clk_struct.clockdiv=Clock_DIV_BY_32;
				break;
			case 64:
				clk_struct.clockdiv=Clock_DIV_BY_64;
				break;
			default:

				break;
		}
		busdiv=(frequency/bus_freq)-1;
		switch(busdiv)
		{
			case 0:
				clk_struct.busdiv=Bus_DIV_BY_1;
				break;
			case 1:
				clk_struct.busdiv=Bus_DIV_BY_2;
				break;
			case 2:
				clk_struct.busdiv=Bus_DIV_BY_3;
				break;
			case 3:
				clk_struct.busdiv=Bus_DIV_BY_4;
				break;
			case 4:
				clk_struct.busdiv=Bus_DIV_BY_5;
				break;
			case 5:
				clk_struct.busdiv=Bus_DIV_BY_6;
				break;
			case 6:
				clk_struct.busdiv=Bus_DIV_BY_7;
				break;
			case 7:
				clk_struct.busdiv=Bus_DIV_BY_8;
				break;
			case 8:
				clk_struct.busdiv=Bus_DIV_BY_9;
				break;
			case 9:
				clk_struct.busdiv=Bus_DIV_BY_10;
				break;
			case 10:
				clk_struct.busdiv=Bus_DIV_BY_11;
				break;
			case 11:
				clk_struct.busdiv=Bus_DIV_BY_12;
				break;
			case 12:
				clk_struct.busdiv=Bus_DIV_BY_13;
				break;
			case 13:
				clk_struct.busdiv=Bus_DIV_BY_14;
				break;
			case 14:
				clk_struct.busdiv=Bus_DIV_BY_15;
				break;
			case 15:
				clk_struct.busdiv=Bus_DIV_BY_16;
				break;
			default:

				break;
		}

		slowdiv=(frequency/(slow_freq*2));
		switch(slowdiv)
		{
			case 0:
				clk_struct.slowdiv=Slow_DIV_BY_1;
				break;
			case 1:
				clk_struct.slowdiv=Slow_DIV_BY_2;
				break;
			case 2:
				clk_struct.slowdiv=Slow_DIV_BY_3;
				break;
			case 3:
				clk_struct.slowdiv=Slow_DIV_BY_4;
				break;
			case 4:
				clk_struct.slowdiv=Slow_DIV_BY_5;
				break;
			case 5:
				clk_struct.slowdiv=Slow_DIV_BY_6;
				break;
			case 6:
				clk_struct.slowdiv=Slow_DIV_BY_7;
				break;
			case 7:
				clk_struct.slowdiv=Slow_DIV_BY_8;
				break;
			default:
				break;
		}
		corediv=(frequency/core_freq)-1;
		switch(corediv)
		{
			case 0:
				clk_struct.corediv=Core_DIV_BY_1;
				break;
			case 1:
				clk_struct.corediv=Core_DIV_BY_2;
				break;
			case 2:
				clk_struct.corediv=Core_DIV_BY_3;
				break;
			case 3:
				clk_struct.corediv=Core_DIV_BY_4;
				break;
			case 4:
				clk_struct.corediv=Core_DIV_BY_5;
				break;
			case 5:
				clk_struct.corediv=Core_DIV_BY_6;
				break;
			case 6:
				clk_struct.corediv=Core_DIV_BY_7;
				break;
			case 7:
				clk_struct.corediv=Core_DIV_BY_8;
				break;
			case 8:
				clk_struct.corediv=Core_DIV_BY_9;
				break;
			case 9:
				clk_struct.corediv=Core_DIV_BY_10;
				break;
			case 10:
				clk_struct.corediv=Core_DIV_BY_11;
				break;
			case 11:
				clk_struct.corediv=Core_DIV_BY_12;
				break;
			case 12:
				clk_struct.corediv=Core_DIV_BY_13;
				break;
			case 13:
				clk_struct.corediv=Core_DIV_BY_14;
				break;
			case 14:
				clk_struct.corediv=Core_DIV_BY_15;
				break;
			case 15:
				clk_struct.corediv=Core_DIV_BY_16;
				break;
			default:

				break;
		}
		clk_struct.range=clk_drvstruct->range;

		SCG->FIRCDIV = SCG_FIRCDIV_FIRCDIV1(clk_struct.clockdiv)|
					   SCG_FIRCDIV_FIRCDIV2(clk_struct.clockdiv);
		if(FIRC_SYS_CLK==sys)
		{
		SCG->RCCR    =SCG_RCCR_SCS(FIRC_CLK_SELECT)                /* Select FIRC as clock source 								*/
		            |SCG_RCCR_DIVCORE(clk_struct.corediv)      /* DIVCORE=corediv, div-> by (->corediv+1): Core clock = 48 MHz/(corediv+1) */
		            |SCG_RCCR_DIVBUS(clk_struct.busdiv)        /* DIVBUS=busdiv, div. by (.busdiv+1): bus clock = 48 MHz/(busdiv+1)	*/
		            |SCG_RCCR_DIVSLOW(clk_struct.slowdiv);     /* DIVSLOW=slowdiv, div. by (.slowdiv+1): SCG slow, flash clock= 48 MHz/(.slowdiv+1)*/
		retfreq.busfreq=bus_freq;
		retfreq.corefreq=core_freq;
		retfreq.slowfreq=slow_freq;
		}

		SCG->FIRCCFG  =	SCG_FIRCCFG_RANGE(clk_struct.range);		/* Range=3: High frequency range selected	*/

		while (((SCG->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT ) != 3)
		{
			/* Wait for sys clk src = SPLL */
		}
		retfreq.fircfreq=frequency;
		break;
	case SIRC_CLK_SELECT:
		/*! Slow IRC is enabled and trimmed to 8 MHz in reset (default).
			 * Set SIRCDIV1 & SIRCDIV2 = .clockdiv: divide by .clockdiv = 8 MHz/.clockdiv .
			 * */
		frequency=freq->sircfreq;
		bus_freq=freq->busfreq;
		core_freq=freq->corefreq;
		slow_freq=freq->slowfreq;
		clockdiv=(8000000/frequency);
		switch(clockdiv)
		{
			case 1:
				clk_struct.clockdiv=Clock_DIV_BY_1;
				break;
			case 2:
				clk_struct.clockdiv=Clock_DIV_BY_2;
				break;
			case 4:
				clk_struct.clockdiv=Clock_DIV_BY_4;
				break;
			case 8:
				clk_struct.clockdiv=Clock_DIV_BY_8;
				break;
			case 16:
				clk_struct.clockdiv=Clock_DIV_BY_16;
				break;
			case 32:
				clk_struct.clockdiv=Clock_DIV_BY_32;
				break;
			case 64:
				clk_struct.clockdiv=Clock_DIV_BY_64;
				break;
			default:

				break;
		}
		busdiv=(frequency/bus_freq)-1;
		switch(busdiv)
		{
			case 0:
				clk_struct.busdiv=Bus_DIV_BY_1;
				break;
			case 1:
				clk_struct.busdiv=Bus_DIV_BY_2;
				break;
			case 2:
				clk_struct.busdiv=Bus_DIV_BY_3;
				break;
			case 3:
				clk_struct.busdiv=Bus_DIV_BY_4;
				break;
			case 4:
				clk_struct.busdiv=Bus_DIV_BY_5;
				break;
			case 5:
				clk_struct.busdiv=Bus_DIV_BY_6;
				break;
			case 6:
				clk_struct.busdiv=Bus_DIV_BY_7;
				break;
			case 7:
				clk_struct.busdiv=Bus_DIV_BY_8;
				break;
			case 8:
				clk_struct.busdiv=Bus_DIV_BY_9;
				break;
			case 9:
				clk_struct.busdiv=Bus_DIV_BY_10;
				break;
			case 10:
				clk_struct.busdiv=Bus_DIV_BY_11;
				break;
			case 11:
				clk_struct.busdiv=Bus_DIV_BY_12;
				break;
			case 12:
				clk_struct.busdiv=Bus_DIV_BY_13;
				break;
			case 13:
				clk_struct.busdiv=Bus_DIV_BY_14;
				break;
			case 14:
				clk_struct.busdiv=Bus_DIV_BY_15;
				break;
			case 15:
				clk_struct.busdiv=Bus_DIV_BY_16;
				break;
			default:

				break;
		}

		slowdiv=(frequency/(slow_freq*2));
		switch(slowdiv)
		{
			case 0:
				clk_struct.slowdiv=Slow_DIV_BY_1;
				break;
			case 1:
				clk_struct.slowdiv=Slow_DIV_BY_2;
				break;
			case 2:
				clk_struct.slowdiv=Slow_DIV_BY_3;
				break;
			case 3:
				clk_struct.slowdiv=Slow_DIV_BY_4;
				break;
			case 4:
				clk_struct.slowdiv=Slow_DIV_BY_5;
				break;
			case 5:
				clk_struct.slowdiv=Slow_DIV_BY_6;
				break;
			case 6:
				clk_struct.slowdiv=Slow_DIV_BY_7;
				break;
			case 7:
				clk_struct.slowdiv=Slow_DIV_BY_8;
				break;
			default:
				break;
		}
		corediv=(frequency/core_freq)-1;
		switch(corediv)
		{
			case 0:
				clk_struct.corediv=Core_DIV_BY_1;
				break;
			case 1:
				clk_struct.corediv=Core_DIV_BY_2;
				break;
			case 2:
				clk_struct.corediv=Core_DIV_BY_3;
				break;
			case 3:
				clk_struct.corediv=Core_DIV_BY_4;
				break;
			case 4:
				clk_struct.corediv=Core_DIV_BY_5;
				break;
			case 5:
				clk_struct.corediv=Core_DIV_BY_6;
				break;
			case 6:
				clk_struct.corediv=Core_DIV_BY_7;
				break;
			case 7:
				clk_struct.corediv=Core_DIV_BY_8;
				break;
			case 8:
				clk_struct.corediv=Core_DIV_BY_9;
				break;
			case 9:
				clk_struct.corediv=Core_DIV_BY_10;
				break;
			case 10:
				clk_struct.corediv=Core_DIV_BY_11;
				break;
			case 11:
				clk_struct.corediv=Core_DIV_BY_12;
				break;
			case 12:
				clk_struct.corediv=Core_DIV_BY_13;
				break;
			case 13:
				clk_struct.corediv=Core_DIV_BY_14;
				break;
			case 14:
				clk_struct.corediv=Core_DIV_BY_15;
				break;
			case 15:
				clk_struct.corediv=Core_DIV_BY_16;
				break;
			default:

				break;
		}
		clk_struct.range=clk_drvstruct->range;
		SCG->SIRCDIV = SCG_SIRCDIV_SIRCDIV1(clk_struct.clockdiv)|
					   SCG_SIRCDIV_SIRCDIV2(clk_struct.clockdiv);
		if(SIRC_SYS_CLK==sys)
		{
		SCG->RCCR    =SCG_RCCR_SCS(SIRC_CLK_SELECT)                /* Select SIRC as clock source 								*/
		            |SCG_RCCR_DIVCORE(clk_struct.corediv)      /* DIVCORE=corediv, div. by (.corediv+1): Core clock = 8 MHz/(corediv+1) */
		            |SCG_RCCR_DIVBUS(clk_struct.busdiv)        /* DIVBUS=busdiv, div. by (.busdiv+1): bus clock = 8 MHz/(busdiv+1)	*/
		            |SCG_RCCR_DIVSLOW(clk_struct.slowdiv);     /* DIVSLOW=slowdiv, div. by (.slowdiv+1): SCG slow, flash clock= 8 MHz/(.slowdiv+1)*/
		retfreq.busfreq=bus_freq;
		retfreq.corefreq=core_freq;
		retfreq.slowfreq=slow_freq;
		}
		SCG->SIRCCFG  =	SCG_SIRCCFG_RANGE(clk_struct.range);		/* Range=3: High frequency range selected	*/

		while (!(SCG->SIRCCSR & SCG_SIRCCSR_SIRCVLD_MASK))
				; /* Wait for sys OSC clk valid */
		retfreq.sircfreq=frequency;
		break;
	case SOSC_CLK_SELECT:
			 /* Set SOSCDIV1 & SOSCDIV2 = .clockdiv: divide by .clockdiv = 4_40 MHz/.clockdiv .
			 * */
		frequency=freq->soscfreq;
		bus_freq=freq->busfreq;
		core_freq=freq->corefreq;
		slow_freq=freq->slowfreq;
		clk_struct.clockdiv=1;
		clockdiv=(40000000/frequency);   //Assuming the external crystal will produce 40Mhz
		switch(clockdiv)
		{
			case 1:
				clk_struct.clockdiv=Clock_DIV_BY_1;
				break;
			case 2:
				clk_struct.clockdiv=Clock_DIV_BY_2;
				break;
			case 4:
				clk_struct.clockdiv=Clock_DIV_BY_4;
				break;
			case 8:
				clk_struct.clockdiv=Clock_DIV_BY_8;
				break;
			case 16:
				clk_struct.clockdiv=Clock_DIV_BY_16;
				break;
			case 32:
				clk_struct.clockdiv=Clock_DIV_BY_32;
				break;
			case 64:
				clk_struct.clockdiv=Clock_DIV_BY_64;
				break;
			default:

				break;
		}
		busdiv=(frequency/bus_freq)-1;
		switch(busdiv)
		{
			case 0:
				clk_struct.busdiv=Bus_DIV_BY_1;
				break;
			case 1:
				clk_struct.busdiv=Bus_DIV_BY_2;
				break;
			case 2:
				clk_struct.busdiv=Bus_DIV_BY_3;
				break;
			case 3:
				clk_struct.busdiv=Bus_DIV_BY_4;
				break;
			case 4:
				clk_struct.busdiv=Bus_DIV_BY_5;
				break;
			case 5:
				clk_struct.busdiv=Bus_DIV_BY_6;
				break;
			case 6:
				clk_struct.busdiv=Bus_DIV_BY_7;
				break;
			case 7:
				clk_struct.busdiv=Bus_DIV_BY_8;
				break;
			case 8:
				clk_struct.busdiv=Bus_DIV_BY_9;
				break;
			case 9:
				clk_struct.busdiv=Bus_DIV_BY_10;
				break;
			case 10:
				clk_struct.busdiv=Bus_DIV_BY_11;
				break;
			case 11:
				clk_struct.busdiv=Bus_DIV_BY_12;
				break;
			case 12:
				clk_struct.busdiv=Bus_DIV_BY_13;
				break;
			case 13:
				clk_struct.busdiv=Bus_DIV_BY_14;
				break;
			case 14:
				clk_struct.busdiv=Bus_DIV_BY_15;
				break;
			case 15:
				clk_struct.busdiv=Bus_DIV_BY_16;
				break;
			default:

				break;
		}

		slowdiv=(frequency/(slow_freq*2));
		switch(slowdiv)
		{
			case 0:
				clk_struct.slowdiv=Slow_DIV_BY_1;
				break;
			case 1:
				clk_struct.slowdiv=Slow_DIV_BY_2;
				break;
			case 2:
				clk_struct.slowdiv=Slow_DIV_BY_3;
				break;
			case 3:
				clk_struct.slowdiv=Slow_DIV_BY_4;
				break;
			case 4:
				clk_struct.slowdiv=Slow_DIV_BY_5;
				break;
			case 5:
				clk_struct.slowdiv=Slow_DIV_BY_6;
				break;
			case 6:
				clk_struct.slowdiv=Slow_DIV_BY_7;
				break;
			case 7:
				clk_struct.slowdiv=Slow_DIV_BY_8;
				break;
			default:
				break;
		}
		corediv=(frequency/core_freq)-1;
		switch(corediv)
		{
			case 0:
				clk_struct.corediv=Core_DIV_BY_1;
				break;
			case 1:
				clk_struct.corediv=Core_DIV_BY_2;
				break;
			case 2:
				clk_struct.corediv=Core_DIV_BY_3;
				break;
			case 3:
				clk_struct.corediv=Core_DIV_BY_4;
				break;
			case 4:
				clk_struct.corediv=Core_DIV_BY_5;
				break;
			case 5:
				clk_struct.corediv=Core_DIV_BY_6;
				break;
			case 6:
				clk_struct.corediv=Core_DIV_BY_7;
				break;
			case 7:
				clk_struct.corediv=Core_DIV_BY_8;
				break;
			case 8:
				clk_struct.corediv=Core_DIV_BY_9;
				break;
			case 9:
				clk_struct.corediv=Core_DIV_BY_10;
				break;
			case 10:
				clk_struct.corediv=Core_DIV_BY_11;
				break;
			case 11:
				clk_struct.corediv=Core_DIV_BY_12;
				break;
			case 12:
				clk_struct.corediv=Core_DIV_BY_13;
				break;
			case 13:
				clk_struct.corediv=Core_DIV_BY_14;
				break;
			case 14:
				clk_struct.corediv=Core_DIV_BY_15;
				break;
			case 15:
				clk_struct.corediv=Core_DIV_BY_16;
				break;
			default:

				break;
		}
		clk_struct.range=clk_drvstruct->range;
		SCG->SOSCDIV = SCG_SOSCDIV_SOSCDIV1(clk_struct.clockdiv)|
					   SCG_SOSCDIV_SOSCDIV2(clk_struct.clockdiv);
		if(SOSC_SYS_CLK==sys)
		{
		SCG->RCCR    =SCG_RCCR_SCS(SOSC_CLK_SELECT)                /* Select SOSC as clock source 								*/
		            |SCG_RCCR_DIVCORE(clk_struct.corediv)      /* DIVCORE=corediv, div. by (.corediv+1): Core clock = 48 MHz/(corediv+1) */
		            |SCG_RCCR_DIVBUS(clk_struct.busdiv)        /* DIVBUS=busdiv, div. by (.busdiv+1): bus clock = 48 MHz/(busdiv+1)	*/
		            |SCG_RCCR_DIVSLOW(clk_struct.slowdiv);     /* DIVSLOW=slowdiv, div. by (.slowdiv+1): SCG slow, flash clock= 24 MHz/(.slowdiv+1)*/
		retfreq.busfreq=bus_freq;
		retfreq.corefreq=core_freq;
		retfreq.slowfreq=slow_freq;
		}
		SCG->SOSCCFG  =	SCG_SOSCCFG_RANGE(clk_struct.range)|		/*   frequency range selected	*/
				SCG_SOSCCFG_EREFS(clk_struct.osctype);		/* HGO=0:   Config xtal osc for low power 	*/
		  	  	  	  	  	  	  	  	  	  	  	  	/* EREFS=.osctype ,Input Crystal selected		*/

		  while(SCG->SOSCCSR & SCG_SOSCCSR_LK_MASK); 	/* Ensure SOSCCSR unlocked 							*/
		  SCG->SOSCCSR = SCG_SOSCCSR_SOSCEN_MASK; 		/* LK=0:          SOSCCSR can be written 				*/
														/* SOSCCMRE=0:    OSC CLK monitor IRQ if enabled 		*/
														/* SOSCCM=0:      OSC CLK monitor disabled 			*/
														/* SOSCERCLKEN=0: Sys OSC 3V ERCLK output clk disabled */
														/* SOSCLPEN=0:    Sys OSC disabled in VLP modes 		*/
														/* SOSCSTEN=0:    Sys OSC disabled in Stop modes 		*/
														/* SOSCEN=1:      Enable oscillator 					*/

		while(!(SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK));	/* Wait for sys OSC clk valid */
		retfreq.soscfreq=frequency;
		break;
	default:
		break;

}

}
static uint32_t CLOCK_SYS_GetLpoFreq(void)
{
    uint32_t freq = 0U;

    switch (SIM_GetLpoClkSelectorValue(SIM))
    {
        case 0U:  /* SIM_LPO_CLK_SEL_LPO_128K */
            freq =128000;
            break;
        case 1U:  /* SIM_LPO_CLK_SEL_NO_CLOCK: */
            freq = 0U;
            break;
        case 2U:  /* SIM_LPO_CLK_SEL_LPO_32K: */
            freq = SIM_GetLpo32KStatus(SIM) ? 32000: 0UL;
            break;
        case 3U:  /* SIM_LPO_CLK_SEL_LPO_1K:  */
            freq = SIM_GetLpo1KStatus(SIM) ? 1000 : 0UL;
            break;
        default:
            /* Invalid LPOCLKSEL selection.*/

            break;
    }

    return freq;
}

/*****************************************************************************
 *
 * Function Name			: CLOCK_DRV_GetFreq
 * Function Description		:get different clock source frequency
 *
 *
 * Parameters				: clock_names_type
 * Returfn value			: uint32_t
 *
 *****************************************************************************/
uint32_t CLOCK_DRV_GetFreq(clock_names_type clockName)
{
	uint32_t freq=0;
	  /* Frequency of the clock name from SCG */
	    if (clockName < 10)
	    {
	    	 switch (clockName)
	    	    {
	    	        /* Main clocks */
	    	        case CORE_CLOCK:
	    	            freq = retfreq.corefreq;
	    	            return (uint32_t)freq;
	    	            break;
	    	        case BUS_CLOCK:
	    	            freq = retfreq.busfreq;
	    	            return (uint32_t)freq;
	    	            break;
	    	        case SLOW_CLOCK:
	    	            freq = retfreq.slowfreq;
	    	            return (uint32_t)freq;
	    	            break;

	    	        /* Other internal clocks used by peripherals. */
	    	        case SIRC_CLOCK:
	    	            freq = retfreq.sircfreq;
	    	            return (uint32_t)freq;
	    	            break;
	    	        case FIRC_CLOCK:
	    	            freq = retfreq.fircfreq;
	    	            return (uint32_t)freq;
	    	            break;
	    	        case SOSC_CLOCK:
	    	            freq = retfreq.soscfreq;
	    	            return (uint32_t)freq;
	    	            break;
	    	        case LPO_CLK:
	    	        	if (PMC_GetLpoMode(PMC))
	    	        	{
	    	            freq =CLOCK_SYS_GetLpoFreq();
	    	        	}
	    	            return (uint32_t)freq;
	    	            break;
	    	        case LPO_1K_CLK:
	    	        	if (PMC_GetLpoMode(PMC))
	    	        	{
	    	            freq =CLOCK_SYS_GetLpoFreq();
	    	        	}
	    	            return (uint32_t)freq;
	    	            break;
	    	        case LPO_32K_CLK:
	    	        	if (PMC_GetLpoMode(PMC))
	    	        	{
	    	            freq =CLOCK_SYS_GetLpoFreq();
	    	        	}
	    	            return (uint32_t)freq;
	    	            break;
	    	        case LPO_128K_CLK:
	    	        	if (PMC_GetLpoMode(PMC))
	    	        	{
	    	            freq =CLOCK_SYS_GetLpoFreq();
	    	        	}
	    	        	return (uint32_t)freq;
	    	            break;
	    	        default:

	    	            break;
	    	    }

	    }

return freq;
}
/*****************************************************************************
 *
 * Function Name			: LPO_CLOCK_Source
 * Function Description		:lpo clock source select
 *
 *
 * Parameters				: clock_names_type
 * Returfn value			: uint32_t
 *
 *****************************************************************************/
void LPO_CLOCK_Source(lpo_clock_type lpo)
{
	SIM->LPOCLKS|=SIM_LPOCLKS_LPOCLKSEL(lpo);
}
/*****************************************************************************
 *
 * Function Name			: LPO_CLOCK_Enable
 * Function Description		:Enable LPO_clock(Default in enable state)
 *
 *
 * Parameters				: clock_names_type
 * Returfn value			: uint32_t
 *
 *****************************************************************************/
void LPO_CLOCK_Enable(void)
{
PMC->REGSC |=PMC_REGSC_LPODIS(0);
}
/*****************************************************************************
 *
 * Function Name			: LPO_CLOCK_Disable
 * Function Description		:Enable LPO_clock(Default in enable state)
 *
 *
 * Parameters				: clock_names_type
 * Returfn value			: uint32_t
 *
 *****************************************************************************/
void LPO_CLOCK_Disable(void)
{
PMC->REGSC |=PMC_REGSC_LPODIS(1);
}
