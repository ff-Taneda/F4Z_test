/*
 * sh_fx_init.c
 *
 *  Created on: 2024/05/25, 23:17:42
 *      Author: phy1335
 *        Note: 初始化
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "app_filex.h"
#include "sd_api.h"
#include "sdio.h"
#include "tim.h"
#include "fxsh_api.h"
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
fxshCmdInfo_t sh_info_fx_init = {
    .usage = "init",
    .description = "Initialize FileX and SDIO SD Card.",
    .options = "None",
    .example = ">init",
    .example_description = "Initialize FileX and SDIO SD Card."};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void sh_fx_init(int argc, char **argv)
{
    UINT status = 0;
    char media_name[12] = {0};
    char current_default_dir[256] = {0};

    printf("Initializing SDIO...\n");
    if (HAL_SD_GetState(&hsd) != HAL_SD_CARD_TRANSFER)
    {
        MX_SDIO_SD_Init();
    }

    printf("Initializing FileX...\n");
    MX_FileX_Init();

    printf("Initializing Timer14...\n");
    HAL_TIM_Base_Start_IT(&htim14);

    printf("Opening SD card...\n");
    status = sd_open();
    if (status == FX_SUCCESS)
    {
        printf("Open SD media successfully!\n");
        is_filex_init = 1;
        if (sd_volume_get(media_name) != FX_SUCCESS)
        {
            strcpy(media_name, "GET FAILED");
        }
        printf("===Media Name:[%s]\n", media_name);
        printf("Initialize FileX successfully!\n");
        sd_dir_default_get(current_default_dir);
        printf("Current dir [%s]\n", current_default_dir);
        strncpy(fxsh_profile.cwd, "/", 2);
    }
    else
    {
        printf("Open SD media failed!\n");
        printf("Initialize FileX failed!\n");
        is_filex_init = 0;
    }
}
