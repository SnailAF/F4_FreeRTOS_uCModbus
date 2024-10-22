/**
 ******************************************************************************
 * @file    bsp.c
 * @author
 * @version
 * @date    2021.8.18
 * @brief
 ******************************************************************************
 * @attention
 * 可添加程序说明及注意事项：
 * 1.代码风格：ANSI风格
 * 2.
 ******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "shTimer.h"
#include "shUtils.h"
/* Private types -------------------------------------------------------------*/

/*定时器编号枚举*/
typedef enum {
#if defined(TIM1_BASE)
	TIMER1,
#endif
#if defined(TIM2_BASE)
	TIMER2,
#endif
#if defined(TIM3_BASE)
	TIMER3,
#endif
#if defined(TIM4_BASE)
	TIMER4,
#endif
#if defined(TIM5_BASE)
	TIMER5,
#endif
#if defined(TIM6_BASE)
	TIMER6,
#endif
#if defined(TIM7_BASE)
	TIMER7,
#endif
#if defined(TIM8_BASE)
	TIMER8,
#endif
#if defined(TIM9_BASE)
	TIMER9,
#endif
#if defined(TIM10_BASE)
	TIMER10,
#endif
#if defined(TIM11_BASE)
	TIMER11,
#endif
#if defined(TIM12_BASE)
	TIMER12,
#endif
#if defined(TIM13_BASE)
	TIMER13,
#endif
#if defined(TIM14_BASE)
	TIMER14,
#endif
#if defined(TIM15_BASE)
    TIMER15,
#endif
#if defined(TIM16_BASE)
    TIMER16,
#endif
#if defined(TIM17_BASE)
    TIMER17,
#endif
#if defined(TIM18_BASE)
    TIMER18,
#endif
#if defined(TIM19_BASE)
    TIMER19,
#endif
#if defined(TIM20_BASE)
    TIMER20,
#endif
#if defined(TIM21_BASE)
    TIMER21,
#endif
#if defined(TIM22_BASE)
    TIMER22,
#endif
	TIMER_MAX
} TIMER_Type;
/* Private macro -------------------------------------------------------------*/
#define TIMx_GetTimHandler(n) shtim##n
/*取绝对值*/
#define CLOCK_ABS(x) (((x)>0)?(x):-(x))

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/*定时中断回调函数指针数组*/
static Timer_CallbackFunction_t TIMx_Function[TIMER_MAX] = { 0 };
#if defined(TIM1_BASE)
static TIM_HandleTypeDef shtim1;
#endif
#if defined(TIM2_BASE)
static TIM_HandleTypeDef shtim2;
#endif
#if defined(TIM3_BASE)
static TIM_HandleTypeDef shtim3;
#endif
#if defined(TIM4_BASE)
static TIM_HandleTypeDef shtim4;
#endif
#if defined(TIM5_BASE)
static TIM_HandleTypeDef shtim5;
#endif
#if defined(TIM6_BASE)
static TIM_HandleTypeDef shtim6;
#endif
#if defined(TIM7_BASE)
static TIM_HandleTypeDef shtim7;
#endif
#if defined(TIM8_BASE)
static TIM_HandleTypeDef shtim8;
#endif
#if defined(TIM9_BASE)
static TIM_HandleTypeDef shtim9;
#endif
#if defined(TIM10_BASE)
static TIM_HandleTypeDef shtim10;
#endif
#if defined(TIM11_BASE)
static TIM_HandleTypeDef shtim11;
#endif
#if defined(TIM12_BASE)
static TIM_HandleTypeDef shtim12;
#endif
#if defined(TIM13_BASE)
static TIM_HandleTypeDef shtim13;
#endif
#if defined(TIM14_BASE)
static TIM_HandleTypeDef shtim14;
#endif
#if defined(TIM15_BASE)
static TIM_HandleTypeDef shtim15;
#endif
#if defined(TIM16_BASE)
static TIM_HandleTypeDef shtim16;
#endif
#if defined(TIM17_BASE)
static TIM_HandleTypeDef shtim17;
#endif
#if defined(TIM18_BASE)
static TIM_HandleTypeDef shtim18;
#endif
#if defined(TIM19_BASE)
static TIM_HandleTypeDef shtim19;
#endif
#if defined(TIM20_BASE)
static TIM_HandleTypeDef shtim20;
#endif
#if defined(TIM21_BASE)
static TIM_HandleTypeDef shtim21;
#endif
#if defined(TIM22_BASE)
static TIM_HandleTypeDef shtim22;
#endif
/* Exported variables --------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
static void timer_clock_enable(TIM_TypeDef *TIMx);
static void timer_clock_disable(TIM_TypeDef *TIMx);
static float Qsqrt(float number);
static int32_t timer_freq_to_arr_prescaler(uint32_t freq, uint32_t clock,
		uint16_t *period, uint16_t *prescaler);
static void timer_time_to_arr_prescaler(uint32_t time, uint32_t clock, uint16_t *period,
		uint16_t *prescaler);
static IRQn_Type timer_get_IRQn(TIM_TypeDef *TIMx);
static TIMER_Type timer_get_timer(TIM_TypeDef *TIMx);
static TIM_HandleTypeDef * timer_get_tim_handler(TIM_TypeDef *TIMx);

/**
 * @brief  定时中断配置
 * @param  TIMx:定时器地址
 * @param  time: 中断时间(微秒)
 * @param  function: 定时中断回调函数
 * @retval 无
 */
