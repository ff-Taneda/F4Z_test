/*
 * sd_com_delete.c
 *
 *  Created on: 2024-06-16 T01:17:55+08:00
 *      Author: phy1335
 *        Note: 删除
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "sd_api.h"
#include "fxsh_api.h"
#include "fxsh.h"
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
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
uint16_t sd_com_delete(char *path, uint8_t rm_opt)
{
    uint16_t status = 0;

    /* 删除计数 */
    uint32_t total_cnt = 0;     // 总删除尝试计数
    uint32_t file_cnt = 0;      // 删除文件计数
    uint32_t dir_cnt = 0;       // 删除目录计数
    uint32_t delete_failed = 0; // 删除失败计数

    /* 用到的参数 */
    uint8_t current_search_depth = 0;                    // 当前搜索深度
    uint8_t dir_seek_index = 0;                          // 记录当前正在遍历的目录条目的索引
    uint8_t dir_seek_init = 0;                           // 记录是否已经初始化了目录，=0使用first获取第一个条目;=1使用next获取下一个条目
    char entry_name[SD_MAX_NAME] = {0};                  // 寻找到的目录或文件
    char root[SD_MAX_NAME] = {0};                        // 上级搜索目录
    char *current_dir = NULL;                            // 当前搜索目录
    UINT attributes = 0;                                 // 得到条目属性 在这里换成uint32_t怎么会报警告？
    uint16_t old_seek_index[FX_MAX_LONG_NAME_LEN] = {0}; // 存储在不同深度遍历到的条目序号的数组
    uint8_t delete_failed_times = 0;                     // 删除一个条目失败的次数
    char get_yes[4] = {0};                               // 删除前获得y

#ifdef PRINT_TOTAL_TIME
    uint32_t t1 = 0, t2 = 0;
    t1 = GetRunTime();
