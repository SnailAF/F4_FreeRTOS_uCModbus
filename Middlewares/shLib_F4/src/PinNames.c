/**
  ******************************************************************************
  * @file 	PinNames.c
  * @author	ZGQ
  * @version
  * @date  	2022年5月20日 下午5:33:00
  * @brief
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Private includes ----------------------------------------------------------*/
#include "PinNames.h"
/* Private types -------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/
const PinInfo_TypeDef PIN_MAP[PIN_NUM] =
{
    /*GPIO_TypeDef* GPIOx;  //对应GPIOx地址
    TIM_TypeDef* TIMx;      //对应TIMx地址
    ADC_TypeDef* ADCx;      //对应ADCx地址

    uint16_t GPIO_Pin_x;    //对应GPIO_Pin位
    uint8_t TimerChannel;   //对应定时器通道
    uint8_t ADC_Channel;*/  //对应ADC通道
    {GPIOA, TIM2, ADC1,  GPIO_PIN_0, 1, ADC_CHANNEL_0}, /* PA0 */
    {GPIOA, TIM2, ADC1,  GPIO_PIN_1, 2, ADC_CHANNEL_1}, /* PA1 */
    {GPIOA, TIM2, ADC1,  GPIO_PIN_2, 3, ADC_CHANNEL_2}, /* PA2 */
    {GPIOA, TIM2, ADC1,  GPIO_PIN_3, 4, ADC_CHANNEL_3}, /* PA3 */
    {GPIOA, NULL, ADC1,  GPIO_PIN_4, 0, ADC_CHANNEL_4}, /* PA4 */
    {GPIOA, NULL, ADC1,  GPIO_PIN_5, 0, ADC_CHANNEL_5}, /* PA5 */
    {GPIOA, TIM3, ADC1,  GPIO_PIN_6, 1, ADC_CHANNEL_6}, /* PA6 */
    {GPIOA, TIM3, ADC1,  GPIO_PIN_7, 2, ADC_CHANNEL_7}, /* PA7 */
    {GPIOA, TIM1, NULL,  GPIO_PIN_8, 1, ADC_Channel_X}, /* PA8 */
    {GPIOA, TIM1, NULL,  GPIO_PIN_9, 2, ADC_Channel_X}, /* PA9 */
    {GPIOA, TIM1, NULL, GPIO_PIN_10, 3, ADC_Channel_X}, /* PA10 */
    {GPIOA, TIM1, NULL, GPIO_PIN_11, 4, ADC_Channel_X}, /* PA11 */
    {GPIOA, NULL, NULL, GPIO_PIN_12, 0, ADC_Channel_X}, /* PA12 */
    {GPIOA, NULL, NULL, GPIO_PIN_13, 0, ADC_Channel_X}, /* PA13 */
    {GPIOA, NULL, NULL, GPIO_PIN_14, 0, ADC_Channel_X}, /* PA14 */
    {GPIOA, TIM2, NULL, GPIO_PIN_15, 1, ADC_Channel_X}, /* PA15 */

    {GPIOB, TIM3, ADC1,  GPIO_PIN_0, 3, ADC_CHANNEL_8}, /* PB0 */
    {GPIOB, TIM3, ADC1,  GPIO_PIN_1, 4, ADC_CHANNEL_9}, /* PB1 */
    {GPIOB, NULL, NULL,  GPIO_PIN_2, 0, ADC_Channel_X}, /* PB2 */
    {GPIOB, TIM2, NULL,  GPIO_PIN_3, 2, ADC_Channel_X}, /* PB3 */
    {GPIOB, TIM3, NULL,  GPIO_PIN_4, 1, ADC_Channel_X}, /* PB4 */
    {GPIOB, TIM3, NULL,  GPIO_PIN_5, 2, ADC_Channel_X}, /* PB5 */
    {GPIOB, TIM4, NULL,  GPIO_PIN_6, 1, ADC_Channel_X}, /* PB6 */
    {GPIOB, TIM4, NULL,  GPIO_PIN_7, 2, ADC_Channel_X}, /* PB7 */
    {GPIOB, TIM4, NULL,  GPIO_PIN_8, 3, ADC_Channel_X}, /* PB8 */
    {GPIOB, TIM4, NULL,  GPIO_PIN_9, 4, ADC_Channel_X}, /* PB9 */
    {GPIOB, TIM2, NULL, GPIO_PIN_10, 3, ADC_Channel_X}, /* PB10 */
    {GPIOB, TIM2, NULL, GPIO_PIN_11, 4, ADC_Channel_X}, /* PB11 */
    {GPIOB, NULL, NULL, GPIO_PIN_12, 0, ADC_Channel_X}, /* PB12 */
    {GPIOB, NULL, NULL, GPIO_PIN_13, 0, ADC_Channel_X}, /* PB13 */
    {GPIOB, TIM12, NULL, GPIO_PIN_14, 1, ADC_Channel_X},/* PB14 */
    {GPIOB, TIM12, NULL, GPIO_PIN_15, 2, ADC_Channel_X}, /* PB15 */

	#if defined(GPIOC_BASE)
    {GPIOC, NULL, ADC1,  GPIO_PIN_0, 0, ADC_CHANNEL_10},/* PC0 */
    {GPIOC, NULL, ADC1,  GPIO_PIN_1, 0, ADC_CHANNEL_11},/* PC1 */
    {GPIOC, NULL, ADC1,  GPIO_PIN_2, 0, ADC_CHANNEL_12},/* PC2 */
    {GPIOC, NULL, ADC1,  GPIO_PIN_3, 0, ADC_CHANNEL_13},/* PC3 */
    {GPIOC, NULL, ADC1,  GPIO_PIN_4, 0, ADC_CHANNEL_14},/* PC4 */
    {GPIOC, NULL, ADC1,  GPIO_PIN_5, 0, ADC_CHANNEL_15},/* PC5 */
    {GPIOC, TIM3, NULL,  GPIO_PIN_6, 1, ADC_Channel_X}, /* PC6 */
    {GPIOC, TIM3, NULL,  GPIO_PIN_7, 2, ADC_Channel_X}, /* PC7 */
    {GPIOC, TIM3, NULL,  GPIO_PIN_8, 3, ADC_Channel_X}, /* PC8 */
    {GPIOC, TIM3, NULL,  GPIO_PIN_9, 4, ADC_Channel_X}, /* PC9 */
    {GPIOC, NULL, NULL, GPIO_PIN_10, 0, ADC_Channel_X}, /* PC10 */
    {GPIOC, NULL, NULL, GPIO_PIN_11, 0, ADC_Channel_X}, /* PC11 */
    {GPIOC, NULL, NULL, GPIO_PIN_12, 0, ADC_Channel_X}, /* PC12 */
    {GPIOC, NULL, NULL, GPIO_PIN_13, 0, ADC_Channel_X}, /* PC13 */
    {GPIOC, NULL, NULL, GPIO_PIN_14, 0, ADC_Channel_X}, /* PC14 */
    {GPIOC, NULL, NULL, GPIO_PIN_15, 0, ADC_Channel_X}, /* PC15 */
	#endif
	#if defined(GPIOD_BASE)
    {GPIOD, NULL, NULL,  GPIO_PIN_0, 0, ADC_Channel_X}, /* PD0 */
    {GPIOD, NULL, NULL,  GPIO_PIN_1, 0, ADC_Channel_X}, /* PD1 */
    {GPIOD, NULL, NULL,  GPIO_PIN_2, 0, ADC_Channel_X}, /* PD2 */
    {GPIOD, NULL, NULL,  GPIO_PIN_3, 0, ADC_Channel_X}, /* PD3 */
    {GPIOD, NULL, NULL,  GPIO_PIN_4, 0, ADC_Channel_X}, /* PD4 */
    {GPIOD, NULL, NULL,  GPIO_PIN_5, 0, ADC_Channel_X}, /* PD5 */
    {GPIOD, NULL, NULL,  GPIO_PIN_6, 0, ADC_Channel_X}, /* PD6 */
    {GPIOD, NULL, NULL,  GPIO_PIN_7, 0, ADC_Channel_X}, /* PD7 */
    {GPIOD, NULL, NULL,  GPIO_PIN_8, 0, ADC_Channel_X}, /* PD8 */
    {GPIOD, NULL, NULL,  GPIO_PIN_9, 0, ADC_Channel_X}, /* PD9 */
    {GPIOD, NULL, NULL, GPIO_PIN_10, 0, ADC_Channel_X}, /* PD10 */
    {GPIOD, NULL, NULL, GPIO_PIN_11, 0, ADC_Channel_X}, /* PD11 */
    {GPIOD, TIM4, NULL, GPIO_PIN_12, 1, ADC_Channel_X}, /* PD12 */
    {GPIOD, TIM4, NULL, GPIO_PIN_13, 2, ADC_Channel_X}, /* PD13 */
    {GPIOD, TIM4, NULL, GPIO_PIN_14, 3, ADC_Channel_X}, /* PD14 */
    {GPIOD, TIM4, NULL, GPIO_PIN_15, 4, ADC_Channel_X}, /* PD15 */
	#endif
	#if defined(GPIOE_BASE)
    {GPIOE, NULL, NULL,  GPIO_PIN_0, 0, ADC_Channel_X}, /* PE0 */
    {GPIOE, NULL, NULL,  GPIO_PIN_1, 0, ADC_Channel_X}, /* PE1 */
    {GPIOE, NULL, NULL,  GPIO_PIN_2, 0, ADC_Channel_X}, /* PE2 */
    {GPIOE, NULL, NULL,  GPIO_PIN_3, 0, ADC_Channel_X}, /* PE3 */
    {GPIOE, NULL, NULL,  GPIO_PIN_4, 0, ADC_Channel_X}, /* PE4 */
    {GPIOE, TIM9, NULL,  GPIO_PIN_5, 1, ADC_Channel_X}, /* PE5 */
    {GPIOE, TIM9, NULL,  GPIO_PIN_6, 2, ADC_Channel_X}, /* PE6 */
    {GPIOE, NULL, NULL,  GPIO_PIN_7, 0, ADC_Channel_X}, /* PE7 */
    {GPIOE, NULL, NULL,  GPIO_PIN_8, 0, ADC_Channel_X}, /* PE8 */
    {GPIOE, TIM1, NULL,  GPIO_PIN_9, 1, ADC_Channel_X}, /* PE9 */
    {GPIOE, NULL, NULL, GPIO_PIN_10, 0, ADC_Channel_X}, /* PE10 */
    {GPIOE, TIM1, NULL, GPIO_PIN_11, 2, ADC_Channel_X}, /* PE11 */
    {GPIOE, NULL, NULL, GPIO_PIN_12, 0, ADC_Channel_X}, /* PE12 */
    {GPIOE, TIM1, NULL, GPIO_PIN_13, 3, ADC_Channel_X}, /* PE13 */
    {GPIOE, TIM1, NULL, GPIO_PIN_14, 4, ADC_Channel_X}, /* PE14 */
    {GPIOE, NULL, NULL, GPIO_PIN_15, 0, ADC_Channel_X}, /* PE15 */
	#endif
	#if defined(GPIOF_BASE)
    {GPIOF, NULL, NULL,  GPIO_PIN_0, 0, ADC_Channel_X}, /* PF0 */
    {GPIOF, NULL, NULL,  GPIO_PIN_1, 0, ADC_Channel_X}, /* PF1 */
    {GPIOF, NULL, NULL,  GPIO_PIN_2, 0, ADC_Channel_X}, /* PF2 */
    {GPIOF, NULL, ADC3,  GPIO_PIN_3, 0, ADC_CHANNEL_9}, /* PF3 */
    {GPIOF, NULL, ADC3,  GPIO_PIN_4, 0, ADC_CHANNEL_14},/* PF4 */
    {GPIOF, NULL, ADC3,  GPIO_PIN_5, 0, ADC_CHANNEL_15},/* PF5 */
    {GPIOF, TIM10, ADC3,  GPIO_PIN_6, 1, ADC_CHANNEL_4}, /* PF6 */
    {GPIOF, TIM11, ADC3,  GPIO_PIN_7, 1, ADC_CHANNEL_5}, /* PF7 */
    {GPIOF, TIM13, ADC3,  GPIO_PIN_8, 1, ADC_CHANNEL_6}, /* PF8 */
    {GPIOF, TIM14, ADC3,  GPIO_PIN_9, 1, ADC_CHANNEL_7}, /* PF9 */
    {GPIOF, NULL, ADC3, GPIO_PIN_10, 0, ADC_CHANNEL_8}, /* PF10 */
    {GPIOF, NULL, NULL, GPIO_PIN_11, 0, ADC_Channel_X}, /* PF11 */
    {GPIOF, NULL, NULL, GPIO_PIN_12, 0, ADC_Channel_X}, /* PF12 */
    {GPIOF, NULL, NULL, GPIO_PIN_13, 0, ADC_Channel_X}, /* PF13 */
    {GPIOF, NULL, NULL, GPIO_PIN_14, 0, ADC_Channel_X}, /* PF14 */
    {GPIOF, NULL, NULL, GPIO_PIN_15, 0, ADC_Channel_X}, /* PF15 */
	#endif
	#if defined(GPIOG_BASE)
    {GPIOG, NULL, NULL,  GPIO_PIN_0, 0, ADC_Channel_X}, /* PG0 */
    {GPIOG, NULL, NULL,  GPIO_PIN_1, 0, ADC_Channel_X}, /* PG1 */
    {GPIOG, NULL, NULL,  GPIO_PIN_2, 0, ADC_Channel_X}, /* PG2 */
    {GPIOG, NULL, NULL,  GPIO_PIN_3, 0, ADC_Channel_X}, /* PG3 */
    {GPIOG, NULL, NULL,  GPIO_PIN_4, 0, ADC_Channel_X}, /* PG4 */
    {GPIOG, NULL, NULL,  GPIO_PIN_5, 0, ADC_Channel_X}, /* PG5 */
    {GPIOG, NULL, NULL,  GPIO_PIN_6, 0, ADC_Channel_X}, /* PG6 */
    {GPIOG, NULL, NULL,  GPIO_PIN_7, 0, ADC_Channel_X}, /* PG7 */
    {GPIOG, NULL, NULL,  GPIO_PIN_8, 0, ADC_Channel_X}, /* PG8 */
    {GPIOG, NULL, NULL,  GPIO_PIN_9, 0, ADC_Channel_X}, /* PG9 */
    {GPIOG, NULL, NULL, GPIO_PIN_10, 0, ADC_Channel_X}, /* PG10 */
    {GPIOG, NULL, NULL, GPIO_PIN_11, 0, ADC_Channel_X}, /* PG11 */
    {GPIOG, NULL, NULL, GPIO_PIN_12, 0, ADC_Channel_X}, /* PG12 */
    {GPIOG, NULL, NULL, GPIO_PIN_13, 0, ADC_Channel_X}, /* PG13 */
    {GPIOG, NULL, NULL, GPIO_PIN_14, 0, ADC_Channel_X}, /* PG14 */
    {GPIOG, NULL, NULL, GPIO_PIN_15, 0, ADC_Channel_X}, /* PG15 */
	#endif
	#if defined(GPIOH_BASE)
    {GPIOH, NULL, NULL,  GPIO_PIN_0, 0, ADC_Channel_X}, /* PH0 */
    {GPIOH, NULL, NULL,  GPIO_PIN_1, 0, ADC_Channel_X}, /* PH1 */
    {GPIOH, NULL, NULL,  GPIO_PIN_2, 0, ADC_Channel_X}, /* PH2 */
    {GPIOH, NULL, NULL,  GPIO_PIN_3, 0, ADC_Channel_X}, /* PH3 */
    {GPIOH, NULL, NULL,  GPIO_PIN_4, 0, ADC_Channel_X}, /* PH4 */
    {GPIOH, NULL, NULL,  GPIO_PIN_5, 0, ADC_Channel_X}, /* PH5 */
    {GPIOH, NULL, NULL,  GPIO_PIN_6, 0, ADC_Channel_X}, /* PH6 */
    {GPIOH, NULL, NULL,  GPIO_PIN_7, 0, ADC_Channel_X}, /* PH7 */
    {GPIOH, NULL, NULL,  GPIO_PIN_8, 0, ADC_Channel_X}, /* PH8 */
    {GPIOH, NULL, NULL,  GPIO_PIN_9, 0, ADC_Channel_X}, /* PH9 */
    {GPIOH, NULL, NULL, GPIO_PIN_10, 0, ADC_Channel_X}, /* PH10 */
    {GPIOH, NULL, NULL, GPIO_PIN_11, 0, ADC_Channel_X}, /* PH11 */
    {GPIOH, NULL, NULL, GPIO_PIN_12, 0, ADC_Channel_X}, /* PH12 */
    {GPIOH, NULL, NULL, GPIO_PIN_13, 0, ADC_Channel_X}, /* PH13 */
    {GPIOH, NULL, NULL, GPIO_PIN_14, 0, ADC_Channel_X}, /* PH14 */
    {GPIOH, NULL, NULL, GPIO_PIN_15, 0, ADC_Channel_X}, /* PH15 */
	#endif
};

