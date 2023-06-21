/*
 * app_sas.h
 *
 *  Created on: 27-July-2022
 *      Author: Admin
 */

#ifndef APP_SAS_H_
#define APP_SAS_H_

#include "main.h"


//---------------------------------------------------------------------------------------------@
//---------------------------------------------------------------------------------------------@

extern uint8_t u8RegReadValMagPresent;
extern uint8_t u8RegReadValAngleLSB;
extern uint8_t u8RegReadValAngleMSB;
extern uint16_t u8RegAngleData;


/*********************************************************************************************************************************************/
void SAS_10MsPeriodic_Msg(void);
void SAS_ScanToolResponce_Msg(void);
uint8_t ReadAndDisplayAngle(void);
void SAS_GetAngleAndVelocity(void);
uint16_t SAS_ReadAngelSensorAt(uint8_t);
void SAS_Write_CONF_Reg(void);
void sleep_actions(void);
void wakeup_actions(void);
#endif /* APP_SAS_H_*/
