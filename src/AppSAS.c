/*
 * AppSAS.c
 *
 *      Created on: 27-Jul-2022
 *      Author: Admin
 */

#include "main.h"
#include "lpi2c.h"
uint8_t flag_anticlockwise	=0;
uint8_t	flag_clockwise		=0;
/*****************************************************************************
 *
 * Function Name			: sleep_actions
 * Function Description		: This function for handling the sleep_actions
 * Parameters				: None
 * Return value				: None
 *
 *****************************************************************************/
void sleep_actions(void)
{
	gu8STSnibble=1;
	gu8CSASCanDataFrame[0]|=(gu8STSnibble<<4);
	CAN_Transmit_msg(&gCANMailBoxNo, CSASGRP1ARBID_DATA, CAN_ID_STD,
	        		CSAS_SIZE8BYTE, gu8CSASCanDataFrame);
	ADC_Disable();
	I2C_SENSOR_OFF;
	CAN_TRANS_OFF;
	PIN_Pull_Select(PORTA,2,PULL_DOWN);
	PIN_Pull_Select(PORTA,3,PULL_DOWN);
	PIN_Interrupt_Enable(PORTB,2,FALLING_EDGE); //External interrupt

}
/*****************************************************************************
 *
 * Function Name			: wakeup_actions
 * Function Description		: This function for handling the wakeup_actions
 * Parameters				: None
 * Return value				: None
 *
 *****************************************************************************/
