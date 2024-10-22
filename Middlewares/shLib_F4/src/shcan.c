/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Private includes ----------------------------------------------------------*/
#include "shcan.h"
#include <math.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include "shGpio.h"
#include "BSP_pin_cof.h"
/* Private types -------------------------------------------------------------*/
typedef struct
{
 PinName _td;
 PinName _rd;
} CANPin_TypeDef;

/**
 * @struct  CanRxMsg
 * @brief   CAN接收时使用的结构体
**/
typedef struct
{
    CAN_RxHeaderTypeDef  Header;
    uint8_t              Data[8];
    //uint32_t             RxMailbox;
} CanRxMsg;

/**
 * @struct  can_t
 * @brief   CAN 内部使用的can真正句柄。
**/
typedef struct can_s {
	CAN_HandleTypeDef CanHandle;
	int index;
	int hz;
	int rxIrqEnabled;
	uint16_t _rx_buffer_head;
	uint16_t _rx_buffer_tail;
	CAN_Message _rx_buffer[CAN_RX_BUFFER_SIZE];
//	void (* TxCallback)(void);        	/*!< CAN Tx  Callback      暂时没有用到，所以注释掉了  */
	void (* RxCallback)(CAN_Message*);     /*!< CAN Rx  Callback        */
}can_t;

/* Private macro -------------------------------------------------------------*/

///* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static const CANPin_TypeDef Default_Pin[CAN_NUM] = {
								{CAN1_TX_PIN,CAN1_RX_PIN},		//CAN1 默认引脚
								{CAN2_TX_PIN,CAN2_RX_PIN},		//CAN2 默认引脚
								//{PB10,PB11},	//CAN3 默认引脚
								};

/* Public variables ----------------------------------------------------------*/
can_t   can_obj[CAN_NUM] = {0};			// 注册实体
/* Private function prototypes -----------------------------------------------*/
static void SH_CAN_Register_RxFifo0MsgPendingCallback( CAN_HandleTypeDef *hcan );
static unsigned int can_speed(unsigned int pclk, unsigned int cclk, unsigned char psjw);

/* Public function prototypes ------------------------------------------------*/

/**
  * @brief 	初始化can，并返回HAL库的can句柄
  * @note	不指定can引脚则使用默认的can引脚
  * @param 	num can1，can2
  * @param	hz 频率
  * @retval HAL库的can句柄
  * @date
  */
CAN_HandleTypeDef* can_open(CANName num,uint32_t hz)
{
	PinName rd = Default_Pin[num]._rd;
	PinName td = Default_Pin[num]._td;
	return can_open_Pin(num,hz,rd,td);

}
/**
  * @brief 	初始化can，并返回HAL库的can句柄
  * @note	指定can引脚，并初始化can
  * @param 	num can1，can2
  * @param 	rd td 使用的can引脚，可以使用的引脚在PinMap_CAN_TD
  * 		和PinMap_CAN_RD中
  * @param	hz 频率
  * @retval HAL库的can句柄
  * @date
  */
