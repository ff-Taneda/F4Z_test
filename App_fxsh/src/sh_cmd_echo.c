/*
 * sh_cmd_echo.c
 *
 *  Created on: 2024/05/26, 15:52:07
 *      Author: phy1335
 *        Note: 回显或者写入文件
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "fxsh_api.h"
#include "string.h"
#include "stdlib.h"
#include "app_filex.h"
#include "sd_api.h"
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
fxshCmdInfo_t sh_info_cmd_echo = {
    .usage = "echo [>/>>file] [text]",
    .description = "Display a line of text or write to file",
    .options = ">: Redirect output to a file.\n\
\t>>: Append output to a file.",
    .example = ">echo Hello, World!\n\
\t>echo >file.txt Hello, World!\n\
\t>echo >>file.txt Hello, World!",
    .example_description = "Print 'Hello, World!' to the console.\n\
\tRedirect 'Hello, World!' to file.txt.\n\
\tAppend 'Hello, World!' to file.txt.",
};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void sh_cmd_echo(int argc, char **argv)
{
    // PRINT_ARG;
    uint16_t status = 0;

    char *cmd = NULL;
    char *path = NULL;
    uint8_t append = 0;

    if (argc == 3) // arg[0]echo arg[1]>/>>file arg[2]txt
    {
        cmd = argv[1];
        if (cmd[0] != '>') // [1]!>
        {
            goto __no_re_dir;
        }
        else // [1]=='>' 覆盖
        {
            if (cmd[1] == '>') // [1]== '>>' 追加
            {
                append = 1;
                cmd++;
            }
            path = cmd + 1; // 找到路径 [1] >file
            uint32_t len = strlen(argv[2]);
            status = sd_file_write(path, argv[2], len, append);
            if (status == FX_SUCCESS) // 成功写入
            {
            }
            else if (status == FX_NOT_FOUND) // 没有这个文件则创建
            {
                status = sd_file_create(path);
                if (status == FX_SUCCESS) // 创建后再次写入
                {
                    status = sd_file_write(path, argv[2], len, append);
                    if (status != FX_SUCCESS)
                    {
                        printf("File [%s] created but write failed:%#X\n", path, status);
                    }
                }
                else // 创建失败
                {
                    printf("Create [%s] failed:%#X\n", path, status);
                }
            }
            else // 文件存在 写入失败
            {
                printf("Write to [%s] failed:%#X \n", path, status);
            }
        }
    }
    else
    {
    __no_re_dir:
        // 从 argv[1] 开始迭代参数，argv[0] 通常是命令本身
        for (int i = 1; i < argc; i++)
        {
            // 打印参数
            printf("%s", argv[i]);

            // 如果不是最后一个参数，添加一个空格
            if (i < argc - 1)
            {
                printf(" ");
            }
        }

        // 打印换行符
        printf("\n");
    }
}