typedef struct {
    PinName pin;
    int peripheral;
    int function;
} PinMap;


//typedef enum {
//    PWM_1  = (int)TIM1_BASE,
//#if defined TIM2_BASE
//    PWM_2  = (int)TIM2_BASE,
//#endif
//#if defined TIM3_BASE
//    PWM_3  = (int)TIM3_BASE,
//#endif
//#if defined TIM4_BASE
//    PWM_4  = (int)TIM4_BASE,
//#endif
//#if defined TIM5_BASE
//    PWM_5  = (int)TIM5_BASE,
//#endif
//#if defined TIM8_BASE
//    PWM_8  = (int)TIM8_BASE,
//#endif
//#if defined TIM9_BASE
//    PWM_9  = (int)TIM9_BASE,
//#endif
//#if defined TIM10_BASE
//    PWM_10 = (int)TIM10_BASE,
//#endif
//#if defined TIM11_BASE
//    PWM_11 = (int)TIM11_BASE,
//#endif
//#if defined TIM12_BASE
//    PWM_12 = (int)TIM12_BASE,
//#endif
//#if defined TIM13_BASE
//    PWM_13 = (int)TIM13_BASE,
//#endif
//#if defined TIM14_BASE
//    PWM_14 = (int)TIM14_BASE
//#endif
//} PWMName;
// TIM5 cannot be used because already used by the us_ticker

