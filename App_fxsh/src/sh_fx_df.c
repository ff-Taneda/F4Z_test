/*
 * sh_fx_df.c
 *
 *  Created on: 2024/06/01, 23:13:22
 *      Author: phy1335
 *        Note: 显示SD卡的总大小，格式化大小，剩余大小
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "fxsh_api.h"
#include "sd_api.h"
#include <unistd.h>
#include "string.h"
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
fxshCmdInfo_t sh_info_fx_df = {
    .usage = "df [-h]",
    .description = "Report file system disk space usage.",
    .options = "-h: Display in a human-readable format.",
    .example = ">df\n\
\t>df -h.",
    .example_description = "Display the disk usage statistics in block size.\n\
\tDisplay the disk usage statistics in a human-readable format."};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void format_size(char *buffer, uint64_t size, const char *info, int df_opt)
{
    float tmp = 0;
    char *unit[2] = {"MB", "GB"};
    if (df_opt == 1)
    {
        if (size > (4 * 1024 * 1024 * 1024ULL))
        {
            tmp = (float)size / 1024.0 / 1024.0 / 1024.0;
        }
        else
        {
            tmp = (float)size / 1024.0 / 1024.0;
        }
        snprintf(buffer, 24, "%s %.2f %s", info, tmp, unit[size > (4 * 1024 * 1024 * 1024ULL)]);
    }
    else
    {
        snprintf(buffer, 24, "%s %llu B", info, size);
    }
}
/* Exported functions --------------------------------------------------------*/
void sh_fx_df(int argc, char **argv)
{
    uint8_t df_opt = 0;
    int option;
    // 解析命令行参数
    while ((option = getopt(argc, argv, "h")) != -1)
    {
        switch (option)
        {
        case 'h':
            df_opt = 1; // 友好显示大小
            break;
        default:
            printf("Usage: df [-h]\n");
            break;
        }
    }

    uint64_t total_byte = sd_space_get(3);     // 总字节
    uint64_t format_byte = sd_space_get(2);    // 格式化字节
    uint64_t available_byte = sd_space_get(1); // 可用字节
    char total_show[24] = {0};
    char format_show[24] = {0};
    char available_show[24] = {0};

    format_size(total_show, total_byte, "Total     ", df_opt);
    format_size(format_show, format_byte, "Formatted ", df_opt);
    format_size(available_show, available_byte, "Available ", df_opt);

    char bar[103] = {0};
    memset(bar + 1, '.', 100);
    memset(bar + 1, '_', (size_t)(((float)format_byte / (float)total_byte) * 100));
    memset(bar + 1, '#', (size_t)((((float)format_byte - (float)available_byte) / (float)total_byte) * 100));
    bar[0] = '[';
    bar[101] = ']';
    bar[102] = '\0';
    printf("\n%s\n%s\n", total_show, format_show);
    printf("%s\nUtilization %.2f%%\n", available_show, (((float)format_byte - (float)available_byte) / (float)total_byte) * 100.0);
    printf("%s\n", bar);

    optind = 1; // 再次调用时应该设为1，以初始化getop,否则不能用
}
