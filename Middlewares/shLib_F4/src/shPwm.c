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
#include "shPwm.h"
#include "shGpio.h"
#include "shTimer.h"

/**
  * @brief  定时器输出捕获初始化
  * @param  TIMx: 定时器地址
  * @param  arr: 自动重装值
  * @param  psc: 时钟预分频数
  * @param  TimerChannel: 定时器通道
  * @retval 无
  */
void TIMx_OCxInit(TIM_TypeDef* TIMx, uint16_t arr, uint16_t psc, uint8_t TimerChannel)
{
    LL_TIM_InitTypeDef TIM_TimeBaseStructure = {0};
    LL_TIM_OC_InitTypeDef TIM_OCInitStructure = {0};
    
    if(!IS_TIM_INSTANCE(TIMx))
        return;
    
    timer_clock_cmd(TIMx, ENABLE);

    TIM_TimeBaseStructure.Autoreload = arr;
    TIM_TimeBaseStructure.Prescaler = psc;
    TIM_TimeBaseStructure.ClockDivision = 0;
    TIM_TimeBaseStructure.CounterMode = LL_TIM_COUNTERMODE_UP;
    LL_TIM_Init(TIMx, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.OCMode = LL_TIM_OCMODE_PWM2;
    TIM_OCInitStructure.OCState = LL_TIM_OCSTATE_ENABLE;
    TIM_OCInitStructure.OCPolarity = LL_TIM_OCPOLARITY_LOW;
    switch(TimerChannel)
    {
    case 1:
        LL_TIM_OC_Init(TIMx, LL_TIM_CHANNEL_CH1, &TIM_OCInitStructure);
        LL_TIM_OC_EnablePreload(TIMx, LL_TIM_CHANNEL_CH1);
        break;
    case 2:
        LL_TIM_OC_Init(TIMx, LL_TIM_CHANNEL_CH2, &TIM_OCInitStructure);
        LL_TIM_OC_EnablePreload(TIMx, LL_TIM_CHANNEL_CH2);
        break;
    case 3:
        LL_TIM_OC_Init(TIMx, LL_TIM_CHANNEL_CH3, &TIM_OCInitStructure);
        LL_TIM_OC_EnablePreload(TIMx, LL_TIM_CHANNEL_CH3);
        break;
    case 4:
        LL_TIM_OC_Init(TIMx, LL_TIM_CHANNEL_CH4, &TIM_OCInitStructure);
        LL_TIM_OC_EnablePreload(TIMx, LL_TIM_CHANNEL_CH4);
        break;
    }

    LL_TIM_EnableCounter(TIMx);
    
    if(IS_APB2_TIM(TIMx))
    {
        LL_TIM_EnableAllOutputs(TIMx);
    }
}

/**
  * @brief  PWM输出初始化
  * @param  Pin:引脚编号
  * @param  PWM_DutyCycle: PWM分级数
  * @param  PWM_Frequency: PWM频率
  * @retval 引脚对应的定时器通道
  */
uint8_t pwm_start(PinName Pin, uint16_t PWM_DutyCycle, uint32_t PWM_Frequency)
{
    uint32_t arr, psc;
    
    if(!IS_PWM_PIN(Pin))
        return 0;
    
    if(PWM_DutyCycle == 0 || PWM_Frequency == 0 || (PWM_DutyCycle * PWM_Frequency) > F_CPU)
        return 0;

    pinMode(Pin, OUTPUT_AF);
    if(TIM_GetPinSourcex(Pin) < 8)
    {
    	LL_GPIO_SetAFPin_0_7(PIN_MAP[Pin].GPIOx, PIN_MAP[Pin].GPIO_Pin_x, TIM_GetGPIO_AF(Pin));
    }
    else
    {
    	LL_GPIO_SetAFPin_8_15(PIN_MAP[Pin].GPIOx, PIN_MAP[Pin].GPIO_Pin_x, TIM_GetGPIO_AF(Pin));
    }
    arr = PWM_DutyCycle;
    psc = F_CPU / PWM_DutyCycle / PWM_Frequency;
    
    if(!IS_APB2_TIM(PIN_MAP[Pin].TIMx))
        psc /= 2;

    LL_TIM_DisableCounter(PIN_MAP[Pin].TIMx);
    TIMx_OCxInit(PIN_MAP[Pin].TIMx, arr - 1, psc - 1, PIN_MAP[Pin].TimerChannel);
    return PIN_MAP[Pin].TimerChannel;
}
/**
  * @brief  PWM输出初始化，指定定时器
  * @param[in]  Pin:引脚编号
  * @param[in]  PWM_DutyCycle: PWM分级数
  * @param[in]  PWM_Frequency: PWM频率
  * @param[in]  TIM:使用的定时器
  * @param[in]	TimerChannel：定时器通道
  * @retval 引脚对应的定时器通道
  */
uint8_t pwm_startTim(PinName Pin, uint16_t PWM_DutyCycle, uint32_t PWM_Frequency,TIM_TypeDef* TIM,uint8_t TimerChannel)
{
    uint32_t arr, psc;
    uint8_t i = 0;
    /* 查找是否合法*/
    while(1)
    {
    	if(Pin == PinMap_PWM[i].Pin && TIM == PinMap_PWM[i].TIMx &&  TimerChannel == PinMap_PWM[i].Channel)
    	{
    		break;
    	}else if(0 == PinMap_PWM[i].Pin && 0 == PinMap_PWM[i].TIMx &&  0 == PinMap_PWM[i].Channel){
    		log_e("TIM PWM ERR\r\n");
    		return -1;
		}
    	i++;
    }
    //if(!IS_PWM_PIN(Pin))
     //   return 0;

    if(PWM_DutyCycle == 0 || PWM_Frequency == 0 || (PWM_DutyCycle * PWM_Frequency) > F_CPU)
        return -1;

    pinMode(Pin, OUTPUT_AF);
    if(TIM_GetPinSourcex(Pin) < 8)
    {
    	LL_GPIO_SetAFPin_0_7(PIN_MAP[Pin].GPIOx, PIN_MAP[Pin].GPIO_Pin_x, TIM_GetGPIO_AF(Pin));
    }
    else
    {
    	LL_GPIO_SetAFPin_8_15(PIN_MAP[Pin].GPIOx, PIN_MAP[Pin].GPIO_Pin_x, TIM_GetGPIO_AF(Pin));
    }
    arr = PWM_DutyCycle;
    psc = F_CPU / PWM_DutyCycle / PWM_Frequency;

    if(!IS_APB2_TIM(TIM))
        psc /= 2;

    LL_TIM_DisableCounter(TIM);
    TIMx_OCxInit(TIM, arr - 1, psc - 1, TimerChannel);
    return TimerChannel;
}
/**
  * @brief  获取引脚对应的定时器复用编号
  * @param  Pin: 引脚编号
  * @retval 定时器复用编号
  */
uint8_t TIM_GetGPIO_AF(PinName Pin)
{
    uint8_t GPIO_AF_x = 0;
    TIM_TypeDef* TIMx = PIN_MAP[Pin].TIMx;
    
    if(!IS_TIM_INSTANCE(TIMx))
        return 0;
    if(TIMx == TIM1){
    	GPIO_AF_x = LL_GPIO_AF_1;
    }else if(TIMx == TIM2){
    	GPIO_AF_x = LL_GPIO_AF_1;
    }else if(TIMx == TIM3){
    	GPIO_AF_x = LL_GPIO_AF_2;
    }else if(TIMx == TIM4){
    	GPIO_AF_x = LL_GPIO_AF_2;
    } else if(TIMx == TIM5){
    	GPIO_AF_x = LL_GPIO_AF_2;
    }else if(TIMx == TIM8){
    	GPIO_AF_x = LL_GPIO_AF_3;
    }else if(TIMx == TIM9){
    	GPIO_AF_x = LL_GPIO_AF_3;
    }else if(TIMx == TIM10){
    	GPIO_AF_x = LL_GPIO_AF_3;
    }else if(TIMx == TIM11){
    	GPIO_AF_x = LL_GPIO_AF_3;
    }else if(TIMx == TIM12){
    	GPIO_AF_x = LL_GPIO_AF_9;
    }else if(TIMx == TIM13){
    	GPIO_AF_x = LL_GPIO_AF_9;
    }else if(TIMx == TIM14){
    	GPIO_AF_x = LL_GPIO_AF_9;
    }
    return GPIO_AF_x;
}

/**
  * @brief  输出PWM信号
  * @param  Pin: 引脚编号
  * @param  val:PWM占空比值
  * @retval PWM占空比值
  */
uint16_t pwmWrite(PinName Pin, uint16_t val)
{
    switch(PIN_MAP[Pin].TimerChannel)
    {
    case 1:
        PIN_MAP[Pin].TIMx->CCR1 = val;
        break;
    case 2:
        PIN_MAP[Pin].TIMx->CCR2 = val;
        break;
    case 3:
        PIN_MAP[Pin].TIMx->CCR3 = val;
        break;
    case 4:
        PIN_MAP[Pin].TIMx->CCR4 = val;
        break;
    }
    return val;
}
/**
  * @brief  读当前PWM信号
  * @param  Pin: 引脚编号
  * @retval PWM占空比值
  */
uint16_t pwmRead(PinName Pin)
{
	uint16_t val = 0;
    switch(PIN_MAP[Pin].TimerChannel)
    {
    case 1:
    	val = PIN_MAP[Pin].TIMx->CCR1;
        break;
    case 2:
    	val = PIN_MAP[Pin].TIMx->CCR2;
        break;
    case 3:
    	val = PIN_MAP[Pin].TIMx->CCR3;
        break;
    case 4:
    	val = PIN_MAP[Pin].TIMx->CCR4;
        break;
    }
    return val;
}
/**
  * @brief  输出PWM信号
  * @note   有些信号取反了。在这个地方反一下
  * 		例如周期为1000,正常val=100时，占空比为10%，但是由于外部取反了,
  * 		真实占空比为90%。
  * @param  Pin: 引脚编号
  * @param  val:PWM占空比值 取反
  * @retval 真实的PWM占空比值
  *
  */
uint16_t pwmWriteNegative(PinName Pin, uint16_t val)
{
	uint32_t temp;
	if(PIN_MAP[Pin].TIMx->ARR > val ){
		temp = PIN_MAP[Pin].TIMx->ARR -  val;
	}else {
		temp = 0;
	}
    switch(PIN_MAP[Pin].TimerChannel)
    {
    case 1:
        PIN_MAP[Pin].TIMx->CCR1 = temp;
        break;
    case 2:
        PIN_MAP[Pin].TIMx->CCR2 = temp;
        break;
    case 3:
        PIN_MAP[Pin].TIMx->CCR3 = temp;
        break;
    case 4:
        PIN_MAP[Pin].TIMx->CCR4 = temp;
        break;
    }
    return val;
}
/**
  * @brief  停止输出PWM信号,引脚设置为
  * @param  Pin: 引脚编号
  * @param  val:PWM占空比值
  * @retval PWM占空比值
  */
uint8_t pwm_stop(PinName Pin,uint8_t pin_state)
{
	uint8_t i;
	uint8_t tim_sate = 0;
	uint8_t tim_ch;
	TIM_TypeDef* TIMx;
	TIMx = PIN_MAP[Pin].TIMx;
	tim_ch = PIN_MAP[Pin].TimerChannel;

	for(i=0;i<4;i++)
	{
		if(i != tim_ch && LL_TIM_OC_IsEnabledPreload(TIMx,i)){
			tim_sate++;
		}
	}
	LL_TIM_CC_DisableChannel(TIMx, tim_ch);
	if(tim_sate == 0)
	{
		LL_TIM_DisableCounter(TIMx);
	}

	digitalWrite(Pin, pin_state);
	return tim_sate;
}

/**
  * @brief  获取捕获值
  * @param  TIMx: 定时器地址
  * @param  TimerChannel: 定时器通道
  * @retval 捕获值
  */
uint16_t timer_get_compare(TIM_TypeDef* TIMx, uint8_t TimerChannel)
{
    uint16_t compare = 0;
    switch(TimerChannel)
    {
    case 1:
        compare = TIMx->CCR1;
        break;
    case 2:
        compare = TIMx->CCR2;
        break;
    case 3:
        compare = TIMx->CCR3;
        break;
    case 4:
        compare = TIMx->CCR4;
        break;
    }
    return compare;
}

/**
  * @brief  更新定时器时钟预分频数
  * @param  TIMx: 定时器地址
  * @param  psc: 时钟预分频数
  * @retval 无
  */
void timer_set_prescaler(TIM_TypeDef* TIMx, uint16_t psc)
{
    TIMx->PSC = psc;
}

/**
  * @brief  更新定时器自动重装值
  * @param  TIMx: 定时器地址
  * @param  arr: 自动重装值
  * @retval 无
  */
void timer_set_reload(TIM_TypeDef* TIMx, uint16_t arr)
{
    TIMx->ARR = arr;
}

/**
  * @brief  应用定时器更改
  * @param  TIMx: 定时器地址
  * @retval 无
  */
void timer_generate_update(TIM_TypeDef* TIMx)
{
    TIMx->EGR = TIM_EGR_UG; //TIM_PSCReloadMode_Immediate;
}
