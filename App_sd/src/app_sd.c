/*
 * app_sd.c
 *
 *  Created on: 2024 05 15, 23 54 52
 *      Author: phy1335
 *        Note:
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "sd_api.h"
#include "fxsh.h"
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__section(".ccmram") FX_MEDIA __sdio_disk;                               // FileX sd卡媒体结构体
__aligned(32) uint8_t sdio_buffer[16 * FX_STM32_SD_DEFAULT_SECTOR_SIZE]; // 16个扇区大小 8KB 32字节对齐的FileX sd卡buffer
__aligned(32) uint8_t sdio_test[32 * 1024] = {0};                        // 32KB
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

