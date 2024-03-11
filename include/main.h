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
/*     FILE NAME   :  main.c                                                      */
/*                                                                                */
/*     DESCRIPTION :  Main entry after reset.                                     */
/*                                                                                */
/* +--------------+---------+---------------------------------------------------+ */
/* |Rev. HISTORY  |  DATE   |           MODIFIED POINT                          | */
/* +--------------+---------+---------------------------------------------------+ */
/* |    0.1       |28-MAR-22| Initial Version                                   | */
/* +--------------+---------+---------------------------------------------------+ */
/**********************************************************************************/
#ifndef MAIN_H_
#define MAIN_H_
/**********************************************************************************/
/*Header File Inclusion                                                           */
/**********************************************************************************/

#include "S32K116.h"
#include "system_S32K116.h"
#include "App_Flag.h"
#include "analog_sense.h"
#include "PWRMode_Driver.h"
#include "GPIO_Driver.h"
#include "GPIO_app.h"
#include "Flash.h"
#include "status.h"
#include "string.h"
#include "lpi2c.h"
#include "stdio.h"
#include "FlexCAN.h"
#include "lptmr.h"
#include "CAN_app.h"
#include "app_sas.h"
#include "AS5600.h"
#include "watchdog_driver.h"
#include "Clock_app.h"
#include "Clock_Driver.h"
#define SET 	1
#define RESET 	0
#define TRUE 	1
#define FALSE 	0
#define SENSOR_TYPE_131 0
#define OUT_PHASE 20
#define IN_PHASE 2
#define ENABLE_UART_DEBUG   (0x01)
#define PHASE_LOW_LIMIT (float)(0.00001)
#define PHASE_HIGH_LIMIT (float)(10)
#define UPPER_PHASE_RATIO (float)(1.05)
#define LOWER_PHASE_RATIO (float)(0.97)
#define CSASGRP1ARBID_DATA 0x25
#define CSASGRP1ARBID1SEC_DATA 0x423
#define CSASGRP1ARBID960MSEC_DATA 0x4C6
#define CSAS_SIZE8BYTE   0x08
#define DELTA 3
#define FILTER 0.5
/*************************** struct********************************************/

typedef struct
{
	uint8_t  F_flagcw;
	uint8_t  F_flagccw;
	uint16_t F_ZeroPositionS1;
	uint16_t F_ZeroPositionS2;
	uint16_t F_countervalueS1;
	uint16_t F_countervalueS2;
	uint16_t F_previousS1;
	uint16_t F_previousS2;

	uint8_t  F_checksum;

}flashdata_struct;


/* Variable Extensions ************************************************************/
extern uint8_t gu8CSASCanDataFrame[8];
extern uint8_t CalOk;
extern flashdata_struct flash_struct_data;
//extern uint32_t start_address;
extern uint8_t clear[12];
extern uint16_t gu16ZeroPositionS1;
extern uint16_t gu16ZeroPositionS2;
extern float speed;
extern uint8_t zero_point_set;
extern uint8_t IsSasAngleDataValid;
extern uint8_t IsSasAngleExceed;
extern uint8_t IsSasHardFault;
extern uint8_t IsCalibarationComp;
extern uint8_t flash_array[12];
extern uint16_t gu16CounterClockWiseCounterS1;
extern uint16_t gu16CounterClockWiseCounterS2;
extern uint16_t gu16ClockWiseCounterS1;
extern uint16_t gu16ClockWiseCounterS2;
extern uint8_t isFlagCCWS1;
extern uint8_t isFlagCCWS2;
extern uint8_t isFlagCWS1;
extern uint8_t isFlagCWS2;
extern uint8_t isFlagpoweroff;
extern uint8_t isFlagpoweroff_1;
extern uint8_t cal_stuck;
extern uint8_t IsSasAngleoutphase;
extern uint8_t gu8STSnibble;
extern float gu16FinalSasAngle;
extern uint8_t decrement_flag;
extern uint8_t SSAZ;
extern uint16_t gu16AS5600s1Angle;
extern uint16_t gu16AS5600s2Angle;
extern uint16_t AngleDataCAN;
extern uint16_t difference;
extern float gu16AbsAngleS1;
extern float gu16AbsAngleS2;
extern uint16_t gu16RevCntS1;
extern uint16_t gu16RevCntS2;
extern float gu16AHFromS1;
extern float gu16AHFromS2;
extern float gu16FinalSasAngle;
extern uint16_t gu16TempVar;
extern uint8_t  gu8STS;
extern float gu16CWAngleS1;
extern float gu16CCWAngleS1;
extern float gu16CWAngleS2;
extern float gu16CCWAngleS2;
extern uint16_t gu16PreviousAngleData1;
extern uint16_t gu16CurrentAngleData1;
extern uint16_t gu16PreviousAngleData2;
extern uint16_t gu16CurrentAngleData2;
extern uint16_t gu16PreviousAngleData1_phase;
extern uint16_t gu16CurrentAngleData1_phase;
extern uint16_t gu16PreviousAngleData2_phase;
extern uint16_t gu16CurrentAngleData2_phase;
extern float gu16phaseangleratio;
extern uint8_t gu8CSASCanDataFrame_Anglelimit[8];
extern uint8_t gu8STDIDbit;
extern uint8_t isMovedS1;
extern uint8_t isMovedS2;
extern uint8_t phase_check;
extern uint16_t TempVar;
extern float TempVar_1;
extern float Temp_angle;
extern uint16_t velocity;
extern uint8_t gflag_5sec;
extern uint8_t gTime;
extern uint8_t can_comm_state;
extern uint8_t first_read;
extern uint16_t gu16Difference;
/* Variable Extensions ************************************************************/

/* Constant definitions                                          */
/**********************************************************************************/


/* Function Declarations **********************************************************/
void Init_SysTick(void);
void Init_Sys_Peripherals(void);
void Scheduler_Task(void);
void Task_1ms(void);
void Task_12ms(void);
void Task_50ms(void);
void Task_960msec(void);
void Task_1sec(void);
void Port_Init (void);
void NVIC_Init_IRQs (void);
void ADC_Disable(void);
uint8_t CSAS_checksum_Byte(uint8_t *);
void Flash_write_struct(void);
uint8_t flash_checksum_write(void);
uint8_t flash_checksum_read(flashdata_struct *);
flashdata_struct Flash_read_struct(void);
void Flash_array_store(uint8_t * );
void Pulse_generate(float);
uint16_t SAS_Velocity_Calc(uint16_t );
/* Function Declarations **********************************************************/

uint16_t GetTheAngleCounterValueCW(uint16_t CurrentAngle, uint16_t PreviousAngle,
		uint16_t CounterValue,uint8_t SensorSelect);
uint16_t GetTheAngleCounterValueCCW(uint16_t CurrentAngle, uint16_t PreviousAngle,
		uint16_t CounterValue,uint8_t SensorSelect);

#endif /* MAIN_H_ */
