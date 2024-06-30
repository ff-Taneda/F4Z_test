/*
 * sd_media.h
 *
 *  Created on: 2024/06/01, 18:29:38 
 *      Author: phy1335
 *        Note:   
 * 
 */
#ifndef __SD_MEIDA_H
#define __SD_MEIDA_H

#ifdef __cplusplus
extern "C"{
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "sd_com.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Global variables declaration ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
uint16_t sd_open(void);
uint16_t sd_close(void);
uint16_t sd_volume_get(char *name);
uint16_t sd_volume_set(char *name);
uint64_t sd_space_get(uint8_t opt);
uint16_t sd_media_check(void);
uint16_t sd_media_format(char *volume_name, uint16_t sector_bytes, uint16_t cluster_sectors, uint64_t total_bytes);
uint16_t sd_test_write(uint8_t test_opt);
uint16_t sd_abort(void);
/* END -----------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif /* __SD_MEIDA_H */
