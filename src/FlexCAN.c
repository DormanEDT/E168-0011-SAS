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

#include <CAN_app.h>
#include "main.h"

uint32_t RxCODE; /*< Received message buffer code 			*/
uint32_t RxID; /*< Received message ID 					*/
uint32_t RxLENGTH; /*< Recieved message number of data bytes 	*/
uint32_t RxDATA[2]; /*< Received message data (2 words) 		*/
uint32_t RxTIMESTAMP; /*< Received message time 					*/
uint8_t RxIDType;


/* Implementation of CAN0 IRQ handler for interrupts indicating a successful
 transmission or reception for Message Buffers 0-31. */
void CAN0_ORed_0_31_MB_IRQHandler(void)
{
	uint32_t CAN_rx_int_status = 0;
	CAN_rx_int_status = (uint32_t)CAN0->IFLAG1;
	CAN_App_Rx_Handler(CAN_rx_int_status);
}

void CAN_Init(baud_rate_t baud_rate, can_rx_msg_t can_rx_flt[], uint8_t rx_msg_flt_cnt) {
	uint8_t lPresdiv = 0;
	uint32_t i = 0;
	uint8_t cnt = 0;

	PCC->PCCn[PCC_FlexCAN0_INDEX] |= PCC_PCCn_CGC_MASK; /* CGC=1: enable clock to FlexCAN0 */
	CAN0->MCR |= CAN_MCR_MDIS_MASK; /* MDIS=1: Disable module before selecting clock 	*/
	CAN0->CTRL1 |= CAN_CTRL1_CLKSRC_MASK; /* CLKSRC=0: Clock Source = Internal 		    */
	//CAN0->CTRL1 &= ~CAN_CTRL1_CLKSRC_MASK; /* CLKSRC=0: Clock Source = SOSCDIV2		    */
	CAN0->MCR &= ~CAN_MCR_MDIS_MASK; /* MDIS=0; Enable module config. (Sets FRZ, HALT)	*/

	while (!((CAN0->MCR & CAN_MCR_FRZACK_MASK) >> CAN_MCR_FRZACK_SHIFT)) {
	}
	/*!
	 * Good practice:
	 * ===================================================
	 * wait for FRZACK=1 on freeze mode entry/exit
	 */
#ifdef BOARD_EVB
	switch (baud_rate) {
	case CAN_BAUD_500K:
		lPresdiv = 4; 	/* Sclock = PEclock/(PRESDIV+1) = 40MHz/(4+1) =  8MHz*/
		break;
	case CAN_BAUD_250K:
		lPresdiv = 9; 	/* Sclock = PEclock/(PRESDIV+1) = 40MHz/(9+1) =  4MHz*/
		break;
	case CAN_BAUD_125K:
		lPresdiv = 19; 	/* Sclock = PEclock/(PRESDIV+1) = 40MHz/(19+1) =  2MHz*/
		break;
	default:
		break;
	}
#else
	switch (baud_rate) {
	case CAN_BAUD_500K:
		//lPresdiv = 1; 	/* Sclock = PEclock/(PRESDIV+1) = 16MHz/(1+1) =  8MHz*/
		lPresdiv = 5; 	/* Sclock = PEclock/(PRESDIV+1) = 48MHz/(5+1) =  8MHz*/
		break;
	case CAN_BAUD_250K:
		lPresdiv = 3; 	/* Sclock = PEclock/(PRESDIV+1) = 16MHz/(3+1) =  4MHz*/
		break;
	case CAN_BAUD_125K:
		lPresdiv = 7; 	/* Sclock = PEclock/(PRESDIV+1) = 16MHz/(7+1) =  2MHz*/
		break;
	default:
		break;
	}
#endif
	CAN0->CTRL1 = 0 | CAN_CTRL1_PRESDIV(lPresdiv) /* If PRESDIV=1: Sclock=PEclock/(PRESDIV+1) = 16MHz/2 = 8MHz    */
												  /* If PRESDIV=3: Sclock=PEclock/(PRESDIV+1) = 16MHz/4 = 4MHz   */
												  /* If PRESDIV=7: Sclock=PEclock/(PRESDIV+1) = 16MHz/8 = 2MHz  */
					| CAN_CTRL1_PSEG2(3) 		  /* Configure for 500/250/125 KHz bit time 						*/
					| CAN_CTRL1_PSEG1(3) 		  /* Tq freq = 16 Tq x 500 KHz bit time = 8MHz / 16 Tq x 250 KHz	bit time = 4MHz / 16 Tq x 125 KHz bit time = 2MHz*/
					| CAN_CTRL1_PROPSEG(6) 		  /* PROPSEG= Prop_Seg - 1 = 7 - 1 = 6 							*/
					| CAN_CTRL1_RJW(3) 			  /* PSEG2 = Phase_Seg2 - 1 = 4 - 1 = 3 							*/
					| CAN_CTRL1_SMP(1); 		  /* PSEG1 = PSEG2 = 3 											*/
	/* RJW: since Phase_Seg2 >=4, RJW+1=4 so RJW=3. 				*/
	/* SMP = 1: use 3 bits per CAN sample 							*/
	/* CLKSRC=0 (unchanged): Fcanclk= Fosc= 8 MHz 					*/
	/* CAN0: clear 32 msg bufs x 4 words/msg buf = 128 words */
	for (i = 0; i < 128; i++) {
		/* Clear msg buf word */
		CAN0->RAMn[i] = 0;
	}
	/* In FRZ mode, init CAN0 16 msg buf filters */
	for (i = 0; i < 16; i++) {
		/* Check all ID bits for incoming messages */
		CAN0->RXIMR[i] = 0xFFFFFFFF;
	}
	/* Global acceptance mask: check all ID bits 	*/
	CAN0->RXMGMASK = 0x1FFFFFFF;

	for (cnt = 0; cnt < rx_msg_flt_cnt; cnt++) {
		CAN0->RAMn[(4 + cnt) * MSG_BUF_SIZE + 0] = 0x04000000 | (can_rx_flt[cnt].CAN_ID_TYPE << CAN_WMBn_CS_IDE_SHIFT);

		if (can_rx_flt[cnt].CAN_ID_TYPE == CAN_ID_STD) {
			CAN0->RAMn[(4 + cnt) * MSG_BUF_SIZE + 1] = can_rx_flt[cnt].CAN_ID
					<< 18;
		} else {
			CAN0->RAMn[(4 + cnt) * MSG_BUF_SIZE + 1] = can_rx_flt[cnt].CAN_ID;
		}

		CAN0->IFLAG1 |= 1 << (4 + cnt); /* Clear CAN 0 MB flag without clearing others*/
		CAN0->IMASK1 |= 1 << (4 + cnt); /* Enable CAN 0 MB Receive Interrupt flag for specific MB*/
	}

	/* PRIO = 0: CANFD not used */
	CAN0->MCR = 0x0000001F; /* Negate FlexCAN 1 halt state for 32 MBs */

	while ((CAN0->MCR && CAN_MCR_FRZACK_MASK) >> CAN_MCR_FRZACK_SHIFT) {
	}
	/* Good practice: wait for FRZACK to clear (not in freeze mode) */

	while ((CAN0->MCR && CAN_MCR_NOTRDY_MASK) >> CAN_MCR_NOTRDY_SHIFT) {
	}
	/* Good practice: wait for NOTRDY to clear (module ready) */

}

