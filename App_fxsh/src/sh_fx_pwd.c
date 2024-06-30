/*
 * sh_fx_pwd.c
 *
 *  Created on: 2024/05/26, 11:55:13
 *      Author: phy1335
 *        Note:
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "stdlib.h"
#include "app_filex.h"
#include "sd_api.h"

#include "fxsh_api.h"
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
fxshCmdInfo_t sh_info_fx_pwd = {
    .usage = "pwd [-]",
    .description = "Print the current working directory.",
    .options = "-  print previous working directory",
    .example = ">pwd\n\
\t>pwd -",
    .example_description = "First example shows the current working directory.\n\
\tSecond example shows the previous working directory if available."

};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void sh_fx_pwd(int argc, char **argv)
{
    uint16_t status = 0;
    char *default_path = NULL;

    // 检查是否有 - 参数
    if (argc > 1 && strcmp(argv[1], "-") == 0)
    {
        // 显示上一次的工作目录
        if (fxsh_profile.lwd[0] != '\0')
        {
            printf("pwd -:%s\n", fxsh_profile.lwd);
        }
        else
        {
            printf("pwd -: No previous working directory\n");
        }
    }
    else
    {
        // 显示当前工作目录
        status = sd_dir_default_get(default_path);
        if (status == FX_SUCCESS)
        {
            if (*default_path == FX_NULL)
            {
                default_path[0] = '/';
                default_path[1] = FX_NULL;
            }
            printf("cwd:%s\n", default_path);

            // 更新cwd
            strlcpy(fxsh_profile.cwd, default_path, FXSH_MAX_PATH);
        }
        else
        {
            printf("pwd failed:%#X\n", status);
            PRINT_FX_ERROR(status);
        }
    }
}
