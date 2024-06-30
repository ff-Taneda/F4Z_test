/*
 * sh_fx_ls.c
 *
 *  Created on: 2024/05/27, 01:09:53
 *      Author: phy1335
 *        Note:
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "app_filex.h"
#include "sd_api.h"
#include <unistd.h>
#include "fxsh_api.h"
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
fxshCmdInfo_t sh_info_fx_ls = {
    .usage = "ls [-a] [-h] [path]",
    .description = "List directory contents. Attributes:\n\
\t\t\tv:volumn; d:directory; f:file; a:archve;\n\
\t\t\ts:system; h:hide;      r:read; w:write;",
    .options = "-a: Show hidden files.\n\
\t-h: Display file sizes in a human-readable format.",
    .example = ">ls\n\
\t>ls -a\n\
\t>ls -h\n\
\t>ls /path/to/directory",
    .example_description = "List files in the current directory.\n\
\tList all contents in current directory, including hidden ones.\n\
\tList contents with human-readable file sizes.\n\
\tList contents in the specified directory."};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void sh_fx_ls(int argc, char **argv)
{
    int option = 0;

    uint8_t ls_opt = 0;

    while ((option = getopt(argc, argv, "ah")) != -1)
    {
        switch (option)
        {
        case 'a':
            ls_opt |= SD_SHOW_HIDE; // 显示隐藏文件
            break;
        case 'h':
            ls_opt |= SD_SHOW_FRIENDLY; // 友好显示大小
            break;
        default:
            ls_opt = 0;
            break;
        }
    }

    char *path = NULL;
    /* 获取文件路径 */
    if (argc - optind > 1)
    {
        printf("Usage: ls [-a] [-h] [path]\n");
    }
    else if (argc - optind == 1)
    {
        path = argv[optind];
    }
    else
    {
        path = fxsh_profile.cwd;
    }
    optind = 1; // 再次调用时应该设为1，以初始化getop,否则不能用

    if (path != NULL)
    {
        sd_com_ls(path, ls_opt);
    }
}