TIM_HandleTypeDef* timer_open(TIM_TypeDef *TIMx, uint32_t time,
		Timer_CallbackFunction_t function) {
	uint16_t period, prescaler;
	uint32_t clock = (IS_APB2_TIM(TIMx) ? F_CPU : (F_CPU / 2));


	if (!IS_TIM_PERIPH(TIMx) || time == 0)
		return 0;

	/*将定时中断时间转换为重装值和时钟分频值*/
	timer_time_to_arr_prescaler(time, clock, &period, &prescaler);

	/*定时中断配置*/
	return timer_open_interrupt_base(TIMx, period, prescaler, function,
	Timer_PreemptionPriority_Default,
	Timer_SubPriority_Default);
}
/**
 * @brief  关闭定时
 * @note
 * @param[in] TIMx: 定时器号, ex: TIM1, TIM2, TIM3, TIM4, TIM5, TIM6,
 * @retval  none
 */
void timer_closed(TIM_TypeDef *TIMx)
{
	if (IS_TIM_PERIPH(TIMx)) {
        timer_clock_disable(TIMx);
    }
}
/**
 * @brief  启动定时器
 * @note
 * @param[in] TIMx: 定时器号, ex: TIM1, TIM2, TIM3, TIM4, TIM5, TIM6,
 * @retval  none
 */
void timer_start(TIM_TypeDef *TIMx)
{

	TIM_HandleTypeDef *htim;
	if (!IS_TIM_PERIPH(TIMx))
		return;
	htim = timer_get_tim_handler(TIMx);
	__HAL_TIM_CLEAR_FLAG(htim, TIM_FLAG_UPDATE);
	__HAL_TIM_ENABLE_IT(htim, TIM_FLAG_UPDATE);
	__HAL_TIM_ENABLE(htim);
}
/**
 * @brief  暂停定时器
 * @note
 * @param[in] TIMx: 定时器号, ex: TIM1, TIM2, TIM3, TIM4, TIM5, TIM6,
 * @retval  none
 */
void timer_stop(TIM_TypeDef *TIMx)
{
	TIM_HandleTypeDef *htim;
	if (!IS_TIM_PERIPH(TIMx))
		return;
	htim = timer_get_tim_handler(TIMx);
	__HAL_TIM_CLEAR_FLAG(htim, TIM_FLAG_UPDATE);
	__HAL_TIM_DISABLE_IT(htim, TIM_FLAG_UPDATE);
	__HAL_TIM_DISABLE(htim);
}

/**
 * @brief  改变定时周期
 * @note
 * @param[in] TIMx: 定时器号, ex: TIM1, TIM2, TIM3, TIM4, TIM5, TIM6,
 * @param[in] time: 定时时间，单位us。
 * @retval  none
 */
void timer_change_period(TIM_TypeDef *TIMx, uint32_t time)
{
	uint16_t period, prescaler;
	TIM_HandleTypeDef *htim;
    uint32_t clock = (IS_APB2_TIM(TIMx)? F_CPU : (F_CPU / 2));

    if (!IS_TIM_PERIPH(TIMx) || time == 0)
        return;
    htim = timer_get_tim_handler(TIMx);
    /*将定时中断时间转换为重装值和时钟分频值*/
    timer_time_to_arr_prescaler(time, clock, &period, &prescaler);
	__HAL_TIM_SET_AUTORELOAD(htim, period - 1);
	__HAL_TIM_SET_PRESCALER(htim, prescaler - 1);
	__HAL_TIM_ENABLE_IT(htim, TIM_IT_UPDATE);
}
/**
 * @brief  更新定时中断频率
 * @param  TIMx:定时器地址
 * @param  freq:中断频率
 * @retval 无
 */
