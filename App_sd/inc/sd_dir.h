/*
 * sd_dir.h
 *
 *  Created on: 2024/06/01, 15:56:22 
 *      Author: phy1335
 *        Note:   
 * 
 */
#ifndef __Sd_DIR_H
#define __Sd_DIR_H

#ifdef __cplusplus
extern "C"{
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "sd_api.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Global variables declaration ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

uint16_t sd_dir_create(char *path);

uint16_t sd_dir_default_set(char *path);

uint16_t sd_dir_default_get(char *path);

uint16_t sd_dir_exist(char *path);

/* END -----------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif /* __Sd_DIR_H */
