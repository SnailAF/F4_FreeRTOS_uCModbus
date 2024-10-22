/*
 * MIT License
 * Copyright (c) 2019 _VIFEXTech
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "shGpio.h"

#include "shAnalog.h"
#include "shPwm.h"

/* Private macro -------------------------------------------------------------*/
#define GPIO_HIGH(GPIOx,GPIO_Pin_x)    HAL_GPIO_WritePin( GPIOx, GPIO_Pin_x, GPIO_PIN_SET )
#define GPIO_LOW(GPIOx,GPIO_Pin_x)     HAL_GPIO_WritePin( GPIOx, GPIO_Pin_x, GPIO_PIN_RESET )
#define GPIO_READ(GPIOx,GPIO_Pin_x)    (((GPIOx)->IDR   & (GPIO_Pin_x))!=0)
#define GPIO_TOGGLE(GPIOx,GPIO_Pin_x)   ((GPIOx)->ODR  ^= (GPIO_Pin_x))

#define digitalWrite_HIGH(Pin) (GPIO_HIGH  (PIN_MAP[Pin].GPIOx,PIN_MAP[Pin].GPIO_Pin_x))
#define digitalWrite_LOW(Pin)  (GPIO_LOW   (PIN_MAP[Pin].GPIOx,PIN_MAP[Pin].GPIO_Pin_x))
#define digitalRead_FAST(Pin)  (GPIO_READ  (PIN_MAP[Pin].GPIOx,PIN_MAP[Pin].GPIO_Pin_x))
#define togglePin(Pin)         (GPIO_TOGGLE(PIN_MAP[Pin].GPIOx,PIN_MAP[Pin].GPIO_Pin_x))

/* public variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void GPIOx_Init( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x, pinMode_TypeDef pinMode_x, uint32_t GPIO_Speed_x );
uint8_t GPIO_GetPortNum( PinName Pin );
uint8_t GPIO_GetPinSource( uint16_t GPIO_Pin_x );
uint8_t GPIO_GetPinNum( PinName Pin );

/* public function prototypes ------------------------------------------------*/
void  GPIO_RCC_ENABLE( GPIO_TypeDef* port );
void pinMode(PinName Pin, pinMode_TypeDef pinMode_x);
void digitalWrite(PinName Pin, uint8_t val);
uint8_t digitalRead(PinName Pin);
void digitalToggle(PinName Pin);

/* ----------------------- function ------------------------------------------*/

void  GPIO_RCC_ENABLE( GPIO_TypeDef* port )
{
    
    if( port == GPIOA )
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }
    else if( port == GPIOB )
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }
    else if( port == GPIOC )
    {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }
    else if( port == GPIOD )
    {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    }
    else if( port == GPIOE )
    {
        __HAL_RCC_GPIOE_CLK_ENABLE();
    }
    else if( port == GPIOF )
    {
        __HAL_RCC_GPIOF_CLK_ENABLE();
    }
    else if( port == GPIOG )
    {
        __HAL_RCC_GPIOG_CLK_ENABLE();
    }
    else if( port == GPIOH )
    {
        __HAL_RCC_GPIOH_CLK_ENABLE();
    }
    else if( port == GPIOI )
    {
        __HAL_RCC_GPIOI_CLK_ENABLE();
    }
}


/**
  * @brief  GPIO初始化
  * @param  GPIOx: GPIO地址
  * @param  GPIO_Pin_x: GPIO对应位
  * @param  GPIO_Mode_x: GPIO模式
  * @param  GPIO_Speed_x: GPIO速度
  * @retval 无
  */
void GPIOx_Init( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x, pinMode_TypeDef pinMode_x, uint32_t GPIO_Speed_x )
{
    uint32_t GPIO_Mode_x;
    uint32_t GPIO_PuPd_x;

    GPIO_RCC_ENABLE( GPIOx );

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if( pinMode_x == INPUT )
    {
        GPIO_Mode_x  = GPIO_MODE_INPUT;
        GPIO_PuPd_x  = GPIO_NOPULL;
    }
    else if( pinMode_x == INPUT_PULLUP )
    {
        GPIO_Mode_x  = GPIO_MODE_INPUT;
        GPIO_PuPd_x  = GPIO_PULLUP;
    }
    else if( pinMode_x == INPUT_PULLDOWN )
    {
        GPIO_Mode_x  = GPIO_MODE_INPUT;
        GPIO_PuPd_x  = GPIO_PULLDOWN;
    }
    else if( pinMode_x == INPUT_ANALOG )
    {
        GPIO_Mode_x  = GPIO_MODE_ANALOG;
        GPIO_PuPd_x  = GPIO_NOPULL;
    }
    else if( pinMode_x == OUTPUT )
    {
        GPIO_Mode_x  = GPIO_MODE_OUTPUT_PP;
        GPIO_PuPd_x  = GPIO_NOPULL;
    }
    else if( pinMode_x == OUTPUT_OPEN_DRAIN )
    {
        GPIO_Mode_x  = GPIO_MODE_OUTPUT_OD;
        GPIO_PuPd_x  = GPIO_NOPULL;
    }
    else if( pinMode_x == OUTPUT_AF )
    {
        GPIO_Mode_x  = GPIO_MODE_AF_PP;
        GPIO_PuPd_x  = GPIO_NOPULL;
    }
    else
    {
        return;
    }

    GPIO_InitStruct.Pin = GPIO_Pin_x;
    GPIO_InitStruct.Mode = GPIO_Mode_x;
    GPIO_InitStruct.Pull = GPIO_PuPd_x;
    GPIO_InitStruct.Speed = GPIO_Speed_x;
    HAL_GPIO_Init( GPIOx, &GPIO_InitStruct );
}

