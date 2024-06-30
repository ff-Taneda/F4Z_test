/*
 * sd_com_move.c
 *
 *  Created on: 2024/06/01, 18:28:13 
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
 * @brief mv，可以用来重命名或者指定路径时移动
 * @param old 原来的名字/路径
 * @param new 新的名字/路径
 * @return
 */
uint16_t sd_com_move(char *old, char *new)
{
    UINT status = 0;

    status = fx_file_rename(FX_SD_MEDIA, old, new);
    if (status != FX_SUCCESS)
    {
        if (status == FX_NOT_A_FILE)
        {
            status = fx_directory_rename(FX_SD_MEDIA, old, new);
        }
    }

    if (status != FX_SUCCESS)
    {
        printf("Move [%s] to [%s] failed\n", old, new);
        PRINT_FX_ERROR(status);
        return status;
    }
    else
    {
        status = fx_media_flush(FX_SD_MEDIA);
        if (status != FX_SUCCESS)
        {
            return status;
        }
        printf("Move [%s] to [%s] successfully\n", old, new);
    }

    return status;
}
