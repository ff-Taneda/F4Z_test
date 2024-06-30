#include "PrintArray.h"

/**
 * @brief 打印数组成表格，每隔16行打印一次元素序号
 * @param array 要打印的数组
 * @param num_per_row 每行打印元素个数
 * @param size 要打印的元素个数
 */
void PrintArray(uint8_t *array, uint8_t num_per_row, uint16_t size)
{
    // printf("\n----------print begin----------\n\n");
    uint16_t i = 0, j = 0;  /* 循环变量 */
    uint16_t row_count = 0; /* 行计数器，用于跟踪已打印的行数，从0开始 */

    for (i = 0; i < size; i++)
    {
        /* 每打印 num_per_row 个元素后 */
        if (i % num_per_row == 0)
        {
            /* 如果是每16行的开始，打印元素序号 */
            if ((row_count % 16) == 0)
            {
                printf("\n    |");
                for (j = 0; j < num_per_row; j++)
                {
                    printf("%02X ", j);
                }
                printf("\n");
            }
            /* 打印当前行的行号 */
            printf("%04X|", row_count);
            row_count++; /* 行数增加 */
        }

        /* 打印数组元素 */
        printf("%02X ", array[i]);

        /* 检查是否是当前行或数组的最后一个元素 */
        if ((i + 1) % num_per_row == 0 || i == size - 1)
        {
            /* 在每行末尾或数据结束时添加竖线 */
            printf("|");

            /* 如果不是数组的最后元素，则打印换行符 */
            if (i != size - 1)
            {
                printf("\n");
            }
        }
    }
    /* 打印结束 */
    // printf("\n----------print complete----------\n\n");
}
