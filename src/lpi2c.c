
//#include "device_registers.h"	/* include peripheral declarations */
#include "main.h"

/*******************************************************************************
 * Static variables
 *******************************************************************************/
extern uint8_t gI2c_state;
uint8_t cur_tx_fifo_cnt = 0;
uint8_t gAngle_raw_msb = 0;
uint8_t gAngle_raw_lsb = 0;
uint8_t u8RegMuxSelect=0;
uint8_t cur_channel = 0;
uint8_t magnet_status = 0;
uint8_t i2c_state = 0;
uint8_t anglemsb_data1=0;
uint8_t anglelsb_data1=0;
uint8_t anglemsb_data2=0;
uint8_t anglelsb_data2=0;
magnet_state_machine magnet_state=0;
uint8_t error=0;
uint8_t u8RegReadValMagPresent=0;
uint8_t one_time_reject=0;

void LPI2C_init(void)
{

	PCC->PCCn[PCC_LPI2C0_INDEX] |= PCC_PCCn_PCS(2) /* Clk src: SIRCDIV2_CLK Assumed as 8mhz  */
	| PCC_PCCn_CGC_MASK; /* Enable clock for LPI2C0 */

	LPI2C0->MCFGR1 = LPI2C_MCFGR1_PRESCALE(2) | LPI2C_MCFGR1_IGNACK_MASK;
	 LPI2C0->MCFGR2=LPI2C_MCFGR2_BUSIDLE(256);
	LPI2C0->MCCR0 = 0x01040303;   //400k
	LPI2C0->MFCR = LPI2C_MFCR_TXWATER(2) /* Transmitter Water mark set to 0*/
		| LPI2C_MFCR_RXWATER(2); /* Receiver Water mark set to 3*/

	LPI2C0->MCR |= LPI2C_MCR_MEN_MASK /* Enable LPI2C as master */
	| LPI2C_MCR_DBGEN_MASK;

}



void LPI2C_Transmit_Int(void)
{
	i2c_state = 0;
	LPI2C0->MTDR = (4 << 8) | ((0x70<<1)|0); //Start I2C
	LPI2C0->MIER = 0x3F01;
}


void LPI2C_Transmit(void)
{
	LPI2C0->MTDR = (0x05 << 8) | ((0x36 << 1) | 0);
}
//-------------------------------------------------------------------------------------

void LPI2C_Generate_Start_Ack(uint8_t data)
{
	uint8_t u8RegVal = 0;
	u8RegVal = (data << 1) | 0;	//to read last bit should be 1 and for write it should be 0
	LPI2C0->MTDR = (0x04 << 8) | (u8RegVal); //Start and transmit the address - 04
}
void LPI2C_AddressToRead(uint8_t data)
{
	LPI2C0->MTDR = data;
}
void LPI2C_DataToRead(uint8_t data) {
	uint8_t u8RegVal = 0;
	u8RegVal = (data << 1) | 1; //to read last bit should be 1 and for write it should be 0
	LPI2C0->MTDR = (0x04 << 8) | (u8RegVal); //Start and transmit the address - 04
}
void LPI2C_Transmit_Data(uint8_t data)
{
	LPI2C0->MTDR = data;
}
void LPI2C_Receive_Data(uint8_t *p_buffer, uint8_t n_bytes)
{
	uint8_t n=0;
	uint16_t time=0;
	uint16_t timeout_r = (READING_TIMEOUT * n_bytes);
	uint16_t command=0;

	command = 0x0100;
	command |= (n_bytes - 1); //(n_bytes + 1);//(n_bytes - 1);
	LPI2C0->MTDR = command;

	while (((LPI2C0->MFSR) >> 16 != n_bytes) && (time < timeout_r))
		++time;

	if (time >= timeout_r)
	{
		LPI2C0->MCR |= (1 << 9);     // reset receive FIFO
		error |= (1 << NO_DATA_RECEIVED);

	} else
	{
		for (n = 0; n < n_bytes; ++n)
		{
			p_buffer[n] = (uint8_t) (LPI2C0->MRDR & 0x000000FF);
		}
	}

}
uint8_t LPI2C_Check_Busy(void)
{

	uint8_t error = 0; // CLEAR ALL ERRORS
	uint16_t timeout_b = 0;
	while ((LPI2C0->MSR & (1 << 25)) && (timeout_b < BUSY_TIMEOUT))
		++timeout_b;
	if (timeout_b >= BUSY_TIMEOUT)
		return (error |= (1 << BUSY));
	return OK;
}

