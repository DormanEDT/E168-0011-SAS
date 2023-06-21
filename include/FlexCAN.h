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

#ifndef FLEXCAN_H_
#define FLEXCAN_H_

#include <CAN_app.h>
#include "device_registers.h"

#define MSG_BUF_SIZE  4		/* Msg Buffer Size. (CAN 2.0AB: 2 hdr +  2 data= 4 words) */
#define MAX_TX_MAIL_BOX_NO 4

/*! @brief Baud rate names. */
typedef enum {
	CAN_BAUD_500K = 0u, /*!< Baud rate 500kbps                     */
	CAN_BAUD_250K = 1u, /*!< Baud rate 250kbps                     */
	CAN_BAUD_125K = 2u, /*!< Baud rate 125kbps                     */
} baud_rate_t;

typedef enum {
	CAN_ID_STD = 0u,
	CAN_ID_EXT = 1u,
} can_id_type_t;

typedef struct {
	uint32_t CAN_ID;
	can_id_type_t CAN_ID_TYPE;
} can_rx_msg_t;
void CAN_Init(baud_rate_t baud_rate, can_rx_msg_t can_rx_flt[], uint8_t rx_msg_flt_cnt);
void CAN_Port_Init(void);
void CAN_ITConfig(void);
void CAN_Transmit_msg(uint8_t *mb_no, uint32_t can_id, can_id_type_t id_type, uint8_t dlc, uint8_t data[]);
void CAN_Receive_msg(uint8_t mb_no);
void CAN0_ORed_0_31_MB_IRQHandler(void);

#endif /* FLEXCAN_H_ */
