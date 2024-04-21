#include <stdio.h>
#include <pthread.h>
#include <unistd.h> // sleep
#include <stdlib.h> // malloc
#include "threadPool.h"

void taskFunc(void* arg) {
    int num = *(int*)arg; // type conversion -> dereference
    printf("thread %ld is working, number=%d\n", pthread_self(), num);
    sleep(1); // 任务就是print+睡一会儿
}

int main() {
    // 创建线程池
    ThreadPool* pool = threadPoolCreate(3, 10, 100);
    // 添加100个任务
    for (int i=0; i<100; ++i) {
        int* num = (int*)malloc(sizeof(int));
        *num = i + 100;
        threadPoolAdd(pool, taskFunc, num);
    }
    // 在关闭线程池(所有子线程)之前 为了保证所有子线程执行完毕 让*主*线程睡一段时间
    sleep(10);
    threadPoolDestory(pool);
    return 0;
}