/*
 * sh_fx_tree.c
 *
 *  Created on: 2024/06/23, 20:26:23
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
fxshCmdInfo_t sh_info_fx_tree = {
    .usage = "tree [-a] [-d depth] [-h] [path]",
    .description = "Print path ASCII Tree.",
    .options = "-",
    .example = ">tree path\n",
    .example_description = "Print path ASCII Tree."};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void sh_fx_tree(int argc, char **argv)
{

    uint8_t depth = 1;
    uint8_t tree_opt = 0;

    int option = 0;
    while ((option = getopt(argc, argv, "ad:h")) != -1)
    {
        switch (option)
        {
        case 'a':
            tree_opt |= SD_SHOW_HIDE; // 显示隐藏文件
            break;
        case 'd':
            depth = atoi(optarg); // 遍历深度
            break;
        default:
            printf("Usage: tree [-a] [-d depth] [-h] [path]\n");
            break;
        }
    }

    char *path = NULL;
    // 获取文件路径
    if (optind < argc)
    {
        path = argv[optind];
    }
    else // 没添加路径则为当前路径
    {
        path = fxsh_profile.cwd;
    }

    optind = 1; // 再次调用时应该设为1，以初始化getop,否则不能用
    if (path != NULL)
    {
        sd_com_tree(path, depth, tree_opt);
    }
}
