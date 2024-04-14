# 网络编程基础API

<!-- 1.判断机器字节序

```c
#include <stdio.h>
void byteorder() {
    union {
        short value;
        char union_bytes[sizeof(short)]; // char array, 长度为2, 所以能存16bit
    } test; // union可以在同一块内存中存储不同类型的数据
    test.value = 0x0102;
    if ((test.union_bytes[0] == 1) && (test.union_bytes[1] == 2)) {
        printf("big endian\n");
    } else if ((test.union_bytes[0] == 2) && (test.union_bytes[1] == 1)) {
        printf("little endian\n");
    } else {
        printf("unknown...\n");
    }
}
``` -->