void timer_change_freq(TIM_TypeDef *TIMx, uint32_t freq) {
	uint16_t period, prescaler;
	uint32_t clock = (IS_APB2_TIM(TIMx) ? F_CPU : (F_CPU / 2));
	TIM_HandleTypeDef *htim;
	if (!IS_TIM_PERIPH(TIMx) || freq == 0)
		return;
	htim = timer_get_tim_handler(TIMx);
	timer_freq_to_arr_prescaler(freq, clock, &period, &prescaler);
	__HAL_TIM_SET_AUTORELOAD(htim, period - 1);
	__HAL_TIM_SET_PRESCALER(htim, prescaler - 1);
	__HAL_TIM_ENABLE_IT(htim, TIM_IT_UPDATE);
}
/**
 * @brief  获取定时器中断周期
 * @param  TIMx:定时器地址
 * @retval 中断周期
 */
uint32_t timer_get_period(TIM_TypeDef *TIMx)
{
	uint32_t clock = (IS_APB2_TIM(TIMx) ? F_CPU : (F_CPU / 2));
    SH_ASSERT(IS_TIM_PERIPH(TIMx));
    return (((TIMx->ARR + 1) * (TIMx->PSC + 1) * 1000000U) / clock );
}
/**
 * @brief  获取定时器中断频率
 * @param  TIMx:定时器地址
 * @retval 中断频率
 */
uint32_t timer_get_freq(TIM_TypeDef *TIMx) {
	uint32_t clock = (IS_APB2_TIM(TIMx) ? F_CPU : (F_CPU / 2));
	SH_ASSERT(IS_TIM_PERIPH(TIMx));
	return (clock / ((TIMx->ARR + 1) * (TIMx->PSC + 1)));
}
/**
 * @brief  根据定时器地址获取定时器中断地址
 * @param  TIMx:定时器地址
 * @retval 定时器中断地址
 */
static IRQn_Type timer_get_IRQn(TIM_TypeDef *TIMx) {

	SH_ASSERT(IS_TIM_PERIPH(TIMx));
	IRQn_Type iqrn;
	if (TIMx == TIM1)
		iqrn = TIM1_UP_TIM10_IRQn;
	else if (TIMx == TIM2)
		iqrn = TIM2_IRQn;
	else if (TIMx == TIM3)
		iqrn = TIM3_IRQn;
	else if (TIMx == TIM4)
		iqrn = TIM4_IRQn;
	else if (TIMx == TIM5)
		iqrn = TIM5_IRQn;
	else if (TIMx == TIM6)
		iqrn = TIM6_DAC_IRQn;
	else if (TIMx == TIM7)
		iqrn = TIM7_IRQn;
	else if (TIMx == TIM8)
		iqrn = TIM8_UP_TIM13_IRQn;
	else if (TIMx == TIM9)
		iqrn = TIM8_UP_TIM13_IRQn;
	else if (TIMx == TIM10)
		iqrn = TIM1_UP_TIM10_IRQn;
	else if (TIMx == TIM11)
		iqrn = TIM1_TRG_COM_TIM11_IRQn;
	else if (TIMx == TIM12)
		iqrn = TIM8_BRK_TIM12_IRQn;
	else if (TIMx == TIM13)
		iqrn = TIM8_UP_TIM13_IRQn;
	else if (TIMx == TIM14)
		iqrn = TIM8_TRG_COM_TIM14_IRQn;

	return iqrn;
}
/**
 * @brief  根据定时获取TIMER值
 * @param  TIMx:定时器地址
 * @retval 定时器数
 */
static TIMER_Type timer_get_timer(TIM_TypeDef *TIMx) {
	TIMER_Type timer;

	SH_ASSERT(IS_TIM_PERIPH(TIMx));
	if (TIMx == TIM1)
		timer = TIMER1;
	else if (TIMx == TIM2)
		timer = TIMER2;
	else if (TIMx == TIM3)
		timer = TIMER3;
	else if (TIMx == TIM4)
		timer = TIMER4;
	else if (TIMx == TIM5)
		timer = TIMER5;
	else if (TIMx == TIM6)
		timer = TIMER6;
	else if (TIMx == TIM7)
		timer = TIMER7;
	else if (TIMx == TIM8)
		timer = TIMER8;
	else if (TIMx == TIM9)
		timer = TIMER9;
	else if (TIMx == TIM10)
		timer = TIMER10;
	else if (TIMx == TIM11)
		timer = TIMER11;
	else if (TIMx == TIM12)
		timer = TIMER12;
	else if (TIMx == TIM13)
		timer = TIMER13;
	else if (TIMx == TIM14)
		timer = TIMER14;

	return timer;
}
/**
 * @brief  根据定时获取TIMER值
 * @param  TIMx:定时器地址
 * @retval 定时器数
 */
