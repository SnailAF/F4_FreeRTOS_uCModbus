/**
  ******************************************************************************
  * @file    shWacthdog.h
  * @author  SH0069
  * @version
  * @date    2023年10月17日下午4:42:23
  * @brief	参考mbedos watchdog_api.c
  * 		主要用到2个函数
  * 		1. 初始化函数watchdog_init();
  * 		2. 喂狗函数 watchdog_kick();
  * 		ex:
  * 		int main(void)
  * 		{
  * 			// hal_init();
  * 			watchdog_init(1000);	// 1s看门狗
  * 			while(1)
  * 			{
  * 				watchdog_kick();
  * 				delay_ms(10);
  * 			}
  * 		}
  *
  ******************************************************************************
  * @attention
  *
  *
  *
  ******************************************************************************
  */
/**
 * \defgroup watchdog Watchdog HAL API
 * Low-level interface to the Independent Watchdog Timer of a target.
 *
 * This module provides platform independent access to the system watchdog timer
 * which is an embedded peripheral that will reset the system in the case of
 * system failures or malfunctions.
 *
 * The watchdog timer initializes a system timer with a time period specified in
 * the configuration. This timer counts down and triggers a system reset when it
 * wraps. To prevent the system reset the timer must be continually
 * kicked/refreshed by calling ::watchdog_kick which will reset the countdown
 * to the user specified reset value.
 *
 * # Defined behavior
 * * Sleep and debug modes don't stop the watchdog timer from counting down.
 * * The function ::watchdog_init is safe to call repeatedly. The
 * function's implementation must not do anything if ::watchdog_init has
 * already initialized the hardware watchdog timer.
 * * Maximum supported timeout is `UINT32_MAX` milliseconds; minimum timeout
 * is 1 millisecond.
 * * The uncalibrated watchdog should trigger at or after the timeout value
 * multiplied by the frequency accuracy ratio of its oscillator (typical_frequency / max_frequency).
 * * The calibrated watchdog should trigger at or after the timeout value.
 * * The watchdog should trigger before twice the timeout value.
 *
 * # Undefined behavior
 * * Calling any function other than ::watchdog_init or
 * ::watchdog_get_platform_features before you have initialized the watchdog.
 *
 * # Notes
 * * A software reset may not stop the watchdog timer; the behavior is platform specific.
 *
 * @{
 */

#ifndef SHLIB_F4_INC_SHWATCHDOG_H_
#define SHLIB_F4_INC_SHWATCHDOG_H_


/* Includes ------------------------------------------------------------------*/
#include "mcu_type.h"
#include <stdbool.h>
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/** Status of a watchdog operation.
*/
typedef enum {
    WATCHDOG_STATUS_OK,                 /**< Operation successful. **/
    WATCHDOG_STATUS_NOT_SUPPORTED,      /**< Operation not supported. **/
    WATCHDOG_STATUS_INVALID_ARGUMENT    /**< Invalid argument. **/
} watchdog_status_t;
/** Watchdog features.
 */
typedef struct {
    /**
     * Maximum timeout value for the watchdog in milliseconds.
     */
    uint32_t max_timeout;
    /**
     * You can update the watchdog configuration after the watchdog has started.
     */
    bool update_config;
    /**
     * You can stop the watchdog after it starts without a reset.
     */
    bool disable_watchdog;
    /**
     * Typical frequency of not calibrated watchdog clock in Hz.
     */
    uint32_t clock_typical_frequency;
    /**
     * Maximum frequency of not calibrated watchdog clock in Hz.
     */
    uint32_t clock_max_frequency;
} watchdog_features_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/** Initialize and start a watchdog timer with the given configuration.
 *
 * If the watchdog timer is configured and starts successfully, this
 * function returns ::WATCHDOG_STATUS_OK.
 *
 * If the timeout specified is outside the range supported by the platform,
 * it returns ::WATCHDOG_STATUS_INVALID_ARGUMENT.
 *
 * @param[in]  config   Configuration settings for the watchdog timer
 *
 * @return ::WATCHDOG_STATUS_OK if the watchdog is configured correctly and
 *         has started. Otherwise a status indicating the fault.
 */
int32_t watchdog_init(uint32_t timeout_ms);

/** Refreshes the watchdog timer.
 *
 * Call this function periodically before the watchdog times out.
 * Otherwise, the system resets.
 *
 * If a watchdog is not running, this function does nothing.
 */
void watchdog_kick(void);


/** Get the watchdog timer refresh value.
 *
 * This function returns the configured refresh timeout of the watchdog timer.
 *
 * @return Reload value for the watchdog timer in milliseconds.
 */
uint32_t watchdog_get_reload_value(void);

/** Get information on the current platforms supported watchdog functionality.
 *
 * @return watchdog_feature_t indicating supported watchdog features on the
 *         current platform
 */
watchdog_features_t watchdog_get_platform_features(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* SHLIB_F4_INC_SHWATCHDOG_H_ */
