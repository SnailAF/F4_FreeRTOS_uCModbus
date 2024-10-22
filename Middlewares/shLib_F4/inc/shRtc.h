/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 * 高频函数
 * 1.rtc_init() 		//初始化函数，调用其他函数前首先调用。
 * 2.rtc_read()			//读取UTC时间
 * 3.rtc_readBJtime()	//读取北京时间UTC+8
 * 4.rtc_write()		//设置RTC时间
 * 5.rtc_get_strtime()	//获取时间字符串 格式为2022-12-10 01:29:51
 *
 * void rtc_test(void){
	static time_t time0;
	time_t buf;
	static uint8_t loop = 0;
	if(loop == 0){
		rtc_init();						// 上电初始化函数
		if(rtc_read() < 0){
			buf = 1670577766; 			// 写入UTC时间
			rtc_write(buf);
		}
		loop = 1;
	}else{
		buf  = rtc_readBJtime();		// 读取北京时间，UTC+8
		if(time0 != buf){
			printf("%lld \n",buf);		//cubeide 注意开启int64支持，
			time0 = buf;
			log_d("%s\n",ctime(&time0));
		}
	}
}
 * */
/** \addtogroup hal */
/** @{*/

#ifndef SH_RTC_API_H
#define SH_RTC_API_H

//#include "device.h"
#include "mcu_type.h"
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 配置项 开始 */
#define SH_CONF_TARGET_LSE_AVAILABLE 1
#define SH_CONF_TARGET_RTC_CLOCK_SOURCE  USE_RTC_CLK_LSE_OR_LSI
#ifndef TARGET_STM32F4
#define TARGET_STM32F4 1
#endif
/* 配置项 结束 */

/**
 * \defgroup hal_rtc RTC hal
 *
 * The RTC hal provides a low level interface to the Real Time Counter (RTC) of a
 * target.
 *
 * # Defined behaviour
 * * The function ::rtc_init is safe to call repeatedly - Verified by test ::rtc_init_test.
 * * RTC accuracy is at least 10% - Verified by test ::rtc_accuracy_test.
 * * Init/free doesn't stop RTC from counting - Verified by test ::rtc_persist_test.
 * * Software reset doesn't stop RTC from counting - Verified by test ::rtc_reset_test.
 * * Sleep modes don't stop RTC from counting - Verified by test ::rtc_sleep_test.
 * * Shutdown mode doesn't stop RTC from counting - Not verified.
 * * The functions ::rtc_write/::rtc_read provides availability to set/get RTC time
 * - Verified by test ::rtc_write_read_test.
 * * The functions ::rtc_isenabled returns 1 if the RTC is counting and the time has been set,
 * 0 otherwise - Verified by test ::rtc_enabled_test.
 *
 * # Undefined behaviour
 * * Calling any function other than ::rtc_init before the initialisation of the RTC
 *
 * # Potential bugs
 * * Incorrect overflow handling - Verified by ::rtc_range_test
 * * Glitches due to ripple counter - Verified by ::rtc_glitch_test
 *
 * @see hal_rtc_tests
 *
 * @{
 */

/**
 * \defgroup hal_rtc_tests RTC hal tests
 * The RTC test validate proper implementation of the RTC hal.
 *
 * To run the RTC hal tests use the command:
 *
 *     mbed test -t <toolchain> -m <target> -n tests-mbed_hal-rtc*
 */


/** Initialize the RTC peripheral
 *
 * Powerup the RTC in perpetration for access. This function must be called
 * before any other RTC functions ares called. This does not change the state
 * of the RTC. It just enables access to it.
 *
 * @note This function is safe to call repeatedly - Tested by ::rtc_init_test
 *
 * Example Implementation Pseudo Code:
 * @code
 * void rtc_init()
 * {
 *     // Enable clock gate so processor can read RTC registers
 *     POWER_CTRL |= POWER_CTRL_RTC_Msk;
 *
 *     // See if the RTC is already setup
 *     if (!(RTC_STATUS & RTC_STATUS_COUNTING_Msk)) {
 *
 *         // Setup the RTC clock source
 *         RTC_CTRL |= RTC_CTRL_CLK32_Msk;
 *     }
 * }
 * @endcode
 */
void rtc_init(void);

/** Deinitialize RTC
 *
 * Powerdown the RTC in preparation for sleep, powerdown or reset. That should only
 * affect the CPU domain and not the time keeping logic.
 * After this function is called no other RTC functions should be called
 * except for ::rtc_init.
 *
 * @note This function does not stop the RTC from counting - Tested by ::rtc_persist_test
 *
 * Example Implementation Pseudo Code:
 * @code
 * void rtc_free()
 * {
 *     // Disable clock gate since processor no longer needs to read RTC registers
 *     POWER_CTRL &= ~POWER_CTRL_RTC_Msk;
 * }
 * @endcode
 */
void rtc_free(void);

/** Check if the RTC has the time set and is counting
 *
 * @retval 0 The time reported by the RTC is not valid
 * @retval 1 The time has been set the RTC is counting
 *
 * Example Implementation Pseudo Code:
 * @code
 * int rtc_isenabled()
 * {
 *     if (RTC_STATUS & RTC_STATUS_COUNTING_Msk) {
 *         return 1;
 *     } else {
 *         return 0;
 *     }
 * }
 * @endcode
 */
