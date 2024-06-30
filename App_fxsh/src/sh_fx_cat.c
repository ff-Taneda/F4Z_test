/*
 * sh_fx_cat.c
 *
 *  Created on: 2024/05/26, 22:06:21
 *      Author: phy1335
 *        Note: 查看文件，可选十六禁止模式
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "stdlib.h"
#include "sd_api.h"
#include <unistd.h>
#include "fxsh_api.h"
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
fxshCmdInfo_t sh_info_fx_cat = {
    .usage = "cat [-n] [-h num] path",
    .description = "Concatenate and display files.",
    .options = "-n: Display line numbers before each line.\n\
\t-h num: Display num number of hexadecimal numbers per line.",
    .example = ">cat file1.txt\n\
\t>cat -n file1.txt\n\
\t>cat -h 16 file1.txt",
    .example_description = "Display the contents of file1.txt.\
\n\tDisplay the contents of file1.txt with line numbers.\
\n\tDisplay the contents of file1.txt with 16 hexadecimal numbers per line."};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void sh_fx_cat(int argc, char **argv)
{
    uint8_t opt = 0;     // 默认不显示行号
    uint8_t hex_num = 0; // 每行显示的十六进制数量
    char *path = NULL;
    int option;

    if (argc > 4)
    {
        printf("Usage: cat [-n] [-h num] path\n");
        return;
    }

    // 解析命令行参数
    while ((option = getopt(argc, argv, "nh:")) != -1)
    {
        switch (option)
        {
        case 'n':
            opt = 1; // 显示行号
            hex_num = 0;
            break;
        case 'h':
            hex_num = atoi(optarg); // 获取每行显示的十六进制数量
            break;
        default:
            printf("Usage: cat [-n] [-h num] path\n");
            break;
        }
    }

    // 获取文件路径
    if (optind < argc)
    {
        path = argv[optind];
        sd_file_cat(opt, path, hex_num);
    }
    else
    {
        printf("Usage: cat [-n] [-h num] path\n");
    }

    optind = 1; // 再次调用时应该设为1，以初始化getop,否则不能用
}
