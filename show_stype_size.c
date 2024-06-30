// #include "main.h"

// #include "fx_api.h"

// void show_type_size(void)
// {
//     size_t size[32] = {0};
//     char *to_show[] = {
//         "size[0] = sizeof(int8_t)",
//         "size[1] = sizeof(int16_t)",
//         "size[2] = sizeof(int32_t)",
//         "size[3] = sizeof(int64_t)",
//         "size[4] = sizeof(intmax_t)",
//         "size[5] = sizeof(uint8_t)",
//         "size[6] = sizeof(uint16_t)",
//         "size[7] = sizeof(uint32_t)",
//         "size[8] = sizeof(uint64_t)",
//         "size[9] = sizeof(uintmax_t)",
//         "size[10] = sizeof(char)",
//         "size[11] = sizeof(short)",
//         "size[12] = sizeof(long)",
//         "size[13] = sizeof(int)",
//         "size[14] = sizeof(size_t)",
//         "size[15] = sizeof(long long)",
//         "size[16] = sizeof(unsigned char)",
//         "size[17] = sizeof(unsigned short)",
//         "size[18] = sizeof(unsigned long)",
//         "size[19] = sizeof(unsigned int)",
//         "size[20] = sizeof(unsigned long long)",
//         "size[21] = sizeof(CHAR)",
//         "size[22] = sizeof(BOOL)",
//         "size[23] = sizeof(SHORT)",
//         "size[24] = sizeof(INT)",
//         "size[25] = sizeof(LONG)",
//         "size[26] = sizeof(UCHAR)",
//         "size[27] = sizeof(USHORT)",
//         "size[28] = sizeof(UINT)",
//         "size[29] = sizeof(ULONG)",
//         "size[30] = sizeof(ULONG64)",
//         "size[30] = sizeof(ULONG64)"};

//     size[0] = sizeof(int8_t);
//     size[1] = sizeof(int16_t);
//     size[2] = sizeof(int32_t);
//     size[3] = sizeof(int64_t);
//     size[4] = sizeof(intmax_t);
//     size[5] = sizeof(uint8_t);
//     size[6] = sizeof(uint16_t);
//     size[7] = sizeof(uint32_t);
//     size[8] = sizeof(uint64_t);
//     size[9] = sizeof(uintmax_t);

//     size[10] = sizeof(char);
//     size[11] = sizeof(short);
//     size[12] = sizeof(long);
//     size[13] = sizeof(int);
//     size[14] = sizeof(size_t);
//     size[15] = sizeof(long long);
//     size[16] = sizeof(unsigned char);
//     size[17] = sizeof(unsigned short);
//     size[18] = sizeof(unsigned long);
//     size[19] = sizeof(unsigned int);
//     size[20] = sizeof(unsigned long long);

//     size[21] = sizeof(CHAR);
//     size[22] = sizeof(BOOL);
//     size[23] = sizeof(SHORT);
//     size[24] = sizeof(INT);
//     size[25] = sizeof(LONG);
//     size[26] = sizeof(UCHAR);
//     size[27] = sizeof(USHORT);
//     size[28] = sizeof(UINT);
//     size[29] = sizeof(ULONG);
//     size[30] = sizeof(ULONG64);
//     size[31] = sizeof(ULONG64);

//     for (size_t i = 0; i < 32; i++)
//     {
//         printf("%s = %d\n", to_show[i], size[i]);
//     }
// }

/* size[0] = sizeof(int8_t) = 1
size[1] = sizeof(int16_t) = 2
size[2] = sizeof(int32_t) = 4
size[3] = sizeof(int64_t) = 8
size[4] = sizeof(intmax_t) = 8
size[5] = sizeof(uint8_t) = 1
size[6] = sizeof(uint16_t) = 2
size[7] = sizeof(uint32_t) = 4
size[8] = sizeof(uint64_t) = 8
size[9] = sizeof(uintmax_t) = 8
size[10] = sizeof(char) = 1
size[11] = sizeof(short) = 2
size[12] = sizeof(long) = 4
size[13] = sizeof(int) = 4
size[14] = sizeof(size_t) = 4
size[15] = sizeof(long long) = 8
size[16] = sizeof(unsigned char) = 1
size[17] = sizeof(unsigned short) = 2
size[18] = sizeof(unsigned long) = 4
size[19] = sizeof(unsigned int) = 4
size[20] = sizeof(unsigned long long) = 8
size[21] = sizeof(CHAR) = 1
size[22] = sizeof(BOOL) = 1
size[23] = sizeof(SHORT) = 2
size[24] = sizeof(INT) = 4
size[25] = sizeof(LONG) = 4
size[26] = sizeof(UCHAR) = 1
size[27] = sizeof(USHORT) = 2
size[28] = sizeof(UINT) = 4
size[29] = sizeof(ULONG) = 4
size[30] = sizeof(ULONG64) = 8 */
