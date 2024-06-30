/*
 * sd_file_create.c
 *
 *  Created on: 2024/06/01, 18:21:50
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

uint16_t sd_file_create(char *path)
{
    UINT status = FX_SUCCESS;
    status = fx_file_create(FX_SD_MEDIA, path);
    if (status != FX_SUCCESS)
    {
        if (status == FX_ALREADY_CREATED)
        {
            printf("File [%s] already exists\n", path);
        }
    }
    else
    {
        status = fx_media_flush(FX_SD_MEDIA);
        if (status != FX_SUCCESS)
        {
            printf("Flush failed\n");
            PRINT_FX_ERROR(status);
        }
    }
    return status;
}
