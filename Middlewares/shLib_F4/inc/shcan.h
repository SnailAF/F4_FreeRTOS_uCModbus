/**
  ******************************************************************************
  * @file    PinNames.h
  * @author	 ZGQ
  * @version
  * @date    2022年5月20日 下午5:31:07
  * @brief
  ******************************************************************************
  * @attention
  * @ex
  * void can_Rcv(CAN_Message* msg){
  *		can_write(CAN_1,msg);
  *	}
  *	void main(void){
  *		can_open_Pin(CAN_1, CAN500kBaud,PD0,PD1);
  *		can_attachRxInterrupt(CAN_1, can_Rcv);
  *		while(1);
  *	}
  *
  * 2022-12-13
  * 1. 增加 int can_isActive(CANName num)函数
  * 2.can_close函数增加 can_obj 清0。
  ******************************************************************************
  */

#ifndef _SHCAN_H_
#define _SHCAN_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mcu_type.h"
#include "shGpio.h"

/* Exported constants --------------------------------------------------------*/
#define CAN_RX_BUFFER_SIZE 5				//CAN接收环形缓存大小

/* Exported types ------------------------------------------------------------*/
/**
 * @enum    CANFormat
 * @brief   Values that represent CAN Format
**/
typedef enum CANFormat {
    CANStandard = 0,
    CANExtended = 1,
    CANAny = 2
}CANFormat;

/**
 * @enum    CANType
 * @brief   Values that represent CAN Type
**/
typedef enum CANType {
    CANData   = 0,
    CANRemote = 1
}CANType;

/**
 * @struct  CAN_Message
 * @brief   Holder for single CAN message.
 *
**/
typedef struct CAN_Message {
    unsigned int   id;                 // 29 bit identifier
    unsigned char  data[8];            // Data field
    unsigned char  len;                // Length of data field in bytes
    CANFormat      format;             // Format ::CANFormat
    CANType        type;               // Type ::CANType
}CAN_Message;


/**
 * @enum    CanIrqType
 * @brief   中断暂时未使用
**/
typedef enum {
    IRQ_RX,
    IRQ_TX,
    IRQ_ERROR,
    IRQ_OVERRUN,
    IRQ_WAKEUP,
    IRQ_PASSIVE,
    IRQ_ARB,
    IRQ_BUS,
    IRQ_READY
} CanIrqType;

/**
 * @enum    CanMode
 * @brief   CAN模式
**/
typedef enum {
    MODE_RESET,
    MODE_NORMAL,
    MODE_SILENT,
    MODE_TEST_LOCAL,
    MODE_TEST_GLOBAL,
    MODE_TEST_SILENT
} CanMode;

/**
 * @struct  can_pinmap_t
 * @brief   CAN 引脚 map 暂时未使用
**/
typedef struct {
    int peripheral;
    PinName rd_pin;
    int rd_function;		//mode afnum speed暂时未使用
    PinName td_pin;
    int td_function;
} can_pinmap_t;

/**
 * @enum    CANName
 * @brief   对应实体CAN,相当于句柄。真正句柄是can_t
**/
typedef enum {
    CAN_1 = 0,
#if defined CAN2_BASE
    CAN_2 = 1,
#endif
#if defined CAN3_BASE
    CAN_3 = 2
#endif
	CAN_NUM
} CANName;

/**
 * 下面均为，中断函数原型。
**/
typedef void(*CAN_RxCallbackFunction_t)(CAN_Message*);
typedef void(*CAN_TxCallbackFunction_t)(void);

/* Exported macro ------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported constant --------------------------------------------------------*/
#define CAN1MBaud   	(1000UL * 1000) 	/* 1 MBit/sec   */
#define CAN800kBaud 	(1000UL * 800) 		/* 800 kBit/sec */
#define CAN500kBaud 	(1000UL * 500)		/* 500 kBit/sec */
#define CAN250kBaud 	(1000UL * 250)  	/* 250 kBit/sec */
#define CAN125kBaud 	(1000UL * 125)  	/* 125 kBit/sec */
#define CAN100kBaud 	(1000UL * 100)  	/* 100 kBit/sec */
#define CAN50kBaud  	(1000UL * 50)   	/* 50 kBit/sec  */
#define CAN20kBaud  	(1000UL * 20)   	/* 20 kBit/sec  */
#define CAN10kBaud  	(1000UL * 10)   	/* 10 kBit/sec  */

/* Exported functions prototypes ---------------------------------------------*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 以下为推荐使用的常用函数
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
  * @brief CAN初始化最简洁函数，内部使用默认的can中断等级，并返回HAL库的can句柄
  * @note 不指定can引脚则使用默认的can引脚 Default_Pin
  * @param[in] num: CAN序号 例如CAN_1,CAN_2
  * @param[in] hz ：波特率 单位hz
  * @retval 		HAL库的can句柄
  * @date	2022-05-22
  * @author ZGQ
  * @ex		CAN_HandleTypeDef* myhcan = can_open(CAN_1,1000000);
  */
