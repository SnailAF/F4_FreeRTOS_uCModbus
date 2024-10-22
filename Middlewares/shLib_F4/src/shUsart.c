/**
 ******************************************************************************
 * @file    bsp.c
 * @author
 * @version
 * @date    2021.8.18
 * @brief
 ******************************************************************************
 * @attention
 * 变更说明：2022-05-10
 * 1.增加 UART_HandleTypeDef* get_USARTHandle(Serial_TypeDef num) 函数
 * 2.增加void serial_writeStr(Serial_TypeDef num,char *str)函数，同时删除
 *    void serial_WriteStr(UART_HandleTypeDef *huart, char *str)函数
 * 3.修改int serial_printf(UART_HandleTypeDef *huart, const char *__restrict __format,
 *		...)
 *	 为int serial_printf(Serial_TypeDef num, const char *__restrict __format,
 *		...)
 *  2022-06-01
 *	1.修改：serial_read(write)为多字节读写，同时增加serial_read(write)Char函数
 *	2.增加485读写使能及注册函数。serial_attachRWCtrl_Pin
 *
 ******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "shUsart.h"
#include "BSP_pin_cof.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

/* Private types -------------------------------------------------------------*/
typedef struct
{
 PinName _tx_pin;
 PinName _rx_pin;
} SerialPin_TypeDef;
/* Private macro -------------------------------------------------------------*/
/* 485读写引脚，如果不为空则设置为对应状态。*/
//#define RW_Ctrl_WriteEnable(num) if(Serial[num].RW_PIN != PIN_NC ) digitalWrite(Serial[num].RW_PIN,Serial[num].WriteState)
//#define RW_Ctrl_ReadEnable(num) if(Serial[num].RW_PIN != PIN_NC ) digitalWrite(Serial[num].RW_PIN,!Serial[num].WriteState)

#define USART_GetWordLength(SERIAL_x)    ((uint16_t)(SERIAL_x&0xF000))
#define USART_GetParity(SERIAL_x)        ((uint16_t)(SERIAL_x&0x0F00))
#define USART_GetStopBits(SERIAL_x)      ((uint16_t)((SERIAL_x&0x00F0)<<8))
/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
SerialPin_TypeDef Default_Pin[SERIAL_NUM] = {
								{U1_TX_PIN,U1_RX_PIN},		//串口1 默认引脚
								{U2_TX_PIN,U2_RX_PIN},		//串口2 默认引脚
								{U3_TX_PIN,U3_RX_PIN},		//串口3 默认引脚
								{U4_TX_PIN,U4_RX_PIN},		//串口4 默认引脚
								{U5_TX_PIN,U5_RX_PIN},		//串口5 默认引脚
								{U6_TX_PIN,U6_RX_PIN},		//串口6 默认引脚
								};
/* Exported variables ---------------------------------------------------------*/
Serial_HandleTypeDef Serial[SERIAL_NUM];

/* Private function prototypes -----------------------------------------------*/


UART_HandleTypeDef* serial_begin(Serial_TypeDef num, uint32_t baudrate);
UART_HandleTypeDef* serial_begin_cof(Serial_TypeDef num, uint32_t BaudRate,
		Serial_Config Config);
UART_HandleTypeDef* serial_begin_cof_IRQpri(Serial_TypeDef num,
		uint32_t BaudRate, Serial_Config Config, uint8_t PreemptionPriority,
		uint8_t SubPriority);
UART_HandleTypeDef* serial_begin_cof_IRQpri_pin(Serial_TypeDef num, uint32_t BaudRate,
		Serial_Config Config, uint8_t PreemptionPriority, uint8_t SubPriority,
		PinName tx_pin, PinName rx_pin);
void serial_end(Serial_TypeDef num);
void serial_attachRxInterrupt(Serial_TypeDef num,
		USART_RxCallbackFunction_t Function);
void serial_attachTxInterrupt(Serial_TypeDef num,
		USART_TxCallbackFunction_t Function);
