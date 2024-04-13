# some system call

```cpp
#include <unistd.h>
pid_t fork(void);
```

fork(): 创建子进程，该子进程时调用进程（父进程）的副本。
调用fork()后，调用fork（）函数后，系统先给新的进程分配资源，例如存储数据和代码的空间。然后把原来的进程的所有值都复制到新的新进程中，只有少数值与原来的进程的值不同。相当于克隆了一个自己。

fork调用后分成完全不同的进程，所以父子进程**拥有完全独立的内存结构**，**只是二者共享一份代码而已**

通常情况下，fork()调用成功后，会在父进程和子进程中分别返回不同的值，以便区分哪个是父进程，哪个是子进程。具体而言，fork()在父进程中返回子进程的PID，而在子进程中返回0。如果fork()调用失败，则会返回一个负值。

```cpp
#include <stdio.h>                                                                                                     #include <unistd.h>                                                                                                   
int main() {
    int a = 10;
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        a += 8;
        printf("in child process, PID=%d, a=%d\n", getpid(), a);
    } else {
        a -= 8;
        printf("in parent process, PID=%d, a=%d\n", getpid(), a);
        printf("child's PID is %d\n", pid);
    }
    return 0;
} 

"""return
in parent process, PID=1236420, a=2
child's PID is 1236421
in child process, PID=1236421, a=18 // 两个空间内一样名称的变量没有联系
"""
```

首先，介绍一下**僵尸进程**产生的原因。
当一个进程创建子进程后,子进程在退出时并不会立即被销毁,而是会留下一个称为"僵尸进程"的数据结构。这是因为子进程退出时,内核会保留一些信息,如退出状态、运行时间等,等待父进程调用**wait()**或**waitpid()**来获取这些信息。
如果父进程没有及时调用wait()或waitpid()来处理子进程的退出状态,那么子进程就会变成僵尸进程,一直占用系统资源,却不执行任何操作,直到父进程退出或被杀死。
另一种情况是,如果父进程先于子进程退出,那么子进程就会变成**孤儿进程**,被init进程(进程ID为1)收养。init进程会自动调用wait()来处理这些孤儿进程,所以孤儿进程不会成为僵尸进程。

> 通过`ps au`查看进程，当状态STAT是Z时候就是僵尸进程。

wait(): 父进程主动请求获取子进程的返回值（main函数return返回值、exit的参数），来销毁子进程。

```cpp
#include <sys/wait.h>
// int status;
pid_t wait(int* statloc);
// pid_t wait(&status);
```

如果参数status的值不是NULL，wait就会把子进程退出时的状态取出并存入其中，这是int，指出了子进程是正常退出还是被非正常结束的，以及正常结束时的返回值，或被哪一个信号结束的等信息。由于这些信息被存放在一个整数的不同二进制位中，所以用常规的方法读取会非常麻烦，人们就设计了一套专门的宏（macro）来完成这项工作，下面我们来学习一下其中最常用的两个：

1.**WIFEXITED**(status): 这个宏用来指出子进程是否为正常退出的，如果是，它会返回一个非零值。
2.**WEXITSTATUS**(status): 当WIFEXITED返回非零值时，我们可以用这个宏来提取子进程的返回值，如果子进程调用exit(5)退出，WEXITSTATUS(status)就会返回5；如果子进程调用exit(7)，WEXITSTATUS(status)就会返回7。请注意，如果进程不是正常退出的，也就是说，WIFEXITED返回0，这个值就毫无意义。

如果没有已终止的子进程，程序将**阻塞**直到有子进程终止，所以谨慎使用。

```cpp
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int status;
    pid_t pid = fork();
    if (pid == 0) {
        printf("in child process, PID=%d\n", getpid());
        return 3;
    } else {
        printf("in parent process, PID=%d\n", getpid());
        printf("child's PID is %d\n", pid);
        wait(&status);
        if (WIFEXITED(status))
            printf("child send one: %d \n", WEXITSTATUS(status));
        sleep(30);
    }
    return 0;
}
```

waitpid()

```cpp
#include <sys/wait.h>
pid_t waitpid(pid_t pid, int* statloc, int options);
// pid: 等待终止的目标子进程的id，若-1，等同于wait，可以等候任意子进程终止。
// options: 传递常量WNOHWANG，没有终止的子进程也不会阻塞。
```