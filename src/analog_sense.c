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

#include "analog_sense.h"



/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :   ADC_Init                                   */
/*                                                                 */
/*   FUNCTION BRIEF :   									       */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :   None                                       */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/
void ADC_Init(void)
{

 PCC->PCCn[PCC_ADC0_INDEX] &=~ PCC_PCCn_CGC_MASK;  /* Disable clock to change PCS */
 //PCC->PCCn[PCC_ADC0_INDEX] |= PCC_PCCn_PCS(1);     /* PCS=1: Select SOSCDIV2 		*/
 PCC->PCCn[PCC_ADC0_INDEX] |= PCC_PCCn_PCS(2);     /* PCS=1: Select internal 		*/
 PCC->PCCn[PCC_ADC0_INDEX] |= PCC_PCCn_CGC_MASK;   /* Enable bus clock in ADC 	*/

	/*!
	 * ADC0 Initialization:
	 * ===================================================
	 */
 ADC0->SC1[0] |= ADC_SC1_ADCH_MASK;	/* ADCH=1F: Module is disabled for conversions	*/
                                 	/* AIEN=0: Interrupts are disabled 			*/
 ADC0->CFG1 |= ADC_CFG1_ADIV_MASK
		  	   |ADC_CFG1_MODE(1);	/* ADICLK=0: Input clk=ALTCLK1=SOSCDIV2 	*/
                                 	/* ADIV=0: Prescaler=1 					*/
                                 	/* MODE=1: 12-bit conversion 				*/

 ADC0->CFG2 = ADC_CFG2_SMPLTS(12);	/* SMPLTS=12(default): sample time is 13 ADC clks 	*/
 ADC0->SC2 = 0x00000000;         	/* ADTRG=0: SW trigger 							*/
                                 	/* ACFE,ACFGT,ACREN=0: Compare func disabled		*/
                                 	/* DMAEN=0: DMA disabled 							*/
                                    /* REFSEL=0: Voltage reference pins= VREFH, VREEFL */
 ADC0->SC3 = 0x00000000;        	/* CAL=0: Do not start calibration sequence 		*/
                                    /* ADCO=0: One conversion performed 				*/
                                    /* AVGE,AVGS=0: HW average function disabled 		*/
}
/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :   read_adc_chx                               */
/*                                                                 */
/*   FUNCTION BRIEF :   									       */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :   None                                       */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/

uint16_t Read_ADC(uint8_t ADC_Channel)
{
	uint8_t loop_count = 0;
	uint16_t adc_result=0;
	uint16_t adc_total = 0;
	uint16_t adc_data = 0;

	for ( loop_count = 0; loop_count < 5; loop_count++ )
	{
		ADC0->SC1[0]&=~ADC_SC1_ADCH_MASK;
		ADC0->SC1[0] = ADC_SC1_ADCH(ADC_Channel);
		while(((ADC0->SC1[0] & ADC_SC1_COCO_MASK)>>ADC_SC1_COCO_SHIFT)==0)
		{

		}

		adc_result=ADC0->R[0];
		adc_total = adc_total + adc_result;

	}
	adc_data = (uint16_t)(adc_total/5);
	if( adc_data > 4096)
	{
		adc_data = 4096;
	}
	else
	{
	}
	return adc_data;

}
