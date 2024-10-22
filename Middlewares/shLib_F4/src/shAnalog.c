/**
 ******************************************************************************
 * @file    aanalog.c
 * @author	 ZGQ
 * @version s10
 * @date    2022.01.14
 * @brief   模拟量读写，
 ******************************************************************************
 * @attention
 * 可添加程序说明及注意事项：
 * 1.代码风格：ANSI风格
 * 2.
 ******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "shAnalog.h"
#include "shGpio.h"
#include "shPwm.h"

/* Private types -------------------------------------------------------------*/
#define ADC_DMA_REGMAX 18

/* Private macro -------------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/*ADC通道注册个数*/
static uint8_t ADC_DMA_RegCnt = 0;

/*ADC通道注册列表*/
static uint16_t ADC_DMA_RegChannelList[ADC_DMA_REGMAX] = { 0 };

/* ADC DMA缓存数组 假定为18*10,当只有2个通道a1、a2,则缓存中的值为{a1,a2},{a1,a2}...{a1,a2} 共10组
 * 如果想要读到正确的值，则为a1vol = (*p + *(p+2) ... + *(p+9*2))/10,具体见analogRead_DMA
 * */
static uint16_t ADC_DMA_ConvertedValue[ADC_DMA_REGMAX*ADC_DMA_AVERAGE_NUM] = { 0 };

static PinName g_current_pin = PIN_NC;
ADC_HandleTypeDef shhadc1 = {.Instance = ADC1};
ADC_HandleTypeDef shhadc2 = {.Instance = ADC2};
#define hdmaadc 	shhadc1
#define hinjectadc	shhadc2
/* Private function prototypes -----------------------------------------------*/
void ADC_DMA_Init();

/************************ (C) COPYRIGHT ROBOT++ *****END OF FILE****/
void ADC_RCCEnable(ADC_HandleTypeDef *hadc) {
	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* ADC Periph clock enable */
	if (hadc->Instance == ADC1) {
#ifdef __HAL_RCC_ADC1_CLK_ENABLE
		__HAL_RCC_ADC1_CLK_ENABLE();
#endif
#ifdef __HAL_RCC_ADC12_CLK_ENABLE
    __HAL_RCC_ADC12_CLK_ENABLE();
#endif
	}
#ifdef ADC2
	else if (hadc->Instance == ADC2) {
#ifdef __HAL_RCC_ADC2_CLK_ENABLE
		__HAL_RCC_ADC2_CLK_ENABLE();
#endif
#ifdef __HAL_RCC_ADC12_CLK_ENABLE
    __HAL_RCC_ADC12_CLK_ENABLE();
#endif
	}
#endif
#ifdef ADC3
	else if (hadc->Instance == ADC3) {
#ifdef __HAL_RCC_ADC3_CLK_ENABLE
		__HAL_RCC_ADC3_CLK_ENABLE();
#endif
#ifdef __HAL_RCC_ADC34_CLK_ENABLE
    __HAL_RCC_ADC34_CLK_ENABLE();
#endif
#if defined(ADC345_COMMON)
    __HAL_RCC_ADC345_CLK_ENABLE();
#endif
	}
#endif
#ifdef ADC4
  else if (hadc->Instance == ADC4) {
#ifdef __HAL_RCC_ADC4_CLK_ENABLE
    __HAL_RCC_ADC4_CLK_ENABLE();
#endif
#ifdef __HAL_RCC_ADC34_CLK_ENABLE
    __HAL_RCC_ADC34_CLK_ENABLE();
#endif
#if defined(ADC345_COMMON)
    __HAL_RCC_ADC345_CLK_ENABLE();
#endif
  }
#endif
#ifdef ADC5
  else if (hadc->Instance == ADC5) {
#if defined(ADC345_COMMON)
    __HAL_RCC_ADC345_CLK_ENABLE();
#endif
  }
#endif
#ifdef __HAL_RCC_ADC_CLK_ENABLE
  __HAL_RCC_ADC_CLK_ENABLE();
#endif

	/* Configure ADC GPIO pin */
	if (IS_ADC_PIN(g_current_pin )) {
		pinMode(g_current_pin, INPUT_ANALOG);
	}
}

