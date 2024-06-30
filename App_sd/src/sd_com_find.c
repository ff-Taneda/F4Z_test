/*
 * sd_com_find.c
 *
 *  Created on: 2024/06/25, 21:50:34
 *      Author: phy1335
 *        Note:
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "sd_api.h"

#include "fxsh_api.h"
#ifdef PRINT_TOTAL_TIME
#include "GetRunTime.h"
#endif
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * @brief 在src里匹配 pattern，包括通配符 * ?
 * @param src 匹配的字符串
 * @param pattern 匹配模式
 * @return 1 两者匹配 0 不匹配
 */
uint8_t match_pattern(const char *src, const char *pattern)
{
    const char *p1 = src;
    const char *p2 = pattern;

    while (*p2)
    {
        if (*p2 == '*')
        {
            while (*p2 == '*') // 跳过多余的 *
            {
                p2++;
            }
            if (*p2 == '\0') // 如果 * 是最后一个字符，则匹配
            {
                return 1;
            }
            while (*p1 && (*p1 != *p2))
            {
                p1++;
            }
        }
        else if (*p2 == '?')
        {
            if (*p1 == '\0') // 如果 ? 后面没有字符，则不匹配
            {
                return 0;
            }
            p1++;
            p2++;
        }
        else if (*p1 == *p2)
        {
            p1++;
            p2++;
        }
        else
        {
            return 0; // 字符不匹配
        }
    }

    return (*p1 == '\0' && *p2 == '\0'); // 两者都到达字符串末尾
}

uint16_t sd_com_find(char *path, char *pattern, uint8_t find_opt)
{
    uint16_t status = 0;
    char entry_name[SD_MAX_NAME] = {0};

    status = fx_directory_default_set(FX_SD_MEDIA, path);
    if (status != FX_SUCCESS)
    {
        FX_POST_ERROR_RETURN(status);
    }

#ifdef PRINT_TOTAL_TIME
    uint32_t t1 = 0, t2 = 0;
    t1 = GetRunTime();
#endif
    size_t i = 0;
    status = fx_directory_first_entry_find(FX_SD_MEDIA, entry_name);
    if (status != FX_SUCCESS)
    {
        if (status != FX_NO_MORE_ENTRIES)
        {
            FX_POST_ERROR_RETURN(status);
        }
    }
    else
    {
        if (match_pattern(entry_name, pattern))
        {
            printf("%s\n", entry_name);
            i++;
        }

        while (1)
        {
            status = fx_directory_next_entry_find(FX_SD_MEDIA, entry_name);
            if (status != FX_SUCCESS)
            {
                if (status != FX_NO_MORE_ENTRIES)
                {
                    FX_POST_ERROR(status);
                }
                break;
            }
            else
            {
                if (match_pattern(entry_name, pattern))
                {
                    printf("%s\n", entry_name);
                    i++;
                }
            }
        }
    }

    printf("Total %ld\n", i);

#ifdef PRINT_TOTAL_TIME
    t2 = GetRunTime();
    printf("===Total Time %ld ms\n", t2 - t1);
#endif

    status = fx_directory_default_set(FX_SD_MEDIA, fxsh_profile.cwd);
    if (status != FX_SUCCESS)
    {
        FX_POST_ERROR_RETURN(status);
    }

    return status;
}
