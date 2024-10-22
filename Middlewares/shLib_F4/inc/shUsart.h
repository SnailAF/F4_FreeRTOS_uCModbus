/**
  ******************************************************************************
  * @file    app_configure.h
  * @author
  * @version
  * @date    2021.8.18
  * @brief   工程模块裁剪用配置文件
  ******************************************************************************
  * @attention
  * 变更说明： 2022-05-10
  * 1.增加串口字符串发送函数。
  * void serial_WriteStr(UART_HandleTypeDef *huart, char *str);
	int serial_printf(Serial_TypeDef num, const char *__restrict __format,...);
	2022-06-01
	1.修改：serial_read(write)为多字节读写，同时增加serial_read(write)Char函数
	2.增加485读写使能及注册函数。serial_attachRWCtrl_Pin
  * 2022-12-08
  * 优化读回调函数增加，char data数据。
  * typedef void(*USART_CallbackFunction_t)(void); ->
  * typedef void(*USART_CallbackFunction_t)(char);
  ******************************************************************************
  */

#ifndef __AUSART_H
#define __AUSART_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mcu_type.h"
#include "shGpio.h"

/* Exported constants --------------------------------------------------------*/
/*
 * 串口默认参数配置
 * */
#define SERIAL_RX_BUFFER_SIZE 128
#define SERIAL_Config_Default SERIAL_8N1
//#define USART_PreemptionPriority_Default 5
//#define USART_SubPriority_Default 0

/* Exported macro ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef enum
{
    SERIAL_8N1 = UART_WORDLENGTH_8B | UART_PARITY_NONE | ( USART_STOPBITS_1>> 8),
    SERIAL_8N2 = UART_WORDLENGTH_8B | UART_PARITY_NONE | (USART_STOPBITS_2 >> 8),
    SERIAL_8E1 = UART_WORDLENGTH_8B | UART_PARITY_EVEN | (USART_STOPBITS_1 >> 8),
    SERIAL_8E2 = UART_WORDLENGTH_8B | UART_PARITY_EVEN | (USART_STOPBITS_2 >> 8),
    SERIAL_8O1 = UART_WORDLENGTH_8B | UART_PARITY_ODD | (USART_STOPBITS_1 >> 8),
    SERIAL_8O2 = UART_WORDLENGTH_8B | UART_PARITY_ODD | (USART_STOPBITS_2 >> 8),
    SERIAL_8N0_5 = UART_WORDLENGTH_8B | UART_PARITY_NONE | (USART_STOPBITS_0_5 >> 8),
    SERIAL_8N1_5 = UART_WORDLENGTH_8B | UART_PARITY_NONE | (USART_STOPBITS_1_5 >> 8),
    SERIAL_8E0_5 = UART_WORDLENGTH_8B | UART_PARITY_EVEN | (USART_STOPBITS_0_5 >> 8),
    SERIAL_8E1_5 = UART_WORDLENGTH_8B | UART_PARITY_EVEN | (USART_STOPBITS_1_5 >> 8),
    SERIAL_8O0_5 = UART_WORDLENGTH_8B | UART_PARITY_ODD | (USART_STOPBITS_0_5 >> 8),
    SERIAL_8O1_5 = UART_WORDLENGTH_8B | UART_PARITY_ODD | (USART_STOPBITS_1_5 >> 8),

    SERIAL_9N1 = UART_WORDLENGTH_9B | UART_PARITY_NONE | (USART_STOPBITS_1 >> 8),
    SERIAL_9N2 = UART_WORDLENGTH_9B | UART_PARITY_NONE | (USART_STOPBITS_2 >> 8),
    SERIAL_9E1 = UART_WORDLENGTH_9B | UART_PARITY_EVEN | (USART_STOPBITS_1 >> 8),
    SERIAL_9E2 = UART_WORDLENGTH_9B | UART_PARITY_EVEN | (USART_STOPBITS_2 >> 8),
    SERIAL_9O1 = UART_WORDLENGTH_9B | UART_PARITY_ODD | (USART_STOPBITS_1 >> 8),
    SERIAL_9O2 = UART_WORDLENGTH_9B | UART_PARITY_ODD | (USART_STOPBITS_2 >> 8),
    SERIAL_9N0_5 = UART_WORDLENGTH_9B | UART_PARITY_NONE | (USART_STOPBITS_0_5 >> 8),
    SERIAL_9N1_5 = UART_WORDLENGTH_9B | UART_PARITY_NONE | (USART_STOPBITS_1_5 >> 8),
    SERIAL_9E0_5 = UART_WORDLENGTH_9B | UART_PARITY_EVEN | (USART_STOPBITS_0_5 >> 8),
    SERIAL_9E1_5 = UART_WORDLENGTH_9B | UART_PARITY_EVEN | (USART_STOPBITS_1_5 >> 8),
    SERIAL_9O0_5 = UART_WORDLENGTH_9B | UART_PARITY_ODD | (USART_STOPBITS_0_5 >> 8),
    SERIAL_9O1_5 = UART_WORDLENGTH_9B | UART_PARITY_ODD | (USART_STOPBITS_1_5 >> 8),
} Serial_Config;

typedef enum
{

#if defined(USART1_BASE)
	serial1 = 1,
#endif

#if defined(USART2_BASE)
	serial2 = 2,
#endif

#if defined(USART3_BASE)
	serial3 = 3,
#endif

#if defined(UART4_BASE) | defined(USART4_BASE)
	serial4 = 4,
#endif

#if defined(UART5_BASE) | defined(USART5_BASE)
    serial5 = 5,
#endif

#if defined(USART6_BASE)
	serial6 = 6,
#endif

#if defined(UART7_BASE) | defined(USART7_BASE)
	serial7 = 7,
#endif


#if defined(UART8_BASE) | defined(USART8_BASE)
	serial8 = 8,
#endif

#if defined(UART9_BASE)
	serial9 = 9,
#endif

#if defined(UART10_BASE)
	serial10 = 10,
#endif
	SERIAL_NUM
} Serial_TypeDef;

typedef struct __Serial_HandleTypeDef
{
	uint16_t _rx_buffer_head;
	uint16_t _rx_buffer_tail;
	uint8_t _rx_buffer[SERIAL_RX_BUFFER_SIZE];
	PinName RW_PIN;
	GPIO_State_Type WriteState;
	void (* TxCallback)(void);        /*!< UART Tx  Callback        */
	void (* RxCallback)(char);        /*!< UART Rx  Callback        */
	UART_HandleTypeDef huart;
} Serial_HandleTypeDef;

