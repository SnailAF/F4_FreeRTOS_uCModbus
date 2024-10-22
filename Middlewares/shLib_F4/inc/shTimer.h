/**
  ******************************************************************************
  * @file    shTimer.h
  * @author
  * @version
  * @date    2023.10.31
  * @brief   定时器使用函数
  *          适用于STM32F4系列
  *
  * ex:
  * void time_callback(void)
  * {
  * 	static uint32_t cnt;
  * 	cnt++;
  * }
  * int main(void)
  * {
  * 	TIM_HandleTypeDef* myhtim;
  * 	// 使用 TIM11,周期100us，回调函数time_callback
  * 	myhtim = timer_open(TIM11,100,time_callback);
  * 	//返回定时器句柄可以使用HAL库调用。
  *		__HAL_TIM_DISABLE_IT(myhtim,TIM_IT_UPDATE);
  * }
  ******************************************************************************
  * @attention
  * 可添加程序说明及注意事项：
  * 1.代码风格：ANSI风格
  * 2. 优化方向：不在使用hal的TIMx作为标识，改为shTIM_x。TIM_HandleTypeDef包含到TIM_x中。
  * void timer_clock_cmd(TIMName TIMx, FunctionalState NewState);
  *
  * 2023.10.31
  * 1.优化函数名字
  * 2.去掉LL库。
  *
  ******************************************************************************
  */

#ifndef __SHTIMER_H
#define __SHTIMER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mcu_type.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

#define IS_APB2_TIM(TIMx) (TIMx==TIM1||TIMx==TIM8||TIMx==TIM9||TIMx==TIM10||TIMx==TIM11)

#define IS_TIM_PERIPH(TIMx) (TIMx==TIM1||TIMx==TIM2||TIMx==TIM3||TIMx==TIM4||TIMx==TIM5||TIMx==TIM6\
								||TIMx==TIM7||TIMx==TIM8||TIMx==TIM9||TIMx==TIM10||TIMx==TIM11||TIMx==TIM12\
								||TIMx==TIM13||TIMx==TIM14)
typedef void(*Timer_CallbackFunction_t)(void);


/* Exported variables --------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

/* 常用函数 */
/**
 * @brief  定时初始化函数，
 * @note   默认自动启动
 * @param[in] TIMx: 定时器号, ex: TIM1, TIM2, TIM3, TIM4, TIM5, TIM6,
 * @param[in] time: 定时时间，单位us。
 * @param[in] function: 定时回调函数
 * @retval hal的定时器句柄
 */
TIM_HandleTypeDef* timer_open(TIM_TypeDef *TIMx, uint32_t time,Timer_CallbackFunction_t function);
/**
 * @brief  关闭定时
 * @note   
 * @param[in] TIMx: 定时器号, ex: TIM1, TIM2, TIM3, TIM4, TIM5, TIM6,
 * @retval  none
 */
void timer_closed(TIM_TypeDef *TIMx);
/**
 * @brief  启动定时器
 * @note   
 * @param[in] TIMx: 定时器号, ex: TIM1, TIM2, TIM3, TIM4, TIM5, TIM6,
 * @retval  none
 */
void timer_start(TIM_TypeDef *TIMx);
/**
 * @brief  暂停定时器
 * @note   
 * @param[in] TIMx: 定时器号, ex: TIM1, TIM2, TIM3, TIM4, TIM5, TIM6,
 * @retval  none
 */
void timer_stop(TIM_TypeDef *TIMx);
/**
 * @brief  改变定时周期
 * @note   
 * @param[in] TIMx: 定时器号, ex: TIM1, TIM2, TIM3, TIM4, TIM5, TIM6,
 * @param[in] time: 定时时间，单位us。
 * @retval  none
 */
void timer_change_period(TIM_TypeDef *TIMx, uint32_t time);


/* 不常用函数 */

void timer_set_priority(TIM_TypeDef *TIMx, uint8_t preemption_priority, uint8_t sub_priority);

/**
 * @brief  更新定时中断频率
 * @param  TIMx:定时器地址
 * @param  freq:中断频率
 * @retval 无
 */
void timer_change_freq(TIM_TypeDef *TIMx, uint32_t freq);

TIM_HandleTypeDef* timer_open_interrupt_base(TIM_TypeDef *TIMx, uint16_t period,
		uint16_t prescaler, Timer_CallbackFunction_t function,
		uint8_t PreemptionPriority, uint8_t SubPriority);

/**
 * @brief  获取定时器中断周期
 * @param  TIMx:定时器地址
 * @retval 中断周期
 */
uint32_t timer_get_period(TIM_TypeDef *TIMx);
/**
 * @brief  获取定时器中断频率
 * @param  TIMx:定时器地址
 * @retval 中断频率
 */
uint32_t timer_get_freq(TIM_TypeDef *TIMx);

void timer_clock_cmd(TIM_TypeDef *TIMx, FunctionalState NewState);
#ifdef __cplusplus
}
#endif

#endif /* __APP_CONFIGURE_H */



/************************ (C) COPYRIGHT ROBOT++ *****END OF FILE****/
