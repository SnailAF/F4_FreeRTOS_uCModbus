/*
*********************************************************************************************************
*                                              uC/Modbus
*                                       The Embedded Modbus Stack
*
*                    Copyright 2003-2020 Silicon Laboratories Inc. www.silabs.com
*
*                                 SPDX-License-Identifier: APACHE-2.0
*
*               This software is subject to an open source license and is distributed by
*                Silicon Laboratories Inc. pursuant to the terms of the Apache License,
*                    Version 2.0 available at www.apache.org/licenses/LICENSE-2.0.
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      MODBUS CONFIGURATION
*
* Filename : mb_cfg.h
* Version  : V2.14.00
*********************************************************************************************************
* Note(s)  : (1) This file contains configuration constants for uC/Modbus
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                  MODBUS MODULES CONFIGURATION
*********************************************************************************************************
*/

#define  MODBUS_CFG_SLAVE_EN              DEF_ENABLED           /* Enable or Disable  Modbus Slave                    */
#define  MODBUS_CFG_MASTER_EN             DEF_ENABLED          /* Enable or Disable  Modbus Master                   */

/*
*********************************************************************************************************
*                                  MODBUS MODES CONFIGURATION
*********************************************************************************************************
*/

#define  MODBUS_CFG_ASCII_EN               DEF_ENABLED          /* Modbus ASCII is supported when DEF_ENABLED         */
#define  MODBUS_CFG_RTU_EN                 DEF_ENABLED          /* Modbus RTU   is supported when DEF_ENABLED         */

/*
*********************************************************************************************************
*                               MODBUS COMMUNICATION CONFIGURATION
*********************************************************************************************************
*/

#define  MODBUS_CFG_MAX_CH                          10           /* Maximum number of Modbus channels.                 */

#define  MODBUS_CFG_BUF_SIZE                       255           /* Maximum outgoing message size.                     */

/*
*********************************************************************************************************
*                                  MODBUS FLOATING POINT SUPPORT
*********************************************************************************************************
*/

#define  MODBUS_CFG_FP_EN                  DEF_DISABLED          /* Enable Floating-Point support.                     */

#if     (MODBUS_CFG_FP_EN == DEF_ENABLED)
#define  MODBUS_CFG_FP_START_IX                  50000           /* Start address of Floating-Point registers          */
#else
#define  MODBUS_CFG_FP_START_IX                 65500           /* Floating point is disabled, set start of ...       */
                                                                /*   ...FP very high                                  */
#endif


/*
*********************************************************************************************************
*                                   MODBUS FUNCTIONS CONFIGURATION
*********************************************************************************************************
*/

#define  MODBUS_CFG_FC01_EN                DEF_ENABLED          /* Enable or Disable support for Modbus functions     */
#define  MODBUS_CFG_FC02_EN                DEF_ENABLED
#define  MODBUS_CFG_FC03_EN                DEF_ENABLED
#define  MODBUS_CFG_FC04_EN                DEF_ENABLED
#define  MODBUS_CFG_FC05_EN                DEF_ENABLED
#define  MODBUS_CFG_FC06_EN                DEF_ENABLED
#define  MODBUS_CFG_FC08_EN                DEF_ENABLED
#define  MODBUS_CFG_FC15_EN                DEF_ENABLED
#define  MODBUS_CFG_FC16_EN                DEF_ENABLED
#define  MODBUS_CFG_FC20_EN                DEF_DISABLED
#define  MODBUS_CFG_FC21_EN                DEF_DISABLED


/*
*********************************************************************************************************
*                                   MODBUS TIMER
*********************************************************************************************************
*/
#define MODBUS_TIMER TIM3
/*
*********************************************************************************************************
*                                    CONFIGURE STANDARD DATA TYPES
*
* Note(s) : (1) Configure standard data types according to CPU-/compiler-specifications.
*
*           (2) (a) (1) 'CPU_FNCT_VOID' data type defined to replace the commonly-used function pointer
*                       data type of a pointer to a function which returns void & has no arguments.
*
*                   (2) Example function pointer usage :
*
*                           CPU_FNCT_VOID  FnctName;
*
*                           FnctName();
*
*               (b) (1) 'CPU_FNCT_PTR'  data type defined to replace the commonly-used function pointer
*                       data type of a pointer to a function which returns void & has a single void
*                       pointer argument.
*
*                   (2) Example function pointer usage :
*
*                           CPU_FNCT_PTR   FnctName;
*                           void          *p_obj
*
*                           FnctName(p_obj);
*********************************************************************************************************
*/

