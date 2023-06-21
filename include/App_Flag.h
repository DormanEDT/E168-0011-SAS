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
/*     FILE NAME   :  App_Flag.h                                                  */
/*                                                                                */
/*     DESCRIPTION :  This is the  file for flag used in the application          */
/*                                                                                */
/* +--------------+---------+---------------------------------------------------+ */
/* |Rev. HISTORY  |  DATE   |           MODIFIED POINT                          | */
/* +--------------+---------+---------------------------------------------------+ */
/* |    0.1       |28-MAR-22| Initial Version                                   | */
/* +--------------+---------+---------------------------------------------------+ */
/**********************************************************************************/

#ifndef APP_FLAG_H_
#define APP_FLAG_H_

typedef struct MyFlag
{
    unsigned  int  Msec1_Flag                  :1;
    unsigned  int  Msec12_Flag                 :1;
    unsigned  int  Msec960_Flag				   :1;
    unsigned  int  Sec1_Flag                   :1;
    unsigned  int  BattVolt_Low                :1;
    unsigned  int  SAZS     				   :1;

}Bit_Flag;
extern struct MyFlag Flag;


#endif /* APP_FLAG_H_ */
