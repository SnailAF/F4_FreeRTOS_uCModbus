#ifndef __MBREG_H
#define __MBREG_H

#include <stdint.h>

#define VENDOR_NAME "robot123"
#define PRODUCT_CODE "test11"
#define MAJOR_MINOR_REVISION "S123"

/* -----------------------Slave Defines -------------------------------------*/
/* holding register address */
#define REG_HOLDING_START 				(40000) //0x0000
#define REG_HOLDING_NREGS 				(0x100)
/* colis register address */
#define REG_COILS_START 				(0)//0x0000
#define REG_COILS_NREGS 				(32)
/* discrete register address */
#define REG_DISCRETE_START 				(10000)//0x0000
#define REG_DISCRETE_NREGS 				(16)
/* input register address */
#define REG_INPUT_START 				(30000)//0x0000
#define REG_INPUT_NREGS 				(0x100)


extern uint16_t 	usSHoldBuf[];
extern uint16_t     usSInBuf[]; 	    //(模拟)输入缓存
extern uint8_t      usSCoilBuf[];	    //输出IO缓存
extern uint8_t      usSDiscInBuf[];	    //离散输入缓存

#define IS_MB_DISCRETE_REG(x)	(x>=REG_DISCRETE_START && (x-REG_DISCRETE_START)<REG_DISCRETE_NREGS)
#define IS_MB_COIL_REG(x)		(x>=REG_COILS_START && (x-REG_COILS_START)<REG_COILS_NREGS)
#define IS_MB_INPUT_REG(x)		(x>=REG_INPUT_START && (x-REG_INPUT_START)<REG_INPUT_NREGS)
#define IS_MB_HOLDING_REG(x)	(x>=REG_HOLDING_START && (x-REG_HOLDING_START)<REG_HOLDING_NREGS)

#endif

