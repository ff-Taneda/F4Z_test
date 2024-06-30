/*
 * sh_fx_time.c
 *
 *  Created on: 2024-05-25T20:57:19+08:00
 *      Author: phy1335
 *        Note: 显示/设置时间
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "fxsh_api.h"
#include "sd_api.h"
#include "string.h"
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
fxshCmdInfo_t sh_info_fx_time = {
    .usage = "time [-s] [hh:mm:ss]",
    .description = "Show/Set the FileX current system time.",
    .options = "\t- :Show the FileX current system time\n \
\t-s :Set the FileX current system time\n \
\t\t[hh:mm:ss] :time to set\n",
    .example = "fx_time -s 10:11:12",
    .example_description = "set FileX system time 10:11:12"};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void sh_fx_time(int argc, char **argv)
{
    UINT hh = 0, mm = 0, ss = 0;
    uint8_t time_ok = 0;
    UINT status = 0;

    if (argc == 1)
    {
        status = fx_system_time_get(&hh, &mm, &ss);
        if (status != FX_SUCCESS)
        {
            printf("Get filex time failed\n");
            FX_POST_ERROR(status);
        }
        else
        {
            printf("FileX time %02d:%02d:%02d\n", hh, mm, ss);
        }
    }
    else if (strcmp(argv[1], "-s") == 0)
    {
        if (argc == 2)
        {
            char time_input[32] = {0};
            printf("Please input time: h:m:s\n");
            if (fgets(time_input, sizeof(time_input), stdin) != NULL)
            {
                if (3 == sscanf(time_input, "%d:%d:%d", &hh, &mm, &ss))
                {
                    time_ok = 1;
                }
                else
                {
                    printf("Failed to read time input format.\n");
                }
            }
            else
            {
                printf("Failed to read time input.\n");
                time_ok = 0;
            }
        }
        else if (argc == 3)
        {
            if (3 == sscanf(argv[2], "%d:%d:%d", &hh, &mm, &ss))
            {
                time_ok = 1;
            }
            else
            {
                printf("Failed to read time input format.\n");
            }
        }
        else
        {
            printf("More than 2 args !\n");
        }
    }
    else
    {
        time_ok = 0;
        printf("Invaild option: %s\n", argv[1]);
    }

    if (time_ok == 1)
    {
        status = fx_system_time_set(hh, mm, ss);
        if (status != FX_SUCCESS)
        {
            printf("Set FileX time failed %d:%d:%d\n", hh, mm, ss);
            FX_POST_ERROR(status);
        }
        else
        {
            printf("Set FileX time successfully %d:%d:%d\n", hh, mm, ss);
        }
    }
}