#endif

    if ((rm_opt & RM_DIR) && (rm_opt & RM_FORCE)) // rm -rf 递归删除目录
    {
        /* 设置指定删除路径 */
        status = sd_dir_default_set(path);
        if (status != FX_SUCCESS)
        {
            FX_POST_ERROR_RETURN(status);
        }

        /* 先获取当前路径，并存储 */
        status = fx_directory_default_get(FX_SD_MEDIA, &current_dir);
        if (FX_SUCCESS != status)
        {
            printf("Get default dir failed 1.");
            FX_POST_ERROR_RETURN(status);
        }

        //  current_dir是指向全局当前默认目录的指针，指针内容会随fx_directory_default_set而改变
        // 所以需要先复制保存
        strncpy(root, current_dir, FX_MAXIMUM_PATH);

        // old_seek_index[0] = 0;//已经初始化为0了
        // dir_seek_init = 0;
        while (1)
        {
            /* 如果是第一次遍历新目录（dir_seek_init= 0），则使用first函数找到目录中的第一个条目 */
            if (dir_seek_init == 0)
            {
                status = fx_directory_first_full_entry_find(FX_SD_MEDIA, entry_name, &attributes, FX_NULL, FX_NULL,
                                                            FX_NULL, FX_NULL, FX_NULL, FX_NULL, FX_NULL);
                dir_seek_init = 1;  // 已经初始化本级搜索的目录
                dir_seek_index = 0; // 本级搜索条目计数置零
            }

            /* 使用next 寻找下一个条目。维护dir seek的位置 */
            for (UINT i = dir_seek_index; i < old_seek_index[current_search_depth]; i++)
            {
                status = fx_directory_next_full_entry_find(FX_SD_MEDIA, entry_name, &attributes, FX_NULL, FX_NULL,
                                                           FX_NULL, FX_NULL, FX_NULL, FX_NULL, FX_NULL);
                dir_seek_index++; // 本级搜索条目计数+1
            }

            if (status != FX_SUCCESS)
            {
                /* 本层目录遍历完成了 */
                if (status != FX_NO_MORE_ENTRIES)
                    FX_POST_ERROR(status);

                /*  获取当前路径 */
                status = fx_directory_default_get(FX_SD_MEDIA, &current_dir);
                if (status != FX_SUCCESS)
                {
                    printf("Get default dir failed 2.");
                    FX_POST_ERROR(status);
                }

                /* 如果处在一级目录，就表示遍历完成了 */
                if (strcmp(root, current_dir) == 0)
                {
                    /* 显示删除过程 */
                    if (rm_opt & RM_SHOW)
                    {
                        printf("Deleting dir [%s]...\n", path);
                    }
                    /* 尝试删除目标文件夹 */
                    status = fx_directory_delete(FX_SD_MEDIA, path);
                    if (status != FX_SUCCESS)
                    {
                        delete_failed++;
                        printf("Delete dir [%s] failed\n", path);
                        FX_POST_ERROR(status);
                    }
                    break;
                }

                /* 不然就返回上一目录 */
                status = fx_directory_default_set(FX_SD_MEDIA, "..");
                if (status != FX_SUCCESS)
                {
                    printf("Set default dir [%s] failed.\n", "..");
                    FX_POST_ERROR(status);
                }

                current_search_depth--; // 返回上一层
                dir_seek_init = 0;      // 需要刷新一下entry_find逻辑
                continue;
            }

            /* 处理"."目录和".."目录 */
            if ((strcmp(entry_name, ".") == 0) || (strcmp(entry_name, "..") == 0))
            {
                old_seek_index[current_search_depth]++;
                continue;
            }

            /* 处理搜到的条目 */
            if (attributes & FX_VOLUME) // 是卷，跳过
            {
                continue;
            }
            else if (attributes & FX_DIRECTORY) // 是目录
            {
                /* 显示删除过程 */
                if (rm_opt & RM_SHOW)
                {
                    printf("Deleting dir [%s]...\n", entry_name);
                }
                /* 尝试删除 */
                status = fx_directory_delete(FX_SD_MEDIA, entry_name);
                if (status != FX_SUCCESS)
                {
                    /* 目录不是空的 */
                    if (status == FX_DIR_NOT_EMPTY)
                    {
                        /* 进入这个目录 */
                        status = fx_directory_default_set(FX_SD_MEDIA, entry_name);
                        if (status != FX_SUCCESS)
                        {
                            printf("Set default dir [%s] failed.\n", entry_name);
                            FX_POST_ERROR(status);
                        }
                        else
                        {
                            /* 当前删除目录不是空的，进入这个目录 */
                            current_search_depth++;                   // 遍历深度加1，进入下一层
                            old_seek_index[current_search_depth] = 0; // 初始化新层的条目序号
                            dir_seek_init = 0;                        // 下一层的搜索需要初始化
                            continue;
                        }
                    }
                    else // 其他错误
                    {
                        delete_failed_times++;
                        /* 删了除次数大于5次判断删除失败 */
                        if (delete_failed_times > 5)
                        {
                            delete_failed_times = 0;
                            delete_failed++;
                            old_seek_index[current_search_depth]++; // 删除5次失败就算，寻找下一个
                            printf("Delete dir[%s] failed\n", entry_name);
                            FX_POST_ERROR(status);
                        }
                    }
                }
                else // 目录是空的，直接删除成功，计数增加
                {
                    total_cnt++;
                    dir_cnt++;
                    old_seek_index[current_search_depth]++; // 增加条目数
                }
            }
            else // 是文件
            {
                /* 显示删除过程 */
                if (rm_opt & RM_SHOW)
                {
                    printf("Deleting file [%s]...\n", entry_name);
                }
                status = fx_file_delete(FX_SD_MEDIA, entry_name);
                if (FX_SUCCESS != status)
                {
                    delete_failed_times++;
                    /* 删除次数大于5次判断删除失败 */
                    if (delete_failed_times > 5)
                    {
                        delete_failed_times = 0;
                        delete_failed++;
                        old_seek_index[current_search_depth]++; // 删除5次失败就算了，寻找下一个
                        printf("Delete file [%s] failed\n", entry_name);
                        FX_POST_ERROR(status);
                    }
                }
                else // 成功删除文件，计数增加
                {
                    total_cnt++;
                    file_cnt++;
                    old_seek_index[current_search_depth]++; // 删除成功，增加条目数
                }
            }
        }

        /* 从root恢复原来路径 */
        // status = sd_dir_default_set(root);
        // if (status != FX_SUCCESS)
        // {
        //     FX_POST_ERROR(status);
        // }
    }
    else if (rm_opt & RM_DIR) // 删除目录 rm -r dir 仅仅删除一个 空 目录，要删除全部内容 rm -rf
    {
        /* 删除前提示 */
        if (rm_opt & RM_NOTICE)
        {
            printf("Delete [%s] ? y:n\n", path);
            fxsh_small_input(get_yes);
            if (strcmp(get_yes, "y") != 0)
            {
                return FX_SUCCESS;
            }
        }
        /* 显示删除过程 */
        if (rm_opt & RM_SHOW)
        {
            printf("Deleting dir [%s]...\n", path);
        }
        status = fx_directory_delete(FX_SD_MEDIA, path);
        if (status != FX_SUCCESS)
        {
            delete_failed++;
            FX_POST_ERROR(status);
        }
        else
        {
            total_cnt++;
            dir_cnt++;
        }
    }
    else // 什么都没加 删除文件 rm file
    {
        /* 删除前提示 */
        if (rm_opt & RM_NOTICE)
        {
            printf("Delete [%s] ? y:n\n", path);
            fxsh_small_input(get_yes);
            if (strcmp(get_yes, "y") != 0)
            {
                return FX_SUCCESS;
            }
        }
        /* 显示删除过程 */
        if (rm_opt & RM_SHOW)
        {
            printf("Deleting file [%s]...\n", path);
        }
        status = fx_file_delete(FX_SD_MEDIA, path);
        if (status != FX_SUCCESS)
        {
            delete_failed++;
            FX_POST_ERROR(status);
        }
        else
        {
            total_cnt++;
            file_cnt++;
        }
    }

    /* 是否需要手动刷新? */
    status = fx_media_flush(FX_SD_MEDIA);
    if (status != FX_SUCCESS)
    {
        printf("Flush failed\n");
        FX_POST_ERROR(status);
    }
    /* 显示统计 */
    printf("Delete Total %ld  Dir %ld    File %ld\nFailed %ld\n", total_cnt, dir_cnt, file_cnt, delete_failed);

#ifdef PRINT_TOTAL_TIME
    t2 = GetRunTime();
    printf("===Total Time %ld ms\n", t2 - t1);
#endif

    return status;
}
