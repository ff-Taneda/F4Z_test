/*
 * mytree.c
 *
 *  Created on: 2024/06/29, 17:00:37
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
#define MAX_TRAVEL_DEPTH 64 // 最大遍历深度，深度越大，占用空间越大
#define _USE_UTF8_          // 使用UTF-8打印目录树，更美观
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t current_search_index[MAX_TRAVEL_DEPTH] = {0}; // 对应不同深度的条目的索引
uint64_t current_search_size[MAX_TRAVEL_DEPTH] = {0};  // 对应不同深度的条目的大小
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#ifndef _USE_UTF8_
/**
 * @brief 打印树形结构的缩进和节点
 * @param entry_name char* 要打印的节点名称
 * @param depth uint8_t 当前节点的深度（即层级）
 * @param is_last uint8_t * 指示在每一层中当前节点是否是该层的最后一个节点
 */
static void print_indent(const char *entry_name, uint8_t depth, uint8_t *is_last)
{
    if (depth == 0) // 是根节点
    {
        if (is_last[depth]) // 当前节点是该层的最后一个节点
        {
            printf("\\-"); // 打印结束引线
        }
        else
        {
            printf("|-"); // 不是最后一个节点，打印中继引线
        }
    }
    else // 不是根节点
    {
        for (size_t i = 0; i < depth; i++) // 循环遍历到当前深度减1的所有层
        {
            if (is_last[i])
            {
                printf("    "); // 该节点前的层级都没东西了。打印空白
            }
            else
            {
                printf("|    "); // 该节点前的某个层级下面还有东西。打印竖线
            }
        }
        if (is_last[depth]) // 当前节点是该层的最后一个节点
        {
            printf("\\"); // 打印结束引线
        }
        else
        {
            printf("+"); // 不是最后一个节点，打印中继引线
        }
    }
    printf("-- %s\n", entry_name); // 打印节点名称
}
#else
/**
 * @brief 打印树形结构的缩进和节点
 * @param entry_name char* 要打印的节点名称
 * @param depth uint8_t 当前节点的深度（即层级）
 * @param is_last uint8_t * 指示在每一层中当前节点是否是该层的最后一个节点
 * @note  注意使用UTF-8 否则自行修改
 */
static void print_indent(const char *entry_name, uint8_t depth, uint8_t *is_last)
{
    if (depth == 0) // 是根节点
    {
        if (is_last[depth]) // 当前节点是该层的最后一个节点
        {
            printf("└"); // 打印结束引线
        }
        else
        {
            printf("├"); // 不是最后一个节点，打印中继引线
        }
    }
    else // 不是根节点
    {
        for (size_t i = 0; i < depth; i++) // 循环遍历到当前深度减1的所有层
        {
            if (is_last[i])
            {
                printf("    "); // 该节点前的层级都没东西了。打印空白
            }
            else
            {
                printf("│   "); // 该节点前的某个层级下面还有东西。打印竖线
            }
        }
        if (is_last[depth]) // 当前节点是该层的最后一个节点
        {
            printf("└"); // 打印结束引线
        }
        else
        {
            printf("├"); // 不是最后一个节点，打印中继引线
        }
    }
    printf("── %s\n", entry_name); // 打印节点名称
}
#endif
/* Exported functions --------------------------------------------------------*/
/**
 * @brief
 * @param path
 * @param depth_max
 * @param tree_opt
 * @return
 */
