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
*
*                                   MODBUS FREERTOS LAYER INTERFACE
*
* Filename : mb_os.c
* Version  : V0.0.00
* date:	   : 2024/08/17
* author   : ZGQ
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define   MB_OS_MODULE
#include <mb.h>
#include "mb_os.h"



/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

#if (MODBUS_CFG_MASTER_EN == DEF_ENABLED)
static SemaphoreHandle_t MB_OS_RxSemTbl[MODBUS_CFG_MAX_CH] = {NULL};
#endif

#if (MODBUS_CFG_SLAVE_EN  == DEF_ENABLED)
TaskHandle_t MB_OS_HandlerSlave;
static QueueHandle_t MB_Queue_HandlerSlave;
#endif


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#if (MODBUS_CFG_MASTER_EN == DEF_ENABLED)
static  void  MB_OS_InitMaster(void);
static  void  MB_OS_ExitMaster(void);
#endif

#if (MODBUS_CFG_SLAVE_EN == DEF_ENABLED)
static  void  MB_OS_InitSlave (void);
static  void  MB_OS_ExitSlave (void);
static  void  MB_OS_RxTask    (void  *p_arg);
#endif


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                              MB_OS_Init()
*
* Description : This function initializes the RTOS interface.  This function creates the following:
*
*               (1) A message queue to signal the reception of a packet.
*
*               (2) A task that waits for packets to be received.
*
* Argument(s) : none
*
* Return(s)   : none.
*
* Caller(s)   : MB_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  MB_OS_Init (void)
{
#if (MODBUS_CFG_MASTER_EN == DEF_ENABLED)
    MB_OS_InitMaster();
#endif

#if (MODBUS_CFG_SLAVE_EN == DEF_ENABLED)
    MB_OS_InitSlave();
#endif
}


/*
*********************************************************************************************************
*                                          MB_OS_InitMaster()
*
* Description : This function initializes and creates the kernel objectes needed for Modbus Master
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : MB_OS_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (MODBUS_CFG_MASTER_EN == DEF_ENABLED)
static  void  MB_OS_InitMaster (void)
{
    CPU_INT08U   i;

    for (i = 0; i < MODBUS_CFG_MAX_CH; i++) {                 /* Create a semaphore for each channel   */
        MB_OS_RxSemTbl[i] = xSemaphoreCreateBinary();
    }
}
#endif


/*
*********************************************************************************************************
*                                          MB_OS_InitSlave()
*
* Description : This function initializes and creates the kernel objectes needed for Modbus Salve
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : MB_OS_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (MODBUS_CFG_SLAVE_EN == DEF_ENABLED)
static  void  MB_OS_InitSlave (void)
{
	xTaskCreate((TaskFunction_t) MB_OS_RxTask, (const char*) "Modbus Rx Task",
			(uint16_t) MB_OS_CFG_RX_TASK_STK_SIZE, (void*) NULL,
			(UBaseType_t) MB_OS_CFG_RX_TASK_PRIO, (TaskHandle_t*) &MB_OS_HandlerSlave);

	MB_Queue_HandlerSlave = xQueueCreate( MB_OS_RX_QUEUE_NUM, sizeof(MODBUS_CH *));
}
#endif


/*
*********************************************************************************************************
*                                             MB_OS_Exit()
*
* Description : This function is called to terminate the RTOS interface for Modbus channels.  We delete
*               the following uC/OS-II objects:
*
*               (1) An event flag group to signal the reception of a packet.
*               (2) A task that waits for packets to be received.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : MB_Exit().
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  MB_OS_Exit (void)
{
#if (MODBUS_CFG_MASTER_EN == DEF_ENABLED)
    MB_OS_ExitMaster();
#endif

#if (MODBUS_CFG_SLAVE_EN == DEF_ENABLED)
    MB_OS_ExitSlave();
#endif
}


/*
*********************************************************************************************************
*                                          MB_OS_ExitMaster()
*
* Description : This function is called to terminate the RTOS interface for Modbus Master channels.  The
*               following objects are deleted.
*
*               (1) An event flag group to signal the reception of a packet.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : MB_OS_Exit().
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (MODBUS_CFG_MASTER_EN == DEF_ENABLED)
static  void  MB_OS_ExitMaster (void)
{
    CPU_INT08U  i;

    for (i = 0; i < MODBUS_CFG_MAX_CH; i++) {                 /* Delete semaphore for each channel     */
        // 确保信号量句柄非空
        if (MB_OS_RxSemTbl[i] != NULL)
        {
            // 删除信号量
            vSemaphoreDelete(MB_OS_RxSemTbl[i]);
        }
    }
}
#endif


