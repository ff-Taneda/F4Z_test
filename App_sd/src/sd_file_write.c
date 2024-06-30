/*
 * sd_file_write.c
 *
 *  Created on: 2024/06/01, 18:21:09
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
 * @brief 将数据写入到 SD 卡文件中，可以选择是否追加到文件末尾。
 * @param path char* 文件路径
 * @param buf 要写入的数据缓冲区
 * @param size 要写入的数据大小，单位为字节
 * @param append 追加模式。0为覆盖，1为追加
 * @return
 */
uint16_t sd_file_write(char *path, void *buf, uint32_t size, uint8_t append)
{
    FX_FILE file;
    UINT status = FX_SUCCESS;

    status = fx_file_open(FX_SD_MEDIA, &file, path, FX_OPEN_FOR_WRITE);
    if (status != FX_SUCCESS)
    {
        printf("Open [%s] failed\n", path);
        FX_POST_ERROR_RETURN(status);
    }

    if (append)
    {
        status = fx_file_relative_seek(&file, 0, FX_SEEK_END);
        if (status != FX_SUCCESS)
        {
            printf("Seek failed\n");
            FX_POST_ERROR_RETURN(status);
        }
    }

    status = fx_file_write(&file, buf, size);
    if (status != FX_SUCCESS)
    {
        printf("Write [%s] failed\n", path);
        FX_POST_ERROR_RETURN(status);
    }

    status = fx_file_close(&file);
    if (status != FX_SUCCESS)
    {
        printf("Close [%s] failed\n", path);
        FX_POST_ERROR_RETURN(status);
    }

    /* 是否需要手动刷新? */
    status = fx_media_flush(FX_SD_MEDIA);
    if (status != FX_SUCCESS)
    {
        printf("Flush failed\n");
        FX_POST_ERROR_RETURN(status);
    }
    return status;
}
