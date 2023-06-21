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
/*     FILE NAME   :  watchdog_driver.h                                           */
/*                                                                                */
/*     DESCRIPTION :  This is the code file for watchdog routine.                 */
/*                                                                                */
/* +--------------+---------+---------------------------------------------------+ */
/* |Rev. HISTORY  |  DATE   |           MODIFIED POINT                          | */
/* +--------------+---------+---------------------------------------------------+ */
/* |    0.1       |13-OCT-22| 			Initial Version                         | */
/* +--------------+---------+---------------------------------------------------+ */
/**********************************************************************************/

/**********************************************************************************/
/*Modifications                                                                   */
/**********************************************************************************/
#ifndef WATCHDOG_H_
#define WATCHDOG_H_

/**********************************************************************************/
/*Header File Inclusion                                                           */
/**********************************************************************************/
#include "main.h"

/**********************************************************************************/
/*Constant and Variable Declaration                                               */
/**********************************************************************************/
#define UNLOCK_WATCHDOG 0xD928C520
#define REFRESH_WATCHDOG 0xB480A602
/* Variable Extensions ************************************************/
typedef enum
{
    WDOG_BUS_CLK = 0x00U,  /*!< Input clock alternative 1. */
    WDOG_LPO_CLOCK= 0x01U,  /*!< Input clock alternative 2. */
    INT_CLK = 0x02U,  /*!< Input clock alternative 3. */
    EXT_CLK= 0x03U   /*!< Input clock alternative 4. */
} wdog_clk_source_t;


typedef enum
{
  WDOG_DISABLE  = 0x00U,
  WDOG_ENABLE   = 0x01U
}wdog_en_dis_t;

typedef enum
{
  WDOG_16BIT  = 0x00U,
  WDOG_32BIT  = 0x01U
}wdog_refresh_t;

typedef enum
{
  WDOG_LOCK    = 0x00U,
  WDOG_UNLOCK   = 0x01U
}wdog_lock_unlock_t;
typedef enum
{
  WDOG_RECONFIG    = 0x00U,
  WDOG_CONFIG  = 0x01U
}wdog_config_reconfig_t;
typedef enum
{
  INTERUPT_DISABLE  = 0x00U,   /*!< Software trigger. */
  INTERUPT_ENABLE   = 0x01U    /*!< Hardware trigger. */
}wdog_interupt_t;

typedef enum
{
    WDOG_TST_DISABLED                     = 0x00U, /*!< Test mode disabled */
    WDOG_TST_USER                         = 0x01U, /*!< User mode enabled. (Test mode disabled.) */
    WDOG_TST_LOW                          = 0x02U, /*!< Test mode enabled, only the low byte is used. */
    WDOG_TST_HIGH                         = 0x03U  /*!< Test mode enabled, only the high byte is used. */
} wdog_op_mode_t;

typedef enum
{
   WDOG_PRESCALER_DISABLE               =0X00U,
   WDOG_PRESCALER_ENABLE                =0X01U

} wdog_prescaler_t;

typedef enum
{
    WDOG_DEBUG_MODE                        = 0x00U, /*!< Debug mode */
    WDOG_WAIT_MODE                         = 0x01U, /*!< Wait mode */
    WDOG_STOP_MODE                         = 0x02U  /*!< Stop mode */
} wdog_set_mode_t;
typedef enum
{
  WDOG_RCS   = 0x00U,
  WDOG_RCS_SUC   = 0x01U
}wdog_rcs_t;

/*!
 * @brief WDOG user configuration structure
 * Implements : wdog_user_config_t_Class
 */
typedef struct
{
    wdog_clk_source_t    clkSource;       /*!< The clock source of the WDOG */
    wdog_op_mode_t       opMode;          /*!< The modes in which the WDOG is functional */
    bool                 updateEnable;    /*!< If true, further updates of the WDOG are enabled */
    bool                 intEnable;       /*!< If true, an interrupt request is generated before reset */
    bool                 winEnable;       /*!< If true, window mode is enabled */
    uint16_t             windowValue;     /*!< The window value */
    uint16_t             timeoutValue;    /*!< The timeout value */
    bool                 prescalerEnable; /*!< If true, a fixed 256 prescaling of the counter reference clock is enabled */
} wdog_user_config_t;

/* Variable Extensions ************************************************/

/* Function Declarations ***********************************************/
void WDOG_init(uint32_t time);
void WDOG_refresh(void);
void WDOG_disable(void);
void WDOG_enable(uint32_t time);
/* Function Declarations ***********************************************/


#endif /* WATCHDOG_H_ */
/*****************************************************************************
 *
 *	End of file LPIT0_Driver.h
 *
 *****************************************************************************/