CAN_HandleTypeDef* can_open_Pin(CANName num,uint32_t hz, PinName rd, PinName td)
{
    can_t *obj = &can_obj[num];
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    uint32_t filter_number;
    IRQn_Type irq;
    switch(num)
    {
    case CAN_1:
    	obj->CanHandle.Instance = CAN1;
    	GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    	filter_number = 0;
    	irq = CAN1_RX0_IRQn;
    	__HAL_RCC_CAN1_CLK_ENABLE();
    	break;
#ifdef CAN2_BASE
    case CAN_2:
    	obj->CanHandle.Instance = CAN2;
    	GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
    	filter_number = 14;
    	irq = CAN2_RX0_IRQn;
        __HAL_RCC_CAN1_CLK_ENABLE(); // needed to set filters
        __HAL_RCC_CAN2_CLK_ENABLE();
    	break;
#endif
#ifdef  CAN3_BASE
    case CAN_3:
    	obj->CanHandle.Instance = CAN3;
    	GPIO_InitStruct.Alternate = GPIO_AF9_CAN3; // 没有用到未验证
    	filter_number = 0;
    	irq = CAN3_RX0_IRQn;
    	__HAL_RCC_CAN3_CLK_ENABLE();
#endif
    	break;
    default:
    	log_a("can name error");
    	break;
    }

    // 设置引脚
	GPIO_RCC_ENABLE(PIN_MAP[rd].GPIOx);
	GPIO_RCC_ENABLE(PIN_MAP[td].GPIOx);
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	GPIO_InitStruct.Pin = PIN_MAP[rd].GPIO_Pin_x;
	HAL_GPIO_Init(PIN_MAP[rd].GPIOx, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = PIN_MAP[td].GPIO_Pin_x;
	HAL_GPIO_Init(PIN_MAP[td].GPIOx, &GPIO_InitStruct);


	/*##-1- Configure the CAN peripheral #######################################*/
	obj->CanHandle.Init.TimeTriggeredMode = DISABLE;
	obj->CanHandle.Init.AutoBusOff = DISABLE;
	obj->CanHandle.Init.AutoWakeUp = DISABLE;
	obj->CanHandle.Init.AutoRetransmission = DISABLE;
	obj->CanHandle.Init.ReceiveFifoLocked = DISABLE;
	obj->CanHandle.Init.TransmitFifoPriority = DISABLE;
	obj->CanHandle.Init.Mode = CAN_MODE_NORMAL;
	obj->CanHandle.Init.SyncJumpWidth = CAN_SJW_1TQ;
	obj->CanHandle.Init.TimeSeg1 = CAN_BS1_6TQ;	//tBS1=7个tCAN时钟周期
	obj->CanHandle.Init.TimeSeg2 = CAN_BS2_8TQ;	//tBS2=2个tCAN时钟周期
	obj->CanHandle.Init.Prescaler = 2;				//ABP总线时钟时钟3分频


	/*  Store frequency to be restored in case of reset */
	obj->hz = hz;

    if (HAL_CAN_Init(&obj->CanHandle) != HAL_OK) {
        log_e("Cannot initialize CAN");
    }
	 /*Register the receive interrupt callback function #######################################*/
    HAL_CAN_RegisterCallback(&obj->CanHandle,HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID,SH_CAN_Register_RxFifo0MsgPendingCallback);
    // Set initial CAN frequency to specified frequency
    if (can_frequency(num, can_obj[num].hz) != 1) {
        log_e("Can frequency could not be set\n");
    }
    /*##-2- Configure the CAN Filter ###########################################*/
	can_filter(num, 0, 0, CANStandard, filter_number);

	/*##-3- Start the CAN peripheral ###########################################*/
	  if (HAL_CAN_Start(&obj->CanHandle) != HAL_OK)
	  {
	    /* Start Error */
		  log_e("can open err");
	    //Error_Handler();
	  }

	  /*##-4- Activate CAN RX notification #######################################*/
	  if (HAL_CAN_ActivateNotification(&obj->CanHandle, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	  {
	    /* Notification Error */
		  log_e("can Notification Error");
	  }

	  /* CAN1 interrupt Init */
	  HAL_NVIC_SetPriority(irq, CAN_PreemptionPriority_Default, CAN_SubPriority_Default);
	  HAL_NVIC_EnableIRQ(irq);
	  return &can_obj[num].CanHandle;
}

void can_close(CANName num)
{
	if(num >= CAN_NUM ){
		return;
	}
    //CANName can = (CANName)can_obj[num].CanHandle.Instance;
    // Reset CAN and disable clock
    if (num == CAN_1) {
        __HAL_RCC_CAN1_FORCE_RESET();
        __HAL_RCC_CAN1_RELEASE_RESET();
        __HAL_RCC_CAN1_CLK_DISABLE();
    }
#if defined(CAN2_BASE)
    if (num == CAN_2) {
        __HAL_RCC_CAN2_FORCE_RESET();
        __HAL_RCC_CAN2_RELEASE_RESET();
        __HAL_RCC_CAN2_CLK_DISABLE();
    }
#endif
#if defined(CAN3_BASE)
    if (num == CAN_3) {
        __HAL_RCC_CAN3_FORCE_RESET();
        __HAL_RCC_CAN3_RELEASE_RESET();
        __HAL_RCC_CAN3_CLK_DISABLE();
    }
#endif
    //
    memset(&can_obj[num],0,sizeof(can_obj));
}

// The following table is used to program bit_timing. It is an adjustment of the sample
// point by synchronizing on the start-bit edge and resynchronizing on the following edges.
// This table has the sampling points as close to 75% as possible (most commonly used).
// The first value is TSEG1, the second TSEG2.
static const int timing_pts[23][2] = {
    {0x0, 0x0},      // 2,  67%
    {0x1, 0x0},      // 3,  75%
    {0x2, 0x0},      // 4,  80%
    {0x2, 0x1},      // 5,  67%
    {0x3, 0x1},      // 6,  71%
    {0x4, 0x1},      // 7,  75%
    {0x4, 0x2},      // 8,  67%
    {0x5, 0x2},      // 9,  70%
    {0x6, 0x2},      // 10, 73%
    {0x7, 0x2},      // 11, 75%
    {0x7, 0x3},      // 12, 69%
    {0x8, 0x3},      // 13, 71%
    {0x9, 0x3},      // 14, 73%
    {0xA, 0x3},      // 15, 75%
    {0xA, 0x4},      // 16, 71%
    {0xB, 0x4},      // 17, 72%
    {0xC, 0x4},      // 18, 74%
    {0xD, 0x4},      // 19, 75%
    {0xD, 0x5},      // 20, 71%
    {0xE, 0x5},      // 21, 73%
    {0xF, 0x5},      // 22, 74%
    {0xF, 0x6},      // 23, 71%
    {0xF, 0x7},      // 24, 68%
};

static unsigned int can_speed(unsigned int pclk, unsigned int cclk, unsigned char psjw)
{
    uint32_t    btr;
    uint32_t    brp = 0;
    uint32_t    calcbit;
    uint32_t    bitwidth;
    int         hit = 0;
    int         bits;

    bitwidth = (pclk / cclk);

    brp = bitwidth / 0x18;
    while ((!hit) && (brp < bitwidth / 4)) {
        brp++;
        for (bits = 22; bits > 0; bits--) {
            calcbit = (bits + 3) * (brp + 1);
            if (calcbit == bitwidth) {
                hit = 1;
                break;
            }
        }
    }

    if (hit) {
        btr = ((timing_pts[bits][1] << CAN_BTR_TS2_Pos) & CAN_BTR_TS2) |
              ((timing_pts[bits][0] << CAN_BTR_TS1_Pos) & CAN_BTR_TS1) |
              ((psjw                << CAN_BTR_SJW_Pos) & CAN_BTR_SJW) |
              ((brp                 << CAN_BTR_BRP_Pos) & CAN_BTR_BRP);
    } else {
        btr = 0xFFFFFFFF;
    }

    return btr;

}

int can_frequency(CANName num, int f)
{
    int pclk = HAL_RCC_GetPCLK1Freq();
    int btr = can_speed(pclk, (unsigned int)f, 1);
    CAN_TypeDef *can = can_obj[num].CanHandle.Instance;
    uint32_t tickstart = 0;
    int status = 1;

    if (btr > 0) {
        can->MCR |= CAN_MCR_INRQ ;
        /* Get tick */
        tickstart = HAL_GetTick();
        while ((can->MSR & CAN_MSR_INAK) != CAN_MSR_INAK) {
            if ((HAL_GetTick() - tickstart) > 2) {
                status = 0;
                break;
            }
        }
        if (status != 0) {
            /*  Do not erase all BTR registers (e.g. silent mode), only the
             *  ones calculated in can_speed */
            can->BTR &= ~(CAN_BTR_TS2 | CAN_BTR_TS1 | CAN_BTR_SJW | CAN_BTR_BRP);
            can->BTR |= btr;

            can->MCR &= ~(uint32_t)CAN_MCR_INRQ;
            /* Get tick */
            tickstart = HAL_GetTick();
            while ((can->MSR & CAN_MSR_INAK) == CAN_MSR_INAK) {
                if ((HAL_GetTick() - tickstart) > 2) {
                    status = 0;
                    break;
                }
            }
            if (status == 0) {
                log_e("can ESR  0x%04" PRIx32 ".%04" PRIx32 " + timeout status %d", (can->ESR & 0xFFFF0000) >> 16, (can->ESR & 0xFFFF), status);
            }
        } else {
            log_e("can init request timeout\n");
        }
    } else {
        status = 0;
    }
    return status;
}

int can_write(CANName num, CAN_Message *msg)
{
    //uint32_t  transmitmailbox = ((uint8_t)0x04); //CAN_TXSTATUS_NOMAILBOX;
    //CAN_TypeDef *can = can_obj[num].CanHandle.Instance;

    uint32_t mailbox;
    CAN_TxHeaderTypeDef  Header = {0};

    switch(msg->format)
    {
    case CANStandard:
    	Header.StdId = msg->id;
    	Header.IDE  = CAN_ID_STD;
    	break;
    case CANExtended:
    	Header.ExtId = msg->id;
    	Header.IDE  = CAN_ID_EXT;
    	break;
    default:
    	break;
    }
     switch(msg->type)
     {
     case CANData:
    	 Header.RTR = CAN_RTR_DATA;
    	 break;
     case CANRemote:
    	 Header.RTR = CAN_RTR_REMOTE;
    	 break;
     default:
    	 break;
     }

    Header.DLC = msg->len>8?8:msg->len;

    if ( HAL_CAN_AddTxMessage( &can_obj[num].CanHandle, &Header, msg->data, &mailbox ) != HAL_OK )
    {
    	return 0;
    }
    return -1;
}

int can_read(CANName num, CAN_Message *msg)
{
	// if the head isn't ahead of the tail, we don't have any characters
	if (can_obj[num]._rx_buffer_head == can_obj[num]._rx_buffer_tail) {
		return -1;
	} else {
		msg = &can_obj[num]._rx_buffer[can_obj[num]._rx_buffer_tail];

		can_obj[num]._rx_buffer_tail = (uint16_t) (can_obj[num]._rx_buffer_tail
				+ 1) % CAN_RX_BUFFER_SIZE;
		return 0;
	}
	return -1;
}

//void can_reset(CANName num)
//{
//    CAN_TypeDef *can = obj->CanHandle.Instance;
//
//    /* Reset IP and delete errors */
//    can->MCR |= CAN_MCR_RESET;
//    can->ESR = 0x0;
//
//    /* restore registers state as saved in obj context */
//    can_registers_init(obj);
//    obj->rxIrqEnabled = false;
//}

unsigned char can_rderror(CANName num)
{
    CAN_TypeDef *can = can_obj[num].CanHandle.Instance;
    return (can->ESR >> 24) & 0xFF;
}

unsigned char can_tderror(CANName num)
{
    CAN_TypeDef *can = can_obj[num].CanHandle.Instance;
    return (can->ESR >> 16) & 0xFF;
}

void can_monitor(CANName num, int silent)
{
    CanMode mode = MODE_NORMAL;
    /*  Update current state w/ or w/o silent */
    if (silent) {
        switch (can_obj[num].CanHandle.Init.Mode) {
            case CAN_MODE_LOOPBACK:
            case CAN_MODE_SILENT_LOOPBACK:
                mode = MODE_TEST_SILENT;
                break;
            default:
                mode = MODE_SILENT;
                break;
        }
    } else {
        switch (can_obj[num].CanHandle.Init.Mode) {
            case CAN_MODE_LOOPBACK:
            case CAN_MODE_SILENT_LOOPBACK:
                mode = MODE_TEST_LOCAL;
                break;
            default:
                mode = MODE_NORMAL;
                break;
        }
    }

    can_mode(num, mode);
}

int can_mode(CANName num, CanMode mode)
{
    int success = 0;
    CAN_TypeDef *can = can_obj[num].CanHandle.Instance;

    can->MCR |= CAN_MCR_INRQ ;
    while ((can->MSR & CAN_MSR_INAK) != CAN_MSR_INAK) {
    }

    switch (mode) {
        case MODE_NORMAL:
            can_obj[num].CanHandle.Init.Mode = CAN_MODE_NORMAL;
            can->BTR &= ~(CAN_BTR_SILM | CAN_BTR_LBKM);
            success = 1;
            break;
        case MODE_SILENT:
            can_obj[num].CanHandle.Init.Mode = CAN_MODE_SILENT;
            can->BTR |= CAN_BTR_SILM;
            can->BTR &= ~CAN_BTR_LBKM;
            success = 1;
            break;
        case MODE_TEST_GLOBAL:
        case MODE_TEST_LOCAL:
            can_obj[num].CanHandle.Init.Mode = CAN_MODE_LOOPBACK;
            can->BTR |= CAN_BTR_LBKM;
            can->BTR &= ~CAN_BTR_SILM;
            success = 1;
            break;
        case MODE_TEST_SILENT:
            can_obj[num].CanHandle.Init.Mode = CAN_MODE_SILENT_LOOPBACK;
            can->BTR |= (CAN_BTR_SILM | CAN_BTR_LBKM);
            success = 1;
            break;
        default:
            success = 0;
            break;
    }

    can->MCR &= ~(uint32_t)CAN_MCR_INRQ;
    while ((can->MSR & CAN_MSR_INAK) == CAN_MSR_INAK) {
    }

    return success;
}

int can_filter(CANName num, uint32_t id, uint32_t mask, CANFormat format, int32_t handle)
{
    int success = 0;

    // filter for CANAny format cannot be configured for STM32
    if ((format == CANStandard) || (format == CANExtended)) {
    	CAN_FilterTypeDef  sFilterConfig = {0};
        sFilterConfig.FilterBank = handle;
        sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
        sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;

        if (format == CANStandard) {
            sFilterConfig.FilterIdHigh = id << 5;
            sFilterConfig.FilterIdLow =  0x0;
            sFilterConfig.FilterMaskIdHigh = mask << 5;
            sFilterConfig.FilterMaskIdLow = 0x0; // allows both remote and data frames
        } else { // format == CANExtended
            sFilterConfig.FilterIdHigh = id >> 13; // EXTID[28:13]
            sFilterConfig.FilterIdLow = (0xFFFF & (id << 3)) | (1 << 2); // EXTID[12:0] + IDE
            sFilterConfig.FilterMaskIdHigh = mask >> 13;
            sFilterConfig.FilterMaskIdLow = (0xFFFF & (mask << 3)) | (1 << 2);
        }

        sFilterConfig.FilterFIFOAssignment = 0;
        sFilterConfig.FilterActivation = ENABLE;
        sFilterConfig.SlaveStartFilterBank = 14;
        if (HAL_CAN_ConfigFilter(&can_obj[num].CanHandle, &sFilterConfig) == HAL_OK) {
            success = 1;
        }
    } else if (format == CANAny) {
        success = 0;	// filter for CANAny is not supported by STM32, return a failure
    }
    return success;
}

//static void can_irq(CANName name, int id)
//{
//    uint32_t tmp1 = 0, tmp2 = 0, tmp3 = 0;
//    CAN_HandleTypeDef CanHandle;
//    CanHandle.Instance = (CAN_TypeDef *)name;
//
//    if (__HAL_CAN_GET_IT_SOURCE(&CanHandle, CAN_IT_TX_MAILBOX_EMPTY)) {
//        tmp1 = __HAL_CAN_TRANSMIT_STATUS(&CanHandle, CAN_TX_MAILBOX0);
//        tmp2 = __HAL_CAN_TRANSMIT_STATUS(&CanHandle, CAN_TX_MAILBOX1);
//        tmp3 = __HAL_CAN_TRANSMIT_STATUS(&CanHandle, CAN_TX_MAILBOX2);
//        if (tmp1) {
//            __HAL_CAN_CLEAR_FLAG(&CanHandle, CAN_FLAG_RQCP0);
//        }
//        if (tmp2) {
//            __HAL_CAN_CLEAR_FLAG(&CanHandle, CAN_FLAG_RQCP1);
//        }
//        if (tmp3) {
//            __HAL_CAN_CLEAR_FLAG(&CanHandle, CAN_FLAG_RQCP2);
//        }
//        if (tmp1 || tmp2 || tmp3) {
//            irq_handler(can_irq_contexts[id], IRQ_TX);
//        }
//    }
//
//    tmp1 = __HAL_CAN_MSG_PENDING(&CanHandle, CAN_FIFO0);
//    tmp2 = __HAL_CAN_GET_IT_SOURCE(&CanHandle, CAN_IT_RX_FIFO0_MSG_PENDING);
//
//    // In legacy can (bxCAN and earlier), reading is the only way to clear rx interrupt. But can_read has mutex locks
//    // since mutexes cannot be used in ISR context, rx interrupt is masked here to temporary disable it
//    // rx interrupts will be unamsked in read operation. reads must be deffered to thread context.
//    // refer to the CAN receive interrupt problem due to mutex and resolution section of README doc.
//    __HAL_CAN_DISABLE_IT(&CanHandle, CAN_IT_RX_FIFO0_MSG_PENDING);
//
//    if ((tmp1 != 0) && tmp2) {
//        irq_handler(can_irq_contexts[id], IRQ_RX);
//    }
//
//    tmp1 = __HAL_CAN_GET_FLAG(&CanHandle, CAN_FLAG_EPV);
//    tmp2 = __HAL_CAN_GET_IT_SOURCE(&CanHandle, CAN_IT_ERROR_PASSIVE);
//    tmp3 = __HAL_CAN_GET_IT_SOURCE(&CanHandle, CAN_IT_ERROR);
//
//    if (tmp1 && tmp2 && tmp3) {
//        irq_handler(can_irq_contexts[id], IRQ_PASSIVE);
//    }
//
//    tmp1 = __HAL_CAN_GET_FLAG(&CanHandle, CAN_FLAG_BOF);
//    tmp2 = __HAL_CAN_GET_IT_SOURCE(&CanHandle, CAN_IT_BUSOFF);
//    tmp3 = __HAL_CAN_GET_IT_SOURCE(&CanHandle, CAN_IT_ERROR);
//    if (tmp1 && tmp2 && tmp3) {
//        irq_handler(can_irq_contexts[id], IRQ_BUS);
//    }
//
//    tmp3 = __HAL_CAN_GET_IT_SOURCE(&CanHandle, CAN_IT_ERROR);
//    if (tmp1 && tmp2 && tmp3) {
//        irq_handler(can_irq_contexts[id], IRQ_ERROR);
//    }
//}
/**
  * @brief 	注册接收回调函数
  * @note
  * @param[in]	num : CAN 驱动号
  * @param[in] Function 注册的回调函数；
  * @retval
  * @date
  */
void 	can_attachRxInterrupt(CANName num,CAN_RxCallbackFunction_t Function)
{
	can_obj[num].RxCallback = Function;
}
/**
  * @brief 	将接收到的can信息转换到CAN_Message中。
  * @note
  * @param[in]	rxmsg : can接收的数据
  * @param[out] canmsg：CAN对外接口数据。
  * @retval
  * @date
  */
void RxmsgToMsg(CanRxMsg *rxmsg,CAN_Message * canmsg)
{
	if(rxmsg->Header.IDE == CAN_ID_STD )
	{
		canmsg->id = rxmsg->Header.StdId;
		canmsg->format = CANStandard;
	}else{
		canmsg->id = rxmsg->Header.ExtId;
		canmsg->format = CANExtended;
	}
	canmsg->type = rxmsg->Header.RTR;
	canmsg->len = rxmsg->Header.DLC;
	memcpy(canmsg->data,rxmsg->Data,8);
}
/************************************************
函数名称 ： CAN_RX_IRQHandler
功    能 ： CAN接收中断
参    数 ： 无
返 回 值 ： 无
作    者 ：
*************************************************/
static void SH_CAN_Register_RxFifo0MsgPendingCallback( CAN_HandleTypeDef *hcan )
{
    CanRxMsg RxMsg = {0};
    CAN_Message* canmsg;
    if( hcan == &can_obj[CAN_1].CanHandle )
    {
    	__HAL_CAN_ENABLE_IT(hcan,CAN_IER_FMPIE0);
        /* Get RX message */
        if ( HAL_CAN_GetRxMessage( hcan, CAN_RX_FIFO0, &RxMsg.Header, RxMsg.Data ) == HAL_OK )
        {
        	uint16_t i = (uint16_t) (can_obj[CAN_1]._rx_buffer_head + 1) % CAN_RX_BUFFER_SIZE;
        	/* 已到末尾，覆盖最后一个，末尾指针后移。*/
        	if (i == can_obj[CAN_1]._rx_buffer_tail) {
        		can_obj[CAN_1]._rx_buffer_tail = (uint16_t) (can_obj[CAN_1]._rx_buffer_tail
        								+ 1) % CAN_RX_BUFFER_SIZE;
        	}
			canmsg = &can_obj[CAN_1]._rx_buffer[can_obj[CAN_1]._rx_buffer_head];
			RxmsgToMsg(&RxMsg,canmsg);
			can_obj[CAN_1]._rx_buffer_head = i;

			if (can_obj[CAN_1].RxCallback) {
				can_obj[CAN_1].RxCallback(canmsg);
			}
        }
    }
#ifdef CAN2_BASE
    if( hcan == &can_obj[CAN_2].CanHandle )
    {
    	__HAL_CAN_ENABLE_IT(hcan,CAN_IER_FMPIE0);
        /* Get RX message */
        if ( HAL_CAN_GetRxMessage( hcan, CAN_RX_FIFO0, &RxMsg.Header, RxMsg.Data ) == HAL_OK )
        {
        	uint16_t i = (uint16_t) (can_obj[CAN_2]._rx_buffer_head + 1) % CAN_RX_BUFFER_SIZE;
        	/* 已到末尾，覆盖最后一个，末尾指针后移。*/
        	if (i == can_obj[CAN_2]._rx_buffer_tail) {
        		can_obj[CAN_2]._rx_buffer_tail = (uint16_t) (can_obj[CAN_2]._rx_buffer_tail
        								+ 1) % CAN_RX_BUFFER_SIZE;
        	}
			canmsg = &can_obj[CAN_2]._rx_buffer[can_obj[CAN_2]._rx_buffer_head];
			RxmsgToMsg(&RxMsg,canmsg);
			can_obj[CAN_2]._rx_buffer_head = i;

			if (can_obj[CAN_2].RxCallback) {
				can_obj[CAN_2].RxCallback(canmsg);
			}
        }
    }
#endif
#ifdef CAN3_BASE
    if( hcan == &can_obj[CAN_3].CanHandle )
    {
    	__HAL_CAN_ENABLE_IT(hcan,CAN_IER_FMPIE0);
        /* Get RX message */
        if ( HAL_CAN_GetRxMessage( hcan, CAN_RX_FIFO0, &RxMsg.Header, RxMsg.Data ) == HAL_OK )
        {
        	uint16_t i = (uint16_t) (can_obj[CAN_3]._rx_buffer_head + 1) % CAN_RX_BUFFER_SIZE;
        	/* 已到末尾，覆盖最后一个，末尾指针后移。*/
        	if (i == can_obj[CAN_3]._rx_buffer_tail) {
        		can_obj[CAN_3]._rx_buffer_tail = (uint16_t) (can_obj[CAN_3]._rx_buffer_tail
        								+ 1) % CAN_RX_BUFFER_SIZE;
        	}
			canmsg = &can_obj[CAN_3]._rx_buffer[can_obj[CAN_3]._rx_buffer_head];
			RxmsgToMsg(&RxMsg,canmsg);
			can_obj[CAN_3]._rx_buffer_head = i;

			if (can_obj[CAN_3].RxCallback) {
				can_obj[CAN_3].RxCallback(&RxMsg);
			}
        }
    }
#endif
}

#if USE_SH_IRQ_ALL || USE_SH_IRQ_CAN_ALL || USE_SH_IRQ_CAN1
	#if defined(TARGET_STM32F0)
void CAN_IRQHandler(void)
{
    can_irq(CAN_1, 0);
}
	#elif defined(TARGET_STM32F3)
void CAN_RX0_IRQHandler(void)
{
    can_irq(CAN_1, 0);
}
void CAN_TX_IRQHandler(void)
{
    can_irq(CAN_1, 0);
}
void CAN_SCE_IRQHandler(void)
{
    can_irq(CAN_1, 0);
}
	#else
void CAN1_RX0_IRQHandler(void)
{
    //can_irq(CAN_1, 0);
    HAL_CAN_IRQHandler(&can_obj[CAN_1].CanHandle);
}
void CAN1_TX_IRQHandler(void)
{
    //can_irq(CAN_1, 0);
    HAL_CAN_IRQHandler(&can_obj[CAN_1].CanHandle);
}
void CAN1_SCE_IRQHandler(void)
{
    //can_irq(CAN_1, 0);
    HAL_CAN_IRQHandler(&can_obj[CAN_1].CanHandle);
}
	#endif
#endif

#ifdef CAN2_BASE
#if USE_SH_IRQ_ALL || USE_SH_IRQ_CAN_ALL || USE_SH_IRQ_CAN2
void CAN2_RX0_IRQHandler(void)
{
    //can_irq(CAN_2, 1);
    HAL_CAN_IRQHandler(&can_obj[CAN_2].CanHandle);
}
void CAN2_TX_IRQHandler(void)
{
    //can_irq(CAN_2, 1);
    HAL_CAN_IRQHandler(&can_obj[CAN_2].CanHandle);
}
void CAN2_SCE_IRQHandler(void)
{
    //can_irq(CAN_2, 1);
    HAL_CAN_IRQHandler(&can_obj[CAN_2].CanHandle);
}
#endif
#endif
#ifdef CAN3_BASE
#if USE_SH_IRQ_ALL || USE_SH_IRQ_CAN_ALL || USE_SH_IRQ_CAN3
void CAN3_RX0_IRQHandler(void)
{
    //can_irq(CAN_3, 2);
    HAL_CAN_IRQHandler(&can_obj[CAN_2].CanHandle);
}
void CAN3_TX_IRQHandler(void)
{
	//can_irq(CAN_3, 2);
	HAL_CAN_IRQHandler(&can_obj[CAN_2].CanHandle);
}
void CAN3_SCE_IRQHandler(void)
{
	//can_irq(CAN_3, 2);
	HAL_CAN_IRQHandler(&can_obj[CAN_2].CanHandle);
}
#endif
#endif

//const PinMap *can_rd_pinmap()
//{
//    return PinMap_CAN_TD;
//}
//
//const PinMap *can_td_pinmap()
//{
//    return PinMap_CAN_RD;
//}
/**
  * @brief 	can 是否已经初始化
  * @note
  * @param 	num can1，can2
  * @retval 1:已经初始化，0：未初始化
  * @date
  */
int can_isActive(CANName num){
	if( can_obj[num].CanHandle.Instance != NULL){
		return 1;
	}else {
		return 0;
	}
}