int serial_available(Serial_TypeDef num);
int serial_read(Serial_TypeDef num,uint8_t len,uint8_t *data);
int serial_peek(Serial_TypeDef num);
void serial_flush(Serial_TypeDef num);
int serial_write(Serial_TypeDef num,uint8_t len,uint8_t *data);
void serial_IRQHandler(Serial_TypeDef num);
UART_HandleTypeDef* get_USARTHandle(Serial_TypeDef num);
void RW_Ctrl_WriteEnable(Serial_TypeDef num);
void RW_Ctrl_ReadEnable(Serial_TypeDef num);
/* function ------------------ -----------------------------------------------*/
void RW_Ctrl_WriteEnable(Serial_TypeDef num){

	if(Serial[num].RW_PIN != PIN_NC ) {
		digitalWrite(Serial[num].RW_PIN,Serial[num].WriteState);
	}
}
void RW_Ctrl_ReadEnable(Serial_TypeDef num){
	if(Serial[num].RW_PIN != PIN_NC ) {
		digitalWrite(Serial[num].RW_PIN,!Serial[num].WriteState);
	}
}
/**
 * @brief  函数初始化基本函数
 * @param  串口号，波特率
 * @retval hal的串口句柄
 */
UART_HandleTypeDef* serial_begin(Serial_TypeDef num, uint32_t baudrate) {
	return serial_begin_cof(num, baudrate, SERIAL_Config_Default);
}
/**
 * @brief  函数初始化
 * @param  串口号，波特率，停止位等配置
 * @retval hal的串口句柄
 */
UART_HandleTypeDef* serial_begin_cof(Serial_TypeDef num, uint32_t BaudRate,
		Serial_Config Config) {
	return serial_begin_cof_IRQpri(num, BaudRate, Config,
			USART_PreemptionPriority_Default, USART_SubPriority_Default);
}
/**
 * @brief  函数初始化带优先级
 * @param  串口号，波特率，停止位等配置，中断优先级
 * @retval hal的串口句柄
 */