int rtc_isenabled(void);

/** Get the current time from the RTC peripheral
 *
 * @return The current time in seconds
 *
 * @note Some RTCs are not synchronized with the main clock. If
 * this is the case with your RTC then you must read the RTC time
 * in a loop to prevent reading the wrong time due to a glitch.
 * The test ::rtc_glitch_test is intended to catch this bug.
 *
 * Example implementation for an unsynchronized ripple counter:
 * @code
 * time_t rtc_read()
 * {
 *     uint32_t val;
 *     uint32_t last_val;
 *
 *     // Loop until the same value is read twice
 *     val = RTC_SECONDS;
 *     do {
 *         last_val = val;
 *         val = RTC_SECONDS;
 *     } while (last_val != val);
 *
 *     return (time_t)val;
 * }
 * @endcode
 */
time_t rtc_read(void);

/** Write the current time in seconds to the RTC peripheral
 *
 * @param t The current time to be set in seconds.
 *
 * Example Implementation Pseudo Code:
 * @code
 * void rtc_write(time_t t)
 * {
 *     RTC_SECONDS = t;
 * }
 * @endcode
 */
void rtc_write(time_t t);

/**@}*/


/* rtc_api_hal.h *************************************************************************/

// Possible choices of the RTC_CLOCK_SOURCE configuration set in json file
#define USE_RTC_CLK_LSE_OR_LSI 1
#define USE_RTC_CLK_LSI 2
#define USE_RTC_CLK_HSE 3

#if !((SH_CONF_TARGET_RTC_CLOCK_SOURCE == USE_RTC_CLK_LSE_OR_LSI) || (SH_CONF_TARGET_RTC_CLOCK_SOURCE == USE_RTC_CLK_LSI) || (SH_CONF_TARGET_RTC_CLOCK_SOURCE == USE_RTC_CLK_HSE))
#error "RTC clock configuration is invalid!"
#endif

#if (SH_CONF_TARGET_RTC_CLOCK_SOURCE == USE_RTC_CLK_HSE) && !(TARGET_STM32F2 || TARGET_STM32F4 || TARGET_STM32F7 || TARGET_STM32F1)
#error "RTC from HSE not supported for this target"
#endif

#if (SH_CONF_TARGET_RTC_CLOCK_SOURCE == USE_RTC_CLK_HSE)
#define RTC_CLOCK 1000000U
#define RTC_HSE_DIV (HSE_VALUE / RTC_CLOCK)
#if RTC_HSE_DIV > 31
#error "HSE value too high for RTC"
#endif
#elif (SH_CONF_TARGET_RTC_CLOCK_SOURCE == USE_RTC_CLK_LSE_OR_LSI) && SH_CONF_TARGET_LSE_AVAILABLE
#define RTC_CLOCK LSE_VALUE
#else
#define RTC_CLOCK LSI_VALUE
#endif

#if DEVICE_LPTICKER && !SH_CONF_TARGET_LPTICKER_LPTIM

#if (SH_CONF_TARGET_RTC_CLOCK_SOURCE == USE_RTC_CLK_HSE)
#error "LPTICKER is not available with HSE as RTC clock source and should be removed from the target configuration."
#endif

/* PREDIV_A : 7-bit asynchronous prescaler */
/* PREDIV_A is set to set LPTICKER frequency to RTC_CLOCK/4 */
#define PREDIV_A_VALUE 3

/** Read RTC counter with sub second precision
 *
 * @return LP ticker counter
 */
uint32_t rtc_read_lp(void);

/** Program a wake up timer event
 *
 * @param timestamp: counter to set
 */
void rtc_set_wake_up_timer(timestamp_t timestamp);

/** Call RTC Wake Up IT
 */
void rtc_fire_interrupt(void);

/** Disable the wake up timer event.
 *
 * The wake up timer use auto reload, you have to deactivate it manually.
 */
void rtc_deactivate_wake_up_timer(void);

#else /* DEVICE_LPTICKER && !SH_CONF_TARGET_LPTICKER_LPTIM */

/* PREDIV_A : 7-bit asynchronous prescaler */
/* PREDIV_A is set to the maximum value to improve the consumption */
#define PREDIV_A_VALUE 127

#endif /* DEVICE_LPTICKER && !SH_CONF_TARGET_LPTICKER_LPTIM */

/* PREDIV_S : 15-bit synchronous prescaler */
/* PREDIV_S is set in order to get a 1 Hz clock */
#define PREDIV_S_VALUE (RTC_CLOCK / (PREDIV_A_VALUE + 1) - 1)

/** Synchronise the RTC shadow registers.
 *
 * Must be called after a deepsleep.
 */
void rtc_synchronize(void);


/**
  * @brief 标准时间+8h，转为北京时间，
  * @note 为了和外部统一，rtc使用标准时间（UTC）作为时间戳，
  * @param  无
  * @retval 标准时间+8h后的时间戳。
  */
time_t rtc_readBJtime(void);
/**
  * @brief 返回北京时间的字符串 格式为2022-12-10 01:29:51
  * @note UTC + 8h
  * @param  s:字符串指针
  * @retval 实际大小
  */
size_t rtc_get_strtime(char *s,size_t maxsize);
#ifdef __cplusplus
}
#endif

#endif

/** @}*/
