## FreeRTOS_uCModbus（STM32F407 + uC/Modbus）

本工程基于 **STM32F407VETx**，使用 **STM32 HAL + FreeRTOS**，移植 **Silicon Labs/Micrium uC/Modbus**（支持 Modbus RTU/ASCII，已适配 FreeRTOS）。
### 与FreeModbus对比
相对于FreeModbus，ucmodbus不但具有官方的主机模式，并且支持多主机和多从机模式。

### 特性

- **同时支持 Modbus Master / Slave**（见 `Middlewares/Third_Party/uC-Modbus/Cfg/Template/mb_cfg.h`）
- **RTU + ASCII** 均开启（默认使用 RTU）
- FreeRTOS 适配：Slave 侧使用队列 + 专用 RX 任务；Master 侧使用信号量等待响应（见 `Middlewares/Third_Party/uC-Modbus/OS/FreeRTOS/mb_os.c`）
- STM32F4 串口 BSP 适配：基于 `Middlewares/shLib_F4` 的串口抽象（见 `Middlewares/Third_Party/uC-Modbus/Ports/STM32F4/mb_bsp.c`）

### 硬件与软件环境

- **MCU**：STM32F407VETx（Cortex-M4F）
- **调试下载**：ST-LINK（任意可用版本）
- **通信**：
  - RS485：示例定义了 2 路方向控制引脚（`RS4851_RW=PD4`、`RS4852_RW=PD10`）
  - 还定义了 UART/RS232 口（具体映射见下文）
- **IDE**：STM32CubeIDE（本仓库包含 Eclipse 工程文件：`.project` / `.cproject`）

> 说明：仓库中未包含 `.ioc`，按现有 STM32CubeIDE 工程直接导入即可。

### 快速开始（STM32CubeIDE）

- **详细移植说明**：见 `移植说明.md`

- **导入工程**
  - 打开 STM32CubeIDE
  - `File` → `Import...` → `Existing Projects into Workspace`
  - 选择本仓库根目录，勾选工程 `FreeRTOS_uCModbus`
- **编译**
  - 选择 `Debug` 或 `Release` 配置
  - 点击构建（锤子图标）或 `Project` → `Build Project`
- **下载/调试**
  - 连接 ST-LINK
  - `Run` / `Debug` 启动下载

### 默认示例逻辑（来自 `UserApp/main.c`）

工程里默认调用了 `MB_Init(1000)` 初始化 Modbus（RTU 定时基准），并在任务中配置了多个通道（`MB_CfgCh()`）：

- **Slave 通道（RTU）**
  - NodeAddr：`1`
  - 端口：`serial_RS4851`（在 `UserApp/BSP_PinNames.h` 中定义为 `serial2`，通常对应 USART2）
  - 波特率：`9600`
- **Master 通道（RTU）**
  - 端口：`serial_UART`（在 `UserApp/BSP_PinNames.h` 中定义为 `serial1`，通常对应 USART1）
  - 波特率：`19200`
  - 超时：`100`（注释写的是 1 秒；具体单位以 `MB_Init()`/OS 层实现为准）
- **Master 通道（RTU，RS232）**
  - 端口：`serial_RS232`（在 `UserApp/BSP_PinNames.h` 中定义为 `serial4`，通常对应 UART4）
  - 波特率：`19200`

并且示例包含：

- `MBM_FC03_HoldingRegRd(pch, 1, 40000, buf, 10)`：从 **从站地址 1** 读取 **40000 起始** 的 10 个保持寄存器
- `MBM_FC16_HoldingRegWrN(pch_232, 1, 40010, buf_w, 10)`：向 **从站地址 1** 写入 **40010 起始** 的 10 个保持寄存器

> 这段逻辑非常适合做“串口 A 读、串口 B 写”的 Modbus 转发/桥接验证；你可以按需要修改从站地址、寄存器地址、波特率等。

### Modbus 配置（uC/Modbus）

