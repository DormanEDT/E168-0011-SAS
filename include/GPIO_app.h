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
/*     FILE NAME   :  GPIO_app.c                                                  */
/*                                                                                */
/*     DESCRIPTION :  This is the code file for GPIO routine.                     */
/*                                                                                */
/* +--------------+----------+----------------------------------------------------+*/
/* |Rev. HISTORY  |  DATE    |           MODIFIED POINT                           |*/
/* +--------------+----------+----------------------------------------------------+*/
/* |    0.1       |22-JUL-22 |          Initial Version                           |*/
/* +--------------+----------+----------------------------------------------------+*/
/* |    0.2       |20-SEPT-22|         GPIO Initialization changed                |*/
/* +--------------+----------+----------------------------------------------------+*/
/***********************************************************************************/

/**********************************************************************************/
/*Modifications                                                                   */
/**********************************************************************************/
#ifndef GPIO_APP_H_
#define GPIO_APP_H_
/**********************************************************************************/
/*Header File Inclusion                                                           */
/**********************************************************************************/
#include "main.h"

/**********************************************************************************/
/*Constant and Macros                                             */
/**********************************************************************************/
#define I2C_SENSOR_ON   PIN_Set(PTD,3)//(PTD->PDOR =((PTD->PDOR & (~(uint32_t)(1<<3)))|(uint32_t)(1<<3)))
#define I2C_SENSOR_OFF  PIN_Clear(PTD,3)//(PTD->PDOR  =(PTD->PDOR & (~(uint32_t)(1<<3))))
#define CAN_TRANS_OFF  PIN_Set(PTD,2)//(PTD->PDOR =((PTD->PDOR & (~(uint32_t)(1<<2)))|(uint32_t)(1<<2)))
#define CAN_TRANS_ON   PIN_Clear(PTD,2)//(PTD->PDOR  =(PTD->PDOR & (~(uint32_t)(1<<2))))
/* Variable Extensions ************************************************/

/* Variable Extensions ************************************************/

/* Function Declarations ***********************************************/
void GPIO_App_init(void);
/* Function Declarations ***********************************************/

#endif /* GPIO_APP_H_ */
/*****************************************************************************
 *
 *	End of file GPIO_app.h
 *
 *****************************************************************************/
