#ifndef __MCU_TYPE
#define __MCU_TYPE

#define F_CPU                   SystemCoreClock
#define CYCLES_PER_MICROSECOND (F_CPU / 1000000U)

/* Includes ------------------------------------------------------------------*/
#include "SH_config.h"
#include "cmsis_ref.h"
#include "ll_includes.h"

#if defined(STM32F0)
#include "stm32f0xx_hal_conf.h"

#elif defined(STM32F1)
#include "stm32f1xx_hal_conf.h"

#elif defined(STM32F2)
#include "stm32f2xx_hal_conf.h"

#elif defined(STM32F3)
#include "stm32f3xx_hal_conf.h"

#elif defined(STM32F4)
#include "stm32f4xx_hal_conf.h"

#elif defined(STM32F7)
#include "stm32f7xx_hal_conf.h"

#elif defined(STM32L0)
#include "stm32l0xx_hal_conf.h"

#elif defined(STM32L1)
#include "stm32l1xx_hal_conf.h"

#elif defined(STM32L4)
#include "stm32l4xx_hal_conf.h"

#endif /* STM32F0 */
#endif