void CAN_Port_Init(void) {
	/*!
	 * Pins definitions
	 * ===================================================
	 *
	 * Pin number        | Function
	 * ----------------- |------------------
	 * PTE4              | CAN0_RX
	 * PTE5              | CAN0_TX
	 * PTD3			     | CAN_STANDBY
	 */
	PORTE->PCR[4] |= PORT_PCR_MUX(5); /* Port E4: MUX = ALT5, CAN0_RX */
	PORTE->PCR[5] |= PORT_PCR_MUX(5); /* Port E5: MUX = ALT5, CAN0_TX */
	PORTD->PCR[2] = PORT_PCR_MUX(1); /* Port D2: CAN STANDBY PIN; MUX = GPIO                 */
	PTD->PDDR |= 1 << 2; /* Port D3: Data direction = output  */
	PTD->PSOR |= 1 << 2;
	PTD->PCOR |= 1 << 2;
}

void CAN_ITConfig(void)
{
	S32_NVIC->ICPR[0] = (uint32_t)1 << CAN0_ORed_0_31_MB_IRQn; /* IRQ11-FlexCAN: buffer 0-31 clear pending IRQ*/
	S32_NVIC->ISER[0] = (uint32_t)1 << CAN0_ORed_0_31_MB_IRQn; /* IRQ11-FlexCAN: buffer 0-31 rx/tx IRQ*/
	S32_NVIC->IPR[2] = S32_NVIC_IPR_PRI_3(0x03);
}