CAN_HandleTypeDef* 	can_open(CANName num,uint32_t hz);

/**
  * @brief CAN初始化函数，内部使用默认的can中断等级，需要指定引脚。并返回HAL库的can句柄
  * @note 不指定can引脚则使用默认的can引脚
  * @param[in] num: CAN序号 例如CAN_1,CAN_2
  * @param[in] hz ：波特率 单位khz
  * @param[in] rd ：接收引脚
  * @param[in] td ：发送引脚
  * @retval 		HAL库的can句柄
  * @date	2022-05-22
  * @author ZGQ
  * @ex		CAN_HandleTypeDef* myhcan = can_open(CAN_1,1000000,PA9,PA10);
  */
CAN_HandleTypeDef* 	can_open_Pin(CANName num,uint32_t hz,PinName rd, PinName td);

/**
  * @brief CAN Deinit函数
  * @note  暂时只是关闭对应时钟。
  * @param[in] num: CAN序号 例如CAN_1,CAN_2
  * @date	2022-05-22
  * @author ZGQ
  * @ex		can_close(CAN_1);
  */
void can_close(CANName num);

/**
  * @brief 注册CAN接收回调函数
  * @note  can接收到数据后，会先进行底层处理，然后调用 'Function' 函数
  * @param[in] num: CAN序号 例如CAN_1,CAN_2
  * @param[in] Function : can中断的回调函数
  * @retval
  * @date	2022-05-22
  * @author ZGQ
  * @ex		void myRx(CAN_Message* msg)
  * 		{
  * 			//msg为接收到的can数据，这个函数注册后，在中断中被调用
  * 		}
  * 		can_attachRxInterrupt(CAN_1,myRx);
  */
void can_attachRxInterrupt(CANName num,CAN_RxCallbackFunction_t Function);

/**
  * @brief CAN 消息发送函数
  * @note  初步处理后调用HAL_CAN_AddTxMessage函数
  * @param[in] num: CAN序号 例如CAN_1,CAN_2
  * @param[in] *msg : 发送的消息指针
  * @retval None
  * @author ZGQ
  * @ex		CAN_Message msg = {	.id = 1,
  * 						   	.data[0] = 1,
  * 						   	.data[1] = 2,
  * 						   	.data[2] = 3,
  * 						   	.data[3] = 4,
  * 						   	.len = 4,				// 四个字节 1,2,3,4,
  * 						   	.format = CANStandard,	// 标准帧
  * 						   	.type = CANData			// 数据帧
  * 						   	};
  * 		can_write(CAN_1,&msg);
  */
int can_write(CANName num, CAN_Message *msg);



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  * 以下函数不太常用,应该优先使用常用函数列表的函数
  *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
  * @brief 设置CAN频率
  * @note  优先在初始化时指定频率
  * @param[in] num: CAN序号 例如CAN_1,CAN_2
  * @param[in] hz ：波特率 单位hz
  * @date	2022-05-22
  * @author ZGQ
  * @ex		can_frequency(CAN_1,1000000);
  */
int can_frequency(CANName num, int hz);

/**
  * @brief 注册CAN发送回调函数
  * @note  can数据发送完成后，会先进行底层处理，然后调用 'Function' 函数
  * @note  tx使用频率低，暂时没有实现。
  * @param[in] num: CAN序号 例如CAN_1,CAN_2
  * @param[in] Function : can中断的回调函数
  * @date	2022-05-22
  * @author ZGQ
  */
void can_attachTxInterrupt(CANName num,CAN_TxCallbackFunction_t Function);

/**
  * @brief CAN 读取接收缓存中的消息
  * @note  读的是缓存，由于CAN的实时性高，建议注册中断函数。
  * @param[in] num: CAN序号 例如CAN_1,CAN_2
  * @param[out] *msg : 读到的数据
  * @retval 0：成功，-1失败
  * @author ZGQ
  * @ex		CAN_Message msg;
  * 		can_read(CAN_1,&msg);
  */
int can_read(CANName num, CAN_Message *msg);
int can_mode(CANName num, CanMode mode);
int can_filter(CANName num, uint32_t id, uint32_t mask, CANFormat format, int32_t handle);
#ifdef __cplusplus
}
#endif

#endif  /* SHCAN_H_ */

/************************ (C) COPYRIGHT ROBOT++ *******************************/
/**
  * @brief 	can 是否已经初始化
  * @note
  * @param 	num can1，can2
  * @retval 1:已经初始化，0：未初始化
  * @date
  */
int can_isActive(CANName num);
