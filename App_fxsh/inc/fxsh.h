/*
 * fxsh.h
 *
 *  Created on: 2024/05/23, 23:19:43
 *      Author: phy1335
 *        Note: FileX-based Shell header
 *
 */
#ifndef __FXSH_H
#define __FXSH_H

#ifdef __cplusplus
extern "C"{
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fx_api.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define FXSH_MAX_INPUT FX_MAXIMUM_PATH // 一次输入最多256字节
#define FXSH_MAX_ARGS 10    // 一次输入参数的最大值
#define FXSH_MAX_HISTORY 20 // 历史记录条数
/* Exported macros -----------------------------------------------------------*/
/* Global variables declaration ----------------------------------------------*/
extern char sh_history[FXSH_MAX_HISTORY][FXSH_MAX_INPUT];
extern uint8_t sh_history_index;
/* Exported functions --------------------------------------------------------*/
uint8_t fxsh_entry(char *user, char *prompt, char *first_cmd);
void fxsh_execute(char *cmd_line);
void fxsh_small_input(char *inputbuf);
#ifdef __cplusplus
}
#endif
#endif /* __FXSH_H */