UART_HandleTypeDef* serial_begin_cof_IRQpri(Serial_TypeDef num,
		uint32_t BaudRate, Serial_Config Config, uint8_t PreemptionPriority,
		uint8_t SubPriority) {
	PinName tx_pin,rx_pin;
	tx_pin = Default_Pin[num-1]._tx_pin;
	rx_pin = Default_Pin[num-1]._rx_pin;
	return serial_begin_cof_IRQpri_pin(num,BaudRate,SERIAL_8N1,USART_PreemptionPriority_Default,
			USART_SubPriority_Default,tx_pin,rx_pin);
}
UART_HandleTypeDef* serial_begin_cof_IRQpri_pin(Serial_TypeDef num, uint32_t BaudRate,
		Serial_Config Config, uint8_t PreemptionPriority, uint8_t SubPriority,
		PinName tx_pin, PinName rx_pin) {

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	IRQn_Type irq;

	Serial[num].RW_PIN = PIN_NC;

	if(tx_pin >= PIN_NUM)
	{
		tx_pin = Default_Pin[num-1]._tx_pin;
	}
	if(rx_pin >= PIN_NUM)
	{
		rx_pin = Default_Pin[num-1]._rx_pin;
	}
	switch (num) {
	case serial1:
		Serial[num].huart.Instance = USART1;
		irq = USART1_IRQn;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
		__HAL_RCC_USART1_CLK_ENABLE();
		break;

	case serial2:
		Serial[num].huart.Instance = USART2;
		irq = USART2_IRQn;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
		__HAL_RCC_USART2_CLK_ENABLE();
		break;
	case serial3:
		Serial[num].huart.Instance = USART3;
		irq = USART3_IRQn;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
		__HAL_RCC_USART3_CLK_ENABLE();
		break;
	case serial4:
		Serial[num].huart.Instance = UART4;
		irq = UART4_IRQn;
		GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
		__HAL_RCC_UART4_CLK_ENABLE();
		break;
	case serial5:
		Serial[num].huart.Instance = UART5;
		irq = UART5_IRQn;
		GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
		__HAL_RCC_UART5_CLK_ENABLE();
		break;
	case serial6:
		Serial[num].huart.Instance = USART6;
		irq = USART6_IRQn;
		GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
		__HAL_RCC_USART6_CLK_ENABLE();
		break;
	default:
		return 0;
	}
	Serial[num].huart.Init.BaudRate = BaudRate;
	Serial[num].huart.Init.WordLength = USART_GetWordLength(Config);
	Serial[num].huart.Init.StopBits = USART_GetStopBits(Config);
	Serial[num].huart.Init.Parity = USART_GetParity(Config);
	Serial[num].huart.Init.Mode = UART_MODE_TX_RX;
	Serial[num].huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	Serial[num].huart.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&Serial[num].huart) != HAL_OK) {
		//Error_Handler();
	}
	GPIO_RCC_ENABLE(PIN_MAP[tx_pin].GPIOx);
	GPIO_RCC_ENABLE(PIN_MAP[rx_pin].GPIOx);
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	GPIO_InitStruct.Pin = PIN_MAP[tx_pin].GPIO_Pin_x;
	HAL_GPIO_Init(PIN_MAP[tx_pin].GPIOx, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = PIN_MAP[rx_pin].GPIO_Pin_x;
	HAL_GPIO_Init(PIN_MAP[rx_pin].GPIOx, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(irq, PreemptionPriority, SubPriority);
	HAL_NVIC_EnableIRQ(irq);

    __HAL_UART_ENABLE(&Serial[num].huart);
	__HAL_UART_ENABLE_IT(&Serial[num].huart, UART_IT_RXNE);
	return &Serial[num].huart;
}
/**
 * @brief  关闭串口
 * @param  无
 * @retval 无
 */
void serial_end(Serial_TypeDef num) {
	USART_TypeDef *USARTx= Serial[num].huart.Instance;
	if (USARTx) {
		LL_USART_Disable(USARTx);
	}
}

/**
 * @brief  串口中断回调
 * @param  Function: 回调函数
 * @retval 无
 */
void serial_attachRxInterrupt(Serial_TypeDef num,
		USART_RxCallbackFunction_t Function) {
	if (num < SERIAL_NUM)
		Serial[num].RxCallback = Function;
}
/**
 * @brief  串口中断回调
 * @param  Function: 回调函数
 * @retval 无
 */
void serial_attachTxInterrupt(Serial_TypeDef num,
		USART_TxCallbackFunction_t Function) {
	if (num < SERIAL_NUM)
		Serial[num].TxCallback = Function;
}
/**
 * @brief  获取可从串行端口读取的字节数
 * @param  无
 * @retval 可读取的字节数
 */
int serial_available(Serial_TypeDef num) {
	if (num >= SERIAL_NUM)
		return 0;
	return ((unsigned int) (SERIAL_RX_BUFFER_SIZE + Serial[num]._rx_buffer_head
			- Serial[num]._rx_buffer_tail)) % SERIAL_RX_BUFFER_SIZE;
}

/**
 * @brief  读取传入的串行数据(字符)
 * @param  无
 * @retval 可用的传入串行数据的第一个字节 (如果没有可用的数据, 则为-1)
 */
int serial_readChar(Serial_TypeDef num) {
	// if the head isn't ahead of the tail, we don't have any characters
	if (Serial[num]._rx_buffer_head == Serial[num]._rx_buffer_tail) {
		return -1;
	} else {
		unsigned char c = Serial[num]._rx_buffer[Serial[num]._rx_buffer_tail];
		Serial[num]._rx_buffer_tail = (uint16_t) (Serial[num]._rx_buffer_tail
				+ 1) % SERIAL_RX_BUFFER_SIZE;
		return c;
	}
}
/**
 * @brief  读取多个传入的串行数据(字符)
 * @param[in] num:串口号
 * @param[in] len:读取长度
 * @param[out] data:读取数据指针
 * @retval 返回读到的个数
 */
int serial_read(Serial_TypeDef num,uint8_t len,uint8_t *data) {

	uint8_t i;
	for (i = 0; i < len; i++) {
		if(serial_available(num))
		{
			*(data + i) = serial_readChar(num);
		}else {
			break;
		}
	}
	return i;
}

/**
 * @brief  返回传入串行数据的下一个字节(字符), 而不将其从内部串行缓冲区中删除
 * @param  无
 * @retval 可用的传入串行数据的第一个字节 (如果没有可用的数据, 则为-1)
 */
int serial_peek(Serial_TypeDef num) {
	if (Serial[num]._rx_buffer_head == Serial[num]._rx_buffer_tail) {
		return -1;
	} else {
		return Serial[num]._rx_buffer[Serial[num]._rx_buffer_tail];
	}
}
/**
 * @brief  注册485读写引脚
 * @note   一定要先打开串口，后注册，因为打开串口会自动删除读写引脚
 * @param[in] num:串口号
 * @param[in] pin:Pin Name
 * @param[in]  _writestate：为写的时候Pin的状态 PIN_LOW,PIN_HIGH
 * @retval
 */
void serial_attachRWCtrl_Pin(Serial_TypeDef num, PinName pin,
		GPIO_State_Type _writestate) {
	if (num < SERIAL_NUM) {
		//Serial[num].isRW_Ctrl = 1;
		Serial[num].RW_PIN = pin;
		Serial[num].WriteState = _writestate;
		pinMode(pin, OUTPUT);
	}

}
/**
 * @brief  清空串口缓存
 * @param  无
 * @retval 无
 */
void serial_flush(Serial_TypeDef num) {
	Serial[num]._rx_buffer_head = Serial[num]._rx_buffer_tail;
}

/**
 * @brief  串口写入一个字节
 * @param  写入的字节
 * @retval 字节
 */
int serial_writeChar(Serial_TypeDef num, uint8_t c) {

	USART_TypeDef *USARTx = Serial[num].huart.Instance;

	if (USARTx) {
		while (!LL_USART_IsActiveFlag_TXE(USARTx)) {
		};
		RW_Ctrl_WriteEnable(num);
		LL_USART_TransmitData8(USARTx, c);
		RW_Ctrl_ReadEnable(num);
	}
	return 1;
}


/**
 * @brief  串口写入多个字节
 * @param[in]  num： 串口号
 * @param[in] len：字节长度
 * @param[in] data：传入的数据指针
 * @retval 字节
 */
int serial_write(Serial_TypeDef num, uint8_t len,uint8_t *data) {

	UART_HandleTypeDef *huart;
	huart = &Serial[num].huart;

	RW_Ctrl_WriteEnable(num);
	HAL_UART_Transmit( huart, (uint8_t*) data, len, 200);
	RW_Ctrl_ReadEnable(num);

	return 1;
}
/**
 * @brief  串口中断入口
 * @param  无
 * @retval 无
 */
void serial_IRQHandler(Serial_TypeDef num) {

	USART_TypeDef *USARTx = Serial[num].huart.Instance;
	if (LL_USART_IsActiveFlag_RXNE(USARTx)) {
		uint8_t c = LL_USART_ReceiveData8(USARTx);
		uint16_t i = (uint16_t) (Serial[num]._rx_buffer_head + 1)
				% SERIAL_RX_BUFFER_SIZE;
		if (i != Serial[num]._rx_buffer_tail) {
			Serial[num]._rx_buffer[Serial[num]._rx_buffer_head] = c;
			Serial[num]._rx_buffer_head = i;
		}
		if (Serial[num].RxCallback) {
			Serial[num].RxCallback(c);
		}
		LL_USART_ClearFlag_RXNE(USARTx);
	} else if (LL_USART_IsActiveFlag_TC(USARTx)) {
		if (Serial[num].TxCallback) {
			Serial[num].TxCallback();
		}
		LL_USART_ClearFlag_TC(USARTx);
	} else {
		LL_USART_ClearFlag_FE(USARTx);
	}
}
/**
 *****************************************************************************
 * @brief 通过串口发送字符串
 * @note  最终通过HAL_UART_Transmit去发送
 * @param num：串口号，例如：serial1
 * 		  *str:需要发送的字符串指针
 * @retval None
 * @author	ZGQ
 * @date	2022-05-10
 * @ex		str[] = "abcd"
 * 			serial_writeStr(serial1,str);
 *****************************************************************************
 */
void serial_writeStr(Serial_TypeDef num,char *str)
{
	UART_HandleTypeDef *huart;
	huart = &Serial[num].huart;
	RW_Ctrl_WriteEnable(num);
	HAL_UART_Transmit( huart, (uint8_t*) str, strlen(str), 200);
	RW_Ctrl_ReadEnable(num);
}
/**
 *****************************************************************************
 * @brief  通过串口发送格式化字符串
 * @note   1.使用方法参考printf
 * 		   2.内部调用serial_writeStr函数完成发送
 * @param num：串口号，例如：serial1
 * 		  __format:格式化数据
 * @retval None
 * @author	ZGQ
 * @date	2022-05-10
 * @ex		uint8_t data = 10
 * 			serial_printf(serial1,"data = %d",data);
 *****************************************************************************
 */
int serial_printf(Serial_TypeDef num, const char *__restrict __format,
		...) {

	char printf_buff[128];

	va_list args;
	va_start(args, __format);
	int ret_status = vsnprintf(printf_buff, sizeof(printf_buff), __format,args);

	va_end(args);
	serial_writeStr(num, printf_buff);

	return ret_status;
}

#if USE_SH_IRQ_ALL || USE_SH_IRQ_UART_ALL || USE_SH_IRQ_UART1
/**
 * @brief This function handles USART1 global interrupt.
 */
 void USART1_IRQHandler(void) {
	serial_IRQHandler(serial1);
}
#endif
#if USE_SH_IRQ_ALL || USE_SH_IRQ_UART_ALL || USE_SH_IRQ_UART2
/**
 * @brief This function handles USART2 global interrupt.
 */
void USART2_IRQHandler(void) {
	serial_IRQHandler(serial2);
}
#endif
#if USE_SH_IRQ_ALL || USE_SH_IRQ_UART_ALL || USE_SH_IRQ_UART3
/**
 * @brief This function handles USART3 global interrupt.
 */
void USART3_IRQHandler(void) {
	serial_IRQHandler(serial3);
}
#endif
#if USE_SH_IRQ_ALL || USE_SH_IRQ_UART_ALL || USE_SH_IRQ_UART4
/**
 * @brief This function handles UART4 global interrupt.
 */
void UART4_IRQHandler(void) {
	serial_IRQHandler(serial4);
}
#endif
#if USE_SH_IRQ_ALL || USE_SH_IRQ_UART_ALL || USE_SH_IRQ_UART5
/**
 * @brief This function handles UART5 global interrupt.
 */
void UART5_IRQHandler(void) {
	serial_IRQHandler(serial5);
}
#endif
#if USE_SH_IRQ_ALL || USE_SH_IRQ_UART_ALL || USE_SH_IRQ_UART6
/**
 * @brief This function handles USART6 global interrupt.
 */
void USART6_IRQHandler(void) {
	serial_IRQHandler(serial6);
}
#endif


/*
 * 	printf 输出串口配置
 * */
UART_HandleTypeDef *pfhuart = NULL;
#ifdef __GNUC__		/* CUBEIDE中printf函数 */

#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)

PUTCHAR_PROTOTYPE {
	if(pfhuart)
	{
		HAL_UART_Transmit(pfhuart, (uint8_t*) &ch, 1, 1);
	}
	return ch;
}
#else	/* keil中printf函数 */
#pragma import(__use_no_semihosting)
struct __FILE
{  
	int handle;
};
FILE __stdout;

FILE __stdin;

//int _sys_exit(int x)
void _sys_exit(int x)
{
	x = x;
}

int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(pfhuart, (uint8_t*) &ch, 1, 1);
	return ch;
}
#endif
/**
 * @brief  默认printf函数的初始化函数
 * @param  使用的串口num，波特率baudrate,更多参数自己配置
 * @retval 无
 */
