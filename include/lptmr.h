/**********************************************************************************/
/*                                                                                */
/*                     CONFIDENTIAL DORMAN PRODUCTS                               */
/*                                                                                */
/*      This is an unpublished work of authorship, which contains trade           */
/*      secrets, created in 2022.Dorman Products owns all rights                  */
/*      to this work and intends to maintain it in confidence to preserve         */
/*      its trade secret status.Dorman Products reserves the right,               */
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
/*     FILE NAME   :  lptmr.h                                                     */
/*                                                                                */
/*     DESCRIPTION :  This is the code file for lowpass timer header.             */
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

/*****************Define to prevent recursive inclusion****************************/

#ifndef LPTMR_H_
#define LPTMR_H_

/**********************************************************************************/

/****************************Header File Inclusion*********************************/

#include "main.h"

/**********************************************************************************/
/********************************Macro Definitions ********************************/

#define COUNT_350 349

/**********************************************************************************/

/*******************************Function Declarations******************************/

void LPTMR_Init(void);
void LPTMR_Enable(uint32_t);
void LPTMR_Disable(void);

/**********************************************************************************/

#endif /* LPTMR_H_ */
