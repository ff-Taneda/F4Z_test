/*
 * sd_meida_open.c
 *
 *  Created on: 2024/06/01, 18:25:24
 *      Author: phy1335
 *        Note:
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "sd_api.h"
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * @brief 打开活动的sd卡
 * @param none
 * @return 0成功;FileX返回值
 */
uint16_t sd_open(void)
{
    uint16_t status = fx_media_open(FX_SD_MEDIA,           // FX_MEDIA结构体
                                "STM32_SD_DISK",    // 打开后的名字
                                fx_stm32_sd_driver,   // IO驱动
                                0,                    // 自定义的信息结构体，可以在驱动里使用
                                (VOID *)sdio_buffer,  // 缓存
                                sizeof(sdio_buffer)); // 缓存大小
    return status;
}
