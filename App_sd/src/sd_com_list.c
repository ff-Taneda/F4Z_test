/*
 * sd_com_list.c
 *
 *  Created on: 2024/06/01, 18:20:07
 *      Author: phy1335
 *        Note:
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "sd_api.h"
#include "fxsh_api.h"
#include "string.h"
#ifdef PRINT_TOTAL_TIME
#include "GetRunTime.h"
#endif
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void get_attributes(UINT attributes, char *attr);
/* Private functions ---------------------------------------------------------*/
/**
 * @brief 从attributes解释属性存放在attr数组里
 * @param attributes 属性
 * @param attr 数组存放解释的属性
 */
static void get_attributes(UINT attributes, char *attr)
{
    attr[0] = (attributes & FX_DIRECTORY) ? 'd' : 'f';  // 目录还是文件
    attr[0] = (attributes & FX_VOLUME) ? 'v' : attr[0]; // 是卷吗
    attr[1] = (attributes & FX_ARCHIVE) ? 'a' : '-';    // 存档
    attr[2] = (attributes & FX_SYSTEM) ? 's' : '-';     // 系统文件
    attr[3] = (attributes & FX_HIDDEN) ? 'h' : '-';     // 隐藏文件
    // attr[4] = (attributes & FX_READ_ONLY) ? 'R' : 'R';//只读
    attr[4] = 'r';                                     // 可读
    attr[5] = (attributes & FX_READ_ONLY) ? '-' : 'w'; // 可写
    // attr[6] = 'x';//可执行,未使用
    attr[6] = '.';  // 结束
    attr[7] = '\0'; // '\0'
}
/* Exported functions --------------------------------------------------------*/

/**
 * @brief 显示路径下的内容
 * @param path 要显示的路径
 * @returnq
 */
uint16_t sd_com_ls(char *path, uint8_t ls_opt)
{
    UINT status = 0;
    char attr_show[8] = {0}; // V/D/FASHRW
    char entry_name[FXSH_MAX_PATH] = {0};
    UINT attributes = 0;
    ULONG size_bytes = 0;
    /*  修改的年月日时分秒 */
    UINT year_modified = 0, month_modified = 0, day_modified = 0;
    UINT hour_modified = 0, minute_modified = 0, second_modified = 0;

    static char *units[] = {"B", "K", "M", "G", "T", "P"}; // 友好显示单位

    /* 设置指定路径 */
    status = sd_dir_default_set(path);
    if (status != FX_SUCCESS)
    {
        FX_POST_ERROR_RETURN(status);
    }
#ifdef PRINT_TOTAL_TIME
    uint32_t t1 = 0, t2 = 0;
    t1 = GetRunTime();
#endif

    /* 打印表头 */
    printf("[%s]\n", path);
    printf("\ncount|dashrw. ____size___. yyyy-mm-dd hh:mm:ss name\n");

#if 0
    /* 要设置first entry，否则再次调用这个函数会读取出错 */
     status = fx_directory_first_full_entry_find(FX_SD_MEDIA, entry_name,
                                                &attributes, &size_bytes, &year_modified, &month_modified, &day_modified,
                                                &m_hour, &m_minute, &second_modified);
    if (status != FX_SUCCESS)
    {
        printf("[ls] first entry not found\n");
        PRINT_FX_ERROR(status);
        return status;
    }
    get_attributes(attributes, attr_show);
    printf("%s %*ld %04d-%02d-%02d %02d:%02d:%02d %s\n", attr_show, 8, size_bytes, year_modified, month_modified, day_modified,
           m_hour, m_minute, second_modified, entry_name);
#endif

    /* 可以用这个代替上面的寻找第一个条目 */
    (FX_SD_MEDIA)->fx_media_default_path.fx_path_current_entry = 0;

    /* 寻找数目计数 */
    uint32_t total_cnt = 0, file_cnt = 0, dir_cnt = 0;
    uint32_t sys_cnt = 0, hide_cnt = 0;
    while (1)
    {
        

        /* 寻找下一个项目 */
        status = fx_directory_next_full_entry_find(FX_SD_MEDIA, entry_name, &attributes,
                                                   &size_bytes, &year_modified, &month_modified, &day_modified,
                                                   &hour_modified, &minute_modified, &second_modified);
        /* 寻找出错 */
        if (status != FX_SUCCESS || entry_name[0] == 0)
        {
            if (status != FX_NO_MORE_ENTRIES) // 不是寻找到头了
            {
                FX_POST_ERROR(status);
            }
            break;
        }

        /* 转换属性 */
        get_attributes(attributes, attr_show);

        /* 判断目录还是文件 */
        if (attr_show[0] == 'd')
        {
            dir_cnt++;
        }
        if (attr_show[0] == 'f')
        {
            file_cnt++;
        }

        /* 处理系统文件 */
        if (attr_show[2] == 's')
        {
            sys_cnt++;
        }

        if (strcmp(entry_name, ".") == 0 || strcmp(entry_name, "..") == 0)
        {
            /* 不显示隐藏 */
            if (!(ls_opt & SD_SHOW_HIDE))
            {
                dir_cnt--;
                continue;
            }
        }

        /* 处理隐藏 */
        if (attr_show[3] == 'h')
        {
            /* 显示隐藏 */
            if (ls_opt & SD_SHOW_HIDE)
            {
                hide_cnt++;
            }
            else
            {
                if (attr_show[0] == 'd')
                {
                    dir_cnt--;
                }
                else
                {
                    file_cnt--;
                }
                total_cnt--;
                continue;
            }
        }

        /* 逐行打印序号和属性 */
        printf("%*d|%s ", 5, total_cnt + 1, attr_show);

        /* 打印友好格式的文件大小 */
        if ((size_bytes > 1024) && (ls_opt & SD_SHOW_FRIENDLY))
        {
            float show_size = (float)size_bytes;
            int unit_index = 0;
            while (show_size >= 1024 && unit_index < sizeof(units) / sizeof(units[0]) - 1)
            {
                show_size /= 1024.0f;
                unit_index++;
            }
            /* 打印结果，保留两位小数，10位，右对齐 */
            printf("%*.2f %s", 10, show_size, units[unit_index]);
        }
        else
        {
            /* 直接打印字节大小，10位，无小数，右对齐 */
            printf("%*ld B", 10, size_bytes);
        }
        /* 打印剩下的属性 */
        printf(" %04d-%02d-%02d %02d:%02d:%02d %s\n", year_modified, month_modified, day_modified,
               hour_modified, minute_modified, second_modified, entry_name);
        total_cnt++;
    }

    /* 打印统计 */
    printf("Total:%ld    Dir:%ld    File:%ld    System:%ld    Hide:%ld\n", total_cnt, dir_cnt, file_cnt, sys_cnt, hide_cnt);

#ifdef PRINT_TOTAL_TIME
    t2 = GetRunTime();
    printf("===Total Time %ld ms\n", t2 - t1);
#endif

    /* 恢复原来路径 */
    status = sd_dir_default_set(fxsh_profile.cwd);
    if (status != FX_SUCCESS)
    {
        FX_POST_ERROR(status);
    }

    return status;
}