void printf_Init(Serial_TypeDef num, uint32_t baudrate)
{
	pfhuart = serial_begin(num,baudrate);
}
/**
 * @brief  用户初始化完成串口后，将串口号赋值给printf。
 * 			如果只是配置串口和波特率青使用printf_Init函数
 * @param  使用的串口num，波特率baudrate,更多参数自己配置
 * @retval 无
 */
void prinf_conf(UART_HandleTypeDef *huart)
{
	pfhuart = huart;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/**
 * @brief  实现serial_open重载函数
 * @note   此函数处于测试阶段。不建议使用此函数
 * @note   参数类型提升见https://blog.csdn.net/astrotycoon/article/details/8284501
 * @note   获取参数个数原理【C语言笔记】【宏定义系列】 获取可变参数宏的参数数量
 * 			https://blog.csdn.net/u012028275/article/details/118853297
 * @note   实际调用一下函数，具体使用见具体函数
 * UART_HandleTypeDef* serial_begin(Serial_TypeDef num, uint32_t baudrate);
UART_HandleTypeDef* serial_begin_cof(Serial_TypeDef num, uint32_t BaudRate,
		Serial_Config Config);
UART_HandleTypeDef* serial_begin_cof_IRQpri(Serial_TypeDef num,
		uint32_t BaudRate, Serial_Config Config, uint8_t PreemptionPriority,
		uint8_t SubPriority);
UART_HandleTypeDef* serial_begin_cof_IRQpri_pin(Serial_TypeDef num, uint32_t BaudRate,
		Serial_Config Config, uint8_t PreemptionPriority, uint8_t SubPriority,
		PinName tx_pin, PinName rx_pin);
 * @param
 * @retval 无
 */
UART_HandleTypeDef* _serial_open(uint8_t num,Serial_TypeDef serial,uint32_t baudrate, ...)
{
	Serial_Config Config;
	uint8_t PreemptionPriority;
	uint8_t SubPriority;
	PinName tx_pin;
	PinName rx_pin;
	//uint8_t args_num;	// 参数个数
	UART_HandleTypeDef* huart;


	va_list args;
	va_start(args, baudrate);
	//args_num = COUNT_ARGS(num,baudrate,args);

	switch(num)
	{
	case 2:	//UART_HandleTypeDef* serial_begin(Serial_TypeDef num,uint32_t baudrate);
		huart = serial_begin(serial,baudrate);
		break;
	case 3: //serial_begin_cof(Serial_TypeDef serial, uint32_t BaudRate, Serial_Config Config);
		Config = va_arg(args, int);
		huart = serial_begin_cof(serial,baudrate,Config);

		break;
	case 5://serial_begin_cof_IRQpri(Serial_TypeDef serial, uint32_t BaudRate,Serial_Config Config, uint8_t PreemptionPriority, uint8_t SubPriority);

		Config = va_arg(args, int);
		PreemptionPriority = va_arg(args, int);
		SubPriority = va_arg(args, int);
		huart = serial_begin_cof_IRQpri(serial,baudrate,Config,PreemptionPriority,SubPriority);
		break;
	case 7://serial_begin_cof_IRQpri_pin(Serial_TypeDef serial, uint32_t BaudRate,Serial_Config Config, uint8_t PreemptionPriority, uint8_t SubPriority,PinName tx_pin, PinName rx_pin);

		Config = va_arg(args, int);
		PreemptionPriority = va_arg(args, int);
		SubPriority = va_arg(args, int);
		tx_pin = va_arg(args, int);
		rx_pin = va_arg(args, int);
		huart = serial_begin_cof_IRQpri_pin(serial,baudrate,Config,PreemptionPriority,SubPriority,tx_pin,rx_pin);
		break;
	}
	va_end(args);
	return huart;
}

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
UART_HandleTypeDef * serial_get_handle(Serial_TypeDef num)
{
	return &Serial[num].huart;
}
