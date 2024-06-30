/*
 * sh_fx_find.c
 *
 *  Created on: 2024/06/26, 01:10:15
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
fxshCmdInfo_t sh_info_fx_find = {
    .usage = "find path pattern",
    .description = "Search for files or directories matching the pattern in the given path.",
    .options = "- Supports '*' for matching any character(s) and '?' for matching any single character.",
    .example = ">find path *.txt",
    .example_description = "Search for all *.txt files in the specified path"};
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void sh_fx_find(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Usage: find path pattern\n");
    }
    else
    {
        char *path = argv[1];
        char *pattern = argv[2];
        sd_com_find(path, pattern, 0);
    }
}
