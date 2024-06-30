/*
 * sh_cmd_history.c
 *
 *  Created on: 2024-06-03 T20:10:33+08:00
 *      Author: phy1335
 *        Note: 打印命令历史
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "fxsh.h"
#include "fxsh_api.h"
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
fxshCmdInfo_t sh_info_cmd_history = {
    .usage = "history",
    .description = "Display the command history.",
    .options = "None",
    .example = ">history",
    .example_description = "Display all previously executed commands."};

fxshCmdInfo_t sh_info_cmd_clear = {
    .usage = "clear",
    .description = "Clear the command history.",
    .options = "None",
    .example = ">clear",
    .example_description = "Clear history."};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void sh_cmd_history(int argc, char **argv)
{
    if (argc > 1)
    {
        printf("Too many arguments!\n");
        return;
    }
    printf("history:%d\n", sh_history_index);
    for (size_t i = 0; i < sh_history_index; i++)
    {
        printf("%*d|%s\n", 2, i + 1, sh_history[i]);
    }
}

void sh_cmd_clear(int argc, char **argv)
{
    if (argc > 1)
    {
        printf("Too many arguments!\n");
        return;
    }
    sh_history_index = 0;
    memset(sh_history, 0, FXSH_MAX_HISTORY * FXSH_MAX_INPUT);
}
