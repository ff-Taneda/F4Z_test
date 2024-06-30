/*
 * sd_file_attribute.c
 *
 *  Created on: 2024/06/01, 18:22:20 
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

uint16_t sd_file_set_time(char *path, uint16_t year, uint16_t month, uint16_t day, uint16_t hour, uint16_t minute, uint16_t second)
{
    UINT status = FX_SUCCESS;
    status = fx_file_date_time_set(FX_SD_MEDIA, path, year, month, day, hour, minute, second);
    if (status != FX_SUCCESS)
    {
        printf("Set file [%s] failed\n", path);
        FX_POST_ERROR(status);
    }
    else
    {
        status = fx_media_flush(FX_SD_MEDIA);
        if (status != FX_SUCCESS)
        {
            printf("Flush failed\n");
            FX_POST_ERROR(status);
        }
    }
    return status;
}
