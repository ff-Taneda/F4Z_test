/*
 * sh_fx_touch.c
 *
 *  Created on: 2024/05/27, 00:18:29
 *      Author: phy1335
 *        Note:
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "stdlib.h"
#include "app_filex.h"
#include "sd_api.h"
#include "unistd.h"

#include "fxsh_api.h"
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
fxshCmdInfo_t sh_info_fx_touch = {
    .usage = "touch [-t yyyy-mm-ddThh:mm:ss] file...",
    .description = "Create files or change timestamp of the specified file.",
    .options = "-t: Set the specified time for the files instead of creating them.",
    .example = ">touch file1.txt\n\
\t>touch -t 2023-04-01T12:34:56 file1.txt",
    .example_description = "Create a new file 'file1.txt'.\n\
\tSet the file 'file1.txt' with the specified date and time."};
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
uint16_t set_file_time(char *filepath, char *datetime_str)
{
    uint16_t yyyy, m, d, hh, mm, ss;

    // 解析日期和时间
    if (sscanf(datetime_str, "%hd-%hd-%hdT%hd:%hd:%hd", &yyyy, &m, &d, &hh, &mm, &ss) != 6)
    {
        return 1; // 解析失败
    }

    // 调用 sd_file_set_time 修改文件时间
    return sd_file_set_time(filepath, yyyy, m, d, hh, mm, ss);
}
/* Exported functions --------------------------------------------------------*/

void sh_fx_touch(int argc, char **argv)
{
    int opt;
    char *time_str = NULL;
    int file_index_start = 1; // 默认情况下，从 argv[1] 开始都是文件

    // 解析命令行参数
    while ((opt = getopt(argc, argv, "t:")) != -1)
    {
        switch (opt)
        {
        case 't':
            time_str = optarg;
            file_index_start = optind; // 如果提供了 -t，文件从 optind 开始
            break;
        default:
            printf("Usage: %s [-t yyyy-mm-ddThh:mm:ss] file...\n", argv[0]);
        }
    }

    // 处理文件创建或时间设置
    if (time_str)
    {
        // 如果提供了 -t 选项，设置文件时间
        if (optind < argc)
        {
            char *file_path = argv[optind];
            uint16_t result = set_file_time(file_path, time_str);
            if (result == 0)
            {
                printf("Set %s for File '%s' successfully.\n", time_str, file_path);
            }
            else
            {
                printf("Failed to set file time for '%s'.\n", file_path);
            }
        }
        else
        {
            printf("Usage: %s [-t yyyy-mm-ddThh:mm:ss] file...\n", argv[0]);
        }
    }
    else
    {
        // 如果没有提供 -t 选项，创建文件
        for (int i = file_index_start; i < argc; i++)
        {
            uint16_t result = sd_file_create(argv[i]);
            if (result == FX_SUCCESS)
            {
                printf("Create file [%s] create successfully.\n", argv[i]);
            }
            else
            {
                printf("Failed to create file [%s].\n", argv[i]);
            }
        }
    }
    optind = 1; // 再次调用时应该设为1，以初始化getop,否则不能用
}
