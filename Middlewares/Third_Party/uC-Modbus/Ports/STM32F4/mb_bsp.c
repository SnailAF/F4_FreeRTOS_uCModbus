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
*                                            uC/Modbus
*
*                                    MODBUS BOARD SUPPORT PACKAGE
*                                       STM32F4
*
* Filename : mb_bsp.c
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

#include    "shUsart.h"
#include    "mb_cfg.h"
#include    "mb.h"
#include    "shtimer.h"

void  MB_CommRxISR_1_Handler (char c);
void  MB_CommTxISR_1_Handler( void );
void  MB_CommRxISR_2_Handler (char c);
void  MB_CommTxISR_2_Handler( void );
void  MB_CommRxISR_3_Handler (char c);
void  MB_CommTxISR_3_Handler( void );
void  MB_CommRxISR_4_Handler (char c);
void  MB_CommTxISR_4_Handler( void );
void  MB_CommRxISR_5_Handler (char c);
void  MB_CommTxISR_5_Handler( void );
void  MB_CommRxISR_6_Handler (char c);
void  MB_CommTxISR_6_Handler( void );
/*
*********************************************************************************************************
*                                             MB_CommExit()
*
* Description : This function is called to terminate Modbus communications.  All Modbus channels are close.
*
* Argument(s) : none
*
* Return(s)   : none.
*
* Caller(s)   : MB_Exit()
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  MB_CommExit (void)
{
    CPU_INT08U   ch;
    MODBUS_CH   *pch;   

    pch = &MB_ChTbl[0];
    for (ch = 0; ch < MODBUS_CFG_MAX_CH; ch++) {
        if(pch->BaudRate) {
            serial_end(pch->PortNbr);
        }
        pch++;
    }
}


/*
*********************************************************************************************************
*                                           MB_CommPortCfg()
*
* Description : This function initializes the serial port to the desired baud rate and the UART will be
*               configured for N, 8, 1 (No parity, 8 bits, 1 stop).
*
* Argument(s) : pch        is a pointer to the Modbus channel
*               port_nbr   is the desired serial port number.  This argument allows you to assign a
*                          specific serial port to a sepcific Modbus channel.
*               baud       is the desired baud rate for the serial port.
*               parity     is the desired parity and can be either:
*
*                          MODBUS_PARITY_NONE
*                          MODBUS_PARITY_ODD
*                          MODBUS_PARITY_EVEN
*
*               bits       specifies the number of bit and can be either 7 or 8.
*               stops      specifies the number of stop bits and can either be 1 or 2
*
* Return(s)   : none.
*
* Caller(s)   : MB_CfgCh()
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  MB_CommPortCfg (MODBUS_CH  *pch,
                      CPU_INT08U  port_nbr, 
                      CPU_INT32U  baud, 
                      CPU_INT08U  bits, 
                      CPU_INT08U  parity, 
                      CPU_INT08U  stops)
{
    Serial_Config  cof = 0;
    if (pch != (MODBUS_CH *)0) {
        pch->PortNbr  = port_nbr;
        pch->BaudRate = baud;
        pch->Parity   = parity;
        pch->Bits     = bits;
        pch->Stops    = stops;     
        switch(bits){
            case 9:
                cof = UART_WORDLENGTH_9B;
                break;
            case 8:
            default:
                cof = UART_WORDLENGTH_8B;
            break;
        }
        switch(parity){
            case MODBUS_PARITY_ODD:
                cof |= UART_PARITY_ODD;
                break;
            case MODBUS_PARITY_EVEN:
                cof |= UART_PARITY_EVEN;
                break;
            case MODBUS_PARITY_NONE:
            default:
                cof |= UART_PARITY_NONE;
                break;
        }
        switch (stops)
        {
        case 2:
            cof |= (USART_STOPBITS_2>> 8);
            break;
        case 1:
            cof |= (USART_STOPBITS_1>> 8);
        default:
            break;
        }
        serial_begin_cof(port_nbr,baud,cof);
        switch (port_nbr)
        {
        case serial1:
            serial_attachRxInterrupt(serial1,MB_CommRxISR_1_Handler);
            serial_attachTxInterrupt(serial1,MB_CommTxISR_1_Handler);
            break;
        case serial2:
            serial_attachRxInterrupt(serial2,MB_CommRxISR_2_Handler);
            serial_attachTxInterrupt(serial2,MB_CommTxISR_2_Handler);
            break;
        case serial3:
            serial_attachRxInterrupt(serial3,MB_CommRxISR_3_Handler);
            serial_attachTxInterrupt(serial3,MB_CommTxISR_3_Handler);
            break;
        case serial4:
            serial_attachRxInterrupt(serial4,MB_CommRxISR_4_Handler);
            serial_attachTxInterrupt(serial4,MB_CommTxISR_4_Handler);
            break;
        case serial5:
            serial_attachRxInterrupt(serial5,MB_CommRxISR_5_Handler);
            serial_attachTxInterrupt(serial5,MB_CommTxISR_5_Handler);
            break;
        case serial6:
            serial_attachRxInterrupt(serial6,MB_CommRxISR_6_Handler);
            serial_attachTxInterrupt(serial6,MB_CommTxISR_6_Handler);
            break;
        default:
            break;
        }
    }
}


/*
*********************************************************************************************************
*                                         MB_CommRxIntDis()
*
* Description : This function disables Rx interrupts.
*
* Argument(s) : pch        is a pointer to the Modbus channel
*
* Return(s)   : none.
*
* Caller(s)   : MB_CommExit()
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  MB_CommRxIntDis (MODBUS_CH  *pch)
{
    UART_HandleTypeDef *huart;
    huart = serial_get_handle(pch->PortNbr);
    CPU_CRITICAL_ENTER();
    __HAL_UART_DISABLE_IT(huart,UART_IT_RXNE);
    CPU_CRITICAL_EXIT();

}
/*
*********************************************************************************************************
*                                          MB_CommRxIntEn()
*
* Description : This function enables Rx interrupts.
*
* Argument(s) : pch        is a pointer to the Modbus channel
*
* Return(s)   : none.
*
* Caller(s)   : MB_TxByte()
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  MB_CommRxIntEn (MODBUS_CH  *pch)
{
    UART_HandleTypeDef *huart;
    huart = serial_get_handle(pch->PortNbr);
    RW_Ctrl_ReadEnable(pch->PortNbr);
    CPU_CRITICAL_ENTER();
    __HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);
    CPU_CRITICAL_EXIT();

}
/*
*********************************************************************************************************
*                                         MB_CommTxIntDis()
*
* Description : This function disables Tx interrupts.
*
* Argument(s) : pch        is a pointer to the Modbus channel
*
* Return(s)   : none.
*
* Caller(s)   : MB_CommExit()
*               MB_TxByte()
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  MB_CommTxIntDis (MODBUS_CH  *pch)
{

    UART_HandleTypeDef *huart;
    huart = serial_get_handle(pch->PortNbr);
    CPU_CRITICAL_ENTER();
    __HAL_UART_DISABLE_IT(huart,UART_IT_TC);
    CPU_CRITICAL_EXIT();
}

/*
*********************************************************************************************************
*                                         MB_CommTxIntEn()
*
* Description : This function enables Tx interrupts.
*
* Argument(s) : pch        is a pointer to the Modbus channel
*
* Return(s)   : none.
*
* Caller(s)   : MB_Tx()
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  MB_CommTxIntEn (MODBUS_CH  *pch)
{
    UART_HandleTypeDef *huart;
    huart = serial_get_handle(pch->PortNbr);
    RW_Ctrl_WriteEnable(pch->PortNbr);
    CPU_CRITICAL_ENTER();
    __HAL_UART_ENABLE_IT(huart,UART_IT_TC);
    CPU_CRITICAL_EXIT();
}

/*
*********************************************************************************************************
*                                         MB_get_pch()
*
* Description : This function is Search for the corresponding Modbus channel by serial port number.
*
* Argument(s) : port_nbr    The serial port number for which the corresponding MODBUS channel is to be found.
*
* Return(s)   : Pointer to the MODBUS channel
*
* Caller(s)   : MB_CommRxISR_x_Handler
*
* Note(s)     : If the specified port number is not found in the channel table, the function will return a
*               pointer to the end of the table, which may not point to a valid channel.
*********************************************************************************************************
*/

