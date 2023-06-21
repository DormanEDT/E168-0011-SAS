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
/*     FILE NAME   :  PWRMode_Driver.h                                            */
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
#ifndef PWRMODE_DRIVER_H_
#define PWRMODE_DRIVER_H_
/**********************************************************************************/
/*Header File Inclusion                                                           */
/**********************************************************************************/
#include "main.h"

/**********************************************************************************/
/*Constant and Variable Declaration                                               */
/**********************************************************************************/

/* Variable Extensions ************************************************/

typedef enum
{
	RUN=0,
	VLPR=1,
	STOP1=2,
	STOP2=3,
	VLPS=5
}Powermode_Typedef;

/* Variable Extensions ************************************************/

/* Function Declarations ***********************************************/

void Halt(void);
void SIRC_Init(void);
void VLPR_Init(void);
void SIRC_RUNState(void);
void FIRC_Disable(void);
void Set_Power_Mode(Powermode_Typedef);
void Clkout_Bus(void);

/* Function Declarations ***********************************************/

#endif /* PWRMODE_DRIVER_H_ */
/*****************************************************************************
 *
 *	End of file PWRMode_Driver.h
 *
 *****************************************************************************/





