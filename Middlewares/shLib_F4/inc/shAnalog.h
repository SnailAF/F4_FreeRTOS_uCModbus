/**
  ******************************************************************************
  * @file    shAnalog.h
  * @author
  * @version
  * @date    2022.02.21
  * @brief  ADC1 为DMA使用的adc。
  * 		ADC2 为单次转换使用的adc
  * @note 	如果使用DMA，则读到为最近10次的平均值。
  * @ex1:	单次转换。
  * 		1#设置引脚为模拟量输入，2#读取引脚电压。
  * 		pinMode(PC4,INPUT_ANALOG);
  * 		vol = analogRead(PC4)*3300/4095;
  * @ex2：  使用DMA
  * 		pinMode(PC4,INPUT_ANALOG_DMA);
  * 		vol = analogRead(PC4)*3300/4095;
  ******************************************************************************
  * @attention
  * 变更说明： 2022-06-21
  * 1. 优化adc读，adc只初始化一次
  ******************************************************************************
  */
#ifndef __SHANALOG_H
#define __SHANALOG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mcu_type.h"
#include "shGpio.h"

/* Exported constants --------------------------------------------------------*/
#define ADC_DMA_AVERAGE_NUM	(10)	/* DMA下计算单通道缓存多少个，用于平均值 */
/* Exported macro ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 常用函数，优先使用以下函数
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/**
 * @brief  从指定的模拟引脚读取值,先查看是否使用DMA
 * @note   会自动判断是否使用了DMA，如果使用了DMA则读到的是DMA中的缓存值。
 * @note   使用前，要先设置为模拟量输入,pinMode(PC4,INPUT_ANALOG);
 * @param[in] Pin:使用的引脚
 * @retval   读到的AD值0~4095
 * @ex      vol = analogRead(PC4);
 */
uint16_t analogRead(PinName Pin);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 以下为不常用函数
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/**
 * @brief  注册为DMA ADC
 * @note   只有注册函数，没有注销函数，用户应该使用pinMode(pin,INPUT_ANALOG_DMA);
 * 		   来注册ADC通道，而不是直接使用本函数，
 * @note   可以累积注册多个，新注册的通道不会影响前面注册的通道。
 * @param[in] ADC_Channel:使用到的通道数ADC_CHANNEL_0~ADC_CHANNEL_18
 * @retval   注册成功会返回引脚注册列表对应索引号。
 * 			注册失败错误码： -1:不支持ADC，-2:引脚重复注册，-3:超出最大注册个数
 * @ex      pinMode(PC4,INPUT_ANALOG_DMA);
 * 			pinMode(PC5,INPUT_ANALOG_DMA);
 * 			...
 * 			vol = analogRead(PC4)*3300/4095;
 * 			vol = analogRead(PC5)*3300/4095;
 */
int16_t ADC_DMA_Register(uint8_t ADC_Channel);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 以下为未测试函数
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
uint16_t analogWrite(PinName Pin, uint16_t val);

#ifdef __cplusplus
}
#endif

#endif /* __SHANALOG_H */


/************************ (C) COPYRIGHT ROBOT++ *****END OF FILE****/



