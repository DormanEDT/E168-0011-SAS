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

/**********************************************************************************/
/*Header File Inclusion                                                           */
/**********************************************************************************/

#include "GPIO_Driver.h"
/**********************************************************************************/
/*Constant and Variable Declaration                                               */
/**********************************************************************************/

/* Variable Declarations **********************************************/

/* Variable Declarations **********************************************/

 /*************************************
 FUNCTION_NAME: GPIO_Reg_Deinit
 Description  :De-initialize the GPIOx peripheral registers to their default reset values.
 parameter    :GPIO_Type*
 return type  :void
 **************************************/

 void GPIO_Reg_Deinit(GPIO_Type* PTx)
 {
	 PTx->PDOR = GPIO_PDOR_RESET_VALUE; /* Reset Output Data Register */
     PTx->PDDR = GPIO_PDDR_RESET_VALUE; /* Reset Data Direction Register */
     PTx->PIDR = GPIO_PIDR_RESET_VALUE; /* Reset Input Data Register */
 }

 /*********************************************************************************************
  FUNCTION_NAME: GPIO_Mode_Init
  Description  :Initializes the GPIOx according to the specified parameters.
  parameter    :GPIO_Init_Struct*
  return type  :void
  *********************************************************************************************/

 void GPIO_Mode_Init(GPIO_Init_Struct* gpio)
 {
	 uint8_t loop=0;
	 uint8_t *array;
	 array=(gpio->array_pins);
	 for(loop=0;loop<(gpio->num_pins);loop++)
	 {
		 (gpio->port_t)->PCR[array[loop]]=PORT_PCR_MUX(PORT_MUX_AS_GPIO);

	 }

	 if(gpio->gpio_mode)
	 {
		 for(loop=0;loop<(gpio->num_pins);loop++)
		 {
			 (gpio->gpio_t)->PDDR |=(1<<(array[loop]));
		 }
	 }
	 else
	 {
		 for(loop=0;loop<(gpio->num_pins);loop++)
		 {
			 (gpio->gpio_t)->PDDR &=~(1<<(array[loop]));

		 }
	 }

 }
 /*********************************************************************************************
   FUNCTION_NAME: GPIO_Drv_Init
   Description  :Initializes the GPIOx according to the specified parameters.
   parameter    :GPIO_Init_Struct*,uint8_t(number of pins)
   return type  :void
 **********************************************************************************************/
 void GPIO_Drv_Init(GPIO_Init_Struct* gpio,uint8_t num_conf_pin)
 {
	 uint32_t i;
	 for (i = 0U; i < num_conf_pin; i++)
	 {
		 GPIO_Mode_Init(&gpio[i]);
	 }
 }
/*************************************
FUNCTION_NAME: GPIO_Clk_Init
Description  :GPIO initializations
parameter    :void
return type  :void
**************************************/
void GPIO_Clk_Init(void)
{
	PCC-> PCCn[PCC_PORTA_INDEX] = PCC_PCCn_CGC_MASK; /* Enable clock for PORT A */
	PCC-> PCCn[PCC_PORTB_INDEX] = PCC_PCCn_CGC_MASK; /* Enable clock for PORT B */
	PCC-> PCCn[PCC_PORTC_INDEX] = PCC_PCCn_CGC_MASK; /* Enable clock for PORT C */
	PCC-> PCCn[PCC_PORTD_INDEX] = PCC_PCCn_CGC_MASK; /* Enable clock for PORT D */
	PCC-> PCCn[PCC_PORTE_INDEX] = PCC_PCCn_CGC_MASK; /* Enable clock for PORT E */

}

/**********************************************************************************************************************************
FUNCTION_NAME:PIN_Muxmode_Select
Description  :pin functionality selection
parameter    :PORT_Type*,uint8_t(pin_number),MUX_Mode_Typedef(functionality number(chip specific))
return type  :void
***********************************************************************************************************************************/