static TIM_HandleTypeDef * timer_get_tim_handler(TIM_TypeDef *TIMx) {
	TIM_HandleTypeDef * htim;
	SH_ASSERT(IS_TIM_PERIPH(TIMx));

	if (TIMx == TIM1)
		htim = &shtim1;
	else if (TIMx == TIM2)
		htim = &shtim2;
	else if (TIMx == TIM3)
		htim = &shtim3;
	else if (TIMx == TIM4)
		htim = &shtim4;
	else if (TIMx == TIM5)
		htim = &shtim5;
	else if (TIMx == TIM6)
		htim = &shtim6;
	else if (TIMx == TIM7)
		htim = &shtim7;
	else if (TIMx == TIM8)
		htim = &shtim8;
	else if (TIMx == TIM9)
		htim = &shtim9;
	else if (TIMx == TIM10)
		htim = &shtim10;
	else if (TIMx == TIM11)
		htim = &shtim11;
	else if (TIMx == TIM12)
		htim = &shtim12;
	else if (TIMx == TIM13)
		htim = &shtim13;
	else if (TIMx == TIM14)
		htim = &shtim14;

	return htim;
}
/**
 * @brief  定时中断基本配置
 * @param  TIMx:定时器地址
 * @param  period:重装值
 * @param  prescaler:时钟分频值
 * @param  function: 定时中断回调函数
 * @param  PreemptionPriority: 抢占优先级
 * @param  SubPriority: 子优先级
 * @retval 无
 */
TIM_HandleTypeDef* timer_open_interrupt_base(TIM_TypeDef *TIMx, uint16_t period,
		uint16_t prescaler, Timer_CallbackFunction_t function,
		uint8_t PreemptionPriority, uint8_t SubPriority) {

	TIM_HandleTypeDef *htim;
	IRQn_Type TIMx_IRQn;
	TIMER_Type TIMERx;
	SH_ASSERT(IS_TIM_PERIPH(TIMx));
	if (!IS_TIM_PERIPH(TIMx))
		return 0;

	TIMx_IRQn = timer_get_IRQn(TIMx);
	if (TIMx_IRQn == 0)
		return 0;
	TIMERx = timer_get_timer(TIMx);
	htim = timer_get_tim_handler(TIMx);


	/*register callback function*/
	TIMx_Function[TIMERx] = function;
	/*Enable PeriphClock*/
	HAL_TIM_Base_DeInit(htim);
	timer_clock_cmd(TIMx, ENABLE);

	htim->Instance = TIMx;
	htim->Init.Prescaler = prescaler - 1;	//设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
	htim->Init.CounterMode = TIM_COUNTERMODE_UP;		//TIM向上计数模式
	htim->Init.Period = period - 1;				//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(htim) != HAL_OK) {
		//Error_Handler();
	}
	HAL_NVIC_SetPriority(TIMx_IRQn, PreemptionPriority, SubPriority);
	HAL_NVIC_EnableIRQ(TIMx_IRQn);
	__HAL_TIM_CLEAR_FLAG(htim, TIM_FLAG_UPDATE);
	__HAL_TIM_ENABLE_IT(htim, TIM_FLAG_UPDATE);
	__HAL_TIM_ENABLE(htim);
	return htim;
}