/**
 * @brief  DeInitializes the ADC MSP.
 * @param  hadc: ADC handle
 * @retval None
 */
void ADC_RCCDisable(ADC_HandleTypeDef *hadc) {
//#ifdef __HAL_RCC_ADC_FORCE_RESET
//	__HAL_RCC_ADC_FORCE_RESET();
//#endif
//#ifdef __HAL_RCC_ADC_RELEASE_RESET
//	__HAL_RCC_ADC_RELEASE_RESET();
//#endif

	if (hadc->Instance == ADC1) {
#ifdef __HAL_RCC_ADC1_FORCE_RESET
    __HAL_RCC_ADC1_FORCE_RESET();
#endif
#ifdef __HAL_RCC_ADC1_RELEASE_RESET
    __HAL_RCC_ADC1_RELEASE_RESET();
#endif
#ifdef __HAL_RCC_ADC12_FORCE_RESET
    __HAL_RCC_ADC12_FORCE_RESET();
#endif
#ifdef __HAL_RCC_ADC12_RELEASE_RESET
    __HAL_RCC_ADC12_RELEASE_RESET();
#endif
#ifdef __HAL_RCC_ADC1_CLK_DISABLE
		__HAL_RCC_ADC1_CLK_DISABLE();
#endif
#ifdef __HAL_RCC_ADC12_CLK_DISABLE
    __HAL_RCC_ADC12_CLK_DISABLE();
#endif
	}
#ifdef ADC2
	else if (hadc->Instance == ADC2) {
#ifdef __HAL_RCC_ADC2_FORCE_RESET
    __HAL_RCC_ADC2_FORCE_RESET();
#endif
#ifdef __HAL_RCC_ADC2_RELEASE_RESET
    __HAL_RCC_ADC2_RELEASE_RESET();
#endif
#ifdef __HAL_RCC_ADC12_FORCE_RESET
    __HAL_RCC_ADC12_FORCE_RESET();
#endif
#ifdef __HAL_RCC_ADC12_RELEASE_RESET
    __HAL_RCC_ADC12_RELEASE_RESET();
#endif
#ifdef __HAL_RCC_ADC2_CLK_DISABLE
		__HAL_RCC_ADC2_CLK_DISABLE();
#endif
#ifdef __HAL_RCC_ADC2_CLK_DISABLE
		__HAL_RCC_ADC2_CLK_DISABLE();
#endif
	}
#endif
#ifdef ADC3
	else if (hadc->Instance == ADC3) {
#ifdef __HAL_RCC_ADC3_FORCE_RESET
    __HAL_RCC_ADC3_FORCE_RESET();
#endif
#ifdef __HAL_RCC_ADC3_RELEASE_RESET
    __HAL_RCC_ADC3_RELEASE_RESET();
#endif
#ifdef __HAL_RCC_ADC34_FORCE_RESET
    __HAL_RCC_ADC34_FORCE_RESET();
#endif
#ifdef __HAL_RCC_ADC34_RELEASE_RESET
    __HAL_RCC_ADC34_RELEASE_RESET();
#endif
#ifdef __HAL_RCC_ADC3_CLK_DISABLE
		__HAL_RCC_ADC3_CLK_DISABLE();
#endif
#ifdef __HAL_RCC_ADC34_CLK_DISABLE
    __HAL_RCC_ADC34_CLK_DISABLE();
#endif
#if defined(ADC345_COMMON)
    __HAL_RCC_ADC345_FORCE_RESET();
    __HAL_RCC_ADC345_RELEASE_RESET();
    __HAL_RCC_ADC345_CLK_DISABLE();
#endif
	}
#endif
#ifdef ADC4
  else if (hadc->Instance == ADC4) {
#ifdef __HAL_RCC_ADC4_FORCE_RESET
    __HAL_RCC_ADC4_FORCE_RESET();
#endif
#ifdef __HAL_RCC_ADC4_RELEASE_RESET
    __HAL_RCC_ADC4_RELEASE_RESET();
#endif
#ifdef __HAL_RCC_ADC4_CLK_DISABLE
    __HAL_RCC_ADC4_CLK_DISABLE();
#endif
#ifdef __HAL_RCC_ADC34_FORCE_RESET
    __HAL_RCC_ADC34_FORCE_RESET();
#endif
#ifdef __HAL_RCC_ADC34_RELEASE_RESET
    __HAL_RCC_ADC34_RELEASE_RESET();
#endif
#ifdef __HAL_RCC_ADC34_CLK_DISABLE
    __HAL_RCC_ADC34_CLK_DISABLE();
#endif
#if defined(ADC345_COMMON)
    __HAL_RCC_ADC345_FORCE_RESET();
    __HAL_RCC_ADC345_RELEASE_RESET();
    __HAL_RCC_ADC345_CLK_DISABLE();
#endif
  }
#endif
#ifdef ADC5
  else if (hadc->Instance == ADC5) {
#if defined(ADC345_COMMON)
    __HAL_RCC_ADC345_FORCE_RESET();
    __HAL_RCC_ADC345_RELEASE_RESET();
    __HAL_RCC_ADC345_CLK_DISABLE();
#endif
  }
#endif
#ifdef __HAL_RCC_ADC_CLK_DISABLE
  __HAL_RCC_ADC_FORCE_RESET();
  __HAL_RCC_ADC_RELEASE_RESET();
  __HAL_RCC_ADC_CLK_DISABLE();
#endif
}
/**
 * @brief  This function will set the ADC to the required value
 * @param  pin : the pin to use
 * @param  resolution : resolution for converted data: 6/8/10/12/14/16
 * @retval the value of the adc
 */
