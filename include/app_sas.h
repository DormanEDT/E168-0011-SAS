/*
 * app_sas.h
 *
 *  Created on: 27-July-2022
 *      Author: Admin
 */

#ifndef APP_SAS_H_
#define APP_SAS_H_

#include "main.h"

#define FLAGCW 		1
#define FLAGCCW 	2
//---------------------------------------------------------------------------------------------@
//---------------------------------------------------------------------------------------------@

extern uint8_t u8RegReadValMagPresent;
extern uint8_t u8RegReadValAngleLSB;
extern uint8_t u8RegReadValAngleMSB;
extern uint16_t u8RegAngleData;
extern uint8_t flag_anticlockwise;
extern uint8_t	flag_clockwise;

/*********************************************************************************************************************************************/
void SAS_10MsPeriodic_Msg(void);
void SAS_ScanToolResponce_Msg(void);
uint8_t ReadAndDisplayAngle(void);
void SAS_GetAngleAndVelocity(void);
void App_SAS_Operations(void);
void SAS_Write_CONF_Reg(void);
void LPI2C_CONF_Reg_Write(void);
void sleep_actions(void);
void wakeup_actions(void);
void Position_Check(void);
#endif /* APP_SAS_H_*/