uint8_t LPI2C_Generate_Stop(void)
{
	uint32_t timeout_s = 0;
	uint8_t stop_sent_err = 0;

	LPI2C0->MTDR = 0x0200; //command

	while ((!(LPI2C0->MSR & (1 << 9))) && (!stop_sent_err))
	{
		if (timeout_s > STOP_TIMEOUT) {
			error |= (1 << NO_STOP);
			stop_sent_err = 1;
		}
		timeout_s++;
	}

	if (LPI2C0->MSR & (1 << 9)) {
		LPI2C0->MSR |= (1 << 9); // Clear Stop flag
	}
	if (error)
		return error;
	else
		return OK;
}

uint8_t LPI2C_Read_Location(uint8_t target_address, uint8_t register_address,
		uint8_t* data)
{
	uint8_t error = 0;

	if(!(LPI2C_Check_Busy()))
	{
	 LPI2C_Generate_Start_Ack(target_address);
	}
	else
	{
		return 1;
	}
	if((LPI2C_Check_Busy()))
	{
	LPI2C_AddressToRead(register_address);
	}
	else
	{
		return 1;
	}
	if((LPI2C_Check_Busy()))

	{
	LPI2C_DataToRead(target_address);
	}
	else
	{
		return 1;
	}
	if((LPI2C_Check_Busy()))
	{
		LPI2C_Receive_Data(data, 1);
	}
	else
	{
		return 1;
	}

	LPI2C_Generate_Stop();

	return (error);
}
uint8_t LPI2C_Select_MuxChannel(uint8_t target_address, uint8_t mux_channel,uint8_t* data)
{
	if(!(LPI2C_Check_Busy()))
	{
	 LPI2C_Generate_Start_Ack(target_address);
	}
	else
	{
		return 1;
	}
	if((LPI2C_Check_Busy()))
	{
	LPI2C_AddressToRead(mux_channel);
	}
	else
	{
		return 1;
	}
	if((LPI2C_Check_Busy()))

	{
	LPI2C_DataToRead(target_address);
	}
	else
	{
		return 1;
	}
	if((LPI2C_Check_Busy()))
	{
		LPI2C_Receive_Data(data, 1);
	}
	else
	{
		return 1;
	}

	LPI2C_Generate_Stop();

	return (error);

}

/*******************************************************************************
 Function Name : LPI2C0_Master_IRQHandler
 *******************************************************************************/
