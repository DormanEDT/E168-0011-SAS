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
/*
 Description : The Work is intended for the Dorman Products Clock Spring Steering angle Sensors
 Group-1&2

 */

// Test for S32 and Git.  Delete this line if found. 2 Commits 1 push test

/***********************************************************************************/
/*                                                                                 */
/*     FILE NAME   :  main.c                                                       */
/*                                                                                 */
/*     DESCRIPTION :  Main entry after reset.                                      */
/*                                                                                 */
/* +--------------+----------+---------------------------------------------------+ */
/* |Rev. HISTORY  |  DATE    |           MODIFIED POINT                          | */
/* +--------------+----------+---------------------------------------------------+ */
/* |    0.1       |05-June-22|       Initial Version                             | */
/* +--------------+----------+---------------------------------------------------+ */
/***********************************************************************************/
#include "S32K116.h"
#include "main.h"
unsigned char Count_1ms_Reg = 0;
unsigned char Count_20ms_Reg = 0;
unsigned char Count_10ms_Reg = 0;
unsigned char Count_100ms_Reg = 0;
unsigned char Count_1sec_Reg = 0;

float ADC_Output = 0;
unsigned int ADC_Test = 0;
unsigned int ADC_Array[100] = { 0 };

#define BUFFER_SIZE         0x0Au          /* Size of data source */
uint8_t Source_Buff[BUFFER_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa };
uint8_t Destination_Buff[BUFFER_SIZE];

uint8_t ready;
uint8_t err_0, err_1, err_2, err_3, err_4, err_5, err_6;

uint8_t u8RegVal = 0;

extern uint8_t gCANMailBoxNo;
extern uint8_t can_msg[];

struct MyFlag Flag;

uint8_t Busy_err=0;
uint8_t CalOk=0; //caliberation set for testing
uint8_t  First=1;
uint8_t IsSasAngleDataValid = 1;
uint8_t IsSasAngleExceed = 0;
uint8_t IsSasAngleoutphase = 0;
uint8_t IsSasHardFault = 0;
uint8_t phase_check=0;
uint8_t isMovedS1=0;
uint8_t isMovedS2=0;
uint8_t IsCalibarationComp = 0;
uint8_t IsSasMaskBitEnable = 0;
uint8_t IsSasRollingCounter = 0;
uint8_t gu8increment_value=0;
/*The below values loaded are only for frame test, need to be set to 0 later !!!!!*/
uint8_t gu8SteeringAngleLSB = 0x1B;
uint8_t gu8SteeringAngleMSB = 0x0D;
uint8_t gu8SteeringAngleVelocity = 0x00;
uint8_t gu8STSnibble=0;
uint8_t gu8STDIDbit=0;
uint8_t gu8CSASCanDataFrame[8] =
		{ 0x00, 0x01, 0x98, 0x00, 0x78, 0x78, 0x78, 0xB6 };
