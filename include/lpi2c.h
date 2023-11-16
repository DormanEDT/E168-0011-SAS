/*
 * Copyright (c) 2014 - 2016, Freescale Semiconductor, Inc.
 * Copyright (c) 2016 - 2018, NXP.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LPI2C_H_
#define LPI2C_H_
/*******************************************************************************
* Includes
*******************************************************************************/
#include "S32K116.h"

/*******************************************************************************
* Constants and macros
*******************************************************************************/

//state machine for magnetic sensor read
typedef enum
{
	ANGLE_SENSOR_IDLE,
    MUX1_READ_ADDRESS_1,
    ANGLE_SENSOR1_SLAVE_ADDR_WRITE_1,
	ANGLE_SENSOR1_READ_ADDR_WRITE_1,
	ANGLE_SENSOR1_ADDR_READ_BYTE_1,
	MUX2_SLAVE_ADDR_WRITE_1,
	MUX2_READ_ADDR_WRITE_1,
	MUX2_READ_ADDRESS_1,
	ANGLE_SENSOR2_SLAVE_ADDR_WRITE_1,
	ANGLE_SENSOR2_READ_ADDR_WRITE_1,
	ANGLE_SENSOR2_ADDR_READ_BYTE_1,
	MUX1_SLAVE_ADDR_WRITE,
	MUX1_READ_ADDR_WRITE,
	MUX1_READ_ADDRESS_2,
	ANGLE_SENSOR1_SLAVE_ADDR_WRITE_2,
	ANGLE_SENSOR1_READ_ADDR_WRITE_2,
	ANGLE_SENSOR1_ADDR_READ_BYTE_2,
	MUX2_SLAVE_ADDR_WRITE_2,
	MUX2_READ_ADDR_WRITE_2,
	MUX2_READ_ADDRESS_2,
	ANGLE_SENSOR2_SLAVE_ADDR_WRITE_2,
	ANGLE_SENSOR2_READ_ADDR_WRITE_2,
	ANGLE_SENSOR2_ADDR_READ_BYTE_2,
	ANGLE_SENSOR_STOP

}i2c_state_machine;
typedef enum
{
	MUX1_SELECT,
	MUX1_READ,
	ANGLE_SENSOR1_ADDR_WRITE,
	MAGNET1_STATE_READ,
	ANGLE_SENSOR1_READ,
	MUX2_SELECT,
	MUX2_ADDR_WRITE,
	MUX2_READ,
	ANGLE_SENSOR2_ADDR_WRITE,
	MAGNET2_STATE_READ,
	ANGLE_SENSOR2_READ,
	MAGNET_STATE_STOP

}magnet_state_machine;
enum err
{
    OK,
    BUSY,
    NO_DATA_RECEIVED,
    NO_STOP,
    NDF,
    ALF,
    FEF,
    PLTF
};

extern magnet_state_machine magnet_state;
extern i2c_state_machine i2c_state;
extern uint8_t magnet_status;
#define BUSY_TIMEOUT        3
#define READING_TIMEOUT     2
#define STOP_TIMEOUT        3

uint8_t LPI2C_Select_MuxChannel(uint8_t target_address, uint8_t mux_channel,uint8_t* data);
void LPI2C_Transmit_Int(void);
void LPI2C_Transmit(void);
void LPI2C_Generate_Start_Ack(uint8_t data);
void LPI2C_AddressToRead(uint8_t data);
void LPI2C_Receive_Data(uint8_t *p_buffer, uint8_t n_bytes);
uint8_t LPI2C_Read_Location(uint8_t target_address, uint8_t register_address,uint8_t* data);
void LPI2C_init(void);
uint8_t SAS_ReadMagnet_Status(uint8_t channel);
uint8_t LPI2C_Generate_Stop(void);
uint8_t LPI2C_Check_Busy(void);


#endif /* LPI2C_LPI2C_H_ */
