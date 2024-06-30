/*
 * sd_media_test.c
 *
 *  Created on: 2024/06/27, 23:11:29
 *      Author: phy1335
 *        Note:
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "sd_api.h"
#include "GetRunTime.h"
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define TEST_TIMES 64
#define TEST_SIZE 256 * 1024
#define TEST_BIG_SIZE (64 * 1024 * 1024UL) // 64MB
//#define SD_TEST
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
#ifdef SD_TEST
__section(".exram") uint8_t test_buf[1024 * 64] = {0};
FX_FILE file_test;
#endif /* SD_TEST */
uint16_t sd_test_write(uint8_t test_opt)
{
    uint16_t status = 0;
#ifdef SD_TEST
    char filename[TEST_TIMES][32] = {0};

    /* 填充文件名 */
    if (test_opt == 1)
    {
        for (size_t i = 0; i < TEST_TIMES; i++)
        {
            snprintf(filename[i], 32, "test_bin%ld", i);
        }
    }
    else if (test_opt == 2)
    {
        snprintf(filename[0], 32, "test_bin_big");
    }

    /* 删除先前可能存在的文件 */
    for (size_t i = 0; i < TEST_TIMES; i++)
    {
        status = fx_file_delete(FX_SD_MEDIA, filename[i]);
    }
    status = fx_media_flush(FX_SD_MEDIA);
    if (status != FX_SUCCESS)
    {
        printf("Flush failed:1\n");
        FX_POST_ERROR_RETURN(status);
    }

    /* 填充测试数组 */
    for (size_t i = 0; i < sizeof(test_buf); i++)
    {
        test_buf[i] = i;
    }

    /* 创建测试文件 */
    if (test_opt == 1)
    {
        for (size_t i = 0; i < TEST_TIMES; i++)
        {
            status = fx_file_create(FX_SD_MEDIA, filename[i]);
            if (status != FX_SUCCESS)
            {
                printf("Create file[%s] failed\n", filename[i]);
                FX_POST_ERROR_RETURN(status);
            }
        }
        printf("Test size %ld KB\n", TEST_SIZE / 1024);
    }
    else if (test_opt == 2)
    {
        status = fx_file_create(FX_SD_MEDIA, filename[0]);
        if (status != FX_SUCCESS)
        {
            printf("Create file[%s] failed\n", filename[0]);
            FX_POST_ERROR_RETURN(status);
        }
        printf("Test size %ld KB\n", TEST_BIG_SIZE / 1024);
    }
    status = fx_media_flush(FX_SD_MEDIA);
    if (status != FX_SUCCESS)
    {
        printf("Flush failed:2\n");
        FX_POST_ERROR_RETURN(status);
    }

    uint32_t time[TEST_TIMES * 2] = {0}; // 测试时间
    /* 开始写入 */
    if (test_opt == 1) // 64次 256KB写入测试 总计16MB
    {
        for (size_t i = 0; i < TEST_TIMES; i++)
        {
            status = fx_file_open(FX_SD_MEDIA, &file_test, filename[i], FX_OPEN_FOR_WRITE);
            if (status != FX_SUCCESS)
            {
                printf("Open [%s] failed\n", filename[i]);
                FX_POST_ERROR(status);
                continue;
            }

            /* 每个文件连续写入4次 总计64*4=256KB */
            time[2 * i] = GetRunTime();
            status = fx_file_write(&file_test, test_buf, sizeof(test_buf));
            status = fx_file_write(&file_test, test_buf, sizeof(test_buf));
            status = fx_file_write(&file_test, test_buf, sizeof(test_buf));
            status = fx_file_write(&file_test, test_buf, sizeof(test_buf));
            time[2 * i + 1] = GetRunTime();

            if (status != FX_SUCCESS)
            {
                printf("Write [%s] failed\n", filename[i]);
                FX_POST_ERROR(status);
            }

            status = fx_file_close(&file_test);
            if (status != FX_SUCCESS)
            {
                printf("Close [%s] failed\n", filename[i]);
                FX_POST_ERROR_RETURN(status);
                continue;
            }

            status = fx_media_flush(FX_SD_MEDIA);
            if (status != FX_SUCCESS)
            {
                printf("Flush failed\n");
                FX_POST_ERROR(status);
                continue;
            }
            /* 打印本次结果 */
            printf("[%d] %ld ms. Size %ld KB. Speed %d KB/S\n",
                   i,
                   time[2 * i + 1] - time[2 * i],
                   4 * sizeof(test_buf) / 1024,
                   4 * sizeof(test_buf) / 1024 * 1000 / (time[2 * i + 1] - time[2 * i]));
        }
    }
    else if (test_opt == 2) // 1次 64MB写入测试
    {
        status = fx_file_open(FX_SD_MEDIA, &file_test, filename[0], FX_OPEN_FOR_WRITE);
        if (status != FX_SUCCESS)
        {
            printf("Open [%s] failed\n", filename[0]);
            FX_POST_ERROR_RETURN(status);
        }

        /* 需要先分配空间吗？ */
        // ULONG actual_allocation;
        // status = fx_file_best_effort_allocate(&file_test, TEST_BIG_SIZE, &actual_allocation);
        // if (status != FX_SUCCESS)
        // {
        //     printf("alloc [%s] failed\n", file_big);
        //     FX_POST_ERROR_RETURN(status);
        // }
        // if (actual_allocation != (32 * 1024 * 1024UL))
        // {
        //     printf("alloc 32MB failed\n");
        //     FX_POST_ERROR_RETURN(status);
        // }

        /* 一次写入16*64=1MB,写入64次，总计64*1MB=64MB */
        for (size_t i = 0; i < (TEST_BIG_SIZE / sizeof(test_buf) / 16); i++)
        {
            time[2 * i] = GetRunTime();
            status = fx_file_write(&file_test, test_buf, sizeof(test_buf));
            status = fx_file_write(&file_test, test_buf, sizeof(test_buf));
            status = fx_file_write(&file_test, test_buf, sizeof(test_buf));
            status = fx_file_write(&file_test, test_buf, sizeof(test_buf));
            status = fx_file_write(&file_test, test_buf, sizeof(test_buf));
            status = fx_file_write(&file_test, test_buf, sizeof(test_buf));
            status = fx_file_write(&file_test, test_buf, sizeof(test_buf));
            status = fx_file_write(&file_test, test_buf, sizeof(test_buf));
            status = fx_file_write(&file_test, test_buf, sizeof(test_buf));
            status = fx_file_write(&file_test, test_buf, sizeof(test_buf));
            status = fx_file_write(&file_test, test_buf, sizeof(test_buf));
            status = fx_file_write(&file_test, test_buf, sizeof(test_buf));
            status = fx_file_write(&file_test, test_buf, sizeof(test_buf));
            status = fx_file_write(&file_test, test_buf, sizeof(test_buf));
            status = fx_file_write(&file_test, test_buf, sizeof(test_buf));
            status = fx_file_write(&file_test, test_buf, sizeof(test_buf));
            time[2 * i + 1] = GetRunTime();

            /* 需要手动刷新吗? */
            status = fx_media_flush(FX_SD_MEDIA);
            if (status != FX_SUCCESS)
            {
                printf("Flush failed\n");
                FX_POST_ERROR(status);
                continue;
            }
            /* 打印本次结果 */
            /* 打印本次结果 */
            printf("[%d] %ld ms. Size %ld KB. Speed %d KB/S\n",
                   i,
                   time[2 * i + 1] - time[2 * i],
                   16 * sizeof(test_buf) / 1024,
                   16 * sizeof(test_buf) / 1024 * 1000 / (time[2 * i + 1] - time[2 * i]));
        }
    }

    uint32_t total_time = 0;
    uint32_t total_size = 0;
    uint32_t avg_speed = 0;

    /* 统计结果 */
    for (size_t i = 0; i < TEST_TIMES; i++)
    {
        total_time += (time[2 * i + 1] - time[2 * i]);
    }
    if (test_opt == 1)
    {
        total_size = 4 * sizeof(test_buf) * TEST_TIMES / 1024; // KB
        avg_speed = 4 * sizeof(test_buf) * TEST_TIMES / 1024 * 1000 / total_time;
    }
    else if (test_opt == 2)
    {
        total_size = TEST_BIG_SIZE / 1024;
        avg_speed = TEST_BIG_SIZE / 1024 * 1000 / total_time;
    }

    printf("Total time %ld ms. Total size %ld KB. Avg Speed %ld KB/s\n",
           total_time, total_size, avg_speed);

#endif /* SD_TEST */
    return status;
}
