/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    app_filex.c
 * @author  MCD Application Team
 * @brief   FileX applicative file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_filex.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "GetTime.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint8_t is_filex_init = 0;
char *fxerrorcode[] =
    {
        "FX_SUCCESS ,0x00",
        "FX_BOOT_ERROR ,0x01",
        "FX_MEDIA_INVALID ,0x02",
        "FX_FAT_READ_ERROR ,0x03",
        "FX_NOT_FOUND ,0x04",
        "FX_NOT_A_FILE ,0x05",
        "FX_ACCESS_ERROR ,0x06",
        "FX_NOT_OPEN ,0x07",
        "FX_FILE_CORRUPT ,0x08",
        "FX_END_OF_FILE ,0x09",
        "FX_NO_MORE_SPACE ,0x0A",
        "FX_ALREADY_CREATED ,0x0B",
        "FX_INVALID_NAME ,0x0C",
        "FX_INVALID_PATH ,0x0D",
        "FX_NOT_DIRECTORY ,0x0E",
        "FX_NO_MORE_ENTRIES ,0x0F",
        "FX_DIR_NOT_EMPTY ,0x10",
        "FX_MEDIA_NOT_OPEN ,0x11",
        "FX_INVALID_YEAR ,0x12",
        "FX_INVALID_MONTH ,0x13",
        "FX_INVALID_DAY ,0x14",
        "FX_INVALID_HOUR ,0x15",
        "FX_INVALID_MINUTE ,0x16",
        "FX_INVALID_SECOND ,0x17",
        "FX_PTR_ERROR ,0x18",
        "FX_INVALID_ATTR ,0x19",
        "FX_CALLER_ERROR ,0x20",
        "FX_BUFFER_ERROR ,0x21",
        "FX_NOT_IMPLEMENTED ,0x22",
        "FX_WRITE_PROTECT ,0x23",
        "FX_INVALID_OPTION ,0x24",
        "FX_SECTOR_INVALID ,0x89",
        "FX_IO_ERROR ,0x90",
        "FX_NOT_ENOUGH_MEMORY ,0x91",
        "FX_ERROR_FIXED ,0x92",
        "FX_ERROR_NOT_FIXED ,0x93",
        "FX_NOT_AVAILABLE ,0x94",
        "FX_INVALID_CHECKSUM ,0x95",
        "FX_READ_CONTINUE ,0x96",
        "FX_INVALID_STATE ,0x97"};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/**
  * @brief  Application FileX Initialization.
  * @retval int
  */
UINT MX_FileX_Init(VOID)
{
  UINT status = FX_SUCCESS;

  /* USER CODE BEGIN MX_FileX_Init */

  /* USER CODE END MX_FileX_Init */

  /* Initialize FileX.  */
  fx_system_initialize();

  /* USER CODE BEGIN MX_FileX_Init 1*/
  status = fx_system_date_set(__NOW_YEAR__, __NOW_MONTH__, __NOW_DAY__);
  status = fx_system_time_set(__NOW_HOUR__, __NOW_MINUTE__, __NOW_SECOND__);
  /* USER CODE END MX_FileX_Init 1*/

  return status;
}

/* USER CODE BEGIN 1 */
void print_fx_error(UINT status)
{
  printf("FileX Error:");
  if (status >= 0 && status <= 0x19)
  {
    printf("%s\n", fxerrorcode[status]);
  }
  if (status >= 0x20 && status <= 0x24)
  {
    printf("%s\n", fxerrorcode[status - 6]);
  }
  if (status == 89)
  {
    printf("%s\n", fxerrorcode[status - 106]);
  }
  if (status >= 0x90 && status <= 0x97)
  {
    printf("%s\n", fxerrorcode[status - 112]);
  }
}
/* USER CODE END 1 */
