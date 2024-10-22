/**
  ******************************************************************************
  * @file 	gpio.h
  * @author	ZGQ
  * @version
  * @date  	2022年5月20日 下午5:33:00
  * @brief
  ******************************************************************************
  * @attention
  * 2023-01-13
  * 移除没用到的宏和函数，增加函数注释
  *
  ******************************************************************************
  */
#ifndef __GPIO_H
#define __GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mcu_type.h"
#include "PinNames.h"

#ifndef NULL
#define NULL 0
#endif

typedef enum
{
    INPUT,
    INPUT_PULLUP,
    INPUT_PULLDOWN,
    INPUT_ANALOG,
    INPUT_ANALOG_DMA,
    OUTPUT,
    OUTPUT_OPEN_DRAIN,
    OUTPUT_AF,
    PWM
} pinMode_TypeDef;

#define IS_PIN(Pin)     (Pin < PIN_NUM && Pin >= 0)
#define IS_ADC_PIN(Pin) (IS_PIN(Pin) && PIN_MAP[Pin].ADCx != NULL && PIN_MAP[Pin].ADC_Channel  != ADC_Channel_X)
#define IS_PWM_PIN(Pin) (IS_PIN(Pin) && PIN_MAP[Pin].TIMx != NULL && PIN_MAP[Pin].TimerChannel != 0)

typedef enum {PIN_LOW = 0, PIN_HIGH = !PIN_LOW} GPIO_State_Type;
typedef enum {Off = 0, On = !Off} _Switch_Type;
typedef enum {OFF = 0, ON = !OFF} _SWITCH_Type;

uint8_t GPIO_GetPortNum(PinName Pin);
uint8_t GPIO_GetPinNum(PinName Pin);
uint8_t GPIO_GetPinSource(uint16_t GPIO_Pin_x);
void  GPIO_RCC_ENABLE( GPIO_TypeDef* port );
/**
  * @brief  GPIO引脚初始化
  * @param  Pin:引脚编号
  * @param  pinMode_x: 引脚模式
  * @retval none
  */
void pinMode(PinName Pin, pinMode_TypeDef pinMode_x);

/**
  * @brief  GPIO引脚设置
  * @param  Pin:引脚编号
  * @param  val: 引脚模式 PIN_LOW；PIN_HIGH
  * @retval none
  */
void digitalWrite(PinName Pin, GPIO_State_Type val);

/**
  * @brief  读取GPIO引脚电平
  * @param  Pin:引脚编号
  * @retval 引脚状态
  */
uint8_t digitalRead(PinName Pin);
/**
  * @brief  引脚电平反转
  * @param  Pin:引脚编号
  * @retval none
  */
void digitalToggle(PinName Pin);

#ifdef __cplusplus
}// extern "C"
#endif

#endif