/**
 * @brief 使能定时器时钟
 * @param  TIMx:定时器地址
 * @retval 无
*/
static void timer_clock_enable(TIM_TypeDef *TIMx) {
		   // Enable TIM clock
	SH_ASSERT(IS_TIM_PERIPH(TIMx));
#if defined(TIM1_BASE)
    if (TIMx == TIM1) {
        __HAL_RCC_TIM1_CLK_ENABLE();
    }
#endif
#if defined(TIM2_BASE)
    if (TIMx == TIM2) {
        __HAL_RCC_TIM2_CLK_ENABLE();
    }
#endif
#if defined(TIM3_BASE)
    if (TIMx == TIM3) {
        __HAL_RCC_TIM3_CLK_ENABLE();
    }
#endif
#if defined(TIM4_BASE)
    if (TIMx == TIM4) {
        __HAL_RCC_TIM4_CLK_ENABLE();
    }
#endif
#if defined(TIM5_BASE)
    if (TIMx == TIM5) {
        __HAL_RCC_TIM5_CLK_ENABLE();
    }
#endif
#if defined(TIM8_BASE)
    if (TIMx == TIM8) {
        __HAL_RCC_TIM8_CLK_ENABLE();
    }
#endif
#if defined(TIM9_BASE)
    if (TIMx == TIM9) {
        __HAL_RCC_TIM9_CLK_ENABLE();
    }
#endif
#if defined(TIM10_BASE)
    if (TIMx == TIM10) {
        __HAL_RCC_TIM10_CLK_ENABLE();
    }
#endif
#if defined(TIM11_BASE)
    if (TIMx == TIM11) {
        __HAL_RCC_TIM11_CLK_ENABLE();
    }
#endif
#if defined(TIM12_BASE)
    if (TIMx == TIM12) {
        __HAL_RCC_TIM12_CLK_ENABLE();
    }
#endif
#if defined(TIM13_BASE)
    if (TIMx == TIM13) {
        __HAL_RCC_TIM13_CLK_ENABLE();
    }
#endif
#if defined(TIM14_BASE)
    if (TIMx == TIM14) {
        __HAL_RCC_TIM14_CLK_ENABLE();
    }
#endif
#if defined(TIM15_BASE)
    if (TIMx == TIM15) {
        __HAL_RCC_TIM15_CLK_ENABLE();
    }
#endif
#if defined(TIM16_BASE)
    if (TIMx == TIM16) {
        __HAL_RCC_TIM16_CLK_ENABLE();
    }
#endif
#if defined(TIM17_BASE)
    if (TIMx == TIM17) {
        __HAL_RCC_TIM17_CLK_ENABLE();
    }
#endif
#if defined(TIM18_BASE)
    if (TIMx == TIM18) {
        __HAL_RCC_TIM18_CLK_ENABLE();
    }
#endif
#if defined(TIM19_BASE)
    if (TIMx == TIM19) {
        __HAL_RCC_TIM19_CLK_ENABLE();
    }
#endif
#if defined(TIM20_BASE)
    if (TIMx == TIM20) {
        __HAL_RCC_TIM20_CLK_ENABLE();
    }
#endif
#if defined(TIM21_BASE)
    if (TIMx == TIM21) {
        __HAL_RCC_TIM21_CLK_ENABLE();
    }
#endif
#if defined(TIM22_BASE)
    if (TIMx == TIM22) {
        __HAL_RCC_TIM22_CLK_ENABLE();
    }
#endif

}
/**
 * @brief 关闭定时器时钟
 * @param  TIMx:定时器地址
 * @retval 无
*/
static void timer_clock_disable(TIM_TypeDef *TIMx) {
	SH_ASSERT(IS_TIM_PERIPH(TIMx));

	#if defined(TIM1_BASE)
    if (TIMx == TIM1) {
        __HAL_RCC_TIM1_CLK_DISABLE();
    }
#endif
#if defined(TIM2_BASE)
    if (TIMx == TIM2) {
        __HAL_RCC_TIM2_CLK_DISABLE();
    }
#endif
#if defined(TIM3_BASE)
    if (TIMx == TIM3) {
        __HAL_RCC_TIM3_CLK_DISABLE();
    }
#endif
#if defined(TIM4_BASE)
    if (TIMx == TIM4) {
        __HAL_RCC_TIM4_CLK_DISABLE();
    }
#endif
#if defined(TIM5_BASE)
    if (TIMx == TIM5) {
        __HAL_RCC_TIM5_CLK_DISABLE();
    }
#endif
#if defined(TIM8_BASE)
    if (TIMx == TIM8) {
        __HAL_RCC_TIM8_CLK_DISABLE();
    }
#endif
#if defined(TIM9_BASE)
    if (TIMx == TIM9) {
        __HAL_RCC_TIM9_CLK_DISABLE();
    }
#endif
#if defined(TIM10_BASE)
    if (TIMx == TIM10) {
        __HAL_RCC_TIM10_CLK_DISABLE();
    }
#endif
#if defined(TIM11_BASE)
    if (TIMx == TIM11) {
        __HAL_RCC_TIM11_CLK_DISABLE();
    }
#endif
#if defined(TIM12_BASE)
    if (TIMx == TIM12) {
        __HAL_RCC_TIM12_CLK_DISABLE();
    }
#endif
#if defined(TIM13_BASE)
    if (TIMx == TIM13) {
        __HAL_RCC_TIM13_CLK_DISABLE();
    }
#endif
#if defined(TIM14_BASE)
    if (TIMx == TIM14) {
        __HAL_RCC_TIM14_CLK_DISABLE();
    }
#endif
#if defined(TIM15_BASE)
    if (TIMx == TIM15) {
        __HAL_RCC_TIM15_CLK_DISABLE();
    }
#endif
#if defined(TIM16_BASE)
    if (TIMx == TIM16) {
        __HAL_RCC_TIM16_CLK_DISABLE();
    }
#endif
#if defined(TIM17_BASE)
    if (TIMx == TIM17) {
        __HAL_RCC_TIM17_CLK_DISABLE();
    }
#endif
#if defined(TIM18_BASE)
    if (TIMx == TIM18) {
        __HAL_RCC_TIM18_CLK_DISABLE();
    }
#endif
#if defined(TIM19_BASE)
    if (TIMx == TIM19) {
        __HAL_RCC_TIM19_CLK_DISABLE();
    }
#endif
#if defined(TIM20_BASE)
    if (TIMx == TIM20) {
        __HAL_RCC_TIM20_CLK_DISABLE();
    }
#endif
#if defined(TIM21_BASE)
    if (TIMx == TIM21) {
        __HAL_RCC_TIM21_CLK_DISABLE();
    }
#endif
#if defined(TIM22_BASE)
    if (TIMx == TIM22) {
        __HAL_RCC_TIM22_CLK_DISABLE();
    }
#endif
}
/**
 * @brief  启动或关闭指定定时器的时钟
 * @param  TIMx:定时器地址
 * @param  NewState: ENABLE启动，DISABLE关闭
 * @retval 无
 */