void CAN_Transmit_msg(uint8_t *mb_no, uint32_t can_id, can_id_type_t id_type,
		uint8_t dlc, uint8_t data[]) {
	/*! Assumption:
	 * =================================
	 * Message buffer CODE is INACTIVE
	 */
	CAN0->IFLAG1 = 1 << *mb_no;
	//CAN0->IMASK1 = 1 << *mb_no; //Uncomment this to enable tx success Interrupt

	CAN0->RAMn[*mb_no * MSG_BUF_SIZE + 2] = ((uint32_t) data[0] << 24)
			| ((uint32_t) data[1] << 16) | ((uint32_t) data[2] << 8)
			| ((uint32_t) data[3]); //0xA5112233; /* MB0 word 2: data word 0 */
	CAN0->RAMn[*mb_no * MSG_BUF_SIZE + 3] = ((uint32_t) data[4] << 24)
			| ((uint32_t) data[5] << 16) | ((uint32_t) data[6] << 8)
			| ((uint32_t) data[7]); //0x44556677; /* MB0 word 3: data word 1 */

	if (id_type == CAN_ID_STD) {
		CAN0->RAMn[*mb_no * MSG_BUF_SIZE + 1] = can_id << 18;
	} else {
		CAN0->RAMn[*mb_no * MSG_BUF_SIZE + 1] = can_id;
	}

	CAN0->RAMn[*mb_no * MSG_BUF_SIZE + 0] = 0x0C400000
			| (dlc << CAN_WMBn_CS_DLC_SHIFT)| (id_type << CAN_WMBn_CS_IDE_SHIFT);

	(*mb_no)++;

	if (*mb_no >= MAX_TX_MAIL_BOX_NO) {
		*mb_no = 0;
	}
	/* MB0 word 0: 								*/
	/* EDL,BRS,ESI=0: CANFD not used 				*/
	/* CODE=0xC: Activate msg buf to transmit 		*/
	/* IDE=0: Standard ID 							*/
	/* SRR=1 Tx frame (not req'd for std ID) 		*/
	/* RTR = 0: data, not remote tx request frame	*/
	/* DLC = 8 bytes 								*/

}

void CAN_Receive_msg(uint8_t mb_no)
{
	/*! Receive msg from msg buffer
	 * =============================================
	 */
	uint8_t j;
	uint32_t dummy;

	RxCODE = (CAN0->RAMn[mb_no * MSG_BUF_SIZE + 0] & 0x07000000) >> 24; /* Read CODE field */
	RxIDType = (CAN0->RAMn[mb_no * MSG_BUF_SIZE + 0] >> CAN_WMBn_CS_IDE_SHIFT) & 0x01;
	RxID = (CAN0->RAMn[mb_no * MSG_BUF_SIZE + 1] & CAN_WMBn_ID_ID_MASK)
			>> CAN_WMBn_ID_ID_SHIFT; /* Read ID 			*/
	RxLENGTH = (CAN0->RAMn[mb_no * MSG_BUF_SIZE + 0] & CAN_WMBn_CS_DLC_MASK)
			>> CAN_WMBn_CS_DLC_SHIFT; /* Read Message Length */

	for (j = 0; j < 2; j++) { /* Read two words of data (8 bytes) */
		RxDATA[j] = CAN0->RAMn[mb_no * MSG_BUF_SIZE + 2 + j];
	}
	RxTIMESTAMP = (CAN0->RAMn[mb_no * MSG_BUF_SIZE + 0] & 0x000FFFF);
	dummy = CAN0->TIMER; /* Read TIMER to unlock message buffers */

	CAN0->IFLAG1 |= ((uint32_t)1) << mb_no;
}

