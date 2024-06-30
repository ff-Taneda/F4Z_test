/*
 * CalcluateString.c
 *
 *  Created on: 2024-06-15 T18:57:15+08:00
 *      Author: phy1335
 *        Note:   
 * 
 */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"
#include "stdlib.h"
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
uint64_t Calculate_string(const char *str)
{
    uint64_t result = 0;
    char *endptr;
    double value = strtod(str, &endptr);

    if (endptr == str)
    {
        // No digits were found
        return 0;
    }

    // Start with the first number
    result = (uint64_t)value;

    while (*endptr != '\0')
    {
        char op = *endptr;
        double next_value = strtod(endptr + 1, &endptr);

        if (endptr == (str + 1))
        {
            // No digits were found after the operator
            return 0;
        }

        switch (op)
        {
        case '+':
            result += (uint64_t)next_value;
            break;
        case '-':
            result -= (uint64_t)next_value;
            break;
        case '*':
            result *= (uint64_t)next_value;
            break;
        case '/':
            if (next_value == 0)
            {
                printf("Error: Division by zero.\n");
                return 0;
            }
            result /= (uint64_t)next_value;
            break;
        default:
            printf("Error: Unknown operator '%c'.\n", op);
            return 0;
        }
    }

    return result;
}
