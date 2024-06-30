/*
 * sh_fx_test.c
 *
 *  Created on: 2024/06/29, 11:37:36
 *      Author: phy1335
 *        Note:
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "fxsh_api.h"
#include "sd_api.h"
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
fxshCmdInfo_t sh_info_fx_test = {
    .usage = "sd_test opt1 opt2",
    .description = "Test SD card read or write speed.",
    .options = "opt1 - 1: write test. 2: read test\n\
\topt2 - test size 1: 64 times 256KB. 2:  is 1 times 64MB ",
    .example = ">sd_test 1 1\n",
    .example_description = "Perform 64 write tests with a size of 256KB."};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void sh_fx_test(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Usage: sd_test opt1 opt2\n");
    }
    else
    {
        if (strcmp(argv[1], "1") == 0)
        {
            if (strcmp(argv[2], "1") != 0 || strcmp(argv[2], "2") != 0)
            {
                printf("opt2 must be 1 or 2\n");
            }
            sd_test_write(atoi(argv[2]));
        }
    }
}
