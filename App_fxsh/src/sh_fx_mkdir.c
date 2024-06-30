/*
 * sh_fx_mkdir.c
 *
 *  Created on: 2024/05/25, 23:18:33
 *      Author: phy1335
 *        Note:
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "fxsh_api.h"
#include "string.h"
#include "stdlib.h"
#include <unistd.h>
#include "sd_api.h"
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
fxshCmdInfo_t sh_info_fx_mkdir = {
    .usage = "mkdir [-p] directory",
    .description = "Create a directory.",
    .options = "-p: Create parent directories as needed.",
    .example = ">mkdir new_directory\n\
\t>mkdir -p new_directory/sub_directory",
    .example_description = "Create a new directory named 'new_directory'.\n\
\tCreate a new directory named 'sub_directory' inside 'new_directory'."};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void sh_fx_mkdir(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: mkdir [-p] path\n");
        return;
    }

    UINT status = 0;
    char *dir_path = NULL;      // 最终要创建的目录路径
    uint8_t create_parents = 0; // 是否需要创建中间目录的标志

    /* 处理参数 */
    int option = 0;
    while ((option = getopt(argc, argv, "p")) != -1)
    {
        switch (option)
        {
        case 'p':
            create_parents = 1; // 显示隐藏文件
            break;
        default:
            printf("Usage: mkdir [-p] path\n");
            break;
        }
    }

    /* 获取文件路径 */
    if (optind < argc)
    {
        dir_path = argv[optind];
    }
    else
    {
        printf("Usage: mkdir [-p] path\n");
        return;
    }

    /* 检查目录是否存在 */
    status = sd_dir_exist(dir_path);
    if (status == FX_NOT_FOUND) // 目录不存在
    {
        /* 递归创建目录（-p） */
        if (create_parents)
        {
            char *current_path = strdup(dir_path); // 复制路径
            char *token = NULL;
            char *saveptr = NULL;             // 用于存储上下文
            char path[FX_MAXIMUM_PATH] = {0}; // 中间路径

            if (current_path == NULL)
            {
                printf("Copy path failed\n");
                return;
            }
            /* 切割路径 */
            token = strtok_r(current_path, "/", &saveptr);
            while (token != NULL)
            {
                size_t path_len = strlen(path);
                if (path_len > 0)
                {
                    size_t token_len = strlen(token);
                    /* 检查path和token的总长度，加上1（因为strncat会自动添加终止符'\0'） */
                    if (path_len + token_len + 1 <= FX_MAXIMUM_PATH)
                    {
                        /* 使用strncat，确保不会超过PATH_MAX（路径的最大长度） */
                        strncat(path, "/", FX_MAXIMUM_PATH - path_len - 1);
                    }
                    else
                    {
                        printf("Path too long! %d\n", path_len + token_len + 1);
                        free(current_path);
                        return;
                    }
                }
                strncat(path, token, FX_MAXIMUM_PATH - path_len - 1);

                /* 尝试创建目录 */
                if (sd_dir_exist(path) != FX_SUCCESS)
                {
                    status = sd_dir_create(path);
                    if (status != FX_SUCCESS)
                    {
                        printf("Create dir [%s] failed\n", path);
                        PRINT_FX_ERROR(status);
                        free(current_path);
                        return;
                    }
                }
                /* 路径已存在, 继续到子目录 */
                token = strtok_r(NULL, "/", &saveptr);
            }
            free(current_path);
        }
        else
        {
            /* 直接尝试创建目录 */
            status = sd_dir_create(dir_path);
        }
    }
    else if (status == FX_SUCCESS) // 目录已存在
    {
        printf("Directory '%s' already exists.\n", dir_path);
    }
    else // 其他错误
    {
        PRINT_FX_ERROR(status);
    }
}
