/*
 * sd_meida_check.c
 *
 *  Created on: 2024-06-02T18:31:08+08:00
 *      Author: phy1335
 *        Note: 需要内存>=总簇数/1024 + 1 + 1024
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "sd_api.h"
#ifdef PRINT_TOTAL_TIME
#include "GetRunTime.h"
#endif
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
// #define ENABLE_SD_CHECK
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
#ifdef ENABLE_SD_CHECK
__section(".ccmram") uint8_t sratch_memory[32 * 1024] = {0};
#endif /* ENABLE_SD_CHECK */

uint16_t sd_media_check()
{
    uint16_t status = 0;
#ifdef ENABLE_SD_CHECK

    ULONG detected_errors;

#ifdef PRINT_TOTAL_TIME
    uint32_t t1 = 0, t2 = 0;
    t1 = GetRunTime();
#endif

    /* 停止SD卡的活动 */
    status = sd_abort();
    if (status != FX_SUCCESS)
    {
        printf("Abort SD media failed\n");
        FX_POST_ERROR_RETURN(status);
    }

    /* 再次打开SD卡 */
    status = sd_open();
    if (status != FX_SUCCESS)
    {
        printf("Open SD media failed\n");
        FX_POST_ERROR_RETURN(status);
    }

    /* 开始检查 */
    status = fx_media_check(FX_SD_MEDIA, sratch_memory, sizeof(sratch_memory),
                            FX_FAT_CHAIN_ERROR | FX_DIRECTORY_ERROR | FX_LOST_CLUSTER_ERROR,
                            &detected_errors);
    if (status == FX_SUCCESS && detected_errors == 0)
    {
        printf("SD no error\n");
    }
    else
    {
        printf("Total Errors %ld\n", detected_errors);
        PRINT_FX_ERROR(status);
    }

#ifdef PRINT_TOTAL_TIME
    t2 = GetRunTime();
    printf("===Total Time %ld ms\n", t2 - t1);
#endif

#endif /* ENABLE_SD_CHECK */

    return status;
}
