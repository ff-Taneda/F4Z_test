/*
 * GetRunTime.c
 *
 *  Created on: 2024-06-16 T18:43:53+08:00
 *      Author: phy1335
 *        Note:
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported functions --------------------------------------------------------*/
/**
 * @brief 获取当前运行时间。其中禁用中断。调用两次以获得运行时间
 * @param None
 * @return uint32_t 表示从系统启动开始经过的毫秒数。
 */
uint32_t GetRunTime(void)
{
    __disable_irq();
    __IO uint32_t t = HAL_GetTick();
    __enable_irq();
    return t;
}