const TimPinMap_TypeDef PinMap_PWM[]={
		{PA0,  TIM2, 1}, // TIM2_CH1
		{PA0,  TIM5, 1}, // TIM5_CH1
		{PA1,  TIM2, 2}, // TIM2_CH2
		{PA1,  TIM5, 2}, // TIM5_CH2
		{PA2,  TIM2, 3}, // TIM2_CH3
		{PA2,  TIM5, 3}, // TIM5_CH3
		{PA2,  TIM9, 1}, // TIM9_CH1
		{PA3,  TIM2, 4}, // TIM2_CH4
		{PA3,  TIM5, 4}, // TIM5_CH4
		{PA3,  TIM9, 2}, // TIM9_CH2
		{PA5,  TIM2, 1}, // TIM2_CH1
		{PA6,  TIM3, 1}, // TIM3_CH1
		{PA6,  TIM13, 1}, // TIM13_CH1
		{PA7,  TIM14, 1}, // TIM1_CH1N
		{PA7,  TIM3, 2}, // TIM3_CH2
		{PA8,  TIM1, 1}, // TIM1_CH1
		{PA9,  TIM1, 2}, // TIM1_CH2
		{PA10, TIM1, 3}, // TIM1_CH3
		{PA11, TIM1, 4}, // TIM1_CH4
		{PA15, TIM2, 1}, // TIM2_CH1


		{PB0,  TIM3, 3},  // TIM3_CH3
		{PB1,  TIM3, 4},  // TIM3_CH4
		{PB3,  TIM2, 2},  // TIM2_CH2
		{PB4,  TIM3, 1},  // TIM3_CH1
		{PB5,  TIM3, 2},  // TIM3_CH2
		{PB6,  TIM4, 1},  // TIM4_CH1
		{PB7,  TIM4, 2},  // TIM4_CH2
		{PB8,  TIM4, 3},  // TIM4_CH3
		{PB8,  TIM10,1},  // TIM10_CH1
		{PB9,  TIM4, 4},  // TIM4_CH4
		{PB9,  TIM11,1},  // TIM11_CH1
		{PB10, TIM2, 3},  // TIM2_CH3
		{PB11, TIM2, 4},  // TIM2_CH4

		{PC6,  TIM3, 1},  // TIM3_CH1
		{PC6,  TIM8, 1},  // TIM8_CH1
		{PC7,  TIM3, 2},  // TIM3_CH2
		{PC7,  TIM8, 2},  // TIM8_CH2
		{PC8,  TIM3, 3},  // TIM3_CH3
		{PC8,  TIM8, 3},  // TIM8_CH3
		{PC9,  TIM3, 4},  // TIM3_CH4
		{PC9,  TIM8, 4},  // TIM8_CH8

		{PD12,  TIM4, 1},  // TIM4_CH1
		{PD13,  TIM4, 2},  // TIM4_CH2
		{PD14,  TIM4, 3},  // TIM4_CH3
		{PD15,  TIM4, 4},  // TIM4_CH4

		{PE5,  TIM9, 1},  // TIM9_CH1
		{PE6,  TIM9, 1},  // TIM9_CH2
		{0,    0,    0}
};
/* Private function prototypes -----------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/

/* Function ------------------------------------------------------------------*/
/************************ (C) COPYRIGHT ROBOT++ *******************************/

