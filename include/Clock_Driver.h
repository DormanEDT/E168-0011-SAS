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
/*     FILE NAME   :  Clock_Driver.h                                              */
/*                                                                                */
/*     DESCRIPTION :  Clock driver header.                                        */
/*                                                                                */
/* +--------------+---------+---------------------------------------------------+ */
/* |Rev. HISTORY  |  DATE   |           MODIFIED POINT                          | */
/* +--------------+---------+---------------------------------------------------+ */
/* |    0.1       |06-OCT-22| Initial Version                                   | */
/* +--------------+---------+---------------------------------------------------+ */
/**********************************************************************************/

/**********************************************************************************/
/*Modifications                                                                   */
/**********************************************************************************/

/**********************************************************************************/

#ifndef CLOCK_DRIVER_H_
#define CLOCK_DRIVER_H_
/****************************Header File Inclusion*********************************/

#include "main.h"
#include "stdbool.h"
/**********************************************************************************/
typedef enum
{
FIRC_SYS_CLK=0,
SIRC_SYS_CLK=1,
SOSC_SYS_CLK=2,
DEFAULT_SYS_CLK=3
}
system_clocktype;
typedef enum
{
	Internal_Crystal=0,
	External_Crystal=1
}Oscilator_Type;

typedef enum
{
	FIRC_48Mhz=0,
	SIRC_8Mhz=1,
	SOSC_4_8Mhz=2,
	SOSC_8_40Mhz=3
}Clock_Range_Type;

typedef enum
{
	SOSC_CLK_SELECT=1,
	SIRC_CLK_SELECT=2,
	FIRC_CLK_SELECT=3
}Clock_Selection_Type;

typedef enum
{
	Clock_DIV_BY_1=1,
	Clock_DIV_BY_2=2,
	Clock_DIV_BY_4=3,
	Clock_DIV_BY_8=4,
	Clock_DIV_BY_16=5,
	Clock_DIV_BY_32=6,
	Clock_DIV_BY_64=7,
}Clock_Divider_Type;
typedef enum
{
	Core_DIV_BY_1=0,
	Core_DIV_BY_2=1,
	Core_DIV_BY_3=2,
	Core_DIV_BY_4=3,
	Core_DIV_BY_5=4,
	Core_DIV_BY_6=5,
	Core_DIV_BY_7=6,
	Core_DIV_BY_8=7,
	Core_DIV_BY_9=8,
	Core_DIV_BY_10=9,
	Core_DIV_BY_11=10,
	Core_DIV_BY_12=11,
	Core_DIV_BY_13=12,
	Core_DIV_BY_14=13,
	Core_DIV_BY_15=14,
	Core_DIV_BY_16=15
}Core_clock_Div_Type;
typedef enum
{
	Bus_DIV_BY_1=0,
	Bus_DIV_BY_2=1,
	Bus_DIV_BY_3=2,
	Bus_DIV_BY_4=3,
	Bus_DIV_BY_5=4,
	Bus_DIV_BY_6=5,
	Bus_DIV_BY_7=6,
	Bus_DIV_BY_8=7,
	Bus_DIV_BY_9=8,
	Bus_DIV_BY_10=9,
	Bus_DIV_BY_11=10,
	Bus_DIV_BY_12=11,
	Bus_DIV_BY_13=12,
	Bus_DIV_BY_14=13,
	Bus_DIV_BY_15=14,
	Bus_DIV_BY_16=15
}Bus_clock_Div_Type;

typedef enum
{
	Slow_DIV_BY_1=0,
	Slow_DIV_BY_2=1,
	Slow_DIV_BY_3=2,
	Slow_DIV_BY_4=3,
	Slow_DIV_BY_5=4,
	Slow_DIV_BY_6=5,
	Slow_DIV_BY_7=6,
	Slow_DIV_BY_8=7

}Slow_clock_Div_Type;
typedef struct
{
	Oscilator_Type osctype;
	Clock_Selection_Type clockselect;
	Clock_Range_Type range;

}
Clock_DrvInit_Struct;
typedef struct
{
	Oscilator_Type osctype;
	Clock_Selection_Type clockselect;
	Clock_Range_Type range;
	Clock_Divider_Type clockdiv;
	Core_clock_Div_Type corediv;
	Bus_clock_Div_Type	busdiv;
	Slow_clock_Div_Type slowdiv;


}Clock_Init_Struct;
typedef enum
{
    SCG_SYSTEM_CLOCK_CORE,  /*!< Core clock.        */
    SCG_SYSTEM_CLOCK_BUS,   /*!< BUS clock.         */
    SCG_SYSTEM_CLOCK_SLOW,  /*!< System slow clock. */
    SCG_SYSTEM_CLOCK_MAX,   /*!< Max value.         */
} scg_system_clock_type_t;

typedef struct
{
	uint32_t soscfreq;
	uint32_t fircfreq;
	uint32_t sircfreq;
	uint32_t corefreq;
	uint32_t busfreq;
	uint32_t slowfreq;

}frequency_struct;
typedef enum
{
    /* Main clocks */
    CORE_CLOCK                     = 0u,       /*!< Core clock                     */
    BUS_CLOCK                      = 1u,       /*!< Bus clock                      */
    SLOW_CLOCK                     = 2u,       /*!< Slow clock                     */

    /* Other internal clocks used by peripherals. */
    SIRC_CLOCK                     = 3u,       /*!< SIRC clock                     */
    FIRC_CLOCK                     = 4u,       /*!< FIRC clock                     */
    SOSC_CLOCK                     = 5u,       /*!< SOSC clock                     */
	LPO_CLK                  = 6u,      /*!< LPO clock                      */
	LPO_1K_CLK               = 7u,      /*!< LPO 1KHz clock                 */
	LPO_32K_CLK              = 8u,      /*!< LPO 32KHz clock                */
	LPO_128K_CLK             = 9u,      /*!< LPO 128KHz clock               */
}clock_names_type;
typedef enum
{
LPO_128_Khz=0,
LPO_32_Khz=2,
LPO_1_Khz=3
}
lpo_clock_type;
extern frequency_struct retfreq;

/*******************************Function Declarations******************************/
bool SIM_GetLpo32KStatus(const SIM_Type * );
bool SIM_GetLpo1KStatus(const SIM_Type * );
uint32_t GetCurrentSystemClockSource(const SCG_Type * );
bool PMC_GetLpoMode(const PMC_Type * const baseAddr);
void Clock_Drv_Init(Clock_DrvInit_Struct* ,frequency_struct *,system_clocktype );
void  Clock_Init(Clock_DrvInit_Struct*,frequency_struct *,system_clocktype);
uint32_t CLOCK_DRV_GetFreq(clock_names_type);
void LPO_CLOCK_Source(lpo_clock_type);
void LPO_CLOCK_Enable(void);
void LPO_CLOCK_Disable(void);
/**********************************************************************************/


#endif /* CLOCK_DRIVER_H_ */
