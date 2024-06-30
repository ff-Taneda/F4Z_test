/*
 * fxsh.c
 *
 *  Created on: 2024/05/23, 23:19:54
 *      Author: phy1335
 *        Note: FileX-based Shell source
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "fxsh.h"
#include "fxsh_api.h"
#include "sdio.h"
#include <string.h>
#include <stdlib.h>
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* fxsh历史记录，记录数量可修改 FXSH_MAX_HISTORY */
__section(".ccmram") char sh_history[FXSH_MAX_HISTORY][FXSH_MAX_INPUT];
/* 存储输入命令的缓冲区，最长输入可修改 FXSH_MAX_INPUT */
__section(".ccmram") char sh_inputbuffer[FXSH_MAX_INPUT];
/* 命令列表 */
fxshCmd_t sh_cmd_list[] = {
    {"help", sh_cmd_help, &sh_info_cmd_help},
    {"test", sh_cmd_test, &sh_info_cmd_test},
    {"echo", sh_cmd_echo, &sh_info_cmd_echo},
    {"history", sh_cmd_history, &sh_info_cmd_history},
    {"clear", sh_cmd_clear, &sh_info_cmd_clear},
    {"init", sh_fx_init, &sh_info_fx_init},
    {"deinit", sh_fx_deinit, &sh_info_fx_deinit},
    {"time", sh_fx_time, &sh_info_fx_time},
    {"date", sh_fx_date, &sh_info_fx_date},
    {"mkdir", sh_fx_mkdir, &sh_info_fx_mkdir},
    {"cd", sh_fx_cd, &sh_info_fx_cd},
    {"pwd", sh_fx_pwd, &sh_info_fx_pwd},
    {"cat", sh_fx_cat, &sh_info_fx_cat},
    {"touch", sh_fx_touch, &sh_info_fx_touch},
    {"ls", sh_fx_ls, &sh_info_fx_ls},
    {"mv", sh_fx_mv, &sh_info_fx_mv},
    {"rm", sh_fx_rm, &sh_info_fx_rm},
    {"df", sh_fx_df, &sh_info_fx_df},
    {"chkdsk", sh_fx_chksd, &sh_info_fx_chksd},
    {"format", sh_fx_format, &sh_info_fx_format},
    {"tree", sh_fx_tree, &sh_info_fx_tree},
    {"find", sh_fx_find, &sh_info_fx_find},
    {"sd_test", sh_fx_test, &sh_info_fx_test},
    {NULL, NULL, NULL}};

/* Global variables ----------------------------------------------------------*/
/* Shell配置 */
fxshProfile_t fxsh_profile = {
    .cwd = "(uninit)", // 进去默认是 "(uninit)"
};
uint8_t sh_history_index = 0;
/* Private function prototypes -----------------------------------------------*/
void fxsh_history_add(char *cmd_line);
void fxsh_show_info(void);
/* Private functions ---------------------------------------------------------*/
/**
 * @brief 添加新命令到历史，不会添加history和clear。
 * @param cmd_line 记录的数据
 */
void fxsh_history_add(char *cmd_line)
{
    if (strcmp(cmd_line, "history") == 0 || strcmp(cmd_line, "clear") == 0)
    {
        return;
    }
    /* 记录指针达到最大，移动数组，新命令放最后， */
    if (sh_history_index >= FXSH_MAX_HISTORY)
    {
        sh_history_index = FXSH_MAX_HISTORY - 1;
        memmove(sh_history, sh_history + 1, (FXSH_MAX_HISTORY - 1) * sizeof(sh_history[0])); // 移动数组
        strncpy(sh_history[FXSH_MAX_HISTORY - 1], cmd_line, FXSH_MAX_INPUT);                 // 新命令覆盖最旧的
    }
    /* 添加新命令 */
    else
    {
        strncpy(sh_history[sh_history_index], cmd_line, FXSH_MAX_INPUT); // 复制新命令
        sh_history[sh_history_index][FXSH_MAX_INPUT - 1] = '\0';         // 添加终止符
    }
    sh_history_index++;
}

/**
 * @brief 显示一些信息
 * @param
 */
