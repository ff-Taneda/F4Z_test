/*
 * sd_dir_create.c
 *
 *  Created on: 2024/06/01, 18:19:08
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

uint16_t sd_dir_create(char *path)
{
    UINT status = FX_SUCCESS;
    status = fx_directory_create(FX_SD_MEDIA, path);
    if (status != FX_SUCCESS)
    {
        printf("Creat directory [%s] failed\n", path);
        FX_POST_ERROR_RETURN(status);
    }
    else
    {
        status = fx_media_flush(FX_SD_MEDIA);
        if (status != FX_SUCCESS)
        {
            printf("Flush failed\n");
            FX_POST_ERROR_RETURN(status);
        }
    }
    return status;
}
