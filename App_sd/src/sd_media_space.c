/*
 * sd_media_space.c
 *
 *  Created on: 2024/06/01, 18:26:57
 *      Author: phy1335
 *        Note:
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "sd_api.h"
#include "sdio.h"
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * @brief 返回sd卡的各种空间信息
 * @param  opt 1 可用字节数
 *              2 已格式化字节数
 *              3 总字节数
 *              4 逻辑字节数 没用
 * @return
 */
uint64_t sd_space_get(uint8_t opt)
{

    FX_MEDIA *media_ptr = FX_SD_MEDIA;

    if (media_ptr == FX_NULL)
    {
        return 0;
    }

    uint64_t total_byte;      // 总字节
    uint64_t format_byte;     // 格式化字节
    uint64_t available_bytes; // 可用字节
    // uint64_t logic_byte;


    uint32_t available_clusters; // 可用簇
    // uint32_t sectors_per_cluster; // 簇扇区
    HAL_SD_CardInfoTypeDef cardInfo;

    // 暂时没用
    // sectors_per_cluster = media_ptr->fx_media_sectors_per_cluster;

    if (opt == 1) // 可用字节
    {
        // 获取可用簇
        available_clusters = media_ptr->fx_media_available_clusters;
        // 计算可用字节=可用簇*簇字节
        available_bytes = (ULONG64)available_clusters * (ULONG64)(media_ptr->fx_media_bytes_per_sector * media_ptr->fx_media_sectors_per_cluster);
        return available_bytes;
    }
    else if (opt == 2) // 格式化大小
    {
        // 计算格式化字节=总簇*簇字节
        format_byte = (ULONG64)media_ptr->fx_media_total_clusters * (ULONG64)(media_ptr->fx_media_bytes_per_sector * media_ptr->fx_media_sectors_per_cluster);
        return format_byte;
    }
    else if (opt == 3) // 总大小
    {

        HAL_SD_GetCardInfo(&hsd, &cardInfo); // 获取底层SD card数据
        total_byte = (uint64_t)cardInfo.BlockNbr * (uint64_t)cardInfo.BlockSize;
        return total_byte;
    }
    /* 逻辑大小 暂时没用 */
    else
    {
        // HAL_SD_GetCardInfo(&hsd, &cardInfo); // 获取底层SD card数据
        // logic_byte = (uint64_t)cardInfo.LogBlockNbr * (uint64_t)cardInfo.LogBlockSize;
        // return logic_byte;
        return 0;
    }
}
