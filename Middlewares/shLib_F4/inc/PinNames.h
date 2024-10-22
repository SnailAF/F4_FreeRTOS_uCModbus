/**
  ******************************************************************************
  * @file    PinNames.h
  * @author	 ZGQ
  * @version
  * @date    2022年5月20日 下午5:31:07
  * @brief   
  ******************************************************************************
  * @attention
  *	
  ******************************************************************************
  */

#ifndef _PINNAMES_H_
#define _PINNAMES_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mcu_type.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {
    PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
    PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15,
	#if defined GPIOC_BASE
    PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12, PC13, PC14, PC15,
	#endif
	#if defined GPIOD_BASE
    PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12, PD13, PD14, PD15,
	#endif
	#if defined GPIOE_BASE
    PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7, PE8, PE9, PE10, PE11, PE12, PE13, PE14, PE15,
	#endif
	#if defined GPIOF_BASE
    PF0, PF1, PF2, PF3, PF4, PF5, PF6, PF7, PF8, PF9, PF10, PF11, PF12, PF13, PF14, PF15,
	#endif
	#if defined GPIOG_BASE
    PG0, PG1, PG2, PG3, PG4, PG5, PG6, PG7, PG8, PG9, PG10, PG11, PG12, PG13, PG14, PG15,
	#endif
	#if defined GPIOH_BASE
    PH0, PH1, PH2, PH3, PH4, PH5, PH6, PH7, PH8, PH9, PH10, PH11, PH12, PH13, PH14, PH15,
	#endif
    PIN_NUM,
		// Not connected
    PIN_NC = (int)0xFFFFFFFF,
}PinName;

typedef struct STM32_PinInfo {
    GPIO_TypeDef* GPIOx;
    TIM_TypeDef* TIMx;
    ADC_TypeDef* ADCx;

    uint16_t GPIO_Pin_x;
    uint8_t TimerChannel;
    uint8_t ADC_Channel;
} PinInfo_TypeDef;

typedef struct  {
    PinName  Pin;
    TIM_TypeDef* TIMx;
    uint8_t Channel;
} TimPinMap_TypeDef;

/* Exported constants --------------------------------------------------------*/
#define ADC_Channel_X ((uint8_t)0xFF)

/* Exported macro ------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
extern const PinInfo_TypeDef PIN_MAP[PIN_NUM];
extern const TimPinMap_TypeDef PinMap_PWM[];
/* Exported functions prototypes ---------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif  /* SHLIB_F4_INC_PINNAMES_H_ */

/************************ (C) COPYRIGHT ROBOT++ *******************************/
