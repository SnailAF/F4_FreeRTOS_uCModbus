/**
  ******************************************************************************
  * @file    SH_config.h
  * @author	 ZGQ
  * @version
  * @date    2023年1月11日 下午5:31:50
  * @brief   SH库中使用了中断，如果想自定义中断，则会产生函数冲突。通过此文件可以配置
  * 		 使用哪些中断。
  * 		 配置原则，一旦开启了全局则不再关心具体中断开关，例如开启 USE_SH_IRQ_ALL == 1则无论
  * 		 其他配置，使用SH库的全部中断。
  * 		 USE_SH_IRQ_ALL == 0 &&  USE_SH_IRQ_UART_ALL == 1 则使用SH库的所有串口中断
  * 		 USE_SH_IRQ_ALL == 0 &&  USE_SH_IRQ_UART_ALL == 0 && USE_SH_IRQ_UART1 == 1
  * 		 使用SH库串口1中断。
  ******************************************************************************
  * @attention
  *	
  ******************************************************************************
  */

#ifndef SHLIB_F4_INC_SH_CONFIG_H_
#define SHLIB_F4_INC_SH_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

//#include "uNVIC.h"

#define USE_SH_IRQ_ALL 			(1)			/* 使用所有SH库 		*/
#define USE_SH_IRQ_UART_ALL		(1)			/* 使用所有SH串口 	*/
#define USE_SH_IRQ_TIM_ALL		(1)			/* 使用所有SH定时器 	*/
#define USE_SH_IRQ_CAN_ALL		(1)			/* 使用所有SH定时器 	*/

#define USE_SH_IRQ_UART1		(1)			/* 使用SH库串口1 		*/
#define USE_SH_IRQ_UART2		(1)			/* 使用SH库串口2		*/
#define USE_SH_IRQ_UART3		(1)			/* 使用SH库串口3 		*/
#define USE_SH_IRQ_UART4		(1)			/* 使用SH库串口4 		*/
#define USE_SH_IRQ_UART5		(1)			/* 使用SH库串口5 		*/
#define USE_SH_IRQ_UART6		(1)			/* 使用SH库串口6 		*/

#define USE_SH_IRQ_TIM1			(1)			/* 使用SH库定时器1 	*/
#define USE_SH_IRQ_TIM2			(1)			/* 使用SH库定时器2 	*/
#define USE_SH_IRQ_TIM3			(1)			/* 使用SH库定时器3 	*/
#define USE_SH_IRQ_TIM4			(1)			/* 使用SH库定时器4 	*/
#define USE_SH_IRQ_TIM5			(1)			/* 使用SH库定时器5 	*/
#define USE_SH_IRQ_TIM6			(1)			/* 使用SH库定时器6 	*/
#define USE_SH_IRQ_TIM7			(1)			/* 使用SH库定时器7 	*/
#define USE_SH_IRQ_TIM8			(1)			/* 使用SH库定时器8 	*/
#define USE_SH_IRQ_TIM9			(1)			/* 使用SH库定时器9 	*/
#define USE_SH_IRQ_TIM10		(1)			/* 使用SH库定时器10 	*/
#define USE_SH_IRQ_TIM11		(1)			/* 使用SH库定时器11 	*/
#define USE_SH_IRQ_TIM12		(1)			/* 使用SH库定时器12 	*/
#define USE_SH_IRQ_TIM13		(1)			/* 使用SH库定时器13 	*/
#define USE_SH_IRQ_TIM14		(1)			/* 使用SH库定时器14 	*/
#define USE_SH_IRQ_TIM15		(1)			/* 使用SH库定时器15 	*/
#define USE_SH_IRQ_TIM16		(1)			/* 使用SH库定时器16 	*/
#define USE_SH_IRQ_TIM17		(1)			/* 使用SH库定时器17 	*/
#define USE_SH_IRQ_TIM18		(1)			/* 使用SH库定时器18 	*/
#define USE_SH_IRQ_TIM19		(1)			/* 使用SH库定时器19 	*/
#define USE_SH_IRQ_TIM20		(1)			/* 使用SH库定时器20 	*/
#define USE_SH_IRQ_TIM21		(1)			/* 使用SH库定时器21 	*/
#define USE_SH_IRQ_TIM22		(1)			/* 使用SH库定时器22 	*/

#define USE_SH_IRQ_CAN1			(1)			/* 使用SH库CAN1 		*/
#define USE_SH_IRQ_CAN2			(1)			/* 使用SH库CAN1 		*/

#define USE_FREERTOS			(1)			/* 是否使用freertos 与延时有关 */

/*
 * 中断管理
 */
/* CAN 串口中断 */
#ifdef ufSH_CAN_RX0_IRQn_PRIORITY
#define CAN_PreemptionPriority_Default ufSH_CAN_RX0_IRQn_PRIORITY	//CAN默认中断抢占优先级，后期可以统一管理
#else
#define CAN_PreemptionPriority_Default	(7)
#endif
#define CAN_SubPriority_Default (0)			//CAN默认中断响应优先级

/* 定时器中断 */
#ifdef ufSH_TIM_IRQn_PRIORITY
#define Timer_PreemptionPriority_Default ufSH_TIM_IRQn_PRIORITY
#else
#define Timer_PreemptionPriority_Default	(10)
#endif
#define Timer_SubPriority_Default (0)

/* 串口中断 */
#ifdef ufSH_UART_IRQn_PRIORITY
#define USART_PreemptionPriority_Default ufSH_UART_IRQn_PRIORITY
#else
#define USART_PreemptionPriority_Default	(9)
#endif
#define USART_SubPriority_Default (0)

/*
 * 调试输出
 * 目前只能使用easylog 或者 printf作为日志输出，两者只能二选一，
 * 如果系统中已经使用了elog 则只能使用elog输出
 * 在使用了elog的系统中，不能在此处关闭elog输出。需要再elog中进行关闭输出
 */
#define USE_ELOG 	(0)	/* 使用 easylog 作为日志输出*/
#define USE_PRINTF  (0)	/* 使用默认的printf 作为日志输出 */
#if USE_ELOG
#include "elog.h"
#elif USE_PRINTF
#include "stdio.h"
#define log_a(...) 	printf(__VA_ARGS__)
#define log_e(...) 	printf(__VA_ARGS__)
#define log_w(...) 	printf(__VA_ARGS__)
#define log_i(...) 	printf(__VA_ARGS__)
#define log_d(...) 	printf(__VA_ARGS__)
#define log_v(...) 	printf(__VA_ARGS__)
#else
#define log_a(...) 	((void)0);
#define log_e(...) 	((void)0);
#define log_w(...) 	((void)0);
#define log_i(...) 	((void)0);
#define log_d(...) 	((void)0);
#define log_v(...) 	((void)0);
#endif
#ifdef __cplusplus
}
#endif

#endif  /* SHLIB_F4_INC_SH_CONFIG_H_ */

/************************ (C) COPYRIGHT ROBOT++ *******************************/
