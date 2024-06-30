/*
 * sh_cmd_help.c
 *
 *  Created on: 2024/05/25, 20:09:23
 *      Author: phy1335
 *        Note: 显示帮助
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "fxsh_api.h"
#include "string.h"
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
fxshCmdInfo_t sh_info_cmd_help = {
    .usage = "help [cmd]",
    .description = "Display help information for a specific command or all commands.",
    .options = "[cmd]",
    .example = ">help echo\n\
\t>help",
    .example_description = "Show detailed help for the 'echo' command.\n\
\tShow a list of all available commands with brief descriptions.",
};

fxshCmdInfo_t sh_info_cmd_test = {
    .usage = "A test",
    .description = "A shell test",
    .options = "None",
    .example = "test",
    .example_description = "A shell test"};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void sh_cmd_help(int argc, char **argv)
{
    size_t i = 0;
    if (argc > 1) // 显示命令的帮助
    {
        while (sh_cmd_list[i].name != NULL)
        {
            if (strcmp(argv[1], sh_cmd_list[i].name) == 0)
            {
                printf("??Command:%s\n", sh_cmd_list[i].name);
                printf("Usage:\t%s\n", sh_cmd_list[i].info->usage);
                printf("Description:\n\t%s\n", sh_cmd_list[i].info->description);
                printf("Options:\n\t%s\n", sh_cmd_list[i].info->options);
                printf("Example:\n\t%s\n", sh_cmd_list[i].info->example);
                printf(" Description:\n\t%s\n", sh_cmd_list[i].info->example_description);
                return;
            }
            i++;
        }
        printf("Unknown command: %s\n", argv[1]);
    }
    else // 没输入命令，打印全部命令
    {
        printf("Please input >help [cmd]\n");
        printf("  |cmd        -Description\n");
        while (sh_cmd_list[i].name != NULL)
        {
            printf("%*d|%-10s -%s\n", 2, i + 1, sh_cmd_list[i].name, sh_cmd_list[i].info->description);
            i++;
        }
    }
}

/**
 * @brief 测试命令
 * @param argc
 * @param argv
 */
void sh_cmd_test(int argc, char **argv)
{
    if (argc > 1)
    {
        printf("Test command with usage: ");
        for (int i = 0; i < argc; i++)
        {
            printf("arg[i]=%s\n", argv[i]);
        }
    }
    else
    {
        printf("Test command executed without usage.\n");
    }
}
