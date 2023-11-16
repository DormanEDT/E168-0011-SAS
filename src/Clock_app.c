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
/*     FILE NAME   :  Clock_app.c                                                 */
/*                                                                                */
/*     DESCRIPTION :  Clock application.                                          */
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
#include "Clock_app.h"

/**********************************************************************************/
/*Constant and Variable Declaration                                               */
/**********************************************************************************/

/***************************User configurations  ***************************
 * The below part can modify by user according to their application
 ***************************************************************************/
	Clock_DrvInit_Struct clk_firc_init=
			{
					.clockselect=FIRC_CLK_SELECT,
					.osctype=Internal_Crystal,
					.range=FIRC_48Mhz
			};
	Clock_DrvInit_Struct clk_sirc_init=
				{
						.clockselect=SIRC_CLK_SELECT,
						.osctype=Internal_Crystal,
						.range=SIRC_8Mhz
				};

frequency_struct freq_firc_init=
		{
				.fircfreq=48000000,
				.sircfreq=48000000,
				.soscfreq=40000000,    //external crystal oscillator frequency (it will vary by crystal)
				.busfreq=48000000,
				.corefreq=48000000,
				.slowfreq=24000000   //slowfreq should be less than or equal to half of core frequency
		};
frequency_struct freq_sirc_init=
		{
				.fircfreq=48000000,
				.sircfreq=8000000,
				.soscfreq=40000000,    //external crystal oscillator frequency
				.busfreq=8000000,
				.corefreq=8000000,
				.slowfreq=4000000
		};

/***************************User Configurations Ends  ***************************/

void Clock_App_Init(void)
{
	Clock_Drv_Init(&clk_firc_init,&freq_firc_init,FIRC_SYS_CLK);   //FIRC-48Mhz as system clock
	Clock_Drv_Init(&clk_sirc_init,&freq_sirc_init,DEFAULT_SYS_CLK);   //SIRC-8Mhz
}