/**
  * @brief  获取当前引脚对应的GPIOx编号
  * @param  Pin: 引脚编号
  * @retval 无
  */
uint8_t GPIO_GetPortNum( PinName Pin )
{
    if( PIN_MAP[Pin].GPIOx == GPIOA )return 0;
    else if( PIN_MAP[Pin].GPIOx == GPIOB )return 1;
    else if( PIN_MAP[Pin].GPIOx == GPIOC )return 2;
    else if( PIN_MAP[Pin].GPIOx == GPIOD )return 3;
    else if( PIN_MAP[Pin].GPIOx == GPIOE )return 4;
    else if( PIN_MAP[Pin].GPIOx == GPIOF )return 5;
    else if( PIN_MAP[Pin].GPIOx == GPIOG )return 6;
    else if( PIN_MAP[Pin].GPIOx == GPIOH )return 7;
    else if( PIN_MAP[Pin].GPIOx == GPIOI )return 8;
    else return 0xFF;
}

/**
  * @brief  获取当前引脚对应的 PinSource
  * @param  GPIO_Pin_x: GPIO对应位
  * @retval 无
  */
uint8_t GPIO_GetPinSource( uint16_t GPIO_Pin_x )
{
    uint16_t PinSource = 0;
    while( GPIO_Pin_x > 1 )
    {
        GPIO_Pin_x >>= 1;
        PinSource++;
    }
    return PinSource;
}

/**
  * @brief  获取当前引脚对应的编号
  * @param  Pin: 引脚编号
  * @retval 无
  */
uint8_t GPIO_GetPinNum( PinName Pin )
{
    return GPIO_GetPinSource( PIN_MAP[Pin].GPIO_Pin_x );
}

/**
  * @brief  配置引脚输入输出模式
  * @param  Pin: 引脚编号
  * @param  pinMode_x: 模式
  * @retval 无
  */
void pinMode(PinName Pin, pinMode_TypeDef pinMode_x)
{
    if(!IS_PIN(Pin))
        return;

    if(pinMode_x == INPUT_ANALOG_DMA)
    {
        if(!IS_ADC_PIN(Pin))
            return;

        pinMode(Pin, INPUT_ANALOG);
        ADC_DMA_Register(PIN_MAP[Pin].ADC_Channel);
    }
    else if(pinMode_x == PWM)
    {
        pwm_start(Pin, 1000, 10000);
    }
    else
    {
        GPIOx_Init(
            PIN_MAP[Pin].GPIOx,
            PIN_MAP[Pin].GPIO_Pin_x,
            pinMode_x,
			GPIO_SPEED_FREQ_VERY_HIGH
        );
    }
}


// Enable GPIO clock and return GPIO base address
GPIO_TypeDef *Get_GPIO_Port_Base_Add(PinName Pin)
{
	uint8_t port_idx = Pin >> 8 & 0x0F;
    uint32_t gpio_add = 0;
    switch (port_idx) {
        case 0:
            gpio_add = GPIOA_BASE;
            break;
        case 1:
            gpio_add = GPIOB_BASE;
            break;
        case 2:
            gpio_add = GPIOC_BASE;
            break;
        case 4:
            gpio_add = GPIOD_BASE;
            break;
        case 5:
            gpio_add = GPIOE_BASE;
            break;
        case 6:
            gpio_add = GPIOF_BASE;
            break;
        default:
           // error("Pinmap error: wrong port number.");
            break;
    }
    return (GPIO_TypeDef *) gpio_add;
}
void digitalWrite(PinName Pin, GPIO_State_Type val)
{
    uint32_t GPIO_Pin_x = Pin & 0x0F;;
    GPIO_TypeDef* GPIOx = Get_GPIO_Port_Base_Add(Pin);
    if(val)
    	HAL_GPIO_WritePin( GPIOx, GPIO_Pin_x, GPIO_PIN_SET );
    else
    	HAL_GPIO_WritePin( GPIOx, GPIO_Pin_x, GPIO_PIN_RESET );
}
uint8_t digitalRead(PinName Pin)
{
    uint32_t GPIO_Pin_x = Pin & 0x0F;;
    GPIO_TypeDef* GPIOx = Get_GPIO_Port_Base_Add(Pin);
    return HAL_GPIO_ReadPin(GPIOx,GPIO_Pin_x);
}
