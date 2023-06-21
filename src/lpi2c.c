
//#include "device_registers.h"	/* include peripheral declarations */
#include "main.h"

/*******************************************************************************
 * Static variables
 *******************************************************************************/
static uint8_t error = 0;

void LPI2C_init(void)
{

	PCC->PCCn[PCC_LPI2C0_INDEX] |= PCC_PCCn_PCS(2) /* Clk src: SIRCDIV2_CLK Assumed as 8mhz  */
	| PCC_PCCn_CGC_MASK; /* Enable clock for LPI2C0 */

	LPI2C0->MCFGR1 = LPI2C_MCFGR1_PRESCALE(2) | LPI2C_MCFGR1_IGNACK_MASK;
	/* Prescale = 4 &Ignore NACK*/
	//LPI2C0->MCFGR1 = LPI2C_MCFGR1_PRESCALE(0) | LPI2C_MCFGR1_IGNACK_MASK;
	 LPI2C0->MCFGR2=LPI2C_MCFGR2_BUSIDLE(256);
	//LPI2C0->MCCR0 = 0x02040303; //250K (8000000/(2^2*(3+3+2))) == 8000000/32 = 250,000 - 250K
	LPI2C0->MCCR0 = 0x01040303;   //400k
	LPI2C0->MFCR = LPI2C_MFCR_TXWATER(0) /* Transmitter Water mark set to 0*/
	| LPI2C_MFCR_RXWATER(3); /* Receiver Water mark set to 3*/

	LPI2C0->MCR |= LPI2C_MCR_MEN_MASK /* Enable LPI2C as master */
	| LPI2C_MCR_DBGEN_MASK;
	// Master Interrupt Enable Register (MIER)
	LPI2C0->MIER = 0x3C00;
	// [14] DMIE = 0  (Data match interrupt disabled)
	// [13] PLTIE = 1 (Pin low timeout interrupt enabled)
	// [12] FEIE = 1  (FIFO error interrupt enabled)
	// [11] ALIE = 1  (Arbitration lost interrupt enabled)
	// [10] NDIE = 1  (NACK detect interrupt enabled)
	// [9]  SDIE = 0  (STOP detect interrupt disabled)
	// [8]  EPIE = 0  (End packet interrupt disabled)

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
void LPI2C_DataToRead(uint8_t data)
{
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

uint8_t LPI2C_Read_Location(uint8_t target_address, uint8_t register_address,
							uint8_t* data)
{
	uint8_t error = 0;

	while((LPI2C_Check_Busy()));
	LPI2C_Generate_Start_Ack(target_address);
	while((LPI2C_Check_Busy()));
	LPI2C_AddressToRead(register_address);
	while((LPI2C_Check_Busy()));
	LPI2C_DataToRead(target_address);
	while((LPI2C_Check_Busy()));
	LPI2C_Receive_Data(data, 1);
	LPI2C_Generate_Stop();

	return (error);
}

uint8_t LPI2C_Select_MuxChannel(uint8_t target_address, uint8_t mux_channel,uint8_t* data)
{
	uint8_t error = 0;
	if (LPI2C_Check_Busy())
		while((LPI2C_Check_Busy()));
	LPI2C_Generate_Start_Ack(target_address);
	if (LPI2C_Check_Busy())
		while((LPI2C_Check_Busy()));
	LPI2C_Transmit_Data(mux_channel);
	LPI2C_Generate_Stop();
	if (LPI2C_Check_Busy())
		while((LPI2C_Check_Busy()));
	LPI2C_DataToRead(target_address);
	if (LPI2C_Check_Busy())
		while((LPI2C_Check_Busy()));
	LPI2C_Receive_Data(data, 1);
	LPI2C_Generate_Stop();

	return (error);

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


/*******************************************************************************
 Function Name : LPI2C0_Master_IRQHandler
 *******************************************************************************/
void LPI2C0_Master_IRQHandler(void)
{
	DISABLE_INTERRUPTS();
	if (LPI2C0->MSR & (1 << 10))
	{
		error |= (1 << NDF);
		// NACK/ACK detected and expecting ACK/NACK for address byte
		// When set, the master will transmit a STOP condition and will not initiate a new START
		// condition until this flag has been cleared.
		LPI2C0->MSR = 0x400;     // clear NDF
	}

	if (LPI2C0->MSR & (1 << 11)) {
		error |= (1 << ALF);
		/* When the Arbitration Lost Flag sets, the LPI2C master will release the I2C bus (go idle), and the LPI2C
		 * master will not initiate a new START condition until the Arbitration Lost Flag has been cleared.
		 */
		LPI2C0->MCR |= (1 << 8);  // reset transmit FIFO
		LPI2C0->MCR |= (1 << 9);  // reset receive  FIFO
		LPI2C0->MSR = 0x800;      // clear ALF
	}

	if (LPI2C0->MSR & (1 << 12)) {
		error |= (1 << FEF);
		/* Detects an attempt to send or receive data without first generating a (repeated) START condition. This
		 * can occur if the transmit FIFO underflows when the AUTOSTOP bit is set. When FIFO Error Flag is set,
		 * the LPI2C master will send a STOP condition (if busy), and will not initiate a new START condition until
		 * FIFO Error Flag has been cleared.
		 */
		LPI2C0->MCR |= (1 << 8);  // reset transmit FIFO
		LPI2C0->MCR |= (1 << 9);  // reset receive  FIFO
		LPI2C0->MSR = 0x1000;     // clear FEF
	}

	if (LPI2C0->MSR & (1 << 13)) {
		error |= (1 << PLTF);
		/* Pin low timeout has occurred
		 * SCL (or SDA if MCFGR1[TIMECFG] is set) is low for (MCFGR3[TIMELOW] * 256) prescaler cycles without a pin transition.
		 * Software must respond to the MSR[PTLF] flag to terminate the existing command
		 * either cleanly (by clearing MCR[MEN]), or abruptly (by setting MCR[SWRST]).
		 */

		LPI2C0->MSR = 0x2000; // Clear PLTF, this flag cannot be cleared when the error condition persists

		LPI2C0->MCR |= (1 << 1); // [1] RST clear all LPI2C registers except for MCR
		LPI2C0->MCR &= ~(1 << 1);  // RST remains set until cleared by SW.
		LPI2C_init();
	}
	ENABLE_INTERRUPTS();
}

/*******************************************************************************
 Function Name : LPI2C0_IRQs_init
 *******************************************************************************/

