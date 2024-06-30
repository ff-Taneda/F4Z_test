/*
 * sd_file_read.c
 *
 *  Created on: 2024/06/01, 18:21:33
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

uint16_t sd_file_read(char *path, void *buf, uint32_t request_size, uint32_t *actual_size)
{
    FX_FILE file;
    UINT status = FX_SUCCESS;

    status = fx_file_open(FX_SD_MEDIA, &file, path, FX_OPEN_FOR_READ);
    if (status != FX_SUCCESS)
    {
        if (status != FX_ACCESS_ERROR)
        {
            /* code */
        }
        
        printf("Open %s failed\n", path);
        FX_POST_ERROR_RETURN(status);
    }

    status = fx_file_read(&file, buf, request_size, actual_size);
    if (status != FX_SUCCESS)
    {
        printf("Read %s failed\n", path);
        FX_POST_ERROR_RETURN(status);
    }

    status = fx_file_close(&file);
    if (status != FX_SUCCESS)
    {
        printf("Close %s failed\n", path);
        FX_POST_ERROR_RETURN(status);
    }
    return status;
}