// =====
// Note: Commented lines are alternative possibilities which are not used per default.
//       If you change them, you will have also to modify the corresponding xxx_api.c file
//       for pwmout, analogin, analogout, ...
// =====

//*** ADC ***
//
//const PinMap PinMap_ADC[] = {
//    {PA_0, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 0,  0)}, // ADC1_IN0
//    {PA_1, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 1,  0)}, // ADC1_IN1
//    {PA_2, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 2,  0)}, // ADC1_IN2
//    {PA_3, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 3,  0)}, // ADC1_IN3
//    {PA_4, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 4,  0)}, // ADC1_IN4
//    {PA_5, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 5,  0)}, // ADC1_IN5
//    {PA_6, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 6,  0)}, // ADC1_IN6
//    {PA_7, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 7,  0)}, // ADC1_IN7
//    {PB_0, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 8,  0)}, // ADC1_IN8
//    {PB_1, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 9,  0)}, // ADC1_IN9
//    {PC_0, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 10, 0)}, // ADC1_IN10
//    {PC_1, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 11, 0)}, // ADC1_IN11
//    {PC_2, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 12, 0)}, // ADC1_IN12
//    {PC_3, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 13, 0)}, // ADC1_IN13
//    {PC_4, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 14, 0)}, // ADC1_IN14
//    {PC_5, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 15, 0)}, // ADC1_IN15
//    {NC,   NC,    0}
//};
//
//const PinMap PinMap_ADC_Internal[] = {
//    {NC,   NC,    0}
//};
//
//
////*** DAC ***
//
//const PinMap PinMap_DAC[] = {
//    {PA_4, DAC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0xFF, 1, 0)}, // DAC_OUT1
//    {PA_5, DAC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0xFF, 2, 0)}, // DAC_OUT2
//    {NC,   NC,    0}
//};
//
//
////*** I2C ***
//
//const PinMap PinMap_I2C_SDA[] = {
//    {PB_11, I2C_2, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C2)},
//    {PF_0,  I2C_2, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C2)},
//    {PH_5,  I2C_2, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C2)},
//    {PB_7,  I2C_1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1)},
//    {PB_9,  I2C_1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1)},
//    {PC_9,  I2C_3, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C3)},
//    {PH_8,  I2C_3, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C3)},
//    {NC,    NC,    0}
//};
//
//const PinMap PinMap_I2C_SCL[] = {
//    {PA_8,  I2C_3, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C3)},
//    {PB_6,  I2C_1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1)},
//    {PB_8,  I2C_1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1)},
//    {PB_10, I2C_2, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C2)},
//    {PF_1,  I2C_2, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C2)},
//    {PH_4,  I2C_2, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C2)},
//    {NC,    NC,    0}
//};
//
////*** PWM ***
//
//// TIM5 cannot be used because already used by the us_ticker
//const PinMap PinMap_PWM[] = {
//    {PA_0,  PWM_2, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM2, 1, 0)}, // TIM2_CH1
////  {PA_0,  PWM_5, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM5, 1, 0)}, // TIM5_CH1
//    {PA_1,  PWM_2, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM2, 2, 0)}, // TIM2_CH2
////  {PA_1,  PWM_5, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM5, 2, 0)}, // TIM5_CH2
//    {PA_2,  PWM_2, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM2, 3, 0)}, // TIM2_CH3
////  {PA_2,  PWM_5, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM5, 3, 0)}, // TIM5_CH3
////  {PA_2,  PWM_9, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF3_TIM9, 1, 0)}, // TIM9_CH1
//    {PA_3,  PWM_2, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM2, 4, 0)}, // TIM2_CH4
////  {PA_3,  PWM_5, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM5, 4, 0)}, // TIM5_CH4
////  {PA_3,  PWM_9, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF3_TIM9, 2, 0)}, // TIM9_CH2
//    {PA_5,  PWM_2, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM2, 1, 0)}, // TIM2_CH1
//    {PA_6,  PWM_3, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM3, 1, 0)}, // TIM3_CH1
//    {PA_7,  PWM_1, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM1, 1, 1)}, // TIM1_CH1N
////  {PA_7,  PWM_3, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM3, 2, 0)}, // TIM3_CH2
//    {PA_8,  PWM_1, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM1, 3, 0)}, // TIM1_CH1
//    {PA_9,  PWM_1, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM1, 2, 0)}, // TIM1_CH2
//    {PA_10, PWM_1, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM1, 3, 0)}, // TIM1_CH3
//    {PA_11, PWM_1, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM1, 4, 0)}, // TIM1_CH4
//    {PA_15, PWM_2, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM2, 1, 0)}, // TIM2_CH1
//
//    {PB_0,  PWM_1, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM1, 2, 1)},  // TIM1_CH2N
////  {PB_0,  PWM_3, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM3, 3, 0)},  // TIM3_CH3
//    {PB_1,  PWM_1, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM1, 3, 1)},  // TIM1_CH3N
////  {PB_1,  PWM_3, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM3, 4, 0)},  // TIM3_CH4
//    {PB_3,  PWM_2, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM2, 2, 0)},  // TIM2_CH2
//    {PB_4,  PWM_3, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM3, 1, 0)},  // TIM3_CH1
//    {PB_5,  PWM_3, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM3, 2, 0)},  // TIM3_CH2
//    {PB_6,  PWM_4, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM4, 1, 0)},  // TIM4_CH1
//    {PB_7,  PWM_4, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM4, 2, 0)},  // TIM4_CH2
//    {PB_8,  PWM_4, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM4, 3, 0)},  // TIM4_CH3
////  {PB_8,  PWM_10,STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF3_TIM10,1, 0)},  // TIM10_CH1
//    {PB_9,  PWM_4, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM4, 4, 0)},  // TIM4_CH4
////  {PB_9,  PWM_11,STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF3_TIM11,1, 0)},  // TIM11_CH1
//    {PB_10, PWM_2, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM2, 3, 0)},  // TIM2_CH3
//    {PB_13, PWM_1, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM1, 1, 1)},  // TIM1_CH1N
//    {PB_14, PWM_1, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM1, 2, 1)},  // TIM1_CH2N
//    {PB_15, PWM_1, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM1, 3, 1)},  // TIM1_CH3N
//
//    {PC_6,  PWM_3, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM3, 1, 0)},  // TIM3_CH1
//    {PC_7,  PWM_3, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM3, 2, 0)},  // TIM3_CH2
//    {PC_8,  PWM_3, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM3, 3, 0)},  // TIM3_CH3
//    {PC_9,  PWM_3, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM3, 4, 0)},  // TIM3_CH4
//
//    {NC,    NC,    0}
//};
//
////*** SERIAL ***
//
//const PinMap PinMap_UART_TX[] = {
//    {PA_2,  UART_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2)},
//    {PA_9,  UART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1)},
//    {PA_11, UART_6, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_USART6)},
//    {PB_6,  UART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1)},
//    {PC_6,  UART_6, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_USART6)},
//    {NC,    NC,     0}
//};
//
//const PinMap PinMap_UART_RX[] = {
//    {PA_3,  UART_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2)},
//    {PA_10, UART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1)},
//    {PA_12, UART_6, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_USART6)},
//    {PB_7,  UART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1)},
//    {PC_7,  UART_6, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_USART6)},
//    {NC,    NC,     0}
//};
//
////*** SPI ***
//
//const PinMap PinMap_SPI_MOSI[] = {
//    {PA_7,  SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)},
//    {PB_5,  SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)},
////  {PB_5,  SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)},
//    {PB_15, SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)},
//    {PC_3,  SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)},
//    {PC_12, SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)},
//    {NC,    NC,    0}
//};
//
//const PinMap PinMap_SPI_MISO[] = {
//    {PA_6,  SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)},
//    {PB_4,  SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)},
////  {PB_4,  SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)},
//    {PB_14, SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)},
//    {PC_2,  SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)},
//    {PC_11, SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)},
//    {NC,    NC,    0}
//};
//
//const PinMap PinMap_SPI_SCLK[] = {
//    {PA_5,  SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)},
//    {PB_3,  SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)},
////  {PB_3,  SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)},
//    {PB_10, SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)},
//    {PB_13, SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)},
//    {PC_10, SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)},
//    {NC,    NC,    0}
//};
//
//const PinMap PinMap_SPI_SSEL[] = {
//    {PA_4,  SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF5_SPI1)},
////  {PA_4,  SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF6_SPI3)},
//    {PA_15, SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF5_SPI1)},
////  {PA_15, SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF6_SPI3)},
//    {PB_9,  SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF5_SPI2)},
//    {PB_12, SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF5_SPI2)},
//    {NC,    NC,    0}
//};
//const PinMap PinMap_PWM[] = {
//	{PA_0,  PWM_2, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM2, 1, 0)}, // TIM2_CH1
//	{PA_0,  PWM_5, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM5, 1, 0)}, // TIM5_CH1
//	{PA_1,  PWM_2, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM2, 2, 0)}, // TIM2_CH2
//	{PA_1,  PWM_5, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM5, 2, 0)}, // TIM5_CH2
//	{PA_2,  PWM_2, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM2, 3, 0)}, // TIM2_CH3
//	{PA_2,  PWM_5, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM5, 3, 0)}, // TIM5_CH3
//	{PA_2,  PWM_9, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF3_TIM9, 1, 0)}, // TIM9_CH1
//	{PA_3,  PWM_2, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM2, 4, 0)}, // TIM2_CH4
//	{PA_3,  PWM_5, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM5, 4, 0)}, // TIM5_CH4
//	{PA_3,  PWM_9, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF3_TIM9, 2, 0)}, // TIM9_CH2
//	{PA_5,  PWM_2, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM2, 1, 0)}, // TIM2_CH1
//	{PA_6,  PWM_3, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM3, 1, 0)}, // TIM3_CH1
//	{PA_7,  PWM_1, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM1, 1, 1)}, // TIM1_CH1N
//	{PA_7,  PWM_3, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM3, 2, 0)}, // TIM3_CH2
//	{PA_8,  PWM_1, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM1, 3, 0)}, // TIM1_CH1
//	{PA_9,  PWM_1, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM1, 2, 0)}, // TIM1_CH2
//	{PA_10, PWM_1, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM1, 3, 0)}, // TIM1_CH3
//	{PA_11, PWM_1, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM1, 4, 0)}, // TIM1_CH4
//	{PA_15, PWM_2, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM2, 1, 0)}, // TIM2_CH1
//
//	{PB_0,  PWM_1, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM1, 2, 1)},  // TIM1_CH2N
//	{PB_0,  PWM_3, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM3, 3, 0)},  // TIM3_CH3
//	{PB_1,  PWM_1, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM1, 3, 1)},  // TIM1_CH3N
//	{PB_1,  PWM_3, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM3, 4, 0)},  // TIM3_CH4
//	{PB_3,  PWM_2, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM2, 2, 0)},  // TIM2_CH2
//	{PB_4,  PWM_3, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM3, 1, 0)},  // TIM3_CH1
//	{PB_5,  PWM_3, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM3, 2, 0)},  // TIM3_CH2
//	{PB_6,  PWM_4, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM4, 1, 0)},  // TIM4_CH1
//	{PB_7,  PWM_4, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM4, 2, 0)},  // TIM4_CH2
//	{PB_8,  PWM_4, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM4, 3, 0)},  // TIM4_CH3
//	{PB_8,  PWM_10,STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF3_TIM10,1, 0)},  // TIM10_CH1
//	{PB_9,  PWM_4, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM4, 4, 0)},  // TIM4_CH4
//	{PB_9,  PWM_11,STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF3_TIM11,1, 0)},  // TIM11_CH1
//	{PB_10, PWM_2, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM2, 3, 0)},  // TIM2_CH3
//	{PB_13, PWM_1, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM1, 1, 1)},  // TIM1_CH1N
//	{PB_14, PWM_1, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM1, 2, 1)},  // TIM1_CH2N
//	{PB_15, PWM_1, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM1, 3, 1)},  // TIM1_CH3N
//
//	{PC_6,  PWM_3, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM3, 1, 0)},  // TIM3_CH1
//	{PC_7,  PWM_3, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM3, 2, 0)},  // TIM3_CH2
//	{PC_8,  PWM_3, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM3, 3, 0)},  // TIM3_CH3
//	{PC_9,  PWM_3, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF2_TIM3, 4, 0)},  // TIM3_CH4
//
//    {NC,    NC,    0}
//};