uint16_t sd_com_tree(char *path, uint8_t depth_max, uint8_t tree_opt)
{
    uint16_t status = 0;
    /* 条目计数 */
    uint32_t total_cnt = 0; // 总条目计数
    uint32_t file_cnt = 0;  // 文件计数
    uint32_t dir_cnt = 0;   // 目录计数
    uint32_t hide_cnt = 0;  // 隐藏计数

    /* 遍历参数 */
    uint8_t current_search_depth = 0;                // 当前搜索深度
    uint8_t current_is_last[MAX_TRAVEL_DEPTH] = {0}; // 对应不同深度的条目是否是最后一个

    char entry_name[FX_MAX_LONG_NAME_LEN] = {0};      // 找到的条目
    char entry_name_next[FX_MAX_LONG_NAME_LEN] = {0}; // 下一个找到的条目，用于判读是否是最后一个
    UINT attributes = 0;                              // 属性，判断文件还是目录 换成uint32_t CubeIDE会报警告

    char default_dir[FX_MAXIMUM_PATH] = {0};

    /* 保存全局路径 */
    if (FX_SD_MEDIA->fx_media_id != FX_MEDIA_ID)
    {
        return (FX_MEDIA_NOT_OPEN);
    }
    strncpy(default_dir, FX_SD_MEDIA->fx_media_default_path.fx_path_string, FX_MAXIMUM_PATH);

    /* 设置搜索路径 */
    status = sd_dir_default_set(path);
    if (status != FX_SUCCESS)
    {
        printf("Set default dir failed.%s\n", "0x01");
        FX_POST_ERROR_RETURN(status);
    }
    /* 清除条目索引数组*/
    memset(current_search_index, 0, sizeof(current_search_index));
    memset(current_search_size, 0, sizeof(current_search_size));
    /* 打印表头 */
    printf("\n[%s]\n", path);

#ifdef PRINT_TOTAL_TIME
    uint32_t t1 = 0, t2 = 0;
    t1 = GetRunTime();
#endif

    /* 开始搜索 */
    while (1)
    {
        /* 根据当前深度的索引是否为0设置fx内部的索引值 */
        // if (current_search_index[current_search_depth] == 0)
        // {
            // (FX_SD_MEDIA)->fx_media_default_path.fx_path_current_entry = 0;
        // }
        // else
        // {
            /* 需要同时设置FileX内部的索引值和目录大小 */
            (FX_SD_MEDIA)->fx_media_default_path.fx_path_current_entry = current_search_index[current_search_depth];
//            (FX_SD_MEDIA)->fx_media_default_path.fx_path_directory.fx_dir_entry_file_size = current_search_size[current_search_depth];
        // }

        /* 根据当前是不是最后一个，继续查找*/
        if (current_is_last[current_search_depth] == 0)
        {
            /* 当前不是最后一个，继续查找 */
            status = fx_directory_next_full_entry_find(FX_SD_MEDIA, entry_name, &attributes,
                                                       FX_NULL, FX_NULL, FX_NULL, FX_NULL, FX_NULL, FX_NULL, FX_NULL);
            if (status != FX_SUCCESS)
            {
                if (status == FX_NO_MORE_ENTRIES) // 没有更多了，但不应该在本次find进入这里
                {
                    printf("Can you enter here?\n");
                    break;
                }
                else // 其他错误
                {
                    printf("Find error\n");
                    FX_POST_ERROR(status);
                    break;
                }
            }
            else // 成功找到下一个
            {
                /* 找到一项，保存当前索引 */
                current_search_index[current_search_depth] = (FX_SD_MEDIA)->fx_media_default_path.fx_path_current_entry;
                // current_search_size[current_search_depth] = (FX_SD_MEDIA)->fx_media_default_path.fx_path_directory.fx_dir_entry_file_size;

                /* 再找下一个看看是不是到头了 */
                status = fx_directory_next_entry_find(FX_SD_MEDIA, entry_name_next);
                if (status != FX_SUCCESS)
                {
                    if (status == FX_NO_MORE_ENTRIES) // entry_name是找到的是最后一个
                    {
                        /* 后面没有了，entry_name是最后一个 */
                        current_is_last[current_search_depth] = 1;
                    }
                    else // 其他错误
                    {
                        printf("Find next error\n");
                        FX_POST_ERROR(status);
                        break;
                    }
                }
                else
                {
                    /* 后面还有，恢复索引到entry_name_next之前 */
                    (FX_SD_MEDIA)->fx_media_default_path.fx_path_current_entry = current_search_index[current_search_depth];
                    // (FX_SD_MEDIA)->fx_media_default_path.fx_path_directory.fx_dir_entry_file_size = current_search_size[current_search_depth];
                }
            }
        }
        else
        {
            /* 深度为0，遍历完成了 */
            if (current_search_depth == 0)
            {
                break;
            }
            else
            {
                /* 返回上一层 */
                status = fx_directory_default_set(FX_SD_MEDIA, "..");
                if (status != FX_SUCCESS)
                {
                    printf("Set default dir failed.%s\n", "0x02");
                    FX_POST_ERROR(status);
                    break;
                }
                else
                {
                    /* 清空本层索引参数 */
                    current_search_index[current_search_depth] = 0;
                    // current_is_last[current_search_depth] = 0;
                    current_search_size[current_search_depth] = 0;
                    current_search_depth--; // 退回上层
                    continue;
                }
            }
        }

        /* 处理搜索到的东西 */
        /* 处理"."和".." */
        if ((strcmp(entry_name, ".") == 0) || (strcmp(entry_name, "..") == 0))
        {
            continue;
        }

        if (attributes & FX_VOLUME) // 是卷，跳过
        {
            continue;
        }

        /* 处理隐藏 */
        if (attributes & FX_HIDDEN)
        {
            /* 显示隐藏 */
            if (tree_opt & SD_SHOW_HIDE)
            {
                hide_cnt++;
            }
            else // 跳过
            {
                continue;
            }
        }

        if (attributes & FX_DIRECTORY) // 是目录
        {
            dir_cnt++;
            total_cnt++;
            print_indent(entry_name, current_search_depth, current_is_last);

            /* 达到设定的遍历深度了没? */
            if (current_search_depth >= depth_max - 1)
            {
                continue;
            }
            /* 进入此文件夹 */
            status = fx_directory_default_set(FX_SD_MEDIA, entry_name);
            if (FX_SUCCESS != status)
            {
                printf("Set default dir failed.%s\n", entry_name);
                FX_POST_ERROR(status);
                break;
            }
            else
            {
                current_search_depth++; // 搜索深度+1
                /* 清空下一层索引参数 */
                current_search_index[current_search_depth] = 0;
                current_search_size[current_search_depth] = 0;
                current_is_last[current_search_depth] = 0;
                continue;
            }
        }
        else // 是文件
        {
            file_cnt++;
            total_cnt++;
            print_indent(entry_name, current_search_depth, current_is_last);
        }
    }

    printf("Total:%ld    Dir:%ld    File:%ld\n", total_cnt, dir_cnt, file_cnt);
    if (attributes & FX_HIDDEN)
    {
        printf("hide:%ld\n", hide_cnt);
    }
#ifdef PRINT_TOTAL_TIME
    t2 = GetRunTime();
    printf("===Total Time %ld ms\n", t2 - t1);
#endif

    /* 恢复原来参数 */
    /* 恢复全局路径 */
    status = sd_dir_default_set(default_dir);
    if (status != FX_SUCCESS)
    {
        printf("Set default dir failed.%s\n", default_dir);
        FX_POST_ERROR(status);
    }
    /* 恢复索引条目 */
    (FX_SD_MEDIA)->fx_media_default_path.fx_path_current_entry = 0;

    return status;
}
