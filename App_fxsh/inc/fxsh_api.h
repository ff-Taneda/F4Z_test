/*
 * fxsh_api.h
 *
 *  Created on: 2024/05/25, 19:57:24
 *      Author: phy1335
 *        Note: FileX-based Shell API
 *
 */
#ifndef __FXSH_API_H
#define __FXSH_API_H

#ifdef __cplusplus
extern "C"{
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "PrintError.h"
/* Exported constants --------------------------------------------------------*/
#define FXSH_MAX_PATH 256 // FileX 默认最大工作路径

/* Exported types ------------------------------------------------------------*/
// 命令信息结构体，用于存储命令的信息
typedef struct
{
    const char *usage;
    const char *description;
    const char *options;
    const char *example;
    const char *example_description;
} fxshCmdInfo_t;

// 命令结构体，用于存储命令名称和执行函数和信息
typedef struct
{
    const char *name;
void (*exec)(int argc, char **argv);
    fxshCmdInfo_t *info;
} fxshCmd_t;

//配置结构体，用于存储当前shell的用户名和路径
typedef struct
{
    char user[16];
    char prompt[8];
    char lwd[FXSH_MAX_PATH];
    char cwd[FXSH_MAX_PATH];
} fxshProfile_t;

/* Global variables declaration ----------------------------------------------*/
/* Shell 命令信息结构体 */
extern fxshProfile_t fxsh_profile;
extern fxshCmd_t sh_cmd_list[];
extern fxshCmdInfo_t sh_info_cmd_test;
extern fxshCmdInfo_t sh_info_cmd_help;
extern fxshCmdInfo_t sh_info_cmd_echo;
extern fxshCmdInfo_t sh_info_cmd_history;
extern fxshCmdInfo_t sh_info_cmd_clear;
/* FileX操作命令信息结构体 */
extern fxshCmdInfo_t sh_info_fx_init;
extern fxshCmdInfo_t sh_info_fx_deinit;
extern fxshCmdInfo_t sh_info_fx_time;
extern fxshCmdInfo_t sh_info_fx_date;
extern fxshCmdInfo_t sh_info_fx_mkdir;
extern fxshCmdInfo_t sh_info_fx_cd;
extern fxshCmdInfo_t sh_info_fx_pwd;
extern fxshCmdInfo_t sh_info_fx_cat;
extern fxshCmdInfo_t sh_info_fx_touch;
extern fxshCmdInfo_t sh_info_fx_ls;
extern fxshCmdInfo_t sh_info_fx_mv;
extern fxshCmdInfo_t sh_info_fx_rm;
extern fxshCmdInfo_t sh_info_fx_df;
extern fxshCmdInfo_t sh_info_fx_chksd;
extern fxshCmdInfo_t sh_info_fx_format;
extern fxshCmdInfo_t sh_info_fx_tree;
extern fxshCmdInfo_t sh_info_fx_find;
extern fxshCmdInfo_t sh_info_fx_test;
/* Exported macros -----------------------------------------------------------*/
//打印参数列表
#define PRINT_ARG                            \
    do                                       \
    {                                        \
        for (size_t i = 0; i < argc; i++)    \
        {                                    \
            printf("[%d]=%s\n", i, argv[i]); \
        }                                    \
    } while (0);

/* Exported functions --------------------------------------------------------*/
/* Shell 命令 */
void sh_cmd_help(int argc, char **argv);
void sh_cmd_test(int argc, char **argv);
void sh_cmd_echo(int argc, char **argv);
void sh_cmd_history(int argc, char **argv);
void sh_cmd_clear(int argc, char **argv);

/* FileX操作命令 */
void sh_fx_init(int argc, char **argv);
void sh_fx_deinit(int argc, char **argv);
void sh_fx_time(int argc, char **argv);
void sh_fx_date(int argc, char **argv);
void sh_fx_mkdir(int argc, char **argv);
void sh_fx_cd(int argc, char **argv);
void sh_fx_pwd(int argc, char **argv);
void sh_fx_cat(int argc, char **argv);
void sh_fx_touch(int argc, char **argv);
void sh_fx_ls(int argc, char **argv);
void sh_fx_mv(int argc, char **argv);
void sh_fx_rm(int argc, char **argv);
void sh_fx_df(int argc, char **argv);
void sh_fx_chksd(int argc, char **argv);
void sh_fx_format(int argc, char **argv);
void sh_fx_tree(int argc, char **argv);
void sh_fx_find(int argc, char **argv);
void sh_fx_test(int argc, char **argv);
/* END -----------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif /* __FXSH_API_H */
