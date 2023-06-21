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
#include <GPIO_app.h>

/**********************************************************************************/
/*Constant and Variable Declaration                                               */
/**********************************************************************************/
uint8_t ptd_out[2]={2,3},ptb_out[2]={0,1},ptb_in=2;
/************************* Variable Declarations **********************************************/
GPIO_Init_Struct gpio_struct[3]=
{
		{
				.port_t=PORTD,
				.gpio_t=PTD,
				.num_pins=2,
				.array_pins=ptd_out,
				.gpio_mode=GPIO_OUTPUT_MODE
		},

		{
						.port_t=PORTB,
						.gpio_t=PTB,
						.num_pins=2,
						.array_pins=ptb_out,
						.gpio_mode=GPIO_OUTPUT_MODE
		},
		{
						.port_t=PORTB,
						.gpio_t=PTB,
						.num_pins=1,
						.array_pins=&ptb_in,
						.gpio_mode=GPIO_INPUT_MODE
		}

};
/* Variable Declarations **********************************************/
/*****************************************************************************
 *
 * Function Name			: GPIO_App_init
 * Function Description		: This function for handling the GPIO Initializing
 * Parameters				: None
 * Return value				: None
 *
 *****************************************************************************/
void GPIO_App_init(void)
{
	GPIO_Clk_Init();                        //Initialize gpio clocks
	GPIO_Drv_Init(gpio_struct,3);
	PIN_Clear(PTB,0); 				        /* Making the DIR Pin to 0V / GND */
	PIN_Clear(PTB,1);				        /* Making the DIR1 Pin to 0V / GND */
	PIN_Muxmode_Select(PORTA,2,PORT_MUX_ALT3);/* Port A2: MUX = ALT3, LPI2C_SDA */
	PIN_Muxmode_Select(PORTA,3,PORT_MUX_ALT3);/* Port A3: MUX = ALT3, LPI2C_SCL */
	PIN_Pull_Select(PORTA,2,PULL_UP);
	PIN_Pull_Select(PORTA,3,PULL_UP);
}

/*****************************************************************************
 *
 * Function Name			: PORT_IRQHandler
 * Function Description		: This function for handling the GPIO Interrupt
 *							  function.
 * Parameters				: None
 * Return value				: None
 *
 *****************************************************************************/

void PORT_IRQHandler(void)
{
/********** You can write your code here to execute after interrupt******/
	PORTB->PCR[2] |=PORT_PCR_ISF(1); //  ISF = 1 Clear the interrupt flag
//	wakeup_actions();

}
