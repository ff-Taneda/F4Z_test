/*
 * sh_fx_deinit.c
 *
 *  Created on: 2024/05/30, 21:47:25
 *      Author: phy1335
 *        Note: 复位
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
fxshCmdInfo_t sh_info_fx_deinit = {
    .usage = "deinit",
    .description = "DeInitialize FileX and SDIO SD Card.",
    .options = "None",
    .example = ">deinit",
    .example_description = "DeInitialize FileX and SDIO SD Card."};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void sh_fx_deinit(int argc, char **argv)
{
    uint16_t status = 0;
    printf("Closing SD media...\n");
    status = sd_close();
    if (status == FX_SUCCESS)
    {
        HAL_SD_DeInit(&hsd);
        strncpy(fxsh_profile.cwd, "(reset)", 8);
        printf("Close sd media successfully.\nStop SDIO successfully.\nThe SD card can now be removed safely.\n");
    }
    else
    {
        printf("Stop sd media failed\n");
    }
}