/*
*********************************************************************************************************
*                                          MB_OS_ExitSlave()
*
* Description : This function is called to terminate the RTOS interface for Modbus Salve channels.
*               The following objects are deleted.
*
*               (1) A task that waits for packets to be received.
*               (2) A message queue to signal the reception of a packet.
*
* Argument(s) : none
*
* Return(s)   : none.
*
* Caller(s)   : MB_OS_Exit().
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (MODBUS_CFG_SLAVE_EN == DEF_ENABLED)
void  MB_OS_ExitSlave (void)
{
	vTaskDelete(MB_OS_HandlerSlave);
}
#endif


/*
*********************************************************************************************************
*                                              MB_OS_RxSignal()
*
* Description : This function signals the reception of a packet either from the Rx ISR(s) or the RTU timeout
*               timer(s) to indicate that a received packet needs to be processed.
*
* Argument(s) : pch     specifies the Modbus channel data structure in which a packet was received.
*
* Return(s)   : none.
*
* Caller(s)   : MB_ASCII_RxByte(),
*               MB_RTU_TmrUpdate().
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  MB_OS_RxSignal (MODBUS_CH *pch)
{
	BaseType_t yield;

    if (pch != (MODBUS_CH *)0) {
        switch (pch->MasterSlave) {
#if (MODBUS_CFG_MASTER_EN == DEF_ENABLED)
        case MODBUS_MASTER:
        	if( MB_OS_RxSemTbl[pch->Ch] != NULL ){
        		yield = pdFALSE;
        		if (xSemaphoreGiveFromISR (MB_OS_RxSemTbl[pch->Ch], &yield) == pdTRUE) {
        			portYIELD_FROM_ISR (yield);
        		}
        	}
        	break;
#endif

#if (MODBUS_CFG_SLAVE_EN == DEF_ENABLED)
        case MODBUS_SLAVE:
        default:
            if (xQueueSendFromISR (MB_Queue_HandlerSlave, &pch, &yield) == pdTRUE) {
            	portYIELD_FROM_ISR (yield);
            }
        	break;
#endif
        }
    }
}


/*
*********************************************************************************************************
*                                              MB_OS_RxWait()
*
* Description : This function waits for a response from a slave.
*
* Argument(s) : pch     specifies the Modbus channel data structure to wait on.
*
*               perr    is a pointer to a variable that will receive an error code.  Possible errors are:
*
*                       MODBUS_ERR_NONE        the call was successful and a packet was received
*                       MODBUS_ERR_TIMED_OUT   a packet was not received within the specified timeout
*                       MODBUS_ERR_NOT_MASTER  the channel is not a Master
*                       MODBUS_ERR_INVALID     an invalid error was detected
*
* Return(s)   : none.
*
* Caller(s)   : MBM_FCxx()  Modbus Master Functions
*
* Return(s)   : none.
*********************************************************************************************************
*/

void  MB_OS_RxWait (MODBUS_CH   *pch,
                    CPU_INT16U  *perr)
{
#if (MODBUS_CFG_MASTER_EN == DEF_ENABLED)
    if (pch != (MODBUS_CH *)0) {
        if (pch->MasterSlave == MODBUS_MASTER) {
            if( MB_OS_RxSemTbl[pch->Ch] != NULL )
            {
                if( xSemaphoreTake( MB_OS_RxSemTbl[pch->Ch], pch->RxTimeout ) == pdTRUE )
                {
                    *perr = MODBUS_ERR_NONE;
                }else{
                    *perr = MODBUS_ERR_TIMED_OUT;
                }
            }
        } else {
            *perr = MODBUS_ERR_NOT_MASTER;
        }
    } else {
        *perr = MODBUS_ERR_NULLPTR;
    }
#else
    *perr = MODBUS_ERR_INVALID;
#endif
}

/*
*********************************************************************************************************
*                                            MB_OS_RxTask()
*
* Description : This task is created by MB_OS_Init() and waits for signals from either the Rx ISR(s) or
*               the RTU timeout timer(s) to indicate that a packet needs to be processed.
*
* Argument(s) : p_arg       is a pointer to an optional argument that is passed by uC/OS-II to the task.
*                           This argument is not used.
*
* Return(s)   : none.
*
* Caller(s)   : This is a Task.
*
* Return(s)   : none.
*********************************************************************************************************
*/

#if (MODBUS_CFG_SLAVE_EN == DEF_ENABLED)
static  void  MB_OS_RxTask (void *p_arg)
{
    MODBUS_CH   *pch;
    (void)p_arg;

    while (DEF_TRUE) {
    	 xQueueReceive(
    			 MB_Queue_HandlerSlave,
    	         &pch,
				 portMAX_DELAY);
        MB_RxTask(pch);                          /* Process the packet received                        */
    }
}
#endif

