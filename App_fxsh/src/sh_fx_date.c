/*
 * sh_fx_date.c
 *
 *  Created on: 2024-05-25T22:25:01+08:00
 *      Author: phy1335
 *        Note: 显示/设置日期
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
fxshCmdInfo_t sh_info_fx_date = {
    .usage = "date [-s] [hh:mm:ss]",
    .description = "Show/Set the FileX current system date.",
    .options = "\t- :Show the FileX current system date\n \
\t-s :Set the FileX current system date\n \
\t\t[hh:mm:ss] :date to set\n",
    .example = ">date -s 2024-5-25",
    .example_description = "set FileX system date 2024-5-25"};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void sh_fx_date(int argc, char **argv)
{
    UINT yyyy = 0, mm = 0, dd = 0;
    uint8_t date_ok = 0;
    UINT status = 0;

    if (argc == 1)
    {
        status = fx_system_date_get(&yyyy, &mm, &dd);
        if (status != FX_SUCCESS)
        {
            printf("Get filex date failed\n");
            FX_POST_ERROR(status);
        }
        else
        {
            printf("FileX date %d-%d-%d\n", yyyy, mm, dd);
        }
    }
    else if (strcmp(argv[1], "-s") == 0)
    {
        if (argc == 2)
        {
            char date_input[32];
            printf("Please input date: yyyy-m-d\n");
            if (fgets(date_input, sizeof(date_input), stdin) != NULL)
            {
                if (3 == sscanf(date_input, "%d-%d-%d", &yyyy, &mm, &dd))
                {
                    date_ok = 1;
                }
                else
                {
                    printf("Failed to read date input format.\n");
                }
            }
            else
            {
                printf("Failed to read date input.\n");
                date_ok = 0;
            }
        }
        else if (argc == 3)
        {
            if (3 == sscanf(argv[2], "%d-%d-%d", &yyyy, &mm, &dd))
            {
                date_ok = 1;
            }
            else
            {
                printf("Failed to read date input format.\n");
            }
        }
        else
        {
            printf("More than 2 args !\n");
        }
    }
    else
    {
        date_ok = 0;
        printf("Invaild option: %s\n", argv[1]);
    }

    if (date_ok == 1)
    {
        status = fx_system_date_set(yyyy, mm, dd);
        if (status != FX_SUCCESS)
        {
            printf("Set FileX date failed %d-%d-%d\n", yyyy, mm, dd);
            FX_POST_ERROR(status);
        }
        else
        {
            printf("Set FileX date successfully %d-%d-%d\n", yyyy, mm, dd);
        }
    }
}
