/*
 * sh_fx_rm.c
 *
 *  Created on: 2024/05/27, 00:42:46
 *      Author: phy1335
 *        Note:
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "fxsh.h"
#include "fxsh_api.h"
#include "string.h"
#include "sd_api.h"
#include <unistd.h>
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
fxshCmdInfo_t sh_info_fx_rm = {
    .usage = "rm [-r] [-i] [-f] [-v] path",
    .description = "Remove files or directories.",
    .options = "-r: Remove an EMPTY directory.\n\
\t-i: Confirm each removal.\n\
\t-f: Force removal of directories and their contents recursively without confirmation.\n\
\t-v: Display process.",
    .example = ">rm file1\n\
\t>rm -r dir1/\n\
\t>rm -i file2\n\
\t>rm -rf dir2\n\
\t>rm -rfv dir3",
    .example_description = "Delete the file 'file1'.\n\
\tDelete 'dir1' only if it's empty.\n\
\tConfirm removal before deleting the file 'file2'.\n\
\tRecursively delete all files and subdirectories in 'dir2'.\n\
\tRecursively delete all files and subdirectories in 'dir3' and display the process."};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void sh_fx_rm(int argc, char **argv)
{
    // PRINT_ARG;
    if (argc > 5 || argc < 2)
    {
        printf("Usage: rm [-r] [-i] [-f] path\n");
        return;
    }
    uint8_t rm_opt = 0; // 默认删除文件
    int option;
    // 解析命令行参数
    while ((option = getopt(argc, argv, "rifdv")) != -1)
    {
        switch (option)
        {
        case 'r':
            rm_opt |= RM_DIR; // 删除目录
            break;
        case 'i':
            rm_opt |= RM_NOTICE; // 删除前提示
            break;
        case 'f':
            rm_opt |= RM_FORCE; // 强制删除
            break;
        case 'v':
            rm_opt |= RM_SHOW; // 显示删除过程
            break;
        default:
            printf("Usage: rm [-r] [-i] [-f] [-v] path\n");
            break;
        }
    }

    char *path = NULL;
    /* 获取文件路径 */
    if (optind < argc)
    {
        path = argv[optind];
        if (strcmp(path, "*") == 0)
        {
            *path = '.';
        }
    }
    else
    {
        printf("Usage: rm [-r] [-i] [-f] [-v] path\n");
    }

    optind = 1; // 再次调用时应该设为1，以初始化getop,否则不能用

    sd_com_delete(path, rm_opt);

    /* 如果删除的目录和当前的相同，删除后回到上一级 */
    if ((strcmp(path, fxsh_profile.cwd) == 0))
    {
        fxsh_execute("cd ..");
        
    }
}