void timer_clock_cmd(TIM_TypeDef *TIMx, FunctionalState NewState) {
	SH_ASSERT(IS_TIM_PERIPH(TIMx));
	if(NewState == ENABLE){
		timer_clock_enable(TIMx);
	}else{
		timer_clock_disable(TIMx);
	}
}

/*快速求平方根*/
static float Qsqrt(float number) {
	long i;
	float x2, y;
	const float threehalfs = 1.5f;
	x2 = number * 0.5f;
	y = number;
	i = *(long*) &y;
	i = 0x5f3759df - (i >> 1);
	y = *(float*) &i;
	y = y * (threehalfs - (x2 * y * y));
	y = y * (threehalfs - (x2 * y * y));
	return 1.0f / y;
}

/**
 * @brief  将定时中断频率转换为重装值和时钟分频值
 * @param  freq: 中断频率(Hz)
 * @param  clock: 定时器时钟
 * @param  *period: 重装值地址
 * @param  *prescaler: 时钟分频值地址
 * @retval 误差值(Hz)
 */
static int32_t timer_freq_to_arr_prescaler(uint32_t freq, uint32_t clock,
		uint16_t *period, uint16_t *prescaler) {
	uint32_t prodect;
	uint16_t psc, arr;
	uint16_t max_error = 0xFFFF;

	if (freq == 0 || freq > clock)
		goto failed;

	/*获取arr和psc目标乘积*/
	prodect = clock / freq;

	/*从prodect的平方根开始计算*/
	psc = Qsqrt(prodect);

	/*遍历，使arr*psc足够接近prodect*/
	for (; psc > 1; psc--) {
		for (arr = psc; arr < 0xFFFF; arr++) {
			/*求误差*/
			int32_t newerr = arr * psc - prodect;
			newerr = CLOCK_ABS(newerr);
			if (newerr < max_error) {
				/*保存最小误差*/
				max_error = newerr;
				/*保存arr和psc*/
				*period = arr;
				*prescaler = psc;
				/*最佳*/
				if (max_error == 0)
					goto success;
			}
		}
	}

	/*计算成功*/
	success: return (freq - clock / ((*period) * (*prescaler)));

	/*失败*/
	failed: return (freq - clock);
}

/**
 * @brief  将定时中断时间转换为重装值和时钟分频值
 * @param  time: 中断时间(微秒)
 * @param  clock: 定时器时钟
 * @param  *period: 重装值地址
 * @param  *prescaler: 时钟分频值地址
 * @retval 无
 */
static void timer_time_to_arr_prescaler(uint32_t time, uint32_t clock, uint16_t *period,
		uint16_t *prescaler) {
	uint32_t cyclesPerMicros = clock / 1000000U;
	uint32_t prodect = time * cyclesPerMicros;
	uint16_t arr, psc;

	if (prodect < cyclesPerMicros * 30) {
		arr = 10;
		psc = prodect / arr;
	} else if (prodect < 65535 * 1000) {
		arr = prodect / 1000;
		psc = prodect / arr;
	} else {
		arr = prodect / 20000;
		psc = prodect / arr;
	}
	*period = arr;
	*prescaler = psc;
}

/*
 * @brief  定时中断处理函数
 * @note   所有的定时器中断都进入到这里。
 * @param  TIMx：定时器地址
 * @retval 无
 */
void TIMx_IRQnHadler(TIM_TypeDef *TIMx) {
	TIMER_Type TIMERx;
	TIM_HandleTypeDef *htim;
	SH_ASSERT(IS_TIM_PERIPH(TIMx));

	TIMERx = timer_get_timer(TIMx);
	htim = timer_get_tim_handler(TIMx);
	if (__HAL_TIM_GET_FLAG(htim, TIM_FLAG_UPDATE) != RESET)
  	{
    	if (__HAL_TIM_GET_IT_SOURCE(htim, TIM_IT_UPDATE) != RESET)
    	{
			__HAL_TIM_CLEAR_IT(htim, TIM_IT_UPDATE);
			if (TIMx_Function[TIMERx])
			{
				TIMx_Function[TIMERx]();
			}
    	}
  	}
}
#if USE_SH_IRQ_ALL || USE_SH_IRQ_TIM_ALL || USE_SH_IRQ_TIM1 || USE_SH_IRQ_TIM10
/**
 * @brief  定时中断入口，定时器1,定时器10
 * @param  无
 * @retval 无
 */
