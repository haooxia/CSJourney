#ifndef _THREADPOOL_H
#define _THREADPOOL_H

// 想要在头文件中使用源文件中的定义 需要声明
typedef struct ThreadPool ThreadPool;

// 创建线程池并初始化
ThreadPool* threadPoolCreate(int min, int max, int queueSize);

// 销毁线程池
int threadPoolDestory(ThreadPool* pool);

// 给线程池添加任务
void threadPoolAdd(ThreadPool* pool, void(*func)(void*), void* arg);

// 获取线程池中工作的线程的个数(一定活着)
int threadPoolBusyNum(ThreadPool* pool);

// 获取线程池中活着的线程的个数(未必工作)
int threadPoolLiveNum(ThreadPool* pool);

void* worker(void* arg);

void* manager(void* arg);

void threadExit(ThreadPool* pool);

#endif // _THREADPOOL_H