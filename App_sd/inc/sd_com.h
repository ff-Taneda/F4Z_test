/*
 * sd_com.h
 *
 *  Created on: 2024/06/01, 18:29:45 
 *      Author: phy1335
 *        Note:   
 * 
 */
#ifndef __SD_COM_H
#define __SD_COM_H

#ifdef __cplusplus
extern "C"{
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Global variables declaration ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
uint16_t sd_com_delete(char *path, uint8_t rm_opt);

uint16_t sd_com_ls(char *path, uint8_t ls_opt);

uint16_t sd_com_move(char *old, char *new);

uint16_t sd_com_tree(char *path, uint8_t depth_max, uint8_t tree_opt);

uint16_t sd_com_find(char *path, char *pattern, uint8_t find_opt);
/* END -----------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif /* __SD_COM_H */
