/*
 * sh_fx_format.c
 *
 *  Created on: 2024-06-15 T15:18:10+08:00
 *      Author: phy1335
 *        Note:
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "app_filex.h"
#include "sd_api.h"
#include "fxsh_api.h"
#include "fxsh.h"
#include "stdlib.h"
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
fxshCmdInfo_t sh_info_fx_format = {
    .usage = "format vol_name sector_bytes cluster_sectors total_bytes",
    .description = "Format the SD Card, clearing all files and directories.\n\
\t\t\tSet the parameters to 0 will use the default value.\n\
\t\t\tIt is recommended to use the software 'SD Card Formatter' on a PC\n\
\t\t\tto complete the formatting operation of the SD card.",
    .options = "-",
    .example = ">format STM32 512 8 1048576",
    .example_description = "Format the SD card with volume name 'STM32', sector size 512 bytes,\n\
\tcluster size of 8 sectors(4096B), and total capacity of 1MB."};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void sh_fx_format(int argc, char **argv)
{
    // 检查参数数量
    if (argc != 5)
    {
        PRINT_ARG;
        printf("Usage: format vol_name sector_bytes cluster_sectors total_bytes\n");
        return;
    }
    printf("\n>>>Warning: Formatting will clear all files and directories on SD Card!!!\n");
    printf(">>>It is recommended to use the software 'SD Card Formatter' on a PC\n");
    printf("  to complete the formatting operation of the SD card.\n");
    printf("Are you sure want to format it? Enter 'y' to confirm; 'n' to cancel\n");
    char tmp[4] = {0};

    fxsh_small_input(tmp);
    if (strcmp(tmp, "y") == 0)
    {
        printf("Are you sure? Enter 'y' to confirm; 'n' to cancel\n");
        fxsh_small_input(tmp);
        if (strcmp(tmp, "y") == 0)
        {
            printf("Formatting SD Card...\n");
            sd_media_format(argv[1], atoi(argv[2]), atoi(argv[3]), atoll(argv[4]));
        }
    }
    else
    {
        printf("Format has been cancelled!\n");
    }
}
