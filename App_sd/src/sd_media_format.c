/*
 * sd_media_format.c
 *
 *  Created on: 2024/06/01, 18:25:09
 *      Author: phy1335
 *        Note:
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "fxsh_api.h"
#include "sd_api.h"
#include "string.h"
#include "fxsh.h"
#ifdef PRINT_TOTAL_TIME
#include "GetRunTime.h"
#endif
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * @brief 格式化SD卡，参数设为0为默认值
 * @param volume_name 卷名 最多11字符
 * @param sector_bytes 扇区大小 （以字节为单位） 0为512扇区
 * @param cluster_sectors 簇大小（以扇区为单位） 0为8个扇区簇大小
 * @param total_bytes 格式化容量（以字节为单位） 0为全部扇区
 * @return 格式化状态，0表示成功
 */
uint16_t sd_media_format(char *volume_name, uint16_t sector_bytes, uint16_t cluster_sectors, uint64_t total_bytes)
{
    UINT status = 0;

    CHAR volume_label[12] = {0};
    /* 检查卷名 */
    if (volume_name == NULL) // 设置默认 STM32SD
    {
        strncpy(volume_label, "STM32SD", 11);
    }
    else if (strlen(volume_name) > 11) // 太长了，超过11字符
    {
        printf("Volume_name is too long! It must be less then 11 bytes.\n");
        return 1;
    }
    else // 复制卷名
    {
        strncpy(volume_label, volume_name, 11);
    }

    UINT bytes_per_sector = 0; // 格式化扇区大小
    /* 检查扇区 */
    if (sector_bytes == 0) // 设为默认的512字节
    {
        bytes_per_sector = FX_STM32_SD_DEFAULT_SECTOR_SIZE;
    }
    else if (sector_bytes % 32 != 0) // 不是32的倍数
    {
        printf("The sector size needs to be a multiple of 32 bytes\n");
        printf("Normally 512 1024 2048 4096\n");
        printf("Default 512\n");
        return 1;
    }
    else if (sector_bytes > 4096) // 太大了
    {
        printf("The sector size should be less than 4096.\n");
        return 1;
    }
    else
    {
        bytes_per_sector = sector_bytes;
    }

    UINT sectors_per_cluster = 0; // 格式化簇大小
    /* 检查簇(群集) */
    if (cluster_sectors == 0) // 设为默认的8扇区簇
    {
        sectors_per_cluster = 8; // 8*512/1024=4K
    }
    else if (cluster_sectors > 128)
    {
        printf("The cluster size should be less than 128 sectors.\n");
        return 1;
    }
    else
    {
        sectors_per_cluster = cluster_sectors;
    }

    /* 检查容量 */
    ULONG total_sectors = 0; // 格式化容量
    ULONG64 sd_max_size = sd_space_get(3);
    if (total_bytes == 0) // 默认格式化全部空间
    {
        total_sectors = sd_max_size / bytes_per_sector;
    }
    else if (total_bytes > sd_max_size) // 格式化容量超出SD卡大小
    {
        printf("Format size exceeds actual size %lld > %lld\n", total_bytes, sd_max_size);
        return 1;
    }
    else // 计算格式化扇区数
    {
        total_sectors = total_bytes / bytes_per_sector;
    }
#ifdef PRINT_TOTAL_TIME
    uint32_t t1 = 0, t2 = 0;
    t1 = GetRunTime();
#endif
    status = fx_media_format(FX_SD_MEDIA,          // FX_MEDIA结构体
                             fx_stm32_sd_driver,  // IO驱动
                             0,                   // 自定义的信息结构体，可以在驱动里使用
                             (VOID *)sdio_buffer, // 缓存
                             sizeof(sdio_buffer), // 缓存大小
                             volume_label,        // 卷标 最多11字符
                             1,                   // FAT数量
                             32,                  // 根目录中目录的数目
                             0,                   // 启动扇区前隐藏的扇区数
                             total_sectors,       // 扇区总数
                             bytes_per_sector,    // 每个扇区的字节数
                             sectors_per_cluster, // 每个簇的扇区数
                             1,                   // 物理磁头的数       sd卡没有
                             1);                  // 每个磁道的扇区数   sd卡没有

    if (status != FX_SUCCESS)
    {
        printf("Format SD card failed!\n");
        PRINT_FX_ERROR(status);
    }
    else
    {
        printf("Format SD card successfully!\n");

#ifdef PRINT_TOTAL_TIME
        t2 = GetRunTime();
        printf("===Total Time %ld ms\n", t2 - t1);
#endif

        status = sd_open();
        if (status != FX_SUCCESS)
        {
            PRINT_FX_ERROR(status);
        }
        else
        {
            sd_volume_get(volume_label);
            printf("SD Card [%s]\n", volume_label);
            fxsh_execute("cd /");
            fxsh_execute("df -h");
        }
    }
    return status;
}