void TIM1_UP_TIM10_IRQHandler(void)   //TIM1中断
{
	TIMx_IRQnHadler(TIM1);
	TIMx_IRQnHadler(TIM10);
}
#endif

#if USE_SH_IRQ_ALL || USE_SH_IRQ_TIM_ALL || USE_SH_IRQ_TIM2
/**
 * @brief  定时中断入口，定时器2
 * @param  无
 * @retval 无
 */
void TIM2_IRQHandler(void) {
	TIMx_IRQnHadler(TIM2);
}
#endif

#if USE_SH_IRQ_ALL || USE_SH_IRQ_TIM_ALL || USE_SH_IRQ_TIM3
/**
 * @brief  定时中断入口，定时器3
 * @param  无
 * @retval 无
 */
void TIM3_IRQHandler(void) {
	TIMx_IRQnHadler(TIM3);
}
#endif

#if USE_SH_IRQ_ALL || USE_SH_IRQ_TIM_ALL || USE_SH_IRQ_TIM4
/**
 * @brief  定时中断入口，定时器4
 * @param  无
 * @retval 无
 */
void TIM4_IRQHandler(void) {
	TIMx_IRQnHadler(TIM4);
}
#endif

#if USE_SH_IRQ_ALL || USE_SH_IRQ_TIM_ALL || USE_SH_IRQ_TIM5
/**
 * @brief  定时中断入口，定时器5
 * @param  无
 * @retval 无
 */
void TIM5_IRQHandler(void) {
	TIMx_IRQnHadler(TIM5);
}
#endif

#if USE_SH_IRQ_ALL || USE_SH_IRQ_TIM_ALL || USE_SH_IRQ_TIM6
/**
 * @brief  定时中断入口，定时器6
 * @param  无
 * @retval 无
 */
void TIM6_DAC_IRQHandler(void) {
	TIMx_IRQnHadler(TIM6);
}
#endif

#if USE_SH_IRQ_ALL || USE_SH_IRQ_TIM_ALL || USE_SH_IRQ_TIM7
/**
 * @brief  定时中断入口，定时器7
 * @param  无
 * @retval 无
 */
void TIM7_IRQHandler(void) {
	TIMx_IRQnHadler(TIM7);
}
#endif

#if USE_SH_IRQ_ALL || USE_SH_IRQ_TIM_ALL || USE_SH_IRQ_TIM8 || USE_SH_IRQ_TIM13
/**
 * @brief  定时中断入口，定时器8
 * @param  无
 * @retval 无
 */
void TIM8_UP_TIM13_IRQHandler(void) {
	TIMx_IRQnHadler(TIM8);
	TIMx_IRQnHadler(TIM13);
}
#endif

#if USE_SH_IRQ_ALL || USE_SH_IRQ_TIM_ALL || USE_SH_IRQ_TIM9
/**
 * @brief  定时中断入口，定时器9
 * @param  无
 * @retval 无
 */
void TIM1_BRK_TIM9_IRQHandler(void) {
	TIMx_IRQnHadler(TIM9);
}
#endif

#if USE_SH_IRQ_ALL || USE_SH_IRQ_TIM_ALL || USE_SH_IRQ_TIM11
/**
 * @brief  定时中断入口，定时器11
 * @param  无
 * @retval 无
 */
void TIM1_TRG_COM_TIM11_IRQHandler(void) {
	TIMx_IRQnHadler(TIM11);
}
#endif

#if USE_SH_IRQ_ALL || USE_SH_IRQ_TIM_ALL || USE_SH_IRQ_TIM12
/**
 * @brief  定时中断入口，定时器12
 * @param  无
 * @retval 无
 */
void TIM8_BRK_TIM12_IRQHandler(void) {
	TIMx_IRQnHadler(TIM12);
}
#endif

#if USE_SH_IRQ_ALL || USE_SH_IRQ_TIM_ALL || USE_SH_IRQ_TIM14
/**
 * @brief  定时中断入口，定时器14
 * @param  无
 * @retval 无
 */
void TIM8_TRG_COM_TIM14_IRQHandler(void) {
	TIMx_IRQnHadler(TIM14);
}
#endif

#if 0 /* 中断回调*/
/*
 * @brief  中断回调函数
 * @note
 * @param
 * @retval 无
 */
