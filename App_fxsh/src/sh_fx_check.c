/*
 * sd_fx_check.c
 *
 *  Created on: 2024-06-02 T18:36:43+08:00
 *      Author: phy1335
 *        Note: 
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "fxsh_api.h"
#include "string.h"
#include "app_filex.h"
#include "sd_api.h"
#include "GetRunTime.h"
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
fxshCmdInfo_t sh_info_fx_chksd = {
    .usage = "chksd",
    .description = "Check the SD card media.",
    .options = "None",
    .example = ">chksd",
    .example_description = "Check the SD card media."};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void sh_fx_chksd(int argc, char **argv)
{
    printf("Starting check...\n");

    sd_media_check();
}
