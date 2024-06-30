/*
 * sd_media_volumn.c
 *
 *  Created on: 2024/06/01, 18:25:50
 *      Author: phy1335
 *        Note:
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "sd_api.h"
#include <string.h>
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * @brief 返回卷标
 * @param name 存放的卷标 最多11字节
 * @return 0成功;FileX返回值
 */
uint16_t sd_volume_get(char *name)
{
    UINT status = 0;
    // 尝试从root directory 获取卷标
    status = fx_media_volume_get(FX_SD_MEDIA, name, FX_DIRECTORY_SECTOR);
    if (status != FX_SUCCESS)
    {
        // 从boot sector获取卷标
        status = fx_media_volume_get(FX_SD_MEDIA, name, FX_BOOT_SECTOR);
        if (status != FX_SUCCESS)
        {
            printf("Get volume name failed\n");
            FX_POST_ERROR(status);
        }
    }
    return status;
}

/**
 * @brief 设置卷标
 * @param name 卷标，最多11字节
 * @return 0成功；FileX返回值
 */
uint16_t sd_volume_set(char *name)
{
    UINT status = 0;
    status = fx_media_volume_set(FX_SD_MEDIA, name);
    if (status != FX_SUCCESS)
    {
        printf("Set volume name [%s] failed!\n", name);
        FX_POST_ERROR(status);
    }
    return status;
}