typedef void(*USART_RxCallbackFunction_t)(char);
typedef void(*USART_TxCallbackFunction_t)(void);
/* Exported variables --------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
//UART_HandleTypeDef* serial_open(Serial_TypeDef num,uint32_t baudrate, ...);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 常用函数，优先使用以下函数
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/**
 * @brief  最简串口初始化
 * @note   串口初始化共计四个函数，根据自己需求选择合适自己的初始化函数
 * 			serial_begin
 * 			serial_begin_cof
 * 			serial_begin_cof_IRQpri
 * 			serial_begin_cof_IRQpri
 * @param[in] num:串口号
 * @param[in] baudrate：波特率，
 * @retval hal的串口句柄
 */
UART_HandleTypeDef* serial_begin(Serial_TypeDef num,uint32_t baudrate);


/**
 * @brief  接收串口中断回调注册函数
 * @param[in] num:串口号
 * @param[in]  Function: 回调函数
 * @retval 无
 */
void serial_attachRxInterrupt(Serial_TypeDef num,USART_RxCallbackFunction_t Function);

/**
 * @brief  发送串口中断回调注册函数
 * @param[in] num:串口号
 * @param[in]  Function: 回调函数
 * @retval 无
 */
void serial_attachTxInterrupt(Serial_TypeDef num,USART_TxCallbackFunction_t Function);

/**
 * @brief  注册485读写引脚
 * @note   一定要先打开串口，后注册，因为打开串口会自动删除读写引脚
 * @param[in] num:串口号
 * @param[in] pin:Pin Name
 * @param[in]  _writestate：为写的时候Pin的状态 PIN_LOW,PIN_HIGH
 * @retval
 */
void serial_attachRWCtrl_Pin(Serial_TypeDef num, PinName pin,GPIO_State_Type _writestate);

/**
 * @brief  读取串口缓存数据长度
 * @param[in] num:串口号
 * @retval    当前串口为读取的数据长度。
 */
int serial_available(Serial_TypeDef num);
/**
 * @brief  读取多个传入的串行数据(字符)
 * @param[in] num:串口号
 * @param[in] len:读取长度
 * @param[out] data:读取数据指针
 * @retval 返回读到的个数
 */
int serial_read(Serial_TypeDef num,uint8_t len,uint8_t *data);

/**
 * @brief  串口写入多个字节
 * @param[in]  num： 串口号
 * @param[in] len：字节长度
 * @param[in] data：传入的数据指针
 * @retval 字节
 */
int serial_write(Serial_TypeDef num,uint8_t len,uint8_t *data);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 以下为不常用函数
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/**
 * @brief  串口初始化
 * @note   串口初始化共计四个函数，根据自己需求选择合适自己的初始化函数
 * 			serial_begin
 * 			serial_begin_cof
 * 			serial_begin_cof_IRQpri
 * 			serial_begin_cof_IRQpri
 * @param[in] num:串口号
 * @param[in] baudrate：波特率，
 * @param[in] Config：串口配置 例如SERIAL_8N1
 * @retval hal的串口句柄
 */
UART_HandleTypeDef* serial_begin_cof(Serial_TypeDef num, uint32_t BaudRate, Serial_Config Config);

