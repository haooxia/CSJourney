# question

1.变量a的作用域明明在while块内，理论上每次定义会在栈空间上定义一个全新的变量a，可是为什么实验结果每次a的地址都一样，是有什么优化机制在里面吗？（malloc的结果也一样）

```c
#include <stdio.h>
#include <unistd.h>

int main() {
    int count = 0;

    while (count < 100) {
        int a = 10;
        printf("Loop %d: a = %d, &a = %p\n", count, a, &a);
        count++;
        sleep(1);
    }

    return 0;
}
```

2.变量a打印出来的地址是否是实际的物理地址？和虚拟地址有什么关系吗