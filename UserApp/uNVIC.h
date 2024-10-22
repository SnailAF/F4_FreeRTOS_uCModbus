#ifndef __UNVIC_H
#define __UNVIC_H

/*	此宏定义是用来配置 FreeRTOS 用到的 SysTick 中断和 PendSV 中断的优先级。在 NVIC 分组设置为
	4 的情况下，此宏定义的范围就是 0-15，即专门配置抢占优先级。这里配置为了 0x0f，即 SysTick
	和 PendSV 都是配置为了最低优先级，实际项目中也建议大家配置最低优先级即可
*/
#define ufRTOS_LOWEST_INTERRUPT_PRIORITY			(15)

/*	定义了受 FreeRTOS 管理的最高优先级中断。简单的说就是允许用户在这个中断服务程序里面调用
	FreeRTOS 的 API 的最高优先级。 设置 NVIC 的优先级分组为 4 的情况下。 配置
	configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 为 0x01 表示用户可以在抢占式优先级为 1
	到 15 的中断里面调用 FreeRTOS 的 API 函数，抢占式优先级为 0 的中断里面是不允许调用的
*/
#define ufRTOS_MAX_SYSCALL_INTERRUPT_PRIORITY	(1)


//中断优先级
/*	CAN1 定时器中断优先级	 */
#define  ufCANOPEN_TIM_PRIORITY				(3)

/*	CAN1 接收中断优先级	 */
#define  ufSH_CAN_RX0_IRQn_PRIORITY			(2)

/* SH库通用定时器中断 */
#define ufSH_TIM_IRQn_PRIORITY				(4)

/* SH库通用串口中断 */
#define ufSH_UART_IRQn_PRIORITY				(5)

/*	ADC DMA 传输完成中断优先级	 */
#define ufDMA1CH1adc_INTERRUPT_PRIORITY		(10)

/*	CAN1 接收中断优先级	 */
#define  ufCAN1_RX0_IRQn_PRIORITY			(2)



//任务优先级
/*	用户空闲任务         */
#define ufTask_uIdle_TASK_PRIO				(1)
/*	CANOpen 用户任务      */
#define ufTask_CANOPEN_PRIO					(2)
/* modbus 从机 */
#define ufTask_MODBUS_SALVE_RIO			    (3)
/* modbus 主机 */
#define ufTask_MODBUS_MASTER_RIO			(4)
/* modbus 主机应用 */
#define ufTask_MODBUS_MASTER_APP_RIO		(5)
/* 用户任务 */
#define ufUSERAPP_TASK_PRIO					(6)
/* mavlink 通信 */
#define ufMAVLINK_TASK_PRIO					(7)
/* 机器人运动控制*/
#define ufTask_RobotCtrl_PRIO				(8)
/* 安全任务 */
#define ufSAFE_TASK_PRIO					(9)
/*	CAN 发送任务          */
#define ufTask_CANTX_PRIO					(10)
/*	CAN 接收任务          */
#define ufTask_CANRX_PRIO					(11)
/*	mpu6050 数据处理任务	 */
#define  ufMPU6050_TASK_PRIO				(12)

#endif
