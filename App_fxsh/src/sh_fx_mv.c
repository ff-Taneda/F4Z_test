/*
 * sh_fx_re.c
 *
 *  Created on: 2024/05/29, 01:03:59
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
fxshCmdInfo_t sh_info_fx_mv = {
    .usage = "mv [old] [new]",
    .description = "Move or rename a file or directory.",
    .options = "None",
    .example = ">mv old_file_name new_file_name\n\
\t>mv /path/to/old_directory /path/to/new_directory",
    .example_description = "Rename 'old_file_name' to 'new_file_name'.\n\
\tMove or rename 'old_directory' to 'new_directory'."};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void sh_fx_mv(int argc, char **argv)
{
    // 检查参数数量
    if (argc != 3)
    {
        PRINT_ARG;
        printf("Usage: mv old new\n");
        return;
    }
    sd_com_move(argv[1], argv[2]);
}