//void LPI2C0_Master_IRQHandler(void)
void LPI2C0_Master_Slave_IRQHandler(void)
{
	DISABLE_INTERRUPTS();

	if (LPI2C0->MSR & (1 << 1)) //Check for I2C Received Data Ready
	{
		if(i2c_state == ANGLE_SENSOR1_SLAVE_ADDR_WRITE_1
				|| i2c_state == ANGLE_SENSOR2_SLAVE_ADDR_WRITE_1
				|| i2c_state == ANGLE_SENSOR1_SLAVE_ADDR_WRITE_2
				|| i2c_state == ANGLE_SENSOR2_SLAVE_ADDR_WRITE_2)
		{
			cur_channel =   (uint8_t) LPI2C0->MRDR;
			LPI2C0->MTDR = 0x0200;   //Generate Stop after receive completion
			LPI2C0->MIER = 0x3F01;
		}
		else if(i2c_state == MUX2_SLAVE_ADDR_WRITE_1)
		{

			anglemsb_data1 =   (uint8_t) LPI2C0->MRDR;
			LPI2C0->MTDR = 0x0200;   //Generate Stop after receive completion
			LPI2C0->MIER = 0x3F01;
		}
		else if( i2c_state == MUX1_SLAVE_ADDR_WRITE)
		{

			    anglemsb_data2 =   (uint8_t) LPI2C0->MRDR;
				LPI2C0->MTDR = 0x0200;   //Generate Stop after receive completion
				LPI2C0->MIER = 0x3F01;
		}
		else if(i2c_state == MUX2_SLAVE_ADDR_WRITE_2)
		{

			anglelsb_data1 =   (uint8_t) LPI2C0->MRDR;
			LPI2C0->MTDR = 0x0200;   //Generate Stop after receive completion
			LPI2C0->MIER = 0x3F01;

		}
		else if( i2c_state == ANGLE_SENSOR_STOP)
		{

			anglelsb_data2 =   (uint8_t) LPI2C0->MRDR;
			LPI2C0->MTDR = 0x0200;   //Generate Stop after receive completion
			LPI2C0->MIER = 0x3F01;
			if(one_time_reject)
			{
				gu16AS5600s1Angle=((anglelsb_data1<<8)|anglemsb_data1);
				gu16AS5600s2Angle=((anglemsb_data2<<8)|anglelsb_data2);
				first_read=1;
			}
			else
			{
				one_time_reject=1;
			}
		}
	}
	if((LPI2C0->MSR & (1 << 0))&&(LPI2C0->MIER & 0x01))
	{
		if(LPI2C0->MFSR & (7<<0))
		{
			cur_tx_fifo_cnt = (uint8_t)LPI2C0->MFSR; //Wait for tx complete
		}
		else
		{

			if(i2c_state == ANGLE_SENSOR_IDLE)
			{
				i2c_state++;
				LPI2C0->MTDR = 0x04;  //Mux channel select Address
				LPI2C0->MTDR = 0x0200; //Generate Stop after Address write
			}
			else if(i2c_state == MUX1_READ_ADDRESS_1)
			{
				i2c_state++;
				LPI2C0->MIER = 0x3F02;
				LPI2C0->MTDR = (4 << 8) | ((0x70<<1)|1); //Repeated start to Read value
				LPI2C0->MTDR = 0x0100;  //Read 2 bytes
			}
			else if(i2c_state == ANGLE_SENSOR1_SLAVE_ADDR_WRITE_1)
			{
				i2c_state++;
				LPI2C0->MIER = 0x3F00;
				LPI2C0->MTDR = (4 << 8) | ((0x36<<1)|0); //Start I2C
				LPI2C0->MIER = 0x3F01;
			}
			else if(i2c_state == ANGLE_SENSOR1_READ_ADDR_WRITE_1)
			{
				i2c_state++;
				LPI2C0->MTDR = 0x0C;  //Mux channel select Address
				LPI2C0->MTDR = 0x0200; //Generate Stop after Address write
			}
			else if(i2c_state == ANGLE_SENSOR1_ADDR_READ_BYTE_1)
			{
				i2c_state++;
				LPI2C0->MIER = 0x3F02;
				LPI2C0->MTDR = (4 << 8) | ((0x36<<1)|1); //Repeated start to Read value
				LPI2C0->MTDR = 0x0100;  //Read 2 bytes
			}
			else if(i2c_state == MUX2_SLAVE_ADDR_WRITE_1)
			{
				i2c_state++;

				LPI2C0->MIER = 0x3F00;

				LPI2C0->MTDR = (4 << 8) | ((0x70<<1)|0); //Start I2C
				LPI2C0->MIER = 0x3F01;
			}
			else if(i2c_state == MUX2_READ_ADDR_WRITE_1)
			{
				i2c_state++;
				LPI2C0->MTDR = 0x05;   //Mux channel select Address
				LPI2C0->MTDR = 0x0200; //Generate Stop after Address write
			}
			else if(i2c_state == MUX2_READ_ADDRESS_1)
			{
				i2c_state++;
				LPI2C0->MIER = 0x3F02;
				LPI2C0->MTDR = (4 << 8) | ((0x70<<1)|1); //Repeated start to Read value
				LPI2C0->MTDR = 0x0100;  //Read 2 bytes
			}
			else if(i2c_state == ANGLE_SENSOR2_SLAVE_ADDR_WRITE_1)
			{
				i2c_state++;
				LPI2C0->MIER = 0x3F00;
				LPI2C0->MTDR = (4 << 8) | ((0x36<<1)|0); //Start I2C
				LPI2C0->MIER = 0x3F01;
			}
			else if(i2c_state == ANGLE_SENSOR2_READ_ADDR_WRITE_1)
			{
				i2c_state++;
				LPI2C0->MTDR = 0x0C;  //Mux channel select Address
				LPI2C0->MTDR = 0x0200; //Generate Stop after Address write
			}
			else if(i2c_state == ANGLE_SENSOR2_ADDR_READ_BYTE_1)
			{
				i2c_state++;
				LPI2C0->MIER = 0x3F02;
				LPI2C0->MTDR = (4 << 8) | ((0x36<<1)|1); //Repeated start to Read value
				LPI2C0->MTDR = 0x0100;  //Read 2 bytes
			}
			else if(i2c_state == MUX1_SLAVE_ADDR_WRITE)
			{
				i2c_state++;
				LPI2C0->MIER = 0x3F00;
				LPI2C0->MTDR = (4 << 8) | ((0x70<<1)|0); //Start I2C
				LPI2C0->MIER = 0x3F01;
			}
			else if(i2c_state == MUX1_READ_ADDR_WRITE)
			{
				i2c_state++;
				LPI2C0->MTDR = 0x04;  //Mux channel select Address
				LPI2C0->MTDR = 0x0200; //Generate Stop after Address write
			}
			else if(i2c_state == MUX1_READ_ADDRESS_2)
			{
				i2c_state++;
				LPI2C0->MIER = 0x3F02;
				LPI2C0->MTDR = (4 << 8) | ((0x70<<1)|1); //Repeated start to Read value
				LPI2C0->MTDR = 0x0100;  //Read 2 bytes
			}
			else if(i2c_state == ANGLE_SENSOR1_SLAVE_ADDR_WRITE_2)
			{
				i2c_state++;
				LPI2C0->MIER = 0x3F00;
				LPI2C0->MTDR = (4 << 8) | ((0x36<<1)|0); //Start I2C
				LPI2C0->MIER = 0x3F01;
			}
			else if(i2c_state == ANGLE_SENSOR1_READ_ADDR_WRITE_2)
			{
				i2c_state++;
				LPI2C0->MTDR = 0x0D;  //Mux channel select Address
				LPI2C0->MTDR = 0x0200; //Generate Stop after Address write
			}
			else if(i2c_state == ANGLE_SENSOR1_ADDR_READ_BYTE_2)
			{
				i2c_state++;
				LPI2C0->MIER = 0x3F02;
				LPI2C0->MTDR = (4 << 8) | ((0x36<<1)|1); //Repeated start to Read value
				LPI2C0->MTDR = 0x0100;  //Read 2 bytes
			}
			else if(i2c_state == MUX2_SLAVE_ADDR_WRITE_2)
			{
				i2c_state++;
				LPI2C0->MIER = 0x3F00;
				LPI2C0->MTDR = (4 << 8) | ((0x70<<1)|0); //Start I2C
				LPI2C0->MIER = 0x3F01;
			}
			else if(i2c_state == MUX2_READ_ADDR_WRITE_2)
			{
				i2c_state++;
				LPI2C0->MTDR = 0x05;   //Mux channel select Address
				LPI2C0->MTDR = 0x0200; //Generate Stop after Address write
			}
			else if(i2c_state == MUX2_READ_ADDRESS_2)
			{
				i2c_state++;
				LPI2C0->MIER = 0x3F02;
				LPI2C0->MTDR = (4 << 8) | ((0x70<<1)|1); //Repeated start to Read value
				LPI2C0->MTDR = 0x0100;  //Read 2 bytes
			}
			else if(i2c_state == ANGLE_SENSOR2_SLAVE_ADDR_WRITE_2)
			{
				i2c_state++;
				LPI2C0->MIER = 0x3F00;
				LPI2C0->MTDR = (4 << 8) | ((0x36<<1)|0); //Start I2C
				LPI2C0->MIER = 0x3F01;
			}
			else if(i2c_state == ANGLE_SENSOR2_READ_ADDR_WRITE_2)
			{
				i2c_state++;
				LPI2C0->MTDR = 0x0D;  //Mux channel select Address
				LPI2C0->MTDR = 0x0200; //Generate Stop after Address write
			}
			else if(i2c_state == ANGLE_SENSOR2_ADDR_READ_BYTE_2)
			{
				i2c_state++;
				LPI2C0->MIER = 0x3F02;
				LPI2C0->MTDR = (4 << 8) | ((0x36<<1)|1); //Repeated start to Read value
				LPI2C0->MTDR = 0x0100;  //Read 2 bytes
			}
			else if(i2c_state == ANGLE_SENSOR_STOP)
			{
				LPI2C0->MIER = 0x3F00;
			}
			else
			{

}
		}
	}

	/* Check for any Error Flags or Status Flags which need to be cleared for seamless data transactions beter MCU & Sensor*/

	if (LPI2C0->MSR & (1 << 8)) //EPF - End Packet Flag Check
	{
		LPI2C0->MSR = 0x100;
	}

	if(LPI2C0->MSR & (1 << 9)) //SDF - Stop Detect Flag Check
	{
		LPI2C0->MSR = 0x200;
	}

	if(LPI2C0->MSR & (1 << 10)) //NACK Detect Flag Check
	{
		LPI2C0->MSR = 0x400;
	}

	if(LPI2C0->MSR & (1 << 11)) //Arbitration Lost Flag Check
	{
		LPI2C0->MSR = 0x800; 	//I2C Reset Required?
	}

	if(LPI2C0->MSR & (1 << 12)) //FIFO Error Flag Check
	{
		LPI2C0->MSR = 0x1000; 	//FIFO Reset Required?
	}

	if(LPI2C0->MSR & (1 << 13)) //PINLOW timeout
	{
		LPI2C0->MSR = 0x2000;
	}


	ENABLE_INTERRUPTS();
}
/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :   SAS_ReadMagnet_Status                      */
/*                                                                 */
/*   FUNCTION BRIEF :   Here Magnets  status are done              */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :   uint8_t                                    */
/*                                                                 */
/*   RETURN VALUES  :   uint16_t                                   */
/*                                                                 */
/*******************************************************************/
uint8_t SAS_ReadMagnet_Status(uint8_t channel)
{

	uint8_t error = 0;
	uint8_t mux_channel_selected = 0;
	LPI2C0->MIER = 0x3C00;
	if (channel == M_PCA9540B_MUX_CHANNEL0)
	{
		mux_channel_selected = M_PCA9540B_MUX_CHANNEL0;
	}
	else if (channel == M_PCA9540B_MUX_CHANNEL1)
	{
		mux_channel_selected = M_PCA9540B_MUX_CHANNEL1;
	}
	/* Select the channel in the Multiplexer M_PCA9540B and assigned the channel to be used 0 or 1 */
	error = LPI2C_Select_MuxChannel(M_PCA9540B, mux_channel_selected,
			&u8RegMuxSelect);
	/* Ensure there is no error and the channel in the Multiplexer M_PCA9540B selected is set correctly */
	if ((u8RegMuxSelect == mux_channel_selected))
	{

		/*Read the M_AS5600 Magnetic sensor register and ensure the magnetic field is detected */
		LPI2C_Read_Location(M_AS5600, M_AS5600_STATUS,
				&u8RegReadValMagPresent);

		/* Only Read the angle if the magnetic field is detected as MD */
		if (!(u8RegReadValMagPresent & M_AS5600_STATUS_MD))
		{
			IsSasHardFault=1;
		}
		else
		{
			//Do nothing
		}
	}
	return(IsSasHardFault);
}
/*******************************************************************************
 Function Name : LPI2C0_IRQs_init
 *******************************************************************************/