主要配置文件（当前工程直接使用 Template 目录作为配置源）：

- **协议栈配置**：`Middlewares/Third_Party/uC-Modbus/Cfg/Template/mb_cfg.h`
  - `MODBUS_CFG_SLAVE_EN` / `MODBUS_CFG_MASTER_EN`：均为 `DEF_ENABLED`
  - `MODBUS_CFG_RTU_EN` / `MODBUS_CFG_ASCII_EN`：均为 `DEF_ENABLED`
  - `MODBUS_TIMER`：`TIM3`（RTU 定时器）
  - 常用功能码：FC01/02/03/04/05/06/08/15/16 使能
  - `MODBUS_CFG_FP_EN`：默认关闭（浮点寄存器功能关闭）
- **寄存器映射定义**：`Middlewares/Third_Party/uC-Modbus/Cfg/Template/mbreg.h`
- **寄存器读写实现（数据访问回调）**：`Middlewares/Third_Party/uC-Modbus/Cfg/Template/mb_data.c`

### 默认寄存器映射（Slave）

见 `Middlewares/Third_Party/uC-Modbus/Cfg/Template/mbreg.h`：

- **Holding Register**：40000 起，长度 `0x100`
- **Input Register**：30000 起，长度 `0x100`
- **Coils**：0 起，长度 32
- **Discrete Inputs**：10000 起，长度 16

### 串口与引脚（BSP）

见 `UserApp/BSP_PinNames.h`：

- **RS485 方向控制**
  - `RS4851_RW`：`PD4`
  - `RS4852_RW`：`PD10`
- **串口逻辑命名**
  - `serial_RS4851`：`serial2`（通常对应 USART2）
  - `serial_RS4852`：`serial3`（通常对应 USART3）
  - `serial_UART`：`serial1`（通常对应 USART1）
  - `serial_RS232`：`serial4`（通常对应 UART4）

> 串口底层初始化与中断挂接由 `Middlewares/shLib_F4` 提供（例如 `shUsart.c`），uC/Modbus 的 STM32F4 端口层会在 `MB_CommPortCfg()` 中调用 `serial_begin_cof()` 并挂接 Rx/Tx 中断回调。

### 目录结构说明

- `Core/`：STM32Cube 生成的核心代码（启动文件、HAL 回调、FreeRTOS glue 等）
- `Drivers/`：CMSIS + STM32F4 HAL Driver
- `Middlewares/Third_Party/FreeRTOS/`：FreeRTOS 源码
- `Middlewares/Third_Party/uC-Modbus/`：uC/Modbus 源码 + OS/端口层 + 配置模板
- `Middlewares/shLib_F4/`：项目使用的外设抽象层（串口/定时器/GPIO 等）
- `UserApp/`：用户应用代码（BSP 引脚定义、示例逻辑等）
- `STM32F407VETX_FLASH.ld` / `STM32F407VETX_RAM.ld`：链接脚本

### 常见问题（FAQ）

- **Q：我想修改 Modbus 定时器/功能码开关/通道数量？**
  - A：优先修改 `Middlewares/Third_Party/uC-Modbus/Cfg/Template/mb_cfg.h`，例如 `MODBUS_TIMER`、`MODBUS_CFG_MAX_CH`、各功能码 `MODBUS_CFG_FCxx_EN`。
- **Q：我想改寄存器地址范围与读写行为？**
  - A：修改 `mbreg.h`（地址范围）与 `mb_data.c`（回调实现）。

### 第三方组件与许可证

- **uC/Modbus**：`Middlewares/Third_Party/uC-Modbus/`（仓库内含 `LICENSE` / `NOTICE`，源码头部声明为 Apache-2.0）
- **FreeRTOS**：`Middlewares/Third_Party/FreeRTOS/`（遵循其自带许可证文件）
- **CMSIS/HAL**：`Drivers/`（遵循 ST 提供的 License 文件）

