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
/*     FILE NAME   :  can_app.h                                                   */
/*                                                                                */
/*     DESCRIPTION :  CAN application module declarations                         */
/*                                                                                */
/* +--------------+---------+---------------------------------------------------+ */
/* |Rev. HISTORY  |  DATE   |           MODIFIED POINT                          | */
/* +--------------+---------+---------------------------------------------------+ */
/* |    0.1       |05-JUL-22| Initial Version                                   | */
/* +--------------+---------+---------------------------------------------------+ */
/**********************************************************************************/

/**********************************************************************************/
/*Modifications                                                                   */
/**********************************************************************************/

#ifndef CAN_APP_H_
#define CAN_APP_H_

/**********************************************************************************/
/*Header File Inclusion                                                           */
/**********************************************************************************/
#include "main.h"

/**********************************************************************************/
/*Constant and Variable Declaration                                               */
/**********************************************************************************/
#define CAN_ID_555  0x555
#define CAN_ID_777	0x777

#define CAN_ID_EXT1 0x1E12FF13
#define CAN_ID_EXT2	0x12F85450

#define CAN_MAX_RX_MSG_FILTERS 4
#define CAN_MSG_1_RX_MB_NO 4
#define CAN_MSG_2_RX_MB_NO 5
#define CAN_MSG_3_RX_MB_NO 6
#define CAN_MSG_4_RX_MB_NO 7
#define CSAS524TOOLARBID_SCAN_REQ  (0X7B0)
#define CSAS525ACKARBID_SCAN_TRX 0X7B8
#define CSAS525ABSARBID_SCAN_REQ 0X20
#define CSAS525SASARBID_SCAN_TRX 0X25
#define TESTER_ID               0x101
#define BOOT_REQ_ID             0x7E3
#define BOOT_RES_ID             0x7EB
#define CSAS525_SIZE8BYTE 0x08
extern uint32_t RxCODE; /*< Received message buffer code 			*/
extern uint32_t RxID; /*< Received message ID 					*/
extern uint32_t RxLENGTH; /*< Recieved message number of data bytes 	*/
extern uint32_t RxDATA[2]; /*< Received message data (2 words) 		*/
extern uint32_t RxTIMESTAMP; /*< Received message time 					*/
extern uint8_t RxIDType;
extern uint8_t write_cal_data[5];
extern uint8_t Cal_Stop;
extern uint32_t canrxid;
extern uint8_t gCANMailBoxNo;
extern uint8_t one_time;
extern uint8_t Boot_Enable;
extern uint8_t can_comm_state;
/* Function declarations ********************************************************/
void CAN_App_Config(void);
void CAN_App_Rx_Handler(uint32_t rx_int_level);
void CAN_App_Task(void);
uint32_t CAN_App_GetRxID(void);
uint8_t* CAN_App_GetRxBUF(void);
#endif /* CAN_APP_H_ */
