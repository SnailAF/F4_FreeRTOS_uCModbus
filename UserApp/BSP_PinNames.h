/**
  ******************************************************************************
  * @file    BSP_PinNames.h
  * @author	 ZGQ
  * @version
  * @date    2022年6月7日 下午12:44:16
  * @brief   
  ******************************************************************************
  * @attention
  *	
  ******************************************************************************
  */

#ifndef BSP_PINNAMES_H_
#define BSP_PINNAMES_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "shGpio.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/*串口,CAN,引脚定义*/
#define LED1	PE3
#define LED2	PE4
#define LED3	PC13
#define AI1		PC0
#define AI2		PC1
#define AI3		PC4
#define AI4		PC5
#define AI_VOL	 PB0	/* 电压读取引脚 */

#define IO1_RW	PE10
#define IO2_RW	PA4
#define IO3_RW	PD7

#define DI1		PA0		/* T5-1 */
#define DI2		PA1		/* T5-2 */
#define DI3		PE9		/* T1-1 */
#define DI4		PE11	/* T1-2 */
#define DI5		PA2		/* T5-3 */
#define DI6		PA3		/* T5-4 */
#define DI7		PA6		/* T13-1 */
#define DI8		PA7		/* T14-1 */
#define DI9		PD12	/* T4-1 */
#define DI10	PD13	/* T4-2 */
#define DI11	PD14	/* T4-3 */
#define DI12	PD15	/* T4-4 */

#define DO1		PA0
#define DO2		PA1
#define DO3		PE9
#define DO4		PE11
#define DO5		PA2
#define DO6		PA3
#define DO7		PA6
#define DO8		PA7
#define DO9		PD12
#define DO10	PD13
#define DO11	PD14
#define DO12	PD15
/*PMOS输出*/
#define PDO1	PB14	/* T12-1 */
#define PDO2	PB15	/* T12-2 */
#define PDO3	PE5		/* T9-1 */
#define PDO4	PE6		/* T9-2 */
/*CAN 引脚定义*/
#define CAN_RD	PD0
#define CAN_TD	PD1
/*485 引脚定义*/
#define RS4851_RW		PD4
#define RS4852_RW		PD10

/*通信 定义*/
#define BSP_CAN 		CAN_1
#define serial_RS4851	serial2
#define serial_RS4852	serial3
#define serial_UART		serial1
#define serial_RS232	serial4

/* IO输入输出控制定义*/
#define IO_INPUT 	PIN_LOW
#define IO_OUTPUT 	PIN_HIGH

/*初始化并设置状态，在应用中更改，应该使用digitalWrite */
#define IO_CTRL(PIN,STATE)	do{pinMode(PIN,OUTPUT);digitalWrite(PIN,STATE);}while(0)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 二次定义
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define UL_SQ_PIN		DI9
#define UR_SQ_PIN		DI10
#define DL_SQ_PIN		DI11
#define DR_SQ_PIN		DI12
#define EMERGENCY_PIN	PE8
#define BRAKE_EN_PIN	PB1

#define KEY_R_PIN		DO1
#define KEY_L_PIN		DO2
#define KEY_D_PIN		DO3
#define KEY_U_PIN		DO4
#define KEY_A_PIN		DO5

#define SIGNAL_LAMP_PIN		PDO1

#define AC_TRANSDUCER_PIN		AI1 /* 交流互感器 */
#define ROBOT_LED	PDO1
/* Exported variables --------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif  /* BSP_PINNAMES_H_ */

/************************ (C) COPYRIGHT ROBOT++ *******************************/