/**
 * @brief  串口初始化
 * @note   串口初始化共计四个函数，根据自己需求选择合适自己的初始化函数
 * 			serial_begin
 * 			serial_begin_cof
 * 			serial_begin_cof_IRQpri
 * 			serial_begin_cof_IRQpri
 * @param[in] num:串口号
 * @param[in] baudrate：波特率，
 * @param[in] Config：串口配置 例如SERIAL_8N1
 * @param[in] PreemptionPriority：抢占优先级
 * @param[in] SubPriority：子优先级
 * @retval hal的串口句柄
 */
UART_HandleTypeDef* serial_begin_cof_IRQpri(Serial_TypeDef num, uint32_t BaudRate,
		Serial_Config Config, uint8_t PreemptionPriority, uint8_t SubPriority);

/**
 * @brief  最全串口初始化
 * @note   串口初始化共计四个函数，根据自己需求选择合适自己的初始化函数
 * 			serial_begin
 * 			serial_begin_cof
 * 			serial_begin_cof_IRQpri
 * 			serial_begin_cof_IRQpri
 * @param[in] num:串口号
 * @param[in] baudrate：波特率，
 * @param[in] Config：串口配置 例如SERIAL_8N1
 * @param[in] PreemptionPriority：抢占优先级
 * @param[in] SubPriority：子优先级
 * @param[in] tx_pin：发送引脚
 * @param[in] rx_pin：接收引脚
 * @retval hal的串口句柄
 */
UART_HandleTypeDef* serial_begin_cof_IRQpri_pin(Serial_TypeDef num, uint32_t BaudRate,
		Serial_Config Config, uint8_t PreemptionPriority, uint8_t SubPriority,
		PinName tx_pin, PinName rx_pin);
/**
 * @brief  返回传入串行数据的下一个字节(字符), 而不将其从内部串行缓冲区中删除
 * @param[in]  num： 串口号
 * @retval 可用的传入串行数据的第一个字节 (如果没有可用的数据, 则为-1)
 */
int serial_peek(Serial_TypeDef num);

/**
 * @brief  清空串口缓存
 * @param[in]  num： 串口号
 * @retval 无
 */
void serial_flush(Serial_TypeDef num);

/**
 * @brief  配置printf输出使用哪个串口，
 * @note 使用之前那要先初始化串口，拿到句柄之后，在设置。
 * 		 默认使用串口5
 * @param  huart：串口句柄
 * @retval 可用的传入串行数据的第一个字节 (如果没有可用的数据, 则为-1)
 */
void prinf_conf(UART_HandleTypeDef *huart);

/**
 * @brief  配置并初始化printf使用的串口。
 * @note   高级设置请使用串口初始化函数，并配合prinf_conf使用。
 * @param[in] num： 串口号
 * @param[in] baudrate：波特率，
 * @retval none
 */
void printf_Init(Serial_TypeDef num, uint32_t baudrate);

/**
 * @brief  通过串口发送字符串
 * @note   建议优先使用serial_printf
 * @param[in] num： 串口号
 * @param[in] str：字符串指针
 * @retval none
 */
void serial_writeStr(Serial_TypeDef num,char *str);

/**
 * @brief  特定串口格式化输出
 * @note   使用方法同printf函数
 * @param[in] num： 串口号
 * @retval none
 */
int serial_printf(Serial_TypeDef num, const char *__restrict __format,...);

/**
 * @brief  读取传入的串行数据(字符)
 * @param  无
 * @retval 可用的传入串行数据的第一个字节 (如果没有可用的数据, 则为-1)
 */
int serial_readChar(Serial_TypeDef num);
/**
 * @brief  串口写入一个字节
 * @param[in] num： 串口号
 * @param[in] data： 写入字符
 * @retval 字节
 */
int serial_writeChar(Serial_TypeDef num,uint8_t data);

/**
 * @brief  串口关闭函数。
 * @param[in] num:串口号
 * @retval
 */
void serial_end(Serial_TypeDef num);
/**
 *****************************************************************************
 * @brief 获取串口的hal句柄
 * @note
 * @param num：串口号，例如：serial1
 * @retval HAL库句柄
 * @author	ZGQ
 * @date	2024-08-06
 *****************************************************************************
 */
UART_HandleTypeDef * serial_get_handle(Serial_TypeDef num);


void RW_Ctrl_WriteEnable(Serial_TypeDef num);
void RW_Ctrl_ReadEnable(Serial_TypeDef num);


/*暂时不要使用以下函数*/
#define __COUNT_ARGS(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _n, X...) _n
#define COUNT_ARGS(X...) __COUNT_ARGS(, ##X, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
UART_HandleTypeDef* _serial_open(uint8_t num,Serial_TypeDef serial,uint32_t baudrate, ...);
#define serial_open(...) 	_serial_open(COUNT_ARGS(__VA_ARGS__),__VA_ARGS__)

#ifdef __cplusplus
}
#endif





#endif /* __APP_CONFIGURE_H */



/************************ (C) COPYRIGHT ROBOT++ *****END OF FILE****/
