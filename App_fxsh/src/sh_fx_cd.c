/*
 * sh_fx_cd.c
 *
 *  Created on: 2024/05/26, 18:14:25
 *      Author: phy1335
 *        Note: 改变当前工作目录 其实可以直接调用 fx_directory_default_set
 *              也是可以的，但为了正常显示pwd以及方便其他shell函数使用而插入了大量的判断
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "sd_api.h"
#include "fxsh_api.h"
#include "fxsh.h"
#include "app_filex.h"
#include <string.h>
#include <stdlib.h>
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
fxshCmdInfo_t sh_info_fx_cd = {
    .usage = "cd path",
    .description = "Change the current directory",
    .options = "-  cd -",
    .example = ">cd path\n\
\t>cd -",
    .example_description = "Change the current directory to 'path'\n\
\tChange the current directory to `lwd`"};
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void sh_fx_cd(int argc, char **argv)
{
    if (argc != 2) // 没有参数退出
    {
        printf("Usage: cd path\n");
        return;
    }

    uint16_t status = 0;
    char *new_path = NULL;     // 新的路径
    char *current_path = NULL; // 当前路径
    uint8_t is_relative = 0;   // 是相对路径

    // 从pwd复制当前路径
    current_path = strdup(fxsh_profile.cwd);
    if (current_path == NULL)
    {
        printf("strdup from cwd to current_path failed\n");
        return;
    }

    if (strcmp(argv[1], ".") == 0) // cd . 打印当前目录 等于 ls .
    {
        // fxsh_execute("ls ."); // 执行 ls .
        sh_fx_ls(2, "-h");
    }
    else if (strcmp(argv[1], "-") == 0) // cd - 回到上次目录
    {
        if (fxsh_profile.lwd[0] != 0) // 上次目录非空
        {
            new_path = fxsh_profile.lwd;
        }
        else // 上次目录空
        {
            printf("No previous working directory\n");
        }
    }
    else if (strcmp(argv[1], "..") == 0) // cd .. 回到上级目录
    {
        char *last_slash = strrchr(current_path, '/');            // 找到最后一个"/
        if (last_slash - current_path == 0 || last_slash == NULL) // /123或者/或者_
        {
            new_path = "/"; // 设为根路径
        }
        else
        {
            *last_slash = '\0'; // 去除最后的 "/"
            new_path = current_path;
        }
    }
    else if (argv[1][0] == '/') // cd /a/b.. 绝对路径
    {
        size_t path_len = strlen(argv[1]);
        if (path_len >= FXSH_MAX_PATH) // 路径太长，超过filex限制
        {
            printf("Path too long! MAX %d while intput %d\n", FXSH_MAX_PATH, path_len);
        }
        else
        {
            new_path = argv[1];
        }
    }
    else // cd a/b||cd ../a/b 相对路径 cd ./a/b也行不过pwd的显示有点问题
    {
        char *token = NULL;
        char *saveptr = NULL;

        /* 复制pwd到路径 */
        new_path = (char *)malloc(strlen(current_path) + strlen(argv[1]) + 2);
        strcpy(new_path, current_path);
        strcat(new_path, "/");
        strcat(new_path, argv[1]);

        /* 解释路径中的.. */
        size_t level = 0;
        token = strtok_r(new_path, "/", &saveptr);
        while (token != NULL)
        {
            if (strcmp(token, "..") == 0)
            {
                level++;
            }
            token = strtok_r(NULL, "/", &saveptr);
        }
        /* 存在一个 ../ 则向上移动一次 */
        for (int i = 0; i < level; i++)
        {
            token = strrchr(current_path, '/');
            if (token != current_path)
            {
                *token = '\0'; // 移除最后一个目录
            }
        }
        /* 拼接路径 */
        /* 路径不是根目录则拼接 从 '/' 开始*/
        if ((strcmp(current_path, "/")) != 0)
        {
            strcat(current_path, "/");
        }
        strcat(current_path, argv[1] + (level * 3));
        /* 是相对路径。new_path使用的strdup函数使用了malloc 后面要free */
        new_path = strdup(current_path);
        is_relative = 1;
    }

    free(current_path);

    /* 尝试切换新路径 */
    status = sd_dir_default_set(new_path);
    if (status != FX_SUCCESS)
    {
        printf("cd [%s] failed\n", new_path);
        PRINT_FX_ERROR(status);
    }
    else // cd成功，更新pwd和lwd
    {
        strncpy(fxsh_profile.lwd, fxsh_profile.cwd, FXSH_MAX_PATH);
        strncpy(fxsh_profile.cwd, new_path, FXSH_MAX_PATH);

        /* cd 后显示目录内容 */
        // fxsh_execute("ls -h");
        char *tmp[1] = {"-h"};
        sh_fx_ls(2, tmp);
    }

    if (is_relative == 1) // 是相对路径，free
    {
        free(new_path);
    }
}
