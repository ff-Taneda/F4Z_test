/*
 * sd_file_cat.c
 *
 *  Created on: 2024/06/01, 18:20:43 
 *      Author: phy1335
 *        Note:   
 * 
 */
/* Includes ------------------------------------------------------------------*/
#include "sd_api.h"
#include "PrintArray.h"
#ifdef PRINT_TOTAL_TIME
#include "GetRunTime.h"
#endif
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define CAT_MAX_LINE 5 // 假定行号0-99999
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * @brief 显示path指向的文本文件，与读取时间。num>0 改用hex模式
 * @param opt [0] 不显示行号 [1] 显示行号 0~65535 ，可以修改CAT_MAX_LINE与line决定显示的行数
 * @param path 文件的路径
 * @param num 在hex模式下定每行数字
 * @return 0 success;filex return
 */
uint16_t sd_file_cat(uint8_t opt, char *path, uint8_t num)
{
    FX_FILE file;
    UINT status = 0;

    status = fx_file_open(FX_SD_MEDIA, &file, path, FX_OPEN_FOR_READ);
    if (status != FX_SUCCESS)
    {
        printf("Open [%s] failed\n", path);
        FX_POST_ERROR_RETURN(status);
    }

    uint8_t *cur = NULL;  // 查找\n计数指针
    uint8_t *find = NULL; // 上一行读取开始
    uint8_t *end = NULL;  // 一次读取的结尾
    uint16_t line = 1;    // 行号，从1开始
    ULONG cur_seek = 0;   // 文件当前读写指针

    __aligned(sizeof(ULONG)) uint8_t buffer[512] = {0}; // 读取缓冲区
    ULONG actual_read = 0;                              // 实际读取的

    printf("%s:[%s]\n", num ? "HEX" : "TXT", path);

#ifdef PRINT_TOTAL_TIME
    uint32_t t1 = 0, t2 = 0;
    t1 = GetRunTime();
#endif

    while (1)
    {
        status = fx_file_read(&file, buffer, sizeof(buffer), &actual_read);
        if (actual_read == 0) //=0 EOF
        {
            break;
        }
        if (status != FX_SUCCESS)
        {
            printf("read [%s] failed\n", path);
            PRINT_FX_ERROR(status);
            return status;
        }

        if (num)
        {
            PrintArray(buffer, num, actual_read);
        }
        else
        {
            end = buffer + actual_read;
            cur_seek += actual_read;
            cur = buffer;
            find = cur;

            while (cur < end)
            {
                if (*cur == '\n' || *cur == '\0')
                {
                    *cur = '\0';
                    if (opt)
                    {
                        printf("%*d| %s\n", CAT_MAX_LINE, line, find);
                    }
                    else
                    {
                        printf("%s\n", find);
                    }
                    find = cur + 1;
                    line++;
                }

                cur++;
            }
            if (*end == '\0')
            {
                if (opt)
                {
                    printf("%*d| %s\n", CAT_MAX_LINE, line, find);
                }
                else
                {
                    printf("%s\n", find);
                }
                line++;
                continue;
            }
            if (*(cur - 1) != '\n' && *(cur - 1) != '\0')
            {
                cur_seek -= (sizeof(buffer) - (find - buffer));
                fx_file_seek(&file, cur_seek);
            }
            memset(buffer, 0, sizeof(buffer));
        }
    }

#ifdef PRINT_TOTAL_TIME
    t2 = GetRunTime();
    printf("\n=====Print Time=%ld ms\n", t2 - t1);
#endif
    

    status = fx_file_close(&file);
    if (status != FX_SUCCESS)
    {
        printf("Close [%s] failed\n", path);
        PRINT_FX_ERROR(status);
    }
    return status;
}
