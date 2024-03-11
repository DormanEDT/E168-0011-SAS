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
/*     FILE NAME   :  can_app.c                                                   */
/*                                                                                */
/*     DESCRIPTION :  CAN application module implemented here.                    */
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

/**********************************************************************************/
/*Header File Inclusion                                                           */
/**********************************************************************************/
#include <CAN_app.h>
#include "main.h"
#include "app_sas.h"
/**********************************************************************************/
/*Constant and Variable Declaration                                               */
/**********************************************************************************/

/* Variable Declarations **********************************************/
uint32_t canrxid=0;
uint8_t gCANMailBoxNo = 0;
uint8_t can_msg[8] = { 0 };
uint8_t RxBuff[8];
uint8_t Cal_Stop=0;
uint8_t mem_loop=0;
uint8_t check_mem=0;
uint8_t incr=0;
uint8_t is_WaiteForMTN_FlowCtrl = 0;
uint8_t is_WaiteForGMLAN_FlowCtrl = 0;
uint8_t clear[12]={0};
uint8_t write_cal_data[5]={0};
uint8_t gu8CSAStesterpresentack[8]={0x01,0x7E,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t gu8CSASresponse[8]={0x07,0x61,0xE2,0x00,0x7F,0xD9,0xF5,0x00};
uint8_t gu8CSASresponsepending[8]={0x03,0x7F,0xA6,0x78,0x00,0x00,0x00,0x00};
uint8_t gu8CSASclearcompletemem[8]={0x01,0xE6,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t gu8CSASendzeropoint[8] ={0x01,0x50,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t gu8CSASsendsasoutput[8]={0x0F,0xFF,0x0F,0xFF,0x70,0x00,0x00,0xB9};
uint8_t gu8CSASsendsasoutput2[8]={0x0F,0xFF,0x98,0x00,0x78,0x78,0x78,0x43};
uint8_t gu8CSASsendsasoutput3[8]={0x00,0x01,0x1F,0xFF,0xF0,0x00,0x00,0xB6};
uint8_t gu8CSASdiagnosticack[8]={0x01,0x50,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t byte0_msb=0,byte1_msb;
uint8_t one_time=0;
uint8_t zero_point_set=0;
uint8_t Boot_Enable = RESET;
uint8_t Boot_Step = 0;
uint8_t Tester_Present = FALSE;
uint8_t can_comm_state = TRUE;
uint8_t BOOT_Initial_Response[8]={0x03,0x5A,0xB0,0x13,0xAA,0xAA,0xAA,0xAA};
uint8_t BOOT_Response1[8]= {0x01,0x50,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};
uint8_t BOOT_Response2[8]= {0x01,0x68,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};
uint8_t BOOT_Response3[8]= {0x02,0xE2,0x03,0xAA,0xAA,0xAA,0xAA,0xAA};
uint8_t BOOT_Response4[8]= {0x01,0xE5,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};

uint8_t BOOT_Response5[8]= {0x04,0x67,0x01,0x7D,0x26,0x44,0x4F,0x52};
uint8_t BOOT_Response6[8]= {0x04,0x67,0x02,0x44,0x4F,0x52,0x4D,0x41};
uint8_t BOOT_Response7[8]= {0x02,0x67,0x71,0xAA,0xAA,0xAA,0xAA,0xAA};
/* Variable Declarations **********************************************/

/* Function declarations ********************************************************/

/***********************************************************************************
 *
 *   FUNCTION NAME   : CAN_App_Config
 *
 *   DESCRIPTION     : Application specific Can configuration are done here
 *
 *   PARAMETERS      : None
 *
 *   RETURN VALUES   : None
 *
 ***********************************************************************************/
void CAN_App_Config(void)
{
	baud_rate_t gBaudRate = CAN_BAUD_500K;

	can_rx_msg_t can_app_rx_msgs[4];

	/*Receive Mail box filter1 configuration*/
	can_app_rx_msgs[0].CAN_ID = 0x20;
	can_app_rx_msgs[0].CAN_ID_TYPE = CAN_ID_STD;

	/*Receive Mail box filter2 configuration*/
	can_app_rx_msgs[1].CAN_ID = 0x7B0;
	can_app_rx_msgs[1].CAN_ID_TYPE = CAN_ID_STD;
	/*Receive Mail box filter3 configuration*/
	can_app_rx_msgs[2].CAN_ID = 0x101;
	can_app_rx_msgs[2].CAN_ID_TYPE = CAN_ID_STD;

	/*Receive Mail box filter4 configuration*/
	can_app_rx_msgs[3].CAN_ID = 0x7E3;
	can_app_rx_msgs[3].CAN_ID_TYPE = CAN_ID_STD;

	CAN_Init(gBaudRate, can_app_rx_msgs, CAN_MAX_RX_MSG_FILTERS);
	CAN_Port_Init();
	/*Enable CAN receive Interrupts for configured filters*/
	CAN_ITConfig();
}
/***********************************************************************************
 *
 *   FUNCTION NAME   : CAN_app_Task
 *
 *   DESCRIPTION     : CAN msg will be transmitted according to the received CAN ID
 *
 *   PARAMETERS      : None
 *
 *   RETURN VALUES   : None
 *
 ***********************************************************************************/
void CAN_App_Task(void)
{

	memcpy(can_msg, CAN_App_GetRxBUF(), sizeof(can_msg));


	switch (CAN_App_GetRxID())
	{


	case CSAS524TOOLARBID_SCAN_REQ:        //0X7B0FROMTOOL
		if((can_msg[0] == 0x02) && (can_msg[1] == 0x3E)  && (can_msg[2] == 0x01)  && (can_msg[3] == 0x00)  && (can_msg[4] == 0x00)  && (can_msg[5] == 0x00)  && (can_msg[6] == 0x00)  && (can_msg[7] == 0x00) ) //Not caliberated
		{

		//	CalOk=1;
			CAN_Transmit_msg(&gCANMailBoxNo, CSAS525ACKARBID_SCAN_TRX, CAN_ID_STD,
						      CSAS525_SIZE8BYTE,gu8CSAStesterpresentack);

		}
        else if ((can_msg[0] == 0x02) && (can_msg[1] == 0x21)  && (can_msg[2] == 0xE2)  && (can_msg[3] == 0x00)  && (can_msg[4] == 0x00)  && (can_msg[5] == 0x00)  && (can_msg[6] == 0x00)  && (can_msg[7] == 0x00) )
	    {
			//CalOk=1;
			CAN_Transmit_msg(&gCANMailBoxNo, CSAS525ACKARBID_SCAN_TRX, CAN_ID_STD,
						      CSAS525_SIZE8BYTE,gu8CSASresponse);
		}
        else if ((can_msg[0] == 0x02) && (can_msg[1] == 0xA6)  && (can_msg[2] == 0x00)  && (can_msg[3] == 0x00)  && (can_msg[4] == 0x00)  && (can_msg[5] == 0x00)  && (can_msg[6] == 0x00)  && (can_msg[7] == 0x00) )
        {

		    uint8_t rxpending_counter = 0;
			while ( rxpending_counter <= 2)
			{
			CAN_Transmit_msg(&gCANMailBoxNo, CSAS525ACKARBID_SCAN_TRX, CAN_ID_STD,
						      CSAS525_SIZE8BYTE,gu8CSASresponsepending);
             ++rxpending_counter;
            }

            CAN_Transmit_msg(&gCANMailBoxNo, CSAS525ACKARBID_SCAN_TRX, CAN_ID_STD,
						      CSAS525_SIZE8BYTE,gu8CSASclearcompletemem);
        }
		else if ((can_msg[0] == 0x02) && (can_msg[1] == 0x10)  && (can_msg[2] == 0x01)  && (can_msg[3] == 0x00)  && (can_msg[4] == 0x00)  && (can_msg[5] == 0x00)  && (can_msg[6] == 0x00)  && (can_msg[7] == 0x00) )
		{
			CAN_Transmit_msg(&gCANMailBoxNo, CSAS525ACKARBID_SCAN_TRX, CAN_ID_STD,
						      CSAS525_SIZE8BYTE,gu8CSASendzeropoint);
        }
		else if ((can_msg[0] == 0x02) && (can_msg[1] == 0x10)  && (can_msg[2] == 0x40)  && (can_msg[3] == 0x00)  && (can_msg[4] == 0x00)  && (can_msg[5] == 0x00)  && (can_msg[6] == 0x00)  && (can_msg[7] == 0x00) )
		{

			CAN_Transmit_msg(&gCANMailBoxNo,CSAS525ACKARBID_SCAN_TRX,CAN_ID_STD,
						       CSAS525_SIZE8BYTE,gu8CSASdiagnosticack);
		}

break;

case  CSAS525ABSARBID_SCAN_REQ:
	canrxid=CSAS525ABSARBID_SCAN_REQ;
	decrement_flag=1;
	gu8STSnibble &=~(1<<0);
   	 if (((can_msg[0] & 0x0F)==0x0F) && (can_msg[1] == 0xFF)  && (can_msg[2] == 0x07))
	 {
	   	  /* CLEAR MEMORY COMMAND FROM ABS*/
		byte0_msb=((can_msg[0] & 0xF0)>>4);
		if((byte0_msb % 2))
		{
			for(mem_loop=0,incr=0;mem_loop<(byte0_msb);mem_loop+=2)
			{
				check_mem=8+(incr);
				incr++;
			}
			if(check_mem==((gu8CSASCanDataFrame[2]&0xF0)>>4))
			{
				check_mem=1;
			}
			else
			{
				check_mem=0;
			}
		}
		if((byte0_msb % 2)&&(check_mem))
		{

		//need to reinitialize all angle calculation related variables
			zero_point_set=0;
			CalOk=0;
	//		one_time=0;
			switch(byte0_msb)
			{
				case 1:
					gu8CSASCanDataFrame[2]=0x0F;
					break;
				case 3:
					gu8CSASCanDataFrame[2]=0x1F;
					break;
				case 5:
					gu8CSASCanDataFrame[2]=0x2F;
					break;
				case 7:
					gu8CSASCanDataFrame[2]=0x3F;
					break;
				case 9:
					gu8CSASCanDataFrame[2]=0x4F;
					break;
				case 11:
					gu8CSASCanDataFrame[2]=0x5F;
					break;
				case 13:
					gu8CSASCanDataFrame[2]=0x6F;
					break;
				case 15:
					gu8CSASCanDataFrame[2]=0x7F;
					break;
				default:
					break;
			}
		gu8CSASCanDataFrame[3]=0xFF;
//		gu8CSASCanDataFrame[4]=0x70;
//		gu8CSASCanDataFrame[5]=0x00;
//		gu8CSASCanDataFrame[6]=0x00;
		gu8CSASCanDataFrame[7]=CSAS_checksum_Byte(gu8CSASCanDataFrame);

		}
		else
		{
			//Do nothing
		}
	 }

	 else if (((can_msg[0] & 0x0F)==0x08) && (can_msg[1] == 0x00)  && (can_msg[2] == 0x07))
		//	 && (can_msg[6] ==( can_msg[0]+can_msg[1]+can_msg[2]+can_msg[3]+0x20+7)))
		 {

		 /*********************************TEST MODE***************************************/
		        byte0_msb=((can_msg[0] & 0xF0)>>4);
		 		if((byte0_msb % 2) )
		 		{
		// 		 CalOk=0;
		 	 	 Cal_Stop=0;
				gu16FinalSasAngle=0;
				IsCalibarationComp=1;
				gu8STSnibble   &=~(1<<0); //Test mode clear STS0 bit
				one_time=0;
				switch(byte0_msb)
				{
					case 1:
						gu8CSASCanDataFrame[2] &=0x0F;
						gu8CSASCanDataFrame[2] |=(0x09<<4);
					break;
					case 3:
						gu8CSASCanDataFrame[2] &=0x0F;
						gu8CSASCanDataFrame[2] |=(0x0A<<4);
					break;
					case 5:
						gu8CSASCanDataFrame[2] &=0x0F;
						gu8CSASCanDataFrame[2] |=(0x0B<<4);
					break;
					case 7:
						gu8CSASCanDataFrame[2] &=0x0F;
						gu8CSASCanDataFrame[2] |=(0x0C<<4);
					break;
					case 9:
						gu8CSASCanDataFrame[2] &=0x0F;
						gu8CSASCanDataFrame[2] |=(0x0D<<4);
					break;
					case 11:
						gu8CSASCanDataFrame[2] &=0x0F;
						gu8CSASCanDataFrame[2] |=(0x0E<<4);
					break;
					case 13:
						gu8CSASCanDataFrame[2] &=0x0F;
						gu8CSASCanDataFrame[2] |=(0x0F<<4);
					break;
					case 15:
						gu8CSASCanDataFrame[2] &=0x0F;
						gu8CSASCanDataFrame[2] |=(0x08<<4);
					break;
					default:
						break;
				}
				gu8CSASCanDataFrame[3] =0x00;
//				gu8CSASCanDataFrame[4] =0x70;
//				gu8CSASCanDataFrame[5] =0x00;
//				gu8CSASCanDataFrame[6] =0x00;
				gu8CSASCanDataFrame[7] =CSAS_checksum_Byte(gu8CSASCanDataFrame);

		 		}
	 }
	 else if (((can_msg[0] & 0x0F)==0x00) && (can_msg[2] == 0x07))
			// &&(can_msg[6] ==( can_msg[0]+can_msg[1]+can_msg[2]+can_msg[3]+0x20+7)))
		 {
		 /*********************************Zero Point MODE***************************************/
			byte0_msb=((can_msg[0] & 0xF0)>>4);
			if((byte0_msb % 2))
			{
				for(mem_loop=0,incr=0;mem_loop<(byte0_msb);mem_loop+=2)
				{
					check_mem=8+(incr);
					incr++;
				}
				if(check_mem==((gu8CSASCanDataFrame[2]&0xF0)>>4))
				{
					check_mem=1;
				}
				else
				{
					check_mem=0;
				}
			}
			if((byte0_msb % 2)&&(check_mem)&&(one_time==0) && (CalOk==0))
			{
				one_time=1;
				zero_point_set=1;
				start_address=EERAMBASE;
				FLASH_DRV_EWrite(start_address,12,clear);
				CalOk=1;
				write_cal_data[0]=CalOk;
				write_cal_data[1]=(uint8_t)gu16AS5600s1Angle;;
				write_cal_data[2]=(uint8_t)(gu16AS5600s1Angle>>8);
				write_cal_data[3]=(uint8_t)(gu16AS5600s2Angle);
				write_cal_data[4]=(uint8_t)(gu16AS5600s2Angle>>8);
				FLASH_DRV_EWrite(start_address,5,write_cal_data);
				gu16CounterClockWiseCounterS1=15000;
				gu16CounterClockWiseCounterS2=15000;
				gu16ClockWiseCounterS1=15000;
				gu16ClockWiseCounterS2=15000;
				isFlagCCWS1=0;
				isFlagCCWS2=0;
				isFlagCWS1 =0;
				isFlagCWS2 =0;
				isFlagpoweroff=0x00;
				IsSasAngleDataValid=1;
				IsSasAngleExceed=0;
				IsSasHardFault=0;
				IsSasAngleoutphase=0;
				gu16FinalSasAngle=0;
				gu8CSASCanDataFrame[0]=0x0F;
				gu8CSASCanDataFrame[1]=0xFF;
				gu8CSASCanDataFrame[2] &=0x70;  //clear SAZS and SSAZ bit
				gu8CSASCanDataFrame[3] =can_msg[1];
			}
			else if((byte0_msb % 2))
			{
				gu8CSASCanDataFrame[2] &=0x70;  //clear SAZS and SSAZ bit
			}
			else
			{
				//Do nothing
		    }
		 }
		break;
case BOOT_REQ_ID:

    if((can_msg[0] == 0x02) && (can_msg[1] == 0x27))
    {
		if(can_msg[2]==0x01)
		{
			if(Boot_Step==0x00)
			{
			  Boot_Step |= 0x01;
			  //CAN_Transmit(BOOT_RES_ID,0,0,8,&BOOT_Response5);
			CAN_Transmit_msg(&gCANMailBoxNo, BOOT_RES_ID, CAN_ID_STD,8, BOOT_Response5);

			}
		}
		else if( (can_msg[2]==0x02) && (can_msg[3]==0xBE) && (can_msg[4]==0x05) && (can_msg[5]==0x4D) && (can_msg[6]==0x41) && (can_msg[7]==0x4E))
		{
			if(Boot_Step==0x01)
			{
			  Boot_Step |= 0x02;
			  //  CAN_Transmit(BOOT_RES_ID,0,0,8,&BOOT_Response6);
			  CAN_Transmit_msg(&gCANMailBoxNo, BOOT_RES_ID, CAN_ID_STD,8, BOOT_Response6);

			}
		}
		else if ( (can_msg[2]==0x44) && (can_msg[3]==0x4F) && (can_msg[4]==0x52) && (can_msg[5]==0x4D) && (can_msg[6]==0x41) && (can_msg[7]==0x4E))
		{
			if(Boot_Step==0x03)
			{
				//CAN_Transmit(BOOT_RES_ID,0,0,8,&BOOT_Response7);
				CAN_Transmit_msg(&gCANMailBoxNo, BOOT_RES_ID, CAN_ID_STD,8, BOOT_Response7);
				Boot_Enable = SET;

			}
		}
    }
break;
case TESTER_ID:

	if((can_msg[0] == 0xFE) && (can_msg[1] == 0x01) )
	{
		if(can_msg[2] == 0x3E)
		{
		  Tester_Present = TRUE;
		}
		else if(can_msg[2] == 0x20)
		{
		  can_comm_state = TRUE;
		  Tester_Present = FALSE;
		}
		else if(can_msg[2] == 0x28)
		{
			//   CAN_Transmit(BOOT_RES_ID,0,0,8,&BOOT_Response2);
			can_comm_state = FALSE;
			CAN_Transmit_msg(&gCANMailBoxNo, BOOT_RES_ID, CAN_ID_STD,8, BOOT_Response2);
		}
		else if(can_msg[2] == 0xA2)
		{
			// CAN_Transmit(BOOT_RES_ID,0,0,8,&BOOT_Response3);
			CAN_Transmit_msg(&gCANMailBoxNo, BOOT_RES_ID, CAN_ID_STD,8, BOOT_Response3);

		}
		else
		{

		}

	}
	else if((can_msg[0] == 0xFE) && (can_msg[1] == 0x02) )
	{
		if((can_msg[2] == 0x1A) && (can_msg[3] == 0xB0))
		{
			//CAN_Transmit(BOOT_RES_ID,0,0,8,&BOOT_Initial_Response);
			CAN_Transmit_msg(&gCANMailBoxNo, BOOT_RES_ID, CAN_ID_STD,8, BOOT_Initial_Response);

		}
		else if((can_msg[2] == 0x10) && (can_msg[3] == 0x02))
		{
			// CAN_Transmit(BOOT_RES_ID,0,0,8,&BOOT_Response1);
			CAN_Transmit_msg(&gCANMailBoxNo, BOOT_RES_ID, CAN_ID_STD,8, BOOT_Response1);

		}
		else if((can_msg[2] == 0xA5) && (can_msg[3] == 0x01))
		{
			// CAN_Transmit(BOOT_RES_ID,0,0,8,&BOOT_Response4);
			CAN_Transmit_msg(&gCANMailBoxNo, BOOT_RES_ID, CAN_ID_STD,8, BOOT_Response4);

		}
		else
		{

		}
	}
	else
	{

	}
break;

	default:
		break;
	}

}

/***********************************************************************************
 *
 *   FUNCTION NAME   : CAN_App_GetRxID
 *
 *   DESCRIPTION     : Based on the ID type CAN ID will be generated here
 *
 *   PARAMETERS      : None
 *
 *   RETURN VALUES   : CAN ID
 *
 ***********************************************************************************/
uint32_t CAN_App_GetRxID(void)
{
	uint32_t RxCanID = RxID >> 18;
	if (RxIDType)
	{
		RxCanID = RxID;
	}
	RxID = 0;
	return RxCanID;
}
/***********************************************************************************
 *
 *   FUNCTION NAME   : CAN_App_GetRxBUF
 *
 *   DESCRIPTION     : CAN msg received will be copied to 8 byte array here
 *
 *   PARAMETERS      : None
 *
 *   RETURN VALUES   : Received Data in 8 Byte array
 *
 ***********************************************************************************/
uint8_t* CAN_App_GetRxBUF(void)
{
	for (int i = 0; i < 2; i++)
	{
		RxBuff[(i * 4)] = RxDATA[i] >> 24;
		RxBuff[(i * 4) + 1] = RxDATA[i] >> 16;
		RxBuff[(i * 4) + 2] = RxDATA[i] >> 8;
		RxBuff[(i * 4) + 3] = RxDATA[i];
	}
	return RxBuff;

}

/***********************************************************************************
 *
 *   FUNCTION NAME   : CAN_App_Rx_Handler
 *
 *   DESCRIPTION     : CAN msg received Interrupts handled here
 *
 *   PARAMETERS      : Interrupt triggered Mail Box number
 *
 *   RETURN VALUES   : None
 *
 ***********************************************************************************/
void CAN_App_Rx_Handler(uint32_t rx_int_level)
{
	if(Tester_Present == FALSE)
	{
		if (rx_int_level & (1 << CAN_MSG_1_RX_MB_NO))
		{
			CAN_Receive_msg(CAN_MSG_1_RX_MB_NO);
		}
	/*Check for 5th mail box receive interrupt*/
		if (rx_int_level & (1 << CAN_MSG_2_RX_MB_NO))
		{
			CAN_Receive_msg(CAN_MSG_2_RX_MB_NO);
		}
	}
	/*Check for 6th mail box receive interrupt*/
	if (rx_int_level & (1 << CAN_MSG_3_RX_MB_NO))
	{
		CAN_Receive_msg(CAN_MSG_3_RX_MB_NO);
	}
	/*Check for 7th mail box receive interrupt*/
	if (rx_int_level & (1 << CAN_MSG_4_RX_MB_NO))
	{
		CAN_Receive_msg(CAN_MSG_4_RX_MB_NO);
	}
}
