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

#ifndef GPIO_DRIVER_H_
#define GPIO_DRIVER_H_

/**********************************************************************************/
/*Header File Inclusion                                                           */
/**********************************************************************************/
#include "main.h"

/**********************************************************************************/
/*Constant and Variable Declaration                                               */
/**********************************************************************************/

#define GPIO_PDOR_RESET_VALUE   0x00000000
#define GPIO_PDDR_RESET_VALUE   0x00000000
#define GPIO_PIDR_RESET_VALUE   0x00000000

/* Variable Extensions ************************************************/

typedef enum
{
	PORT_PIN_DISABLED            = 0U,  /*!< corresponding pin is disabled, but is used as an analog pin       */
	PORT_MUX_AS_GPIO             = 1U,  /*!< corresponding pin is configured as GPIO                           */
	PORT_MUX_ALT2                = 2U,  /*!< chip-specific                                                     */
	PORT_MUX_ALT3                = 3U,  /*!< chip-specific                                                     */
    PORT_MUX_ALT4                = 4U,  /*!< chip-specific                                                     */
	PORT_MUX_ALT5                = 5U,  /*!< chip-specific                                                     */
	PORT_MUX_ALT6                = 6U,  /*!< chip-specific                                                     */
    PORT_MUX_ALT7                = 7U   /*!< chip-specific   */

}MUX_Mode_Typedef;

typedef enum
{
	GPIO_OUTPUT_MODE = 1U,
	GPIO_INPUT_MODE  = 0U
}GPIO_Mode_TypeDef;
typedef enum
{
	DMA_RISING_EDGE=1U,
	DMA_FALLING_EDGE=2U,
	DMA_EITHER_EDGE=3U,
	LOGIC_0=8U,
	RISING_EDGE=9U,
	FALLING_EDGE=10U,
	EITHER_EDGE=11U,
	LOGIC_1=12U
}Interrupt_Mode_TypeDef;
typedef enum
{
	PULL_DOWN=0,
	PULL_UP=1
}Pull_mode_Typedef;
typedef struct
{
	PORT_Type* port_t;
	GPIO_Type* gpio_t;
	uint8_t num_pins;
	uint8_t* array_pins;
	GPIO_Mode_TypeDef gpio_mode;

}GPIO_Init_Struct;
/* Variable Extensions ************************************************/

/* Function Declarations ***********************************************/
void GPIO_Reg_Deinit(GPIO_Type*);
void GPIO_Mode_Init(GPIO_Init_Struct*);
void GPIO_Drv_Init(GPIO_Init_Struct*,uint8_t);
void GPIO_Clk_Init(void);
void PIN_Muxmode_Select(PORT_Type*,uint8_t ,MUX_Mode_Typedef);
uint8_t PIN_Input_Read(GPIO_Type *,uint8_t );
uint32_t PORT_Input_Read(GPIO_Type*);
void PIN_Set(GPIO_Type *,uint8_t);
void PIN_Clear(GPIO_Type *,uint8_t);
void PIN_Toggle(GPIO_Type *,uint8_t);
void PORT_Init(void);
void PIN_Interrupt_Enable(PORT_Type *,uint8_t,Interrupt_Mode_TypeDef);
void PIN_Interrupt_Disable(PORT_Type *,uint8_t);
void PIN_Interrupt_Clearflag(PORT_Type*,uint8_t);
void PIN_Pull_Select(PORT_Type*,uint8_t,Pull_mode_Typedef);
/* Function Declarations ***********************************************/

#endif /* GPIO_DRIVER_H_ */
/*****************************************************************************
 *
 *	End of file GPIO_Driver.h
 *
 *****************************************************************************/