void fxsh_show_info(void)
{
    printf("\n===Welcome to the STM32 fxsh(FileX-based Shell) v1.0!===\n");
    printf("===Created by phy1335||Compiled on %s===\n", __TIMESTAMP__);
    printf("===Type 'help [cmd]' to show info of [cmd]===\n");
}
/* Exported functions --------------------------------------------------------*/
/**
 * @brief fxsh主入口，处理用户输入和命令执行
 * @param user 用户名，用于显示，暂未添加用户管理
 * @param prompt 提示符
 * @param first_cmd 首次运行的命令，如果提供则执行该命令
 * @return
 */
uint8_t fxsh_entry(char *user, char *prompt, char *first_cmd)
{
    strncpy(fxsh_profile.user, user, 16);
    strncpy(fxsh_profile.prompt, prompt, 8);

    /*  首次运行命令 */
    if (first_cmd != NULL)
    {
        printf("Running First[%s]\n", first_cmd);
        first_cmd[strcspn(first_cmd, "\r\n")] = 0;
        fxsh_execute(first_cmd);
    }
    // fxsh_execute("clear");
    sh_cmd_clear(1, NULL);

    /* 循环读取并执行命令 */
    while (1)
    {
        /* 清空buffer */
        memset(sh_inputbuffer, 0, FXSH_MAX_INPUT);
        /* 显示提示符 */
        printf("@%s:%s %s ", fxsh_profile.user, fxsh_profile.cwd, fxsh_profile.prompt);

        /* 使用fgets从串口读取命令 */
        if (fgets(sh_inputbuffer, FXSH_MAX_INPUT, stdin) != NULL)
        {
            /* 去除可能的换行符 */
            sh_inputbuffer[strcspn(sh_inputbuffer, "\r\n")] = 0;

            /* 检查输入是否为"exit" */
            if (strcmp(sh_inputbuffer, "exit") == 0)
            {
                // 退出shell
                printf("Exiting fxshell...\n");
                return 0;
            }

            /* 添加到历史 */
            fxsh_history_add(sh_inputbuffer);

            /* 执行命令 */
            fxsh_execute(sh_inputbuffer);

            /* LED闪一下 */
            HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin | LED1_Pin);
        }
    }
    return 1;
}

/**
 * @brief fxsh命令执行函数。可以直接调用命令：fxsh_execute("help");
 * @param cmd_line 待执行的命令字符串
 * @note 函数首先复制命令字符串（避免原始指针被strtok修改），然后使用空格、制表符、换行符和换页符进行分割，存储到argv数组中。
 *      遍历sh_cmd_list，查找与命令名匹配的项，执行相应的函数。如果找不到匹配的命令，输出未知命令信息。
 *       最后释放通过strdup动态分配的命令内存。
 */
void fxsh_execute(char *cmd_line)
{
    int argc = 0;                    // 参数个数
    char *argv[FXSH_MAX_ARGS] = {0}; // 参数列表
    char *token = NULL;              // 分割的字串
    uint8_t is_exec = 0;             // 找到命令执行了

    /* 复制命令。否则直接直接调用fxsh_execute会失败
    因为strtok会直接修改cmd_line指向的空间，而在代码直接执行的命令是存放在flash的,
    flash没有设置可写，那么就会分割命令失败，导致解释命令失败*/
    char *cmd = strdup(cmd_line);

    /* 分割argv */
    token = strtok(cmd, " \t\r\n\f");
    while (token != NULL)
    {
        argv[argc] = token;
        argc++;
        token = strtok(NULL, " \t\r\n\f");
    }
    argv[argc] = NULL; // 将下一个argv设为NULL
    /* 查找并执行命令 */
    for (int i = 0; sh_cmd_list[i].name != NULL; i++)
    {
        if (strcmp(argv[0], sh_cmd_list[i].name) == 0)
        {
            sh_cmd_list[i].exec(argc, argv);
            is_exec = 1;
            break;
        }
    }

    if (!is_exec) // 没找到命令
    {
        printf("Unknown command:[%s]\n", sh_inputbuffer);
    }
    free(cmd); // strdup使用了malloc
}

/**
 * @brief 小型输入，用于获得 y n 之类的单字符
 * @param inputbuf 最多4字节!!!
 */
void fxsh_small_input(char *inputbuf)
{
    char tmp[4] = {0};
    if (fgets(tmp, sizeof(tmp), stdin) != NULL)
    {
        /* 去除可能的换行符 */
        tmp[strcspn(tmp, "\r\n")] = 0;
    }
    strncpy(inputbuf, tmp, 4);
}