uint16_t ADCx_GetValue(PinName pin) {
	ADC_ChannelConfTypeDef AdcChannelConf = {0};
	__IO uint16_t uhADCxConvertedValue = 0;
	uint32_t channel = 0;
	static uint8_t IsActive = 0;

	channel = PIN_MAP[pin].ADC_Channel;
	if (!IS_ADC_CHANNEL(channel)) {
		return 0;
	}

	/*##-1- 之前没有初始化过，则初始化一次 ######################################*/
	if(IsActive == 0){
		IsActive = 1;
		hinjectadc.Instance = ADC2;
		hinjectadc.Init.ClockPrescaler 			= ADC_CLOCK_SYNC_PCLK_DIV4; 	// 4分频，ADCCLK=PCLK2/4=90/4=22.5MHZ
		hinjectadc.Init.Resolution 				= ADC_RESOLUTION_12B;           // 12位模式
		hinjectadc.Init.DataAlign 				= ADC_DATAALIGN_RIGHT;          // 右对齐
		hinjectadc.Init.ScanConvMode 			= DISABLE;                    	// 非扫描模式
		hinjectadc.Init.EOCSelection 			= DISABLE;        				// 关闭EOC中断
		hinjectadc.Init.ContinuousConvMode 		= DISABLE;              		// 关闭连续转换
		hinjectadc.Init.NbrOfConversion 		= 1;                 			// 1个转换在规则序列中 也就是只转换规则序列1
		hinjectadc.Init.DiscontinuousConvMode 	= DISABLE;            			// 禁止不连续采样模式
		hinjectadc.Init.NbrOfDiscConversion 	= 0;                    		// 不连续采样通道数为0
		hinjectadc.Init.ExternalTrigConv 		= ADC_SOFTWARE_START;      		// 软件触发
		hinjectadc.Init.ExternalTrigConvEdge 	= ADC_EXTERNALTRIGCONVEDGE_NONE; //使用软件触发
		hinjectadc.Init.DMAContinuousRequests 	= DISABLE;           			// 关闭DMA请求
		ADC_RCCEnable(&hinjectadc);;                                			// 初始化
		if (HAL_ADC_Init(&hinjectadc) != HAL_OK) {
			return 0;
		}
	}
	/*##-2- Configure ADC regular channel ######################################*/
	AdcChannelConf.Channel = channel; /* Specifies the channel to configure into ADC */
	AdcChannelConf.Rank = 1; /* Specifies the rank in the regular group sequencer */
	AdcChannelConf.Offset = 0; /* Parameter discarded because offset correction is disabled */
	AdcChannelConf.SamplingTime = ADC_SAMPLETIME_56CYCLES;        //采样时间

	if (HAL_ADC_ConfigChannel(&hinjectadc, &AdcChannelConf) != HAL_OK) {
		/* Channel Configuration Error */
		return 0;
	}

	/*##-3- Start the conversion process ####################*/
	if (HAL_ADC_Start(&hinjectadc) != HAL_OK) {
		/* Start Conversion Error */
		return 0;
	}

	/*##-4- Wait for the end of conversion #####################################*/
	/*  For simplicity reasons, this example is just waiting till the end of the
	 conversion, but application may perform other tasks while conversion
	 operation is ongoing. */
	if (HAL_ADC_PollForConversion(&hinjectadc, 10) != HAL_OK) {
		/* End Of Conversion flag not set on time */
		return 0;
	}

	/* Check if the continous conversion of regular channel is finished */
	if ((HAL_ADC_GetState(&hinjectadc) & HAL_ADC_STATE_REG_EOC)
			== HAL_ADC_STATE_REG_EOC) {
	/*##-5- Get the converted value of regular channel  ########################*/
		uhADCxConvertedValue = HAL_ADC_GetValue(&hinjectadc);
	}

	return uhADCxConvertedValue;
}

