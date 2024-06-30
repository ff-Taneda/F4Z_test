/*
 * sd_file.h
 *
 *  Created on: 2024/06/01, 15:55:23 
 *      Author: phy1335
 *        Note:   
 * 
 */
#ifndef __SD_FILE_H
#define __SD_FILE_H

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

uint16_t sd_file_write(char *path, void *buf, uint32_t size, uint8_t append);

uint16_t sd_file_cat(uint8_t opt, char *path, uint8_t num);

uint16_t sd_file_create(char *path);

uint16_t sd_file_read(char *path, void *buf, uint32_t request_size, uint32_t *actual_size);

uint16_t sd_file_set_time(char *path, uint16_t yyyy, uint16_t m, uint16_t d, uint16_t hh, uint16_t mm, uint16_t ss);

uint16_t sd_file_cat(uint8_t opt, char *path, uint8_t num);
/* END -----------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif /* __SD_FILE_H */
