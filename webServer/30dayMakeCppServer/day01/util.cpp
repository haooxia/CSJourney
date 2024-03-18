#include "util.h"
#include <stdio.h>
#include <stdlib.h>

void errif(bool condition, const char *errmsg) {
    if (condition) {
        perror(errmsg); // <stdio.h> 打印出errno的实际意义，前缀errmsg+默认信息
        exit(EXIT_FAILURE); // <stdlib.h>
    }
}