/**
 * @brief  搜索注册列表，找出ADC通道对应的索引号
 * @param  ADC_Channel:ADC通道号
 * @retval ADC通道注册列表对应索引号，-1:未找到对应通道号
 */
static int16_t ADC_DMA_SearchChannel(uint16_t ADC_Channel) {
	uint8_t index;

	for (index = 0; index < ADC_DMA_RegCnt; index++) {
		if (ADC_Channel == ADC_DMA_RegChannelList[index]) {
			return index;
		}
	}
	return -1;
}

/**
 * @brief  注册需要DMA搬运的ADC通道
 * @param  ADC_Channel:ADC通道号
 * @retval 引脚注册列表对应索引号，-1:不支持ADC，-2:引脚重复注册，-3:超出最大注册个数
 */
int16_t ADC_DMA_Register(uint8_t ADC_Channel) {
	/*初始化ADC通道列表*/
	static uint8_t IsInit = 0;
	if (!IsInit) {
		uint8_t i;
		for (i = 0; i < ADC_DMA_REGMAX; i++) {
			ADC_DMA_RegChannelList[i] = 0xFF;
		}
		IsInit = 1;
	}

	/*是否是合法ADC通道*/
	if (!IS_ADC_CHANNEL(ADC_Channel))
		return -1;

	/*是否已在引脚列表重复注册*/
	if (ADC_DMA_SearchChannel(ADC_Channel) != -1)
		return -2;

	/*是否超出最大注册个数*/
	if (ADC_DMA_RegCnt >= ADC_DMA_REGMAX)
		return -3;

	/*写入注册列表*/
	ADC_DMA_RegChannelList[ADC_DMA_RegCnt] = ADC_Channel;

	/*注册个数+1*/
	ADC_DMA_RegCnt++;
	ADC_DMA_Init();
	return ADC_DMA_RegCnt - 1;
}

/**
 * @brief  ADC DMA 配置
 * @param  无
 * @retval 无
 */