#if USE_HAL_TIM_REGISTER_CALLBACKS == 1
void HAL_TIM_register_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
#else
#warning "优先使用注册回调函数"
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
#endif
{
	TIMER_Type TIMERx;
	TIMERx = timer_get_timer(htim->Instance);
	if (TIMx_Function[TIMERx])
	{
		TIMx_Function[TIMERx]();
	}
}

#if USE_SH_IRQ_ALL || USE_SH_IRQ_TIM_ALL || USE_SH_IRQ_TIM1 || USE_SH_IRQ_TIM10
/**
 * @brief  定时中断入口，定时器1,定时器10
 * @param  无
 * @retval 无
 */
void TIM1_UP_TIM10_IRQHandler(void)   //TIM1中断
{
	HAL_TIM_IRQHandler(&shtim1);
	HAL_TIM_IRQHandler(&shtim10);
}
#endif

#if USE_SH_IRQ_ALL || USE_SH_IRQ_TIM_ALL || USE_SH_IRQ_TIM2
/**
 * @brief  定时中断入口，定时器2
 * @param  无
 * @retval 无
 */
void TIM2_IRQHandler(void) {
	HAL_TIM_IRQHandler(&shtim2);
}
#endif

#if USE_SH_IRQ_ALL || USE_SH_IRQ_TIM_ALL || USE_SH_IRQ_TIM3
/**
 * @brief  定时中断入口，定时器3
 * @param  无
 * @retval 无
 */
void TIM3_IRQHandler(void) {
	HAL_TIM_IRQHandler(&shtim3);
}
#endif

#if USE_SH_IRQ_ALL || USE_SH_IRQ_TIM_ALL || USE_SH_IRQ_TIM4
/**
 * @brief  定时中断入口，定时器4
 * @param  无
 * @retval 无
 */
void TIM4_IRQHandler(void) {
	HAL_TIM_IRQHandler(&shtim4);
}
#endif

#if USE_SH_IRQ_ALL || USE_SH_IRQ_TIM_ALL || USE_SH_IRQ_TIM5
/**
 * @brief  定时中断入口，定时器5
 * @param  无
 * @retval 无
 */
void TIM5_IRQHandler(void) {
	HAL_TIM_IRQHandler(&shtim5);
}
#endif

#if USE_SH_IRQ_ALL || USE_SH_IRQ_TIM_ALL || USE_SH_IRQ_TIM6
/**
 * @brief  定时中断入口，定时器6
 * @param  无
 * @retval 无
 */
void TIM6_DAC_IRQHandler(void) {
	HAL_TIM_IRQHandler(&shtim6);
}
#endif

#if USE_SH_IRQ_ALL || USE_SH_IRQ_TIM_ALL || USE_SH_IRQ_TIM7
/**
 * @brief  定时中断入口，定时器7
 * @param  无
 * @retval 无
 */
void TIM7_IRQHandler(void) {
	HAL_TIM_IRQHandler(&shtim7);
}
#endif

#if USE_SH_IRQ_ALL || USE_SH_IRQ_TIM_ALL || USE_SH_IRQ_TIM8 || USE_SH_IRQ_TIM13
/**
 * @brief  定时中断入口，定时器8
 * @param  无
 * @retval 无
 */
void TIM8_UP_TIM13_IRQHandler(void) {
	HAL_TIM_IRQHandler(&shtim8);
	HAL_TIM_IRQHandler(&shtim13);
}
#endif

#if USE_SH_IRQ_ALL || USE_SH_IRQ_TIM_ALL || USE_SH_IRQ_TIM9
/**
 * @brief  定时中断入口，定时器9
 * @param  无
 * @retval 无
 */
void TIM1_BRK_TIM9_IRQHandler(void) {
	HAL_TIM_IRQHandler(&shtim9);
}
#endif

#if USE_SH_IRQ_ALL || USE_SH_IRQ_TIM_ALL || USE_SH_IRQ_TIM11
/**
 * @brief  定时中断入口，定时器11
 * @param  无
 * @retval 无
 */
void TIM1_TRG_COM_TIM11_IRQHandler(void) {
	HAL_TIM_IRQHandler(&shtim11);
}
#endif

#if USE_SH_IRQ_ALL || USE_SH_IRQ_TIM_ALL || USE_SH_IRQ_TIM12
/**
 * @brief  定时中断入口，定时器12
 * @param  无
 * @retval 无
 */
void TIM8_BRK_TIM12_IRQHandler(void) {
	HAL_TIM_IRQHandler(&shtim12);
}
#endif

#if USE_SH_IRQ_ALL || USE_SH_IRQ_TIM_ALL || USE_SH_IRQ_TIM14
/**
 * @brief  定时中断入口，定时器14
 * @param  无
 * @retval 无
 */
void TIM8_TRG_COM_TIM14_IRQHandler(void) {
	HAL_TIM_IRQHandler(&shtim14);
}
#endif
#endif
