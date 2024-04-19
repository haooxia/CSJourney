#ifndef _THREADPOOL_H
#define _THREADPOOL_H

// 想要在头文件中使用源文件中的定义 需要声明
typedef struct ThreadPool ThreadPool;

// 创建线程池
ThreadPool* threadPoolCreate(int min, int max, int queueSize);

// 销毁线程池
void threadPoolDestory();

void* worker(void* arg, ThreadPool* pool);

// 给线程池添加任务

// 获取线程池中工作的线程的个数(一定活着)

// 获取线程池中活着的线程的个数(未必工作)

#endif // _THREADPOOL_H