void ADC_DMA_Init() {
	DMA_HandleTypeDef hdma_adc1;
	ADC_ChannelConfTypeDef sConfig;
	uint8_t index;

	HAL_ADC_Stop_DMA(&hdmaadc);

	hdmaadc.Instance = ADC1;
	hdma_adc1.Instance = DMA2_Stream0;
	HAL_ADC_DeInit(&hdmaadc);
	HAL_DMA_DeInit(&hdma_adc1);
	__HAL_RCC_DMA2_CLK_DISABLE();
	ADC_RCCDisable(&hdmaadc);

	/*##-1- Configure the ADC peripheral #######################################*/

	hdmaadc.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
	hdmaadc.Init.Resolution = ADC_RESOLUTION_12B;
	hdmaadc.Init.ScanConvMode = ENABLE;
	hdmaadc.Init.ContinuousConvMode = ENABLE;
	hdmaadc.Init.DiscontinuousConvMode = DISABLE;
	hdmaadc.Init.NbrOfDiscConversion = 0;
	hdmaadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hdmaadc.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
	hdmaadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hdmaadc.Init.NbrOfConversion = ADC_DMA_RegCnt;
	hdmaadc.Init.DMAContinuousRequests = ENABLE;
	hdmaadc.Init.EOCSelection = DISABLE;

	ADC_RCCEnable(&hdmaadc);
	if (HAL_ADC_Init(&hdmaadc) != HAL_OK) {
		/* Initialization Error */
		//Error_Handler();
	}

	/*##-2- Configure ADC regular channel ######################################*/
	/* Note: Considering IT occurring after each number of size of              */
	/*       "uhADCxConvertedValue"  ADC conversions (IT by DMA end             */
	/*       of transfer), select sampling time and ADC clock with sufficient   */
	/*       duration to not create an overhead situation in IRQHandler.        */
	for (index = 0; index < ADC_DMA_RegCnt; index++) {
		sConfig.Channel = ADC_DMA_RegChannelList[index];
		sConfig.Rank = index + 1;
		sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
		if (HAL_ADC_ConfigChannel(&hdmaadc, &sConfig) != HAL_OK) {
			/* Channel Configuration Error */
			//Error_Handler();
		}
	}
	__HAL_RCC_DMA2_CLK_ENABLE();

	//hdma_adc1.Instance = DMA2_Stream0;
	hdma_adc1.Init.Channel = DMA_CHANNEL_0;
	hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
	hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_adc1.Init.Mode = DMA_CIRCULAR;
	hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
	hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	if (HAL_DMA_Init(&hdma_adc1) != HAL_OK) {
		// Error_Handler();
	}

	__HAL_LINKDMA(&hdmaadc, DMA_Handle, hdma_adc1);
	/*##-3- Start the conversion process and enable interrupt ##################*/
	/* Note: Considering IT occurring after each number of ADC conversions      */
	/*       (IT by DMA end of transfer), select sampling time and ADC clock    */
	/*       with sufficient duration to not create an overhead situation in    */
	/*        IRQHandler. */
	if (HAL_ADC_Start_DMA(&hdmaadc, (uint32_t*) &ADC_DMA_ConvertedValue, ADC_DMA_RegCnt*ADC_DMA_AVERAGE_NUM)
			!= HAL_OK) {
		/* Start Conversation Error */
		//Error_Handler();
	}
}
/**
 * @brief  获取DMA搬运的ADC值
 * @param  ADC_Channel:ADC通道号
 * @retval ADC值
 */
uint16_t ADC_DMA_GetValue(uint8_t ADC_Channel) {
	int16_t index = 0;
	uint32_t value = 0;
	uint8_t i = 0;
	if (!IS_ADC_CHANNEL(ADC_Channel))
		return 0;

	index = ADC_DMA_SearchChannel(ADC_Channel);
	if (index == -1)
		return 0;

	for (i = 0; i < ADC_DMA_AVERAGE_NUM; i++) {
		value += ADC_DMA_ConvertedValue[index+i*ADC_DMA_RegCnt];
	}
	value = value/ADC_DMA_AVERAGE_NUM;
	return value;
}

/**
 * @brief  将模拟值(PWM占空比)写入引脚
 * @param  Pin: 引脚编号
 * @param  val: PWM占空比
 * @retval PWM占空比
 */
uint16_t analogWrite(PinName Pin, uint16_t val) {
	if (!IS_PIN(Pin))
		return 0;

	return (IS_PWM_PIN(Pin) ? pwmWrite(Pin, val) : 0);
}

/**
 * @brief  从指定的模拟引脚读取值,先查看是否使用DMA
 * @param[in]  Pin: 引脚编号
 * @retval ADC值：0~4095
 */
uint16_t analogRead(PinName Pin) {
	int16_t index;
	uint16_t ADC_Channel = PIN_MAP[Pin].ADC_Channel;

	if (!IS_ADC_PIN(Pin))
		return 0;

	if (!IS_ADC_CHANNEL(ADC_Channel))
		return 0;

	index = ADC_DMA_SearchChannel(ADC_Channel);

	if (index != -1) {
		//使用DMA
		return ADC_DMA_GetValue(ADC_Channel);
	} else {
		//未使用DMA
		return  ADCx_GetValue(Pin);
	}
}
