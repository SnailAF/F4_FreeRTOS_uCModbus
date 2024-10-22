/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
//#include "arduino.h"
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "freertos.h"
#include "task.h"
#include "uNVIC.h"
#include "shGpio.h"
#include "BSP_PinNames.h"
#include "shcan.h"
#include "shPwm.h"
#include "shUsart.h"
#include "mb.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//任务优先级 堆栈大小 任务句柄 任务函数
/*  空闲任务  */
#define uIdle_TASK_PRIO		ufTask_uIdle_TASK_PRIO
#define uIdle_STK_SIZE 		2048
TaskHandle_t uIdleTask_Handler;
void uIdle_Task(void *pvParameters);

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void IdleApp_Great(void);
void get_date_time(char *dt);

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */
	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
 	HAL_Init();

	/* USER CODE BEGIN Init */
	SystemClock_Config();

	/* USER CODE END Init */
	/* USER CODE BEGIN SysInit */
	MB_Init(1000);
	/* USER CODE END SysInit */


	/* Initialize all configured peripherals */
	//创建空闲任务
	IdleApp_Great();
	//mbTask_Great_RWctrl(4, serial_RS4852, 115200,RS4852_RW,PIN_HIGH);
	vTaskStartScheduler();
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/* USER CODE BEGIN 4 */

inline void IdleApp_Great(void) {
	xTaskCreate((TaskFunction_t) uIdle_Task, (const char*) "uIdle_task",
			(uint16_t) uIdle_STK_SIZE, (void*) NULL,
			(UBaseType_t) uIdle_TASK_PRIO, (TaskHandle_t*) &uIdleTask_Handler);
}




/*********************************************************************************
 * FUNCTION NAME ：空闲任务
 * CREATE DATE 	：
 * CREATED BY 	：
 * FUNCTION 	：
 * MODIFY DATE 	：
 * INPUT 		：无
 * OUTPUT 		：无
 * RETURN 		：无
 *********************************************************************************/
#include "mb.h"

MODBUS_CH  *MB_CfgCh (CPU_INT08U  node_addr,
                      CPU_INT08U  master_slave,
                      CPU_INT32U  rx_timeout,
                      CPU_INT08U  modbus_mode,
                      CPU_INT08U  port_nbr,
                      CPU_INT32U  baud,
                      CPU_INT08U  bits,
                      CPU_INT08U  parity,
                      CPU_INT08U  stops,
                      CPU_INT08U  wr_en);
void uIdle_Task(void *pvParameters) {
	//uint32_t time;

	MODBUS_CH *pch;
	MODBUS_CH *pch_232;
	MB_CfgCh(   1,        		// ... Modbus Node # for this slave channel
	         	 	 MODBUS_SLAVE,      // ... This is a SLAVE
					 0,              	// ... 0 when a slave
					 MODBUS_MODE_RTU,   // ... Modbus Mode (_ASCII or _RTU)
					 serial_RS4851,              	// ... Specify UART #1
					 9600,              // ... Baud Rate
					 8,              	// ... Number of data bits 7 or 8
					 MODBUS_PARITY_NONE,// ... Parity: _NONE, _ODD or _EVEN   1,
					 1,            		// ... Number of stop bits 1 or 2
					 MODBUS_WR_EN);     // ... Enable (_EN) or disable (_DIS) writes
#if 1
	pch = MB_CfgCh(   1,         // ... Modbus Node # for this slave channel
	         MODBUS_MASTER,     // ... This is a MASTER
	         100,  // ... One second timeout waiting for slave response
			 MODBUS_MODE_RTU,   // ... Modbus Mode (_ASCII or _RTU)
			 serial_UART,              // ... Specify UART #3
	        19200,              // ... Baud Rate
	            8,              // ... Number of data bits 7 or 8
	         MODBUS_PARITY_NONE,// ... Parity: _NONE, _ODD or _EVEN
	            1,              // ... Number of stop bits 1 or 2
	         MODBUS_WR_EN);     // ... Enable (_EN) or disable (_DIS) writes


	pch_232 = MB_CfgCh(   1,         // ... Modbus Node # for this slave channel
		         MODBUS_MASTER,     // ... This is a MASTER
		         100,  // ... One second timeout waiting for slave response
				 MODBUS_MODE_RTU,   // ... Modbus Mode (_ASCII or _RTU)
				 serial_RS232,              // ... Specify UART #3
		        19200,              // ... Baud Rate
		            8,              // ... Number of data bits 7 or 8
		         MODBUS_PARITY_NONE,// ... Parity: _NONE, _ODD or _EVEN
		            1,              // ... Number of stop bits 1 or 2
		         MODBUS_WR_EN);     // ... Enable (_EN) or disable (_DIS) writes
	#endif
	uint16_t buf[10];
	uint16_t buf_w[10];
	while (1) {

		MBM_FC03_HoldingRegRd(pch,1,40000,buf,10);
		for(int i = 0; i< 10; i++){
			buf_w[i] = buf[i];
		}
		//vTaskDelay(1000);
		//vTaskDelay(1000);
		MBM_FC16_HoldingRegWrN(pch_232, 1, 40010, buf_w, 10);
		//vTaskDelay(1000);
		vTaskDelay(10);
	}
}
/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}
/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	log_a(" ");
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
/**
* @brief 返回编译日期时间格式 2308161122
* @note
* @param dt:返回字符串的指针
* @retval None
* @log
*/
void get_date_time(char *dt)
{
    // 获取编译日期和时间的字符串
    const char* compileDate = __DATE__;
    const char* compileTime = __TIME__;

    // 将月份映射到数字
    int month = 0;
    char monthStr[4];
    strncpy(monthStr, compileDate, 3);
    monthStr[3] = '\0';
    if (strstr("JanFebMarAprMayJunJulAugSepOctNovDec", monthStr) != NULL) {
        month = (strstr("JanFebMarAprMayJunJulAugSepOctNovDec", monthStr) - "JanFebMarAprMayJunJulAugSepOctNovDec") / 3 + 1;
    }

    // 解析时间字符串，提取小时和分钟
    int hour, minute;
    sscanf(compileTime, "%d:%d", &hour, &minute);

    // 获取年份的后两位
    int yearLastTwoDigits = (int)(compileDate[9] - '0') * 10 + (int)(compileDate[10] - '0');

    // 输出编译时间和日期的格式化字符串
    sprintf(dt,"%02d%02d%02d%02d%02d", yearLastTwoDigits, month, atoi(compileDate + 4), hour, minute);

}
