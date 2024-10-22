/**
  ******************************************************************************
  * @file    shUtils.h
  * @author  SH0069
  * @version
  * @date    2023年10月31日下午3:02:29
  * @brief  增加断言
  ******************************************************************************
  * @attention
  *
  *
  *
  ******************************************************************************
  */
#ifndef _SHUTILS_H_
#define _SHUTILS_H_


/* Includes ------------------------------------------------------------------*/
#include "SH_config.h"
/* Exported types ------------------------------------------------------------*/
#define SH_ASSERT_ENABLE 1
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#if SH_ASSERT_ENABLE
#include "stdio.h"
#define SH_ASSERT(EXPR)                                                 \
if (!(EXPR)){                                                           \
	log_a( "has assert failed at %s:%d.", __FILE__, __LINE__); \
	while (1);}
#else
    #define MB_ASSERT(EXPR)                    ((void)0);
#endif

#endif /* SHLIB_F4_INC_SHUTILS_H_ */