uint8_t gu8CSASCanDataFrame_960MS[8] =
		{ 0x07, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t gu8CSASCanDataFrame_1Sec[1] =
		{ 0x00};
uint8_t gu8CSASCanDataFrame_Anglelimit[8] =
		{ 0xBD, 0x48, 0xA8, 0x00, 0x08, 0x00, 0x08, 0xEA };
// Need to be removed !!!!!!!!!!!!!!!!!!!!!!!!
uint16_t difference = 0;
//static float Angle_difference=0;
float speed=0;
//-----------------------------------------------------------------------------------------------------

static uint16_t gu16Difference = 0;
static uint16_t gu16CounterValue = 0;


uint8_t u8Revolutions = 0;
uint16_t u16AngleinCwFromS1 = 0;
uint16_t u16AngleinCwFromS2 = 0;
uint16_t u16AngleinCCwFromS1 = 0;
uint16_t u16AngleinCCwFromS2 = 0;

uint8_t is_calibrated = 0;

uint16_t gu16ZeroPositionS1 = 0;
uint16_t gu16ZeroPositionS2 = 0;

uint8_t isFlagCWS1 = 0;
uint8_t isFlagCCWS1 = 0;

uint8_t isFlagCWS2 = 0;
uint8_t isFlagCCWS2 = 0;
uint8_t F_CalOk=0;
uint8_t isFlagpoweroff=0;
uint8_t isFlagpoweroff_1=0;
uint8_t cal_stuck=0;
uint16_t gu16ClockWiseCounterS1 = 15000;
uint16_t gu16CounterClockWiseCounterS1 = 15000;

uint16_t gu16ClockWiseCounterS2 = 15000;
uint16_t gu16CounterClockWiseCounterS2 = 15000;

float BattVoltage = 0;
float IGNVoltage  =0;
float gu16CWAngleS1 = 0;
float gu16CCWAngleS1 = 0;

float gu16CWAngleS2 = 0;
float gu16CCWAngleS2 = 0;
float gu16phaseangleratio=1;
uint16_t gu16SasFinalCWAngle = 0;
uint16_t gu16SasFinalCCWAngle = 0;

signed int test = 0;
uint16_t AngleDataCAN = 0;

uint16_t TempVar = 0;
float TempVar_1 = 0;

float Temp_angle=0;
uint16_t gu16AS5600s1Angle = 0;
uint16_t gu16AS5600s2Angle = 0;
uint16_t velocity=0;
uint16_t gu16PreviousAngleData1 = 0;
uint16_t gu16CurrentAngleData1 = 0;

uint16_t gu16PreviousAngleData2 = 0;
uint16_t gu16CurrentAngleData2 = 0;
uint16_t gu16PreviousAngleData1_phase = 0;
uint16_t gu16CurrentAngleData1_phase = 0;

uint16_t gu16PreviousAngleData2_phase = 0;
uint16_t gu16CurrentAngleData2_phase = 0;

float gGearRatioSensor1 = 1.6;
float gGearRatioSensor2 = 1.85;

uint16_t gu16AngleDataAtS1 = 0;
uint16_t gu16AngleDataAtS2 = 0;

float gu16AbsAngleS1 = 0;
float gu16AbsAngleS2 = 0;

uint16_t gu16RevCntS1 = 0;
uint16_t gu16RevCntS2 = 0;

float gu16AHFromS1 = 0;
float gu16AHFromS2 = 0;

float gu16FinalSasAngle = 0;
uint16_t gu16TempVar = 0;
uint8_t  gu8STS=0;
uint8_t Data[8]={0};
uint8_t msb=0;
uint8_t lsb=0;
flashdata_struct flash_struct_data={0};
uint8_t flash_array[12]={0};
uint8_t gu8Receivedchecksum=0;
uint32_t poweroff_address=0x1400000B;
uint16_t Angle_diff_poweroff_1=0;
uint16_t Angle_diff_poweroff_2=0;
uint8_t clear_poweroff=0xFF;
uint16_t angle_after_powerset=0;
//static uint8_t checksum_count=0;
uint32_t pulse_delay=0;
uint8_t poweroffcount=0;
uint8_t toggle=0;
uint8_t decrement_flag=0;
/*****************************************************************************/
//-----------------------------------------------------------------------------------------------------

/**** dorman rights checking constant array, prototype declaration and variable declaration****/
/*********************************************************************************************************************************************/
__attribute__ ((section(".dorman"))) const char dorman[64] =
		"(c) Copyright 2022 Dorman Products  Inc. All Rights Reserved.";
__attribute__ ((section(".SecSpec"))) const char Sec_Spec_Ver[4] = { '0', '1',
		'0', '5' }; // 1FE40
__attribute__ ((section(".SecLibVer"))) const char Sec_Lib_Ver[4] = { 0x30,
		0x30, 0x30, 0x30 }; // 1FE44
__attribute__ ((section(".ModuleName"))) const char Module_Name[32] = { '9',
		'0', '5', '-', '1', '3', '1', ',', '1', '3', '4', ',', '1', '4', '0' }; // 1FF00
__attribute__ ((section(".HardwareVersion"))) const char Hardware_Version[16] =
		{ 'E', '1', '7', '2', '-', '0', '0', '0', '6', '-', '0', '1', '0', '0',
				'_', '1' }; // 1FF20
__attribute__ ((section(".FirmwareVersion"))) const char Firmware_Version[8] = {
		'0', '0', '.', '0', '1' }; // 1FF30
__attribute__ ((section(".Reserved1"))) const char Reserved1[8] = { 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }; // 1FF38
__attribute__ ((section(".Reserved2"))) const char Reserved2[32] = { 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }; // 1FF40
uint32_t address;
uint8_t dorman_flag = 0, size = 0;
uint8_t test_char_t1[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
void check_dorman_rights(void);

/**** END - dorman rights checking constant array, prototype declaration and variable declaration****/
/*********************************************************************************************************************************************/

int main(void)
{

	/* Initialize all the internal peripheral which are used in the SAS application - MCU S32K116 */
	Init_Sys_Peripherals();

	/*********************************************************************************************************************************************/

	/*********************************************************************************************************************************************/
	/*Read the flash structure and check the calibration completed or not */


	FLASH_DRV_EEERead(start_address, 1, &F_CalOk);
	if(F_CalOk==0x01)
	{
		CalOk=1;
		IsCalibarationComp=1;

	}
	else
	{
		F_CalOk=0;
		CalOk=0;
	}

	while (1)
	{
		if (dorman_flag == 2)
		{
		}

		Scheduler_Task();
	}
}

/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :   check_dorman_rights                        */
/*                                                                 */
/*   FUNCTION BRIEF :   									       */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :   None                                       */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/
void check_dorman_rights(void)
{
	uint8_t check_char[6] = { 'E', 'p', 's', 'n', 'b', 'o' };
	test_char_t1[0] = dorman[19];
	test_char_t1[1] = dorman[20];
	test_char_t1[2] = dorman[21];
	test_char_t1[3] = dorman[22];
	test_char_t1[4] = dorman[23];
	test_char_t1[5] = dorman[24];

	if ((dorman[19] == 'D') && (dorman[20] == 'o') && (dorman[21] == 'r')
			&& (dorman[22] == 'm') && (dorman[23] == 'a')
			&& (dorman[24] == 'n')) {
		dorman_flag = 1;
	} else {
		while (1);
	}

	if ((test_char_t1[0] == (check_char[0] - 1))
			&& (test_char_t1[1] == (check_char[1] - 1))
			&& (test_char_t1[2] == (check_char[2] - 1))
			&& (test_char_t1[3] == (check_char[3] - 1))
			&& (test_char_t1[4] == (check_char[4] - 1))
			&& (test_char_t1[5] == (check_char[5] - 1)))
	{
		dorman_flag = 2;
	} else
	{
		while (1);

	}

}

/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :   NVIC_Init_IRQs                             */
/*                                                                 */
/*   FUNCTION BRIEF :   									       */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :   None                                       */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/
void NVIC_Init_IRQs(void)
{
	S32_NVIC->ICPR[0] |= (1 << LPI2C0_Master_Slave_IRQn); /* IRQ24-LPI2C0 : clr any pending IRQ*/
	S32_NVIC->ISER[0] |= (1 << LPI2C0_Master_Slave_IRQn); /* IRQ24-LPI2C0: enable IRQ */
	S32_NVIC->IPR[6]  |=  S32_NVIC_IPR_PRI_0(0x01);		     /* IRQ24-LPI2C0: priority 1 of 0-15*/

	S32_NVIC->ICPR[0] |= (1 << SCG_CMU_LVD_LVWSCG_IRQn); /* IRQ21-SCG_CMU_LVD_LVWSCG_IRQn : clr any pending IRQ*/
	S32_NVIC->ISER[0] |= (1 << SCG_CMU_LVD_LVWSCG_IRQn); /* IRQ21-SCG_CMU_LVD_LVWSCG_IRQn: enable IRQ */
	S32_NVIC->IPR[6]  |= S32_NVIC_IPR_PRI_1(0x02);		     /* IRQ21-SCG_CMU_LVD_LVWSCG_IRQn: priority 2 of 0-15*/
	/*************External interrupts *******************************************************/

	S32_NVIC->ICPR[0] |= 1 << PORT_IRQn;/* IRQ9-PORT_IRQn: clr any pending IRQ*/
    S32_NVIC->ISER[0] |= 1 << PORT_IRQn;/* IRQ9-PORT_IRQn: enable IRQ */
	S32_NVIC->IPR[2]  |=S32_NVIC_IPR_PRI_1(0x03);/* IRQ9-PORT_IRQn: priority 3 of 0-15*/

}
/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :   Init_SysTick                               */
/*                                                                 */
/*   FUNCTION BRIEF :   									       */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :   None                                       */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/
void Init_SysTick(void)
{
	S32_SysTick->CSR = 0x00;
	S32_SysTick->RVR = 48000;
	S32_SysTick->CVR = 0x00;
	S32_SysTick->CSR |= 0x00000007;
}
/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :   SysTick_Handler                            */
/*                                                                 */
/*   FUNCTION BRIEF :   									       */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :   None                                       */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/
void SysTick_Handler(void)
{
	S32_SysTick->RVR = 48000;
	S32_SysTick->CVR = 0x00;

	Flag.Msec1_Flag = SET;
	if (++Count_20ms_Reg >= 50)
	{
		Task_50ms();
		Count_20ms_Reg = 0;
	}
	if (++Count_1ms_Reg >= 12)
	{
		//Task_12ms();
		Flag.Msec12_Flag = SET;
		Count_1ms_Reg = 0;
		if (++Count_10ms_Reg >= 80)
		{
			Count_10ms_Reg = 0;
			Flag.Msec960_Flag= SET;
		} else
		{

		}
	}
	else
	{
	}
	if (++Count_100ms_Reg >= 100)
	{
		Count_100ms_Reg=0;
		if(++Count_1sec_Reg >= 10)
		{
			Count_1sec_Reg=0;
			Flag.Sec1_Flag=SET;
		}

	}

}

/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :   Scheduler_Task                             */
/*                                                                 */
/*   FUNCTION BRIEF :   Here 1ms, 10ms, 1sec task is updated       */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :   None                                       */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/
void Scheduler_Task(void)
{
	if (Flag.Msec1_Flag == SET)
	{
		Task_1ms();/* 1ms task are done here */
		Flag.Msec1_Flag = RESET;
	} else
	{

	}

	if (Flag.Msec12_Flag == SET)
	{
		Task_12ms();/* 12ms task are done here */
		Flag.Msec12_Flag = RESET;
	} else
	{
	}

	if (Flag.Msec960_Flag == SET)
	{
		Task_960msec();/* 1s task are done here */
		Flag.Msec960_Flag = RESET;
	} else
	{
	}
	if (Flag.Sec1_Flag == SET)
	{
		Task_1sec();/* 1s task are done here */
		Flag.Sec1_Flag = RESET;
	}
	else
	{

	}
}

/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :   Task_1ms                                   */
/*                                                                 */
/*   FUNCTION BRIEF :   Here 1ms tasks are done                    */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :   None                                       */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/
void Task_1ms(void)
{
	char ADC_Channel_3 = 3;
	ADC_Output  = Read_ADC(ADC_Channel_3);
	BattVoltage =((ADC_Output *5) /4095);
	BattVoltage =BattVoltage*(690);
	BattVoltage =BattVoltage/180;

	if (BattVoltage <= 8)
	{
		Flag.BattVolt_Low = SET;
		Flag.SAZS         = SET;
		gu8STSnibble	 &=~(1<<3);
		gu8STSnibble	 |=(1<<3);
		gu8CSASCanDataFrame[2] &= ~(1<<7);
		gu8STDIDbit=0x08;
	}
	else if (BattVoltage >= 11)
	{
		Flag.BattVolt_Low = RESET;
		Flag.SAZS         = RESET;
		gu8STSnibble	 &=~(1<<3);
		gu8STDIDbit=0x00;
	}

}

/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :   Task_12ms                                  */
/*                                                                 */
/*   FUNCTION BRIEF :   Here 12ms tasks are done                   */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :   None                                       */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/
void Task_12ms(void)
{

	LPI2C_Transmit_Int();
	CAN_App_Task();
	App_SAS_Operations();

}
/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :   Task_20ms                                  */
/*                                                                 */
/*   FUNCTION BRIEF :   Here 20ms tasks are done                   */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :   None                                       */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/
void Task_50ms(void)
{
	if(canrxid!=0x20)
	{
		gu8STSnibble   &=~(1<<0);
		gu8STSnibble   |=(1<<0);												//STS0 update to 1 any error

 	}
	else
	{
		//Do nothing
	}
}
/*******************************************************************/
/*   FUNCTION NAME  :   Task_960msec                                 */
/*                                                                 */
/*   FUNCTION BRIEF :   Here 1second tasks are done                */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :   None                                       */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/
void Task_960msec(void)
{

	CAN_Transmit_msg(&gCANMailBoxNo, CSASGRP1ARBID960MSEC_DATA, CAN_ID_STD,
     		CSAS_SIZE8BYTE, gu8CSASCanDataFrame_960MS);
}
/*******************************************************************/
/*   FUNCTION NAME  :   Task_1sec                                  */
/*                                                                 */
/*   FUNCTION BRIEF :   Here 1second tasks are done                */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :   None                                       */
/*                                                                 */
/*   RETURN VALUES  :   None                                       */
/*                                                                 */
/*******************************************************************/
void Task_1sec(void)
{

/****************************Go to sleep when IGN OFF**********************************/
    CAN_Transmit_msg(&gCANMailBoxNo, CSASGRP1ARBID1SEC_DATA, CAN_ID_STD,
     		1, gu8CSASCanDataFrame_1Sec);
	if(canrxid!=0x20 && decrement_flag)
	{
		decrement_flag=0;
		gu8increment_value =1;
        gu8CSASCanDataFrame[2]=(gu8CSASCanDataFrame[2]+(gu8increment_value << 4));
        if(zero_point_set)
        {
        	gu8CSASCanDataFrame[2]&=0x7F;
        	gu8CSASCanDataFrame[2]|=0x80; //SAZS
        }
   	 if((gu8CSASCanDataFrame[2] & 0xF0) == 0xF0)
   	 {
   		gu8CSASCanDataFrame[2] &= 0x8F;
   	 }
	}
	else
	{

		canrxid=0;
		one_time=0;
		//gu8STSnibble   &=~(1<<0); //clear STS0 bit if 0x20 is not received
	}
/****************************Go to sleep when IGN OFF**********************************/
	if (PIN_Input_Read(PTB,2)==1)
	{
		sleep_actions();
		Set_Power_Mode(VLPS);
		wakeup_actions();
		gu8CSASCanDataFrame[2] &=0x7F;
		gu8CSASCanDataFrame[2] |=0x80;  //Set SAZS after every power on reset
	}
	else
	{

	}

	if(CalOk && (!phase_check))
	{

		gu16CurrentAngleData1_phase = gu16AS5600s1Angle;
		gu16CurrentAngleData2_phase = gu16AS5600s2Angle;

		if(((gu16PreviousAngleData1_phase > gu16CurrentAngleData1_phase)
							&& ((gu16PreviousAngleData1_phase - gu16CurrentAngleData1_phase)
									> OUT_PHASE))
							|| ((gu16PreviousAngleData1_phase < gu16CurrentAngleData1_phase)
									&& ((gu16CurrentAngleData1_phase- gu16PreviousAngleData1_phase) > OUT_PHASE)))
		{
			isMovedS1=1;
		}
		else
		{
			isMovedS1=0;
		}
		if(((gu16PreviousAngleData2_phase > gu16CurrentAngleData2_phase)
							&& ((gu16PreviousAngleData2_phase - gu16CurrentAngleData2_phase)
									> OUT_PHASE))
							|| ((gu16PreviousAngleData2_phase < gu16CurrentAngleData2_phase)
									&& ((gu16CurrentAngleData2_phase
											- gu16PreviousAngleData2_phase) > OUT_PHASE)))
		{
			isMovedS2=1;
		}
		else
		{
			isMovedS2=0;
		}

		if((isMovedS1!=isMovedS2))
		{
			IsCalibarationComp=0;
			IsSasAngleoutphase=1;
		}



    	gu16PreviousAngleData1_phase = gu16AS5600s1Angle;
    	gu16PreviousAngleData2_phase = gu16AS5600s2Angle;

    	//Check the position error fixed or not
    if(((isFlagCWS1)&&(isFlagCWS2)&&(((1<=(gu16ClockWiseCounterS1/gu16ClockWiseCounterS2))
    		&&((gu16ClockWiseCounterS1/gu16ClockWiseCounterS2)>(UPPER_PHASE_RATIO+0.02)))
    		||((1>=(gu16ClockWiseCounterS1/gu16ClockWiseCounterS2))
    	    &&((gu16ClockWiseCounterS1/gu16ClockWiseCounterS2)>=(LOWER_PHASE_RATIO-0.02)))))
    		||((isFlagCCWS1)&&(isFlagCCWS2)&&(((1<=(gu16CounterClockWiseCounterS1/gu16CounterClockWiseCounterS2))
	    	&&((gu16CounterClockWiseCounterS1/gu16CounterClockWiseCounterS2)<(UPPER_PHASE_RATIO+0.02)))
	    	||((1>=(gu16CounterClockWiseCounterS1/gu16CounterClockWiseCounterS2))
	    	&&((gu16CounterClockWiseCounterS1/gu16CounterClockWiseCounterS2)>=(LOWER_PHASE_RATIO-0.02))))))


	{
    	IsSasAngleoutphase=0;
	}
}
	else
	{
		 phase_check=0;
		 isMovedS1=0;
		 isMovedS2=0;
		 IsSasAngleoutphase=0;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------

/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :   GetTheAngleCounterValueCW                  */
/*                                                                 */
/*   FUNCTION BRIEF :   this function is used to get the angle     */
/*                      from the clock wise of the SAS             */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :  CurrentAngle,PreviousAngle, CounterValue &  */
/*                     SensorSelect                                */
/*                                                                 */
/*   RETURN VALUES  :   uint16_t -  gu16CounterValue               */
/*                                                                 */
/*******************************************************************/
uint16_t GetTheAngleCounterValueCW(uint16_t CurrentAngle,
		uint16_t PreviousAngle, uint16_t CounterValue, uint8_t SensorSelect)
{

	gu16CounterValue = CounterValue;

if(PreviousAngle==CurrentAngle)
{
	gu16CounterValue = CounterValue;
}
else
{
	if(SensorSelect==1)
	{
	if ((CurrentAngle > PreviousAngle)
			&& (CurrentAngle - PreviousAngle) < 1250)
	{

		gu16Difference = (CurrentAngle - PreviousAngle);
		gu16CounterValue += gu16Difference;

	} else if ((CurrentAngle < PreviousAngle)
			&& (PreviousAngle - CurrentAngle) > 2500)
	{

		gu16Difference = (4095 - PreviousAngle) + CurrentAngle;
		gu16CounterValue += gu16Difference;

	}

	if ((PreviousAngle > CurrentAngle)
			&& (PreviousAngle - CurrentAngle) < 1250)
	{

		gu16Difference = (PreviousAngle - CurrentAngle);
		gu16CounterValue -= gu16Difference;

		if (gu16CounterValue <= 15005)
		{		    //was 15020
			if (SensorSelect == 1)
			{
				isFlagCWS1 = 0;
			}
			else if (SensorSelect == 2)
			{

				isFlagCWS2 = 0;
			}
			gu16CounterValue = 15000;
		}
	}
	else if ((CurrentAngle > PreviousAngle)
			&& (CurrentAngle - PreviousAngle) > 2500)
	{

		gu16Difference = (4095 - CurrentAngle) + PreviousAngle;
		gu16CounterValue -= gu16Difference;

		if (gu16CounterValue <= 15005)
		{		    //was 15020

			if (SensorSelect == 1)
			{
				isFlagCWS1 = 0;
			}
			else if (SensorSelect == 2)
			{
				isFlagCWS2 = 0;
			}
			gu16CounterValue = 15000;
		}
	}
}
	else if(SensorSelect==2)
	{
		if ((CurrentAngle > PreviousAngle)
				&& (CurrentAngle - PreviousAngle) < 1250)
		{

			gu16Difference = (CurrentAngle - PreviousAngle);
			gu16CounterValue -= gu16Difference;
			if (gu16CounterValue <= 15005)
			{		    //was 15020
				if (SensorSelect == 1)
				{
					isFlagCWS1 = 0;
				}
				else if (SensorSelect == 2)
				{

					isFlagCWS2 = 0;
				}
				gu16CounterValue = 15000;
			}

		}
		else if ((CurrentAngle < PreviousAngle)
				&& (PreviousAngle - CurrentAngle) > 2500)
		{

			gu16Difference = (4095 - PreviousAngle) + CurrentAngle;
			gu16CounterValue -= gu16Difference;
			if (gu16CounterValue <= 15005)
			{		    //was 15020

				if (SensorSelect == 1)
				{
					isFlagCWS1 = 0;
				}
				else if (SensorSelect == 2)
				{
					isFlagCWS2 = 0;
				}
				gu16CounterValue = 15000;
			}

		}

		if ((PreviousAngle > CurrentAngle)
				&& (PreviousAngle - CurrentAngle) < 1250)
		{

			gu16Difference = (PreviousAngle - CurrentAngle);
			gu16CounterValue += gu16Difference;


		}
		else if ((CurrentAngle > PreviousAngle)
				&& (CurrentAngle - PreviousAngle) > 2500)
		{

			gu16Difference = (4095 - CurrentAngle) + PreviousAngle;
			gu16CounterValue += gu16Difference;


		}
	}

}

	return (gu16CounterValue);
}

/*******************************************************************/
/*                                                                 */
/*   FUNCTION NAME  :   GetTheAngleCounterValueCCW                 */
/*                                                                 */
/*   FUNCTION BRIEF :   this function is used to get the angle     */
/*                      from the counter clock wise of the SAS     */
/*                                                                 */
/*                                                                 */
/*   PARAMETERS     :  CurrentAngle,PreviousAngle, CounterValue &  */
/*                     SensorSelect                                */
/*                                                                 */
/*   RETURN VALUES  :   uint16_t -  gu16CounterValue               */
/*                                                                 */
/*******************************************************************/
uint16_t GetTheAngleCounterValueCCW(uint16_t CurrentAngle,
		uint16_t PreviousAngle, uint16_t CounterValue, uint8_t SensorSelect)
{

	gu16CounterValue = CounterValue;
	if(PreviousAngle==CurrentAngle)
	{
		gu16CounterValue = CounterValue;
	}
	else
	{
		if(SensorSelect==1)
		{
	if ((PreviousAngle > CurrentAngle)
			&& (PreviousAngle - CurrentAngle) < 1250)
	{

		gu16Difference = (PreviousAngle - CurrentAngle);
		gu16CounterValue += gu16Difference;

	}

	else if ((PreviousAngle < CurrentAngle)
			&& (CurrentAngle - PreviousAngle) > 2500)
	{

		gu16Difference = (4095 - CurrentAngle) + PreviousAngle;
		gu16CounterValue += gu16Difference;

	}

	if ((CurrentAngle > PreviousAngle)
			&& (CurrentAngle - PreviousAngle) < 1250)
	{
		gu16Difference = (CurrentAngle - PreviousAngle);
		gu16CounterValue -= gu16Difference;

		if (gu16CounterValue < 15005)
		{		    //was 15020
			if (SensorSelect == 1)
			{
				isFlagCCWS1 = 0;			// was isFlagCWS1

			}
			else if (SensorSelect == 2)
			{

				isFlagCCWS2 = 0;			// was isFlagCWS1
			}
			gu16CounterValue = 15000;
		}
	}

	else if ((PreviousAngle > CurrentAngle)
			&& (PreviousAngle - CurrentAngle) > 2500)
	{

		gu16Difference = (4095 - PreviousAngle) + CurrentAngle;
		gu16CounterValue -= gu16Difference;
		if (gu16CounterValue < 15005)
		{			//was 15020
			if (SensorSelect == 1)
			{
				isFlagCCWS1 = 0;			// was isFlagCWS1
			}
			else if (SensorSelect == 2)
			{
				isFlagCCWS2 = 0; //was isFlagCWS1
			}
			gu16CounterValue = 15000;
		}
	}
		}
		else if(SensorSelect==2)
		{
			if ((PreviousAngle > CurrentAngle)
					&& (PreviousAngle - CurrentAngle) < 1250)
			{

				gu16Difference = (PreviousAngle - CurrentAngle);
				gu16CounterValue -= gu16Difference;
				if (gu16CounterValue < 15005)
					{		    //was 15020
						if (SensorSelect == 1)
						{
							isFlagCCWS1 = 0;			// was isFlagCWS1

						} else if (SensorSelect == 2)
						{

							isFlagCCWS2 = 0;			// was isFlagCWS1
						}
						gu16CounterValue = 15000;
					}

			}

			else if ((PreviousAngle < CurrentAngle)
					&& (CurrentAngle - PreviousAngle) > 2500)
			{

				gu16Difference = (4095 - CurrentAngle) + PreviousAngle;
				gu16CounterValue -= gu16Difference;
				if (gu16CounterValue < 15005)
					{			//was 15020
						if (SensorSelect == 1)
						{
							isFlagCCWS1 = 0;			// was isFlagCWS1
						}
						else if (SensorSelect == 2)
						{
							isFlagCCWS2 = 0; //was isFlagCWS1
						}
						gu16CounterValue = 15000;
					}

			}

			if ((CurrentAngle > PreviousAngle)
					&& (CurrentAngle - PreviousAngle) < 1250) {
				gu16Difference = (CurrentAngle - PreviousAngle);
				gu16CounterValue += gu16Difference;


			}

			else if ((PreviousAngle > CurrentAngle)
					&& (PreviousAngle - CurrentAngle) > 2500)
			{

				gu16Difference = (4095 - PreviousAngle) + CurrentAngle;
				gu16CounterValue += gu16Difference;

			}
		}
	}
	return (gu16CounterValue);
}

void ADC_Disable(void)
{
	ADC0->SC1[0] = 0x0000003F;
}
uint8_t CSAS_checksum_Byte(uint8_t *data )
{
	uint16_t sum_bytes=0;
	uint16_t checksum=0;
	uint8_t i=0;
	for(i=0;i<7;i++)
	{
		sum_bytes +=data[i];
	}
    checksum=sum_bytes+0x25+8;
	return (uint8_t)checksum;
}

void Low_voltage_detection_enable(void)
{

	PMC->LVDSC1 |=PMC_LVDSC1_LVDIE(1);  //Request a hardware interrupt when LVDF = 1
//	PMC->LVDSC1 |=PMC_LVDSC1_LVDRE(1);  //low voltage reset enable


}
void SCG_CMU_LVD_LVWSCG_IRQHandler(void)
{
	//After low voltage detected need to write essential data to EEPROM
	if(CalOk)
	{
		//Flash_write_struct();
		isFlagpoweroff=0x01;

		FLASH_DRV_EWrite(poweroff_address,1,&isFlagpoweroff);

	}
	PMC->LVDSC1 |=PMC_LVDSC1_LVDACK(1);
}
void Flash_write_struct(void)
{
	start_address=EERAMBASE;
	FLASH_DRV_EWrite((start_address+5),12,flash_array);

}
uint8_t flash_checksum_write(void)
{
	uint8_t checksum=0;

	checksum =( uint8_t)(gu16ZeroPositionS1+gu16ZeroPositionS2+gu16AS5600s1Angle+gu16AS5600s2Angle
			   +isFlagCWS1+isFlagCCWS1+gu16ClockWiseCounterS1+gu16ClockWiseCounterS2+
			   gu16CounterClockWiseCounterS1+gu16CounterClockWiseCounterS2);
	return checksum;

}
uint8_t flash_checksum_read(flashdata_struct *flashdata)
{
	uint8_t checksum=0;
	checksum=(uint8_t)(flashdata->F_ZeroPositionS1+flashdata->F_ZeroPositionS2+
			 flashdata->F_flagccw+ flashdata->F_flagcw+flashdata->F_countervalueS1+
			 flashdata->F_countervalueS2+30000+flashdata->F_previousS1+flashdata->F_previousS2);
	return checksum;

}
flashdata_struct Flash_read_struct(void)
{
	uint8_t flash_read_array[16]={0};
    flashdata_struct flash_struct;
    start_address=EERAMBASE;
	FLASH_DRV_EEERead((start_address+1), 16, flash_read_array);

	flash_struct.F_ZeroPositionS1=flash_read_array[0] |(flash_read_array[1]<<8);
	flash_struct.F_ZeroPositionS2=flash_read_array[2] |(flash_read_array[3]<<8);

	flash_struct.F_previousS1=flash_read_array[11] |(flash_read_array[12]<<8);
	flash_struct.F_previousS2=flash_read_array[13] |(flash_read_array[14]<<8);
    if(flash_read_array[10]==0x01)
    {
    	isFlagpoweroff=0x01;
    	isFlagpoweroff_1=0x01;

    }
    else
    {
    	isFlagpoweroff=0x00;

    }
	if(flash_read_array[4]==0x01)
	{
    flash_struct.F_flagcw=1;
    flash_struct.F_flagccw=0;

	}
	else if((flash_read_array[5]==0x01))
	{
		flash_struct.F_flagccw=1;
		flash_struct.F_flagcw=0;
	}
	else
	{
		flash_struct.F_flagccw=0;
		flash_struct.F_flagcw=0;
	}
	if((flash_struct.F_flagcw==1) || (flash_struct.F_flagccw==1))
	{
		flash_struct.F_countervalueS1=flash_read_array[6] |(flash_read_array[7]<<8);
		flash_struct.F_countervalueS2=flash_read_array[8] |(flash_read_array[9]<<8);
	}
	else
	{
		flash_struct.F_countervalueS1=15000;
		flash_struct.F_countervalueS2=15000;

	}
	flash_struct.F_checksum=flash_read_array[15];
	return flash_struct;
}
void Flash_array_store(uint8_t * flash_array)
{
	flash_array[0]=(uint8_t)(isFlagCWS1);
	flash_array[1]=(uint8_t)(isFlagCCWS1);
	if(isFlagCWS1 && isFlagCWS2 )
	{
		flash_array[2]=(uint8_t)(gu16ClockWiseCounterS1);
		flash_array[3]=(uint8_t)(gu16ClockWiseCounterS1>>8);
		flash_array[4]=(uint8_t)(gu16ClockWiseCounterS2);
		flash_array[5]=(uint8_t)(gu16ClockWiseCounterS2>>8);
	}
	else if(isFlagCCWS1 && isFlagCCWS2)
	{
		flash_array[2]=(uint8_t)(gu16CounterClockWiseCounterS1);
		flash_array[3]=(uint8_t)(gu16CounterClockWiseCounterS1>>8);
		flash_array[4]=(uint8_t)(gu16CounterClockWiseCounterS2);
		flash_array[5]=(uint8_t)(gu16CounterClockWiseCounterS2>>8);
	}
	else
	{
		flash_array[2]=0x00;
		flash_array[3]=0x00;
		flash_array[4]=0x00;
		flash_array[5]=0x00;
	}
	isFlagpoweroff=0x00;
	flash_array[6]=(uint8_t)(isFlagpoweroff);

	flash_array[7]=(uint8_t)(gu16AS5600s1Angle);
	flash_array[8]=(uint8_t)(gu16AS5600s1Angle>>8);
	flash_array[9]=(uint8_t)(gu16AS5600s2Angle);
	flash_array[10]=(uint8_t)(gu16AS5600s2Angle>>8);
	flash_array[11]=flash_checksum_write();
}

//------------------------------------------------------------------------------------------------------------------------------------------
void Init_Sys_Peripherals(void)
{
	/* Disable the watch-dog timer before starting any initializations */
	WDOG_disable();
	/* Initialize the internal peripheral clock to 48Mhz which is its maximum for the S32K116 */
	Clock_App_Init();
	/* Initialize the ADC to read the VBAT which is connected to the PTA7/ ADC0 */
	ADC_Init();
	/* Initialize the gpio's according to the application needs here */
	GPIO_App_init();
	/* Flash Initialization function */
	Flash_Init();
	/* Initialize the I2C @250Khz  according to the application */
	I2C_SENSOR_ON;
	LPI2C_init();

	/* Initialize the SysTic to generate various intervals for the task scheduler */
	Init_SysTick();
	/* Initialize the CAN peripheral to generate various intervals for the task scheduler */
	CAN_App_Config();
	/* Application Security rights validation */
	check_dorman_rights();
	/* Non volatile interrupt configurations according to the application needs */
	Low_voltage_detection_enable();
	NVIC_Init_IRQs();
	LPI2C_CONF_Reg_Write();
}
