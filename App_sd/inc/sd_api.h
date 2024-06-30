/*
 * sd_api.h
 *
 *  Created on: 2024 05 15, 23 55 08 
 *      Author: phy1335
 *        Note:   
 * 
 */
#ifndef __SD_API_H
#define __SD_API_H

#ifdef __cplusplus
extern "C"{
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "PrintError.h"
#include "app_filex.h" 
#include "sd_media.h"
#include "sd_com.h"
#include "sd_dir.h"
#include "sd_file.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define SD_MAX_NAME FX_MAXIMUM_PATH
#define SD_SHOW_HIDE 0x01
#define SD_SHOW_FRIENDLY 0x02
#define RM_DIR 0x01 
#define RM_NOTICE 0x02
#define RM_FORCE 0x04 
#define RM_EDIR 0x8//empty dir
#define RM_SHOW 0x10
#define TREE_MAX_DEPTH SD_MAX_NAME
/* Exported macros -----------------------------------------------------------*/
#define FX_SD_MEDIA (&__sdio_disk)
/* 判断status */
#define FX_POST_ERROR(x)          \
    do                            \
    {                             \
        if (x != FX_SUCCESS)      \
        {                         \
            print_fx_error(x);    \
        }                         \
    } while (0)
/* 判断status并返回 */
#define FX_POST_ERROR_RETURN(x) \
    do                          \
    {                           \
        if (x != FX_SUCCESS)    \
        {                       \
            PRINT_FX_ERROR(x);  \
            return (x);         \
        }                       \
    } while (0)
    
/* 开启操作时间显示 */
#define PRINT_TOTAL_TIME
/* Global variables declaration ----------------------------------------------*/
extern FX_MEDIA __sdio_disk;
extern uint8_t sdio_buffer[16 * FX_STM32_SD_DEFAULT_SECTOR_SIZE];
/* Exported functions --------------------------------------------------------*/


#ifdef __cplusplus
}
#endif
#endif /* __SD_API_H */