MODBUS_CH   * MB_get_pch(uint8_t port_nbr){
    MODBUS_CH   *pch;
    uint8_t ch = 0;
    pch = &MB_ChTbl[0];
    for (ch = 0; ch < MODBUS_CFG_MAX_CH; ch++) {        /* Find the channel assigned to this port       */
        if (pch->PortNbr == port_nbr) {
            ch = MODBUS_CFG_MAX_CH;
        } else {
            pch++;
        }
    } 
    return pch;
}
/*
*********************************************************************************************************
*                                UART #1 Rx Tx Communication handler for Modbus
*********************************************************************************************************
*/
void  MB_CommRxISR_1_Handler (char c)
{
    MODBUS_CH   *pch = MB_get_pch(serial1);
    pch->RxCtr++;
    MB_RxByte(pch, c);
}
void  MB_CommTxISR_1_Handler( void )
{
    MODBUS_CH   *pch = MB_get_pch(serial1);
    pch->TxCtr++;
    MB_TxByte(pch);    
}
/*
*********************************************************************************************************
*                                UART #2 Rx Tx Communication handler for Modbus
*********************************************************************************************************
*/
void  MB_CommRxISR_2_Handler (char c)
{
    MODBUS_CH   *pch = MB_get_pch(serial2);
    pch->RxCtr++;
    MB_RxByte(pch, c);
}
void  MB_CommTxISR_2_Handler( void )
{
    MODBUS_CH   *pch = MB_get_pch(serial2);
    pch->TxCtr++;
    MB_TxByte(pch);    
}
/*
*********************************************************************************************************
*                                UART #3 Rx Tx Communication handler for Modbus
*********************************************************************************************************
*/
void  MB_CommRxISR_3_Handler (char c)
{
    MODBUS_CH   *pch = MB_get_pch(serial3);
    pch->RxCtr++;
    MB_RxByte(pch, c);
}
void  MB_CommTxISR_3_Handler( void )
{
    MODBUS_CH   *pch = MB_get_pch(serial3);
    pch->TxCtr++;
    MB_TxByte(pch);    
}
/*
*********************************************************************************************************
*                                UART #4 Rx Tx Communication handler for Modbus
*********************************************************************************************************
*/
void  MB_CommRxISR_4_Handler (char c)
{
    MODBUS_CH   *pch = MB_get_pch(serial4);
    pch->RxCtr++;
    MB_RxByte(pch, c);
}
void  MB_CommTxISR_4_Handler( void )
{
    MODBUS_CH   *pch = MB_get_pch(serial4);
    pch->TxCtr++;
    MB_TxByte(pch);    
}
/*
*********************************************************************************************************
*                                UART #5 Rx Tx Communication handler for Modbus
*********************************************************************************************************
*/
void  MB_CommRxISR_5_Handler (char c)
{
    MODBUS_CH   *pch = MB_get_pch(serial5);
    pch->RxCtr++;
    MB_RxByte(pch, c);
}
void  MB_CommTxISR_5_Handler( void )
{
    MODBUS_CH   *pch = MB_get_pch(serial5);
    pch->TxCtr++;
    MB_TxByte(pch);    
}
/*
*********************************************************************************************************
*                                UART #6 Rx Tx Communication handler for Modbus
*********************************************************************************************************
*/
void  MB_CommRxISR_6_Handler (char c)
{
    MODBUS_CH   *pch = MB_get_pch(serial6);
    pch->RxCtr++;
    MB_RxByte(pch, c);
}
void  MB_CommTxISR_6_Handler( void )
{
    MODBUS_CH   *pch = MB_get_pch(serial6);
    pch->TxCtr++;
    MB_TxByte(pch);    
}

