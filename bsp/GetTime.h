/*
 * GetTime.h
 *  Created on: 2024 05 18, 15 28 52
 *      Author: phy1335
 *        Note: 将编译时间提取出来
 *  以下宏均为int类型 2024 4 20 17 37 50
 *
 */
#ifndef __GETTIME_H
#define __GETTIME_H

#ifdef __cplusplus
extern "C"{
#endif

/* 年: 2024 int */
#define __NOW_YEAR__ ((__DATE__[9] - '0') * 10 + (__DATE__[10] - '0') + 2000)
/* 月: 4 int*/
#define __NOW_MONTH__ (__DATE__[2] == 'n'   ? (__DATE__[1] == 'a' ? 1 : 6) \
                 : __DATE__[2] == 'b' ? 2                            \
                 : __DATE__[2] == 'r' ? (__DATE__[0] == 'M' ? 3 : 4) \
                 : __DATE__[2] == 'y' ? 5                            \
                 : __DATE__[2] == 'l' ? 7                            \
                 : __DATE__[2] == 'g' ? 8                            \
                 : __DATE__[2] == 'p' ? 9                            \
                 : __DATE__[2] == 't' ? 10                           \
                 : __DATE__[2] == 'v' ? 11                           \
                                      : 12)
/* 日: 20 int */
#define __NOW_DAY__ ((__DATE__[4] == ' ' ? 0 : __DATE__[4] - '0') * 10 + (__DATE__[5] - '0'))

/* 时: 17 int */
#define __NOW_HOUR__ (__TIME__[0] - '0') * 10 + (__TIME__[1] - '0')
/* 分: 34 int */
#define __NOW_MINUTE__ (__TIME__[3] - '0') * 10 + (__TIME__[4] - '0')
/* 秒: 50 int */
#define __NOW_SECOND__ (__TIME__[6] - '0') * 10 + (__TIME__[7] - '0')

#ifdef __cplusplus
}
#endif
#endif /* __GETTIME_H */
