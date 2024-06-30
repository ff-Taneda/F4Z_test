/*
 * sd_dice_default.c
 *
 *  Created on: 2024/06/01, 18:19:43
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

uint16_t sd_dir_default_set(char *path)
{
    return fx_directory_default_set(FX_SD_MEDIA, path);
}

uint16_t sd_dir_default_get(char *path)
{
    char *default_dir = FX_NULL;
    uint16_t status = fx_directory_default_get(FX_SD_MEDIA, &default_dir);
    if (status != FX_SUCCESS)
    {
        printf("Get default directory failed\n", path);
        FX_POST_ERROR_RETURN(status);
    }

    if (strcmp(default_dir, "") == 0)
    {
        // strcpy(path, "/");
        *path = '/';
        *(path + 1) = '\0';
    }
    else
    {
        strncpy(path, default_dir, FX_MAXIMUM_PATH);
    }

    return status;
}