void wakeup_actions(void)
{
	one_time=0;
	gu8CSASCanDataFrame[2]&=0x7F;
	gu8CSASCanDataFrame[2]|=0x80;
	PIN_Interrupt_Disable(PORTB,2);
	ADC_Init();
	I2C_SENSOR_ON;
	CAN_TRANS_ON;
	PIN_Pull_Select(PORTA,2,PULL_UP);
	PIN_Pull_Select(PORTA,3,PULL_UP);
	//check magnet is present or not
	SAS_ReadMagnet_Status(M_PCA9540B_MUX_CHANNEL0);
	SAS_ReadMagnet_Status(M_PCA9540B_MUX_CHANNEL1);


}
void App_SAS_Operations(void)
{
    if((!CalOk) )
     {
   	//CalOk=1;//for testing
		gu16ZeroPositionS1 = gu16AS5600s1Angle;
		gu16ZeroPositionS2 = gu16AS5600s2Angle;
		/*For the Zero position value of Sensor 1*/
		gu16PreviousAngleData1 = gu16AS5600s1Angle;
		gu16CurrentAngleData1 = gu16AS5600s1Angle;
		/*For the Zero position value of Sensor 2*/
		gu16PreviousAngleData2 = gu16AS5600s2Angle;
		gu16CurrentAngleData2 = gu16AS5600s2Angle;

     }
     else if(!Cal_Stop)
     {

   	    gu16CurrentAngleData1 =gu16AS5600s1Angle;
			gu16CurrentAngleData2 = gu16AS5600s2Angle;
			flash_struct_data=Flash_read_struct();
			gu16ZeroPositionS1=flash_struct_data.F_ZeroPositionS1;
			gu16ZeroPositionS2=flash_struct_data.F_ZeroPositionS2;
			if(isFlagpoweroff)
			{

				phase_check=1; //Clear out of phase error after power off
				isMovedS1=0;
				isMovedS2=0;
				IsSasAngleoutphase=0;
				if(flash_struct_data.F_flagcw==0x01)
				{
					gu16ClockWiseCounterS1=flash_struct_data.F_countervalueS1;
					gu16ClockWiseCounterS2=flash_struct_data.F_countervalueS2;
					gu16CounterClockWiseCounterS1=15000;
					gu16CounterClockWiseCounterS2=15000;
					isFlagCWS1=1;
					isFlagCWS2=1;
					isFlagCCWS1=0;
					isFlagCCWS2=0;
				    gu16PreviousAngleData1=flash_struct_data.F_previousS1;
					gu16PreviousAngleData2=flash_struct_data.F_previousS2;
			  	    gu16CurrentAngleData1 =flash_struct_data.F_previousS1;
					gu16CurrentAngleData2 = flash_struct_data.F_previousS2;
				    gu16PreviousAngleData1_phase=flash_struct_data.F_previousS1;
					gu16PreviousAngleData2_phase=flash_struct_data.F_previousS2;
			  	    gu16CurrentAngleData1_phase =flash_struct_data.F_previousS1;
					gu16CurrentAngleData2_phase = flash_struct_data.F_previousS2;

				}
				else if(flash_struct_data.F_flagccw==0x01)
				{
					gu16ClockWiseCounterS1=15000;
					gu16ClockWiseCounterS2=15000;
					gu16CounterClockWiseCounterS1=flash_struct_data.F_countervalueS1;
					gu16CounterClockWiseCounterS2=flash_struct_data.F_countervalueS2;
					isFlagCWS1=0;
					isFlagCWS2=0;
					isFlagCCWS1=1;
					isFlagCCWS2=1;
				    gu16PreviousAngleData1=flash_struct_data.F_previousS1;
					gu16PreviousAngleData2=flash_struct_data.F_previousS2;
			  	    gu16CurrentAngleData1 =flash_struct_data.F_previousS1;
					gu16CurrentAngleData2 = flash_struct_data.F_previousS2;
				    gu16PreviousAngleData1_phase=flash_struct_data.F_previousS1;
					gu16PreviousAngleData2_phase=flash_struct_data.F_previousS2;
			  	    gu16CurrentAngleData1_phase =flash_struct_data.F_previousS1;
					gu16CurrentAngleData2_phase = flash_struct_data.F_previousS2;
				}
				else if((flash_struct_data.F_flagcw==0) && (flash_struct_data.F_flagccw==0))
				{
					gu16ClockWiseCounterS1=15000;
					gu16ClockWiseCounterS2=15000;
					gu16CounterClockWiseCounterS1=15000;
					gu16CounterClockWiseCounterS2=15000;
					isFlagCWS1=0;
					isFlagCWS2=0;
					isFlagCCWS1=0;
					isFlagCCWS2=0;
				    gu16PreviousAngleData1=flash_struct_data.F_ZeroPositionS1;
					gu16PreviousAngleData2=flash_struct_data.F_ZeroPositionS2;
			  	    gu16CurrentAngleData1 =flash_struct_data.F_ZeroPositionS1;
					gu16CurrentAngleData2 = flash_struct_data.F_ZeroPositionS2;
				    gu16PreviousAngleData1_phase=flash_struct_data.F_ZeroPositionS1;
					gu16PreviousAngleData2_phase=flash_struct_data.F_ZeroPositionS2;
			  	    gu16CurrentAngleData1_phase =flash_struct_data.F_ZeroPositionS1;
					gu16CurrentAngleData2_phase = flash_struct_data.F_ZeroPositionS2;
				}
				else
				{

				}


			}
			else
			{
				//Do nothing
			}






			//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			/*If the calibrated flag is set check the steering angle is moved to Clock Wise or Counter Clock Wise  */
			if ((isFlagCWS1 == 0) && (isFlagCCWS1 == 0) && !(isFlagpoweroff))
			{


				//|CCW END|<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<|DELTA|ZERO_POSITION|DELTA|>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>|CW END|

				/* Wait until there is significant difference from the Zero position NB: Currently the Delta is given as 2 may be this need to be increased */
				/*If the significant difference occurred  between the u16CurrentAngleData0 and the u16PreviousAngleData0*/
	//				if (gu16CurrentAngleData1 > (gu16ZeroPositionS1 + 10))
					if ((gu16CurrentAngleData1 > (gu16ZeroPositionS1 +5))&&((gu16CurrentAngleData1-gu16ZeroPositionS1)<3000))
					{ //DELTA WAS 20
						//isMovedS1=1;
						isFlagCWS1 	= 1;/*The steering position moved and is assumed to be in clockwise direction */
						isFlagCCWS1 = 0;/*The steering position moved and is assumed to be in clockwise direction so set the Counter Clock wise flag set as Zero*/
						//Temp_State=FLAGCW;
						gu16ClockWiseCounterS1 += (gu16CurrentAngleData1- gu16ZeroPositionS1);

						gu16PreviousAngleData1 = gu16AS5600s1Angle;
						gu16CurrentAngleData1 = gu16AS5600s1Angle;

					}
				//	else if ((gu16CurrentAngleData1 < (gu16ZeroPositionS1))&&((gu16ZeroPositionS1-gu16CurrentAngleData1)>3500))
					else if((gu16CurrentAngleData1 < (gu16ZeroPositionS1))&&((gu16ZeroPositionS1-gu16CurrentAngleData1)>2500) && ((4095-(gu16ZeroPositionS1-gu16CurrentAngleData1))>5))
					{
						isFlagCWS1 = 1;/*The steering position moved and is assumed to be in clockwise direction */
						isFlagCCWS1 = 0;/*The steering position moved and is assumed to be in clockwise direction so set the Counter Clock wise flag set as Zero*/
						//Temp_State=FLAGCW;
						gu16ClockWiseCounterS1 += (4095 - gu16ZeroPositionS1) + gu16CurrentAngleData1;
								//(4095-(gu16ZeroPositionS1-gu16CurrentAngleData1));
						gu16PreviousAngleData1 = gu16AS5600s1Angle;
						gu16CurrentAngleData1 = gu16AS5600s1Angle;

					}
					//else if (gu16CurrentAngleData1 < (gu16ZeroPositionS1 - 10))
					else if (gu16CurrentAngleData1 < (gu16ZeroPositionS1 - 5)&&((gu16ZeroPositionS1-gu16CurrentAngleData1)<3000))
					{ //DELTA WAS 20
						//isMovedS1=1;
						isFlagCCWS1 = 1; /* The steering position moved and is assumed to be in counter clock wise direction */
						isFlagCWS1 = 0; /* The steering position moved and is assumed to be in Counter Clock wise direction so set the  Clock wise flag set as Zero*/
						//Temp_State=FLAGCCW;
						gu16CounterClockWiseCounterS1 += (gu16ZeroPositionS1
								- gu16CurrentAngleData1);
						gu16PreviousAngleData1 = gu16AS5600s1Angle;
						gu16CurrentAngleData1 = gu16AS5600s1Angle;

					}
			//		else if ((gu16CurrentAngleData1 > (gu16ZeroPositionS1))&&((gu16CurrentAngleData1-gu16ZeroPositionS1)>3500))
					else if ((gu16CurrentAngleData1 > (gu16ZeroPositionS1))&&((gu16CurrentAngleData1-gu16ZeroPositionS1)>2500)&& ((4095-(gu16CurrentAngleData1-gu16ZeroPositionS1))>5))
					{
						isFlagCCWS1 = 1;/*The steering position moved and is assumed to be in clockwise direction */
						isFlagCWS1 = 0;/*The steering position moved and is assumed to be in clockwise direction so set the Counter Clock wise flag set as Zero*/
						//Temp_State=FLAGCCW;
						gu16CounterClockWiseCounterS1 +=(4095 - gu16CurrentAngleData1) + gu16ZeroPositionS1;
								//(4095-(gu16CurrentAngleData1-gu16ZeroPositionS1));
						gu16PreviousAngleData1 = gu16AS5600s1Angle;
						gu16CurrentAngleData1 = gu16AS5600s1Angle;

					}
				}

			/*If the calibrated flag is set check the steering angle is moved to Clock Wise or Counter Clock Wise  */
			if ((isFlagCWS2 == 0) && (isFlagCCWS2 == 0) && !(isFlagpoweroff))
			{


				//|CCW END|<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<|DELTA|ZERO_POSITION|DELTA|>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>|CW END|

				/* Wait until there is significant difference from the Zero position NB: Currently the Delta is given as 2 may be this need to be increased */
				/*If the significant difference occurred  between the u16CurrentAngleData0 and the u16PreviousAngleData0*/
		//			if (gu16CurrentAngleData2 > (gu16ZeroPositionS2 + 10))
				if ((gu16CurrentAngleData2 > (gu16ZeroPositionS2 + 5))&&((gu16CurrentAngleData2-gu16ZeroPositionS2)<3000))
				{ //DELTA WAS 20
					isFlagCWS2 = 0;
					isFlagCCWS2 = 1;
					gu16CounterClockWiseCounterS2 += (gu16CurrentAngleData2- gu16ZeroPositionS2);
					gu16PreviousAngleData2 = gu16AS5600s2Angle;
					gu16CurrentAngleData2 = gu16AS5600s2Angle;

				 }
				//	else if ((gu16CurrentAngleData2 < (gu16ZeroPositionS2))&&((gu16ZeroPositionS2-gu16CurrentAngleData2)>3500))
				else if ((gu16CurrentAngleData2 < (gu16ZeroPositionS2))&&((gu16ZeroPositionS2-gu16CurrentAngleData2)>2500) && ((4095-(gu16ZeroPositionS2-gu16CurrentAngleData2))>5))
				{

					isFlagCWS2 = 0;
					isFlagCCWS2 = 1;
					gu16CounterClockWiseCounterS2 += (4095 - gu16ZeroPositionS2) + gu16CurrentAngleData2;
					gu16PreviousAngleData2 = gu16AS5600s2Angle;
					gu16CurrentAngleData2 = gu16AS5600s2Angle;

				 }
				 //else if (gu16CurrentAngleData2 < (gu16ZeroPositionS2 - 10))
				else if (gu16CurrentAngleData2 < (gu16ZeroPositionS2 -5)&&((gu16ZeroPositionS2-gu16CurrentAngleData2)<3000))
				 { //DELTA WAS 20
						isFlagCWS2 = 1;
						isFlagCCWS2 =0;
						gu16ClockWiseCounterS2 += (gu16ZeroPositionS2- gu16CurrentAngleData2);
						gu16PreviousAngleData2 = gu16AS5600s2Angle;
						gu16CurrentAngleData2 = gu16AS5600s2Angle;
				  }
				 //else if ((gu16CurrentAngleData2 > (gu16ZeroPositionS2))&&((gu16CurrentAngleData2-gu16ZeroPositionS2)>3500))
				else if ((gu16CurrentAngleData2 > (gu16ZeroPositionS2))&&((gu16CurrentAngleData2-gu16ZeroPositionS2)>2500) && ((4095-(gu16CurrentAngleData2-gu16ZeroPositionS2))>5))
				 {
						isFlagCWS2 = 1;
						isFlagCCWS2 =0;
						gu16ClockWiseCounterS2 +=(4095 - gu16CurrentAngleData2) + gu16ZeroPositionS2;
						gu16PreviousAngleData2 = gu16AS5600s2Angle;
						gu16CurrentAngleData2 = gu16AS5600s2Angle;

				  }
					else
					{
						//Do nothing
					}


				}




		//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		/* If there is a significant difference occurred then only we need to do the process, Since the last bit of the
		 * magnetic sensor is having a small fluctuations to avoid this we are using a delta here  */

		if (((isFlagCWS1 == 1) || (isFlagCCWS1 == 1))
				&& ((((gu16PreviousAngleData1 > gu16CurrentAngleData1)
						&& ((gu16PreviousAngleData1 - gu16CurrentAngleData1)
								> DELTA))
						|| ((gu16PreviousAngleData1 < gu16CurrentAngleData1)
								&& ((gu16CurrentAngleData1
										- gu16PreviousAngleData1) > DELTA))) || (gu16PreviousAngleData1==gu16CurrentAngleData1))&& !(isFlagpoweroff))
		{
          // isMovedS1=1;
			if ((isFlagCWS1 == 1) && (isFlagCCWS1 == 0))
			{

				gu16ClockWiseCounterS1 = GetTheAngleCounterValueCW(
						gu16CurrentAngleData1, gu16PreviousAngleData1,
						gu16ClockWiseCounterS1, 1);
			}

			else if ((isFlagCWS1 == 0) && (isFlagCCWS1 == 1))
			{

				gu16CounterClockWiseCounterS1 = GetTheAngleCounterValueCCW(
						gu16CurrentAngleData1, gu16PreviousAngleData1,
						gu16CounterClockWiseCounterS1, 1);

			}
			else
			{
				//Do nothing
			}
			gu16PreviousAngleData1 = gu16AS5600s1Angle;
			gu16CurrentAngleData1 = gu16AS5600s1Angle;
		}
		else
		{
			//isMovedS1=0;
		}

		if (((isFlagCWS2 == 1) || (isFlagCCWS2 == 1))
				&& ((((gu16PreviousAngleData2 > gu16CurrentAngleData2)
						&& ((gu16PreviousAngleData2 - gu16CurrentAngleData2)
								> DELTA))
						|| ((gu16PreviousAngleData2 < gu16CurrentAngleData2)
								&& ((gu16CurrentAngleData2
										- gu16PreviousAngleData2) > DELTA))) || (gu16PreviousAngleData2==gu16CurrentAngleData2)) && !(isFlagpoweroff))

		{
		//	isMovedS2=1;

			if ((isFlagCWS2 == 1) && (isFlagCCWS2 == 0))
			{

				gu16ClockWiseCounterS2 = GetTheAngleCounterValueCW(
						gu16CurrentAngleData2, gu16PreviousAngleData2,
						gu16ClockWiseCounterS2, 2);
			}
			if ((isFlagCWS2 == 0) && (isFlagCCWS2 == 1))
			{

				gu16CounterClockWiseCounterS2 = GetTheAngleCounterValueCCW(
						gu16CurrentAngleData2, gu16PreviousAngleData2,
						gu16CounterClockWiseCounterS2, 2);
			}
			gu16PreviousAngleData2 = gu16AS5600s2Angle;
			gu16CurrentAngleData2 = gu16AS5600s2Angle;

		}
		else
		{
			//isMovedS2=0;
		}

		if((!IsSasAngleoutphase) && (!IsSasAngleExceed) && (!IsSasHardFault))
		{
			Flash_array_store(flash_array);
			Flash_write_struct();
		}
		else
		{
		//Do nothing
		}


		gu16CWAngleS1 = 0;
		gu16CWAngleS2 = 0;
		gu16CCWAngleS1 = 0;
		gu16CCWAngleS2 = 0;

		gu16AbsAngleS1 = 0;  // Represent aS
		gu16AbsAngleS2 = 0;  // Represent aL

		gu16RevCntS1 = 0;    // Represent nS
		gu16RevCntS2 = 0;    // Represent nL

		if ((isFlagCWS1 == 1) && (isFlagCWS2 == 1))
		{
			difference = 0;
			difference = (gu16ClockWiseCounterS1 - 15000);
			gu16TempVar = (int) (difference / 4095);
			gu16RevCntS1 = gu16TempVar;  // revolution at S1  nS/nL
			//gu16AbsAngleS1 =(int) ((difference - (gu16TempVar * 4095) )); // Absolute Angle at S1  aS/aL
			gu16AbsAngleS1 = (int) ((difference % 4095)); // Absolute Angle at S1  aS/aL
			gu16AbsAngleS1 *= .087912;

			difference = 0;
			difference = (gu16ClockWiseCounterS2 - 15000);
			gu16TempVar = (int) (difference / 4095);
			gu16RevCntS2 = gu16TempVar;  // revolution at S1  nS/nL
			//gu16AbsAngleS2 =(int) ((difference - (gu16TempVar * 4095))); // Absolute Angle at S1  aS/aL
			gu16AbsAngleS2 = (int) ((difference % 4095)); // Absolute Angle at S1  aS/aL
			gu16AbsAngleS2 *= .087912;

			gu16AHFromS1 =
					((0.288889) * (gu16AbsAngleS1 + (gu16RevCntS1 * 360)));
			gu16AHFromS2 =
					((0.333334) * (gu16AbsAngleS2 + (gu16RevCntS2 * 360)));
			gu16phaseangleratio=gu16AHFromS2/gu16AHFromS1;

			gu16FinalSasAngle = (gu16AbsAngleS2*26)+(gu16AbsAngleS1*25);

			gu16FinalSasAngle += (26*gu16RevCntS2*360)+(25*gu16RevCntS1*360);

			gu16FinalSasAngle /=130;
		}

		else if ((isFlagCCWS1 == 1) && (isFlagCCWS2 == 1))
		{

			// For the counter clock wise calculations
			difference = 0;
			difference = (gu16CounterClockWiseCounterS1 - 15000);
			gu16TempVar = (int) (difference / 4095);
			gu16RevCntS1 = gu16TempVar;  // revolution at S1  nS/nL
			//gu16AbsAngleS1 =(int) ((difference - (gu16TempVar * 4095) )); // Absolute Angle at S1  aS/aL
			gu16AbsAngleS1 = (int) ((difference % 4095)); // Absolute Angle at S1  aS/aL
			gu16AbsAngleS1 *= .087912;

			difference = 0;
			difference = (gu16CounterClockWiseCounterS2 - 15000);
			gu16TempVar = (int) (difference / 4095);
			gu16RevCntS2 = gu16TempVar; // revolution at S1  nS/nL
			gu16AbsAngleS2 = (int) ((difference % 4095)); // Absolute Angle at S1  aS/aL
			gu16AbsAngleS2 *= .087912;

			gu16AHFromS1 =
					((25/65) * (gu16AbsAngleS1 + (gu16RevCntS1 * 360)));
			gu16AHFromS2 =
					((26/65) * (gu16AbsAngleS2 + (gu16RevCntS2 * 360)));
			gu16phaseangleratio=gu16AHFromS2/gu16AHFromS1;
			gu16FinalSasAngle = (gu16AbsAngleS2*26)+(gu16AbsAngleS1*25);

			gu16FinalSasAngle += (26*gu16RevCntS2*360)+(25*gu16RevCntS1*360);

			gu16FinalSasAngle /=130;

		}
		else
		{
			//Do Nothing
		}
		/***************angle  CAN data *********************/
		if(CalOk)
		{
			gu8CSASCanDataFrame[0] =0;
			gu8CSASCanDataFrame[1] =0;
		}
		else
		{
			//Do nothing
		}
		if ((isFlagCCWS1 == 1) && (isFlagCCWS2 == 1))
		{
			flag_anticlockwise=1;
			flag_clockwise=0;
			AngleDataCAN =(uint16_t)((gu16FinalSasAngle *2)/3);
			if(((float)((gu16FinalSasAngle / 1.5)-(uint16_t)((gu16FinalSasAngle / 1.5)))*10)>=7)
			{
			  AngleDataCAN +=1;
			}

		}
		else if ((isFlagCWS1 == 1) && (isFlagCWS2 == 1))
		{
			flag_anticlockwise=0;
			flag_clockwise=1;
			AngleDataCAN =(uint16_t)((gu16FinalSasAngle *2)/3);
			if(((float)((gu16FinalSasAngle / 1.5)-(uint16_t)((gu16FinalSasAngle / 1.5)))*10)>=7)
			{
			  AngleDataCAN +=1;
			}
			AngleDataCAN =~(uint16_t)(AngleDataCAN);
			//AngleDataCAN +=1;
		}
		else
		{
			//Do nothing
		}
		if ((CalOk) && (((isFlagCCWS1 == 1) && (isFlagCCWS2 == 1))
				|| (((isFlagCWS1 == 1) && (isFlagCWS2 == 1)))))
		{

			TempVar =AngleDataCAN;
			gu8CSASCanDataFrame[1] = ((uint8_t) (TempVar));	//LSB
			TempVar = AngleDataCAN >> 8;
			gu8CSASCanDataFrame[0] = ((uint8_t) (TempVar));	//MSB
			if((isFlagCWS1 == 1) && (isFlagCWS2 == 1))
			{
				AngleDataCAN =~(uint16_t)(AngleDataCAN);
			}
			else
			{
				//Do nothing
			}
			if(gu16FinalSasAngle>(float)(AngleDataCAN*1.5))
			{
				gu8CSASCanDataFrame[4]&=0x0F;
				TempVar_1=(gu16FinalSasAngle-(float)(AngleDataCAN*1.5))*10;
				if((TempVar_1>=1) &&(TempVar_1<=7))
				{

					gu8CSASCanDataFrame[4] |= ((uint8_t) (TempVar_1))<<4;
				}
				else
				{
					gu8CSASCanDataFrame[4] &=0x0F;
				}
			}
			else
			{
				TempVar_1=((float)(AngleDataCAN*1.5)-gu16FinalSasAngle)*10;
				gu8CSASCanDataFrame[4]&=0x0F;
				if((TempVar_1>=1) &&(TempVar_1<=7))
				{
					gu8CSASCanDataFrame[4] |=((uint8_t)(TempVar_1) | (1<<3))<<4;
				}
				else
				{
					gu8CSASCanDataFrame[4] |=((uint8_t)(TempVar_1));
				}

			}


		}


	}
     else
     {
    	 //Do nothing
     }
  	/***velocity and pulses*******/
      if(CalOk)
      {

   	  if(gu16FinalSasAngle<=696 && IsSasAngleDataValid )
   	  {
    		  cal_stuck=0;
       		  if((IsSasHardFault))
       		  {
       			gu8STSnibble	=0x0B;  //STS1 update to 1 any error
       			gu8STDIDbit		=0x08;
       		  }
       		  else
       		  {
       			gu8STSnibble	&=0xFD; //clear STS1
       		  }
    		  IsSasAngleDataValid=1;
    		  IsSasAngleExceed=0;
    		  isFlagpoweroff_1=0;
    		  gu8CSASCanDataFrame[0]&=0x0F;
    		  gu8CSASCanDataFrame[0]|=(gu8STSnibble<<4);
    		  if(Flag.BattVolt_Low )
    		  {
    			  gu8CSASCanDataFrame[2]=0x88;
    		  }
    		  else
    		  {
    			  //Do nothing
    		  }
    		  gu8CSASCanDataFrame[3]=0x00;
    		  gu8CSASCanDataFrame[6]=gu8STDIDbit;
    		  gu8CSASCanDataFrame[7]=(uint8_t)(CSAS_checksum_Byte(gu8CSASCanDataFrame));
    		  CAN_Transmit_msg(&gCANMailBoxNo, CSASGRP1ARBID_DATA, CAN_ID_STD,CSAS_SIZE8BYTE, gu8CSASCanDataFrame);
    	  }
    	  else
    	  {
    		  gu8STSnibble	=0x0B;
    		  gu8CSASCanDataFrame[0]&=0x0F;
    		  gu8CSASCanDataFrame[0]|=(gu8STSnibble<<4);
    		  CAN_Transmit_msg(&gCANMailBoxNo, CSASGRP1ARBID_DATA, CAN_ID_STD,
    				  	  	  	  CSAS_SIZE8BYTE, gu8CSASCanDataFrame_Anglelimit);
    	  }
	}
    else
	{
    	  gu8CSASCanDataFrame[0]&=0x0F;
    	  gu8CSASCanDataFrame[0]|=(gu8STSnibble<<4);
    	  CAN_Transmit_msg(&gCANMailBoxNo, CSASGRP1ARBID_DATA, CAN_ID_STD,
    			  	  	  	  CSAS_SIZE8BYTE, gu8CSASCanDataFrame);
	}


}
/*****************************************************************************
 *
 * Function Name			: LPI2C_CONF_Reg_Write
 * Function Description		: This function for handling the hysterisis of the as5600
 * Parameters				: None
 * Return value				: None
 *
 *****************************************************************************/
void LPI2C_CONF_Reg_Write(void)
{
		uint16_t u8RegVal=0;
		while(LPI2C_Check_Busy());
		u8RegVal = (0x36<<1)|0; //to read last bit should be 1 and for write it should be 0
		LPI2C0->MTDR = (0x04<<8)|(u8RegVal);//Start and transmit the address - 04
		while(LPI2C_Check_Busy());
		u8RegVal = 0x08;// register address to be write
		LPI2C0->MTDR = u8RegVal;
		while(LPI2C_Check_Busy());
		LPI2C0->MTDR =0x08;//hysterisis
		while(LPI2C_Check_Busy());
		LPI2C_Generate_Stop();
}