void PIN_Muxmode_Select(PORT_Type* port,uint8_t pin,MUX_Mode_Typedef alt)
{
	port->PCR[pin] = PORT_PCR_MUX(alt);
}

/**********************************************************************************************************************************
FUNCTION_NAME: PIN_Input_Read
Description  :pin read
parameter    :GPIO_Type*,uint8_t
return type  :uint8_t
***********************************************************************************************************************************/


uint8_t PIN_Input_Read(GPIO_Type * port,uint8_t pin)
{
	if((port->PDIR)&(1<<pin))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**********************************************************************************************************************************
FUNCTION_NAME: PORT_Input_Read
Description  :port read
parameter    :GPIO_Type*
return type  :uint32_t
***********************************************************************************************************************************/

uint32_t PORT_Input_Read(GPIO_Type *port)
{
	return (port->PDIR);
}

/**********************************************************************************************************************************
FUNCTION_NAME: PIN_Set
Description  :Set output as 1 in corresponding pin
parameter    :GPIO_Type*,uint8_t
return type  :void
***********************************************************************************************************************************/


void PIN_Set(GPIO_Type *port,uint8_t pin)
{
	port->PSOR |=(1<<pin);
}

/**********************************************************************************************************************************
FUNCTION_NAME: PIN_Clear
Description  :Set output as 0 in corresponding pin
parameter    :GPIO_Type*,uint8_t
return type  :void
***********************************************************************************************************************************/

void PIN_Clear(GPIO_Type *port,uint8_t pin)
{
	port->PCOR |=(1<<pin);
}

/**********************************************************************************************************************************
FUNCTION_NAME: PIN_Toggle
Description  :Toggle output in corresponding pin
parameter    :GPIO_Type*,uint8_t
return type  :void
***********************************************************************************************************************************/

void PIN_Toggle(GPIO_Type *port,uint8_t pin)
{
	port->PTOR |=(1<<pin);
}

/**********************************************************************************************************************************
FUNCTION_NAME: PIN_Interrupt_Enable
Description  :Enable interrupt
parameter    :PORT_Type *,uint8_t,Interrupt_Mode_TypeDef
return type  :void
***********************************************************************************************************************************/

void PIN_Interrupt_Enable(PORT_Type *port,uint8_t pin,Interrupt_Mode_TypeDef Interrupt)
{
	port->PCR[pin]|= PORT_PCR_IRQC(Interrupt);
}

/**********************************************************************************************************************************
FUNCTION_NAME: PIN_Interrupt_Disable
Description  :disable interrupt
parameter    :PORT_Type *,uint8_t
return type  :void
***********************************************************************************************************************************/
void PIN_Interrupt_Disable(PORT_Type* port,uint8_t pin)
{
	port->PCR[pin]|= PORT_PCR_IRQC(0);
}
/**********************************************************************************************************************************
FUNCTION_NAME:PIN_Interrupt_Clearflag
Description  :ISF clear
parameter    :PORT_Type *,uint8_t
return type  :void
***********************************************************************************************************************************/
void PIN_Interrupt_Clearflag(PORT_Type* port,uint8_t pin)
{
	port->PCR[pin]|=PORT_PCR_ISF(1);
}
/**********************************************************************************************************************************
FUNCTION_NAME: PIN_Pull_Select
Description  :Pull enable
parameter    :PORT_Type *,uint8_t,Pull_mode_Typedef
return type  :void
***********************************************************************************************************************************/
void PIN_Pull_Select(PORT_Type* port,uint8_t pin,Pull_mode_Typedef pull)
{
	port->PCR[pin]|=PORT_PCR_PE(1);
	if(pull)
	{
		port->PCR[pin]|=PORT_PCR_PS(1);
	}
	else
	{
		port->PCR[pin]|=PORT_PCR_PS(0);
	}
}

/*****************************************************************************
 *
 *	End of file GPIO_Driver.c
 *
 *****************************************************************************/
