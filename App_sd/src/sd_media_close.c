/*
 * sd_media_close.c
 *
 *  Created on: 2024/06/01, 18:27:18
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
 * @brief 关闭SD卡,然后可以安全弹出
 * @param void
 * @return 0成功关闭；其他FileX错误
 */
uint16_t sd_close(void)
{
    UINT status = 0;
    /* 关闭前先将数据刷新。似乎在 close 里刷新了，但为了保险，手动刷新 */
    status = fx_media_flush(FX_SD_MEDIA);
    if (status != FX_SUCCESS)
    {
        printf("Flush SD media failed\n");
        FX_POST_ERROR_RETURN(status);
    }

    /* 关闭SD */
    status = fx_media_close(FX_SD_MEDIA);
    if (status != FX_SUCCESS)
    {
        printf("Close SD media failed\n");
        FX_POST_ERROR(status);
    }
    return status;
}
