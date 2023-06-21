/*
 * AppSAS.c
 *
 *      Created on: 27-Jul-2022
 *      Author: Admin
 */

#include "main.h"
#include "lpi2c.h"

uint8_t u8RegReadValMagPresent = 0;
uint8_t u8RegReadValAngleLSB = 0;
uint8_t u8RegReadValAngleMSB = 0;
uint16_t u16RegAngleData = 0;

uint8_t u8RegMuxSelect = 0;
uint8_t data_temp=0;


/*********************************************************************************************************************************************/

void SAS_Write_CONF_Reg(void)
{
	uint8_t u8RegMuxSelect=0;
	LPI2C_Select_MuxChannel(M_PCA9540B, M_PCA9540B_MUX_CHANNEL1,
				&u8RegMuxSelect);
	if(M_PCA9540B_MUX_CHANNEL1==u8RegMuxSelect)
	{
		LPI2C_CONF_Reg_Write();
	}
}
uint16_t SAS_ReadAngelSensorAt(uint8_t channel) {

	uint8_t error = 0;
	uint8_t mux_channel_selected = 0;

	if (channel == M_PCA9540B_MUX_CHANNEL0) {
		mux_channel_selected = M_PCA9540B_MUX_CHANNEL0;
	} else if (channel == M_PCA9540B_MUX_CHANNEL1) {
		mux_channel_selected = M_PCA9540B_MUX_CHANNEL1;
	}
	/* Select the channel in the Multiplexer M_PCA9540B and assigned the channel to be used 0 or 1 */
	error = LPI2C_Select_MuxChannel(M_PCA9540B, mux_channel_selected,
			&u8RegMuxSelect);
	/* Ensure there is no error and the channel in the Multiplexer M_PCA9540B selected is set correctly */
	if ((!error) && (u8RegMuxSelect == mux_channel_selected))
	{

		/*Read the M_AS5600 Magnetic sensor register and ensure the magnetic field is detected */
		if (LPI2C_Read_Location(M_AS5600, M_AS5600_STATUS,
				&u8RegReadValMagPresent))
			return (error |= (1 << BUSY));
		/* Only Read the angle if the magnetic field is detected as MD */
		if (u8RegReadValMagPresent & M_AS5600_STATUS_MD)
		{
			IsSasHardFault=0;


			/* Read the angle Sensor MSB register */
			if (LPI2C_Read_Location(M_AS5600, M_AS5600_RA_MSB,
					&u8RegReadValAngleMSB))
				return (error |= (1 << BUSY));
			/* Read the angle Sensor LSB register */
			if (LPI2C_Read_Location(M_AS5600, M_AS5600_RA_LSB,
					&u8RegReadValAngleLSB))
				return (error |= (1 << BUSY));
		}
		else
		{
			IsSasHardFault=1;
			IsCalibarationComp=0;

			return (error |= (1 << BUSY));
		}
		/* Club the MSB and LSB and return the integer value */
		u16RegAngleData = (u8RegReadValAngleMSB << 8);
		u16RegAngleData |= (u8RegReadValAngleLSB);
	}
	else
	{

	}
	return (u16RegAngleData);

}
void sleep_actions(void)
{
	gu8STSnibble=1;
	 gu8CSASCanDataFrame[0]|=(gu8STSnibble<<4);
	 gu8CSASCanDataFrame[6]&=~(0<<3); //STDID=0;bit 3in Byte 6
	CAN_Transmit_msg(&gCANMailBoxNo, CSASGRP1ARBID_DATA, CAN_ID_STD,
	        		CSAS_SIZE8BYTE, gu8CSASCanDataFrame);
	ADC_Disable();
	I2C_SENSOR_OFF;
	CAN_TRANS_OFF;
	PIN_Interrupt_Enable(PORTB,2,FALLING_EDGE); //External interrupt

}
void wakeup_actions(void)
{
	PIN_Interrupt_Disable(PORTB,2);
	ADC_Init();
	I2C_SENSOR_ON;
	CAN_TRANS_ON;
}