/*
*********************************************************************************************************
*                                             MB_CommTx1()
*
* Description : This function is called to obtain the next byte to send from the transmit buffer.  When
*               all bytes in the reply have been sent, transmit interrupts are disabled and the receiver
*               is enabled to accept the next Modbus request.
*
* Argument(s) : c     is the byte to send to the serial port
*
* Return(s)   : none.
*
* Caller(s)   : MB_TxByte()
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  MB_CommTx1 (MODBUS_CH  *pch, 
                   CPU_INT08U  c)
{
    serial_writeChar(pch->PortNbr,c);
}

/*
*********************************************************************************************************
*                                           MB_RTU_TmrInit()
*
* Description : This function is called to initialize the RTU timeout timer.
*
* Argument(s) : freq          Is the frequency of the modbus RTU timer interrupt.
*
* Return(s)   : none.
*
* Caller(s)   : MB_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (MODBUS_CFG_RTU_EN == DEF_ENABLED)
void  MB_RTU_TmrInit (void)
{
    CPU_INT32U us = 1000*1000/MB_RTU_Freq;
    timer_open(MODBUS_TIMER,us,MB_RTU_TmrISR_Handler);
    MB_RTU_TmrResetAll();                             /* Reset all the RTU timers, we changed freq.         */
}
#endif


/*
*********************************************************************************************************
*                                           MB_RTU_TmrExit()
*
* Description : This function is called to disable the RTU timeout timer.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : MB_Exit()
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (MODBUS_CFG_RTU_EN == DEF_ENABLED)
void  MB_RTU_TmrExit (void)
{
    timer_closed(MODBUS_TIMER);
}
#endif


/*
*********************************************************************************************************
*                                       MB_RTU_TmrISR_Handler()
*
* Description : This function handles the case when the RTU timeout timer expires.
*
* Arguments   : none.
*
* Returns     : none.
*
* Caller(s)   : This is a ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (MODBUS_CFG_RTU_EN == DEF_ENABLED)
void  MB_RTU_TmrISR_Handler (void)
{
    MB_RTU_TmrCtr++;                                  /* Indicate that we had activities on this interrupt. */
    MB_RTU_TmrUpdate();                               /* Check for RTU timers that have expired             */
}
#endif
