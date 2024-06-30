/*
 * PrintError.h
 *
 *  Created on: 2024-06-02 T13:24:44+08:00
 *      Author: phy1335
 *        Note:
 *
 */
#ifndef __PRINT_ERROR_H
#define __PRINT_ERROR_H

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "sdio.h"
#include "app_filex.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#define PRINT_ERR printf("\n[File:%s][FUNC:%s()][Line: %d]:!!Error Hear!!\n", __FILE__, __func__, __LINE__);
#define Error_Handler()  \
    do                   \
    {                    \
        PRINT_ERR;       \
        Error_Handler(); \
    } while (0)

#define PRINT_FX_ERROR(x)  \
    do                     \
    {                      \
        PRINT_ERR          \
        print_fx_error(x); \
    } while (0);

#define PRINT_SDIO_ERROR(x)  \
    do                       \
    {                        \
        PRINT_ERR;           \
        print_sdio_error(x); \
    } while (0);
/* Global variables declaration ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* END -----------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif /* __PRINT_ERROR_H */
