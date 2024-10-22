/**
  ******************************************************************************
  * @file    pwm.h
  * @author
  * @version
  * @date
  * @brief   pwm生成
  ******************************************************************************
  * @attention
  * 变更说明： 2022-06-01
  * 		增加pwm_startTim函数，可以同时指定定时器和引脚。
  ******************************************************************************
  */

#ifndef __PWM_H
#define __PWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mcu_type.h"
#include "shGPio.h"
#define TIM_GetPinSourcex(Pin) GPIO_GetPinNum(Pin)

/**
  * @brief  PWM输出初始化
  * @param  Pin:引脚编号
  * @param  PWM_DutyCycle: PWM分级数
  * @param  PWM_Frequency: PWM频率
  * @retval 引脚对应的定时器通道
  */
uint8_t pwm_start(PinName Pin, uint16_t PWM_DutyCycle, uint32_t PWM_Frequency);

/**
  * @brief  输出PWM信号
  * @param  Pin: 引脚编号
  * @param  val:PWM占空比值
  * @retval PWM占空比值
  */
uint16_t pwmWrite(PinName Pin, uint16_t val);

/**
  * @brief  停止输出PWM信号,引脚设置为
  * @param  Pin: 引脚编号
  * @param  val:PWM占空比值
  * @retval PWM占空比值
  */
uint8_t pwm_stop(PinName Pin,uint8_t pin_state);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 以下为不常用函数
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/**
  * @brief  PWM输出初始化，指定定时器
  * @param[in]  Pin:引脚编号
  * @param[in]  PWM_DutyCycle: PWM分级数
  * @param[in]  PWM_Frequency: PWM频率
  * @param[in]  TIM:使用的定时器
  * @param[in]	TimerChannel：定时器通道
  * @retval 引脚对应的定时器通道
  */
uint8_t pwm_startTim(PinName Pin, uint16_t PWM_DutyCycle, uint32_t PWM_Frequency,TIM_TypeDef* TIM,uint8_t TimerChannel);

void TIMx_OCxInit(TIM_TypeDef* TIMx, uint16_t arr, uint16_t psc, uint8_t TimerChannel);
uint8_t TIM_GetGPIO_AF(PinName Pin);
uint16_t timer_get_compare(TIM_TypeDef* TIMx, uint8_t TimerChannel);
void timer_set_prescaler(TIM_TypeDef* TIMx, uint16_t psc);
void timer_set_reload(TIM_TypeDef* TIMx, uint16_t arr);
void timer_generate_update(TIM_TypeDef* TIMx);

/**
  * @brief  输出PWM信号
  * @note   有些信号取反了。在这个地方反一下
  * 		例如周期为1000,正常val=100时，占空比为10%，但是由于外部取反了,
  * 		真实占空比为90%。
  * @param  Pin: 引脚编号
  * @param  val:PWM占空比值 取反
  * @retval 真实的PWM占空比值
  *
  */
uint16_t pwmWriteNegative(PinName Pin, uint16_t val);
/**
  * @brief  读当前PWM信号
  * @param  Pin: 引脚编号
  * @retval PWM占空比值
  */
uint16_t pwmRead(PinName Pin);
#ifdef __cplusplus
}
#endif

#endif
