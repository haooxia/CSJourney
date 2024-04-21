#include <stdio.h>
#include <stdlib.h> // malloc
#include <string.h> // memset
#include <unistd.h> // sleep
#include "threadPool.h"
#include <pthread.h>


const int NUMBER = 2; // 定义一次添加两个线程

// 创建结构 -> 创建操作函数

// 任务结构体
typedef struct Task {
    void (*function) (void* arg); // void*类型的参数可以传递任意类型
    void* arg;
}Task;


// 线程池结构体
typedef struct ThreadPool {
    // 任务队列相关
    Task* taskQue;      // 任务数组
    int queueCapacity;  // 容量
    int queueSize;      // 当前任务数量
    int queueFront;     // 队头：取数据
    int queueRear;      // 队尾：放数据

    // 线程池相关
    pthread_t managerID;    // 管理者线程ID
    pthread_t *threadIDs;   // 工作线程ID数组
    int minNum;             // 最小工作线程个数
    int maxNum;
    int busyNum;            // 忙线程个数 变化频率很高 需要加锁
    int liveNum;            // 存活线程个数 变化频率不高
    int exitNum;            // 要杀死的线程个数
    pthread_mutex_t mutexPool;  // 锁**整个线程池** 实现互斥访问线程池
    pthread_mutex_t mutexBusy;  // 单独给busyNum加个锁而非锁住整个线程池 增加效率
    pthread_cond_t notFull;     // 任务队列是否满    
    pthread_cond_t notEmpty;    // 任务队列是否空

    // 辅助成员
    int shutdown;           // 是否要销毁线程池 销毁为1 否则为0
}ThreadPool;

ThreadPool* threadPoolCreate(int min, int max, int queueSize) {
    // 创建线程池 = 实例化线程池对象 + 初始化线程池对象所有元素
    // 有的元素还需要malloc然后再进行初始化

    // 由于我们需要通过地址传递这个pool实例，因此我们要保证这块地址不被释放，所以我们需要在堆上申请内存空间，而非栈(函数结束就无了)
    ThreadPool* pool = (ThreadPool*)malloc(sizeof(ThreadPool));
    do {
        if (!pool) {
            printf("malloc threadpool fail...\n");
            break; // return NULL;
        }

        // 先创建再初始化(未定义->置0)（比new要慢得多咯）
        pool->threadIDs = (pthread_t*)malloc(max * sizeof(pthread_t));
        if (!pool->threadIDs) {
            printf("malloc threadIDs fails...\n");
            // free(pool);
            break;
        }
        memset(pool->threadIDs, 0, sizeof(pthread_t) * max); // pthread_t是unsigned long，0表示未占用，其他值表示被占用

        pool->minNum = min;
        pool->maxNum = max;
        pool->busyNum = 0;
        pool->liveNum = min; // 初始化工作线程数时按照min创建
        pool->exitNum = 0;
        pool->shutdown = 0;

        if (pthread_mutex_init(&pool->mutexPool, NULL) != 0 ||
            pthread_mutex_init(&pool->mutexBusy, NULL) != 0 ||
            pthread_cond_init(&pool->notFull, NULL) != 0 ||
            pthread_cond_init(&pool->notEmpty, NULL) != 0) {
                printf("mutex or cond fails...\n");
                // free(pool);
                // free(pool->threadIDs);
                break;
            }

        // 任务队列
        pool->queueCapacity = queueSize;
        pool->taskQue = (Task*)malloc(sizeof(Task) * queueSize);
        if (!pool->taskQue) {
            printf("malloc taskQue falis...\n");
            break;
        }
        pool->queueSize = 0;
        pool->queueFront = 0;
        pool->queueRear = 0;

        // 创建管理者线程和工作线程
        pthread_create(&pool->managerID, NULL, manager, pool);
        for (int i=0; i < min; ++i) {
            // worker工作时是否需要实参呢？worker会去taskQ中取出一个任务，然后执行任务内的函数，所以需要访问该任务
            pthread_create(&pool->threadIDs[i], NULL, worker, pool);
        }

        return pool;
    } while(0); // 这儿是0哦 为了里面可以使用break
    
    /*
    注意如果后面的if寄了，你当时就需要free前面申请成功的堆内存
    但这样有点麻烦，所以整个do while，同时把return NULL都改成break
    后面统一进行释放
    */
    // 如果走到这儿了 就说明前面有地方break了 就需要释放资源了
    // pool, pool->threadIDs, pool->taskQue
    if (pool && pool->threadIDs) free(pool->threadIDs);
    if (pool && pool->taskQue) free(pool->taskQue);
    if (pool) free(pool);

    return NULL;
}

void* worker(void* arg) {
    /* 不断从任务队列取出并处理任务
    */
    ThreadPool* pool = (ThreadPool*)arg;
    while (1) {
        // 任务队列在线程池内 是共享资源 锁住
        pthread_mutex_lock(&pool->mutexPool);
        // 循环判断当前任务队列是否为空
        while (pool->queueSize == 0 && !pool->shutdown) {
            pthread_cond_wait(&pool->notEmpty, &pool->mutexPool); // 消费者需要阻塞等待生产者生产出任务
            // 此时可能被manager唤醒 喊去自杀
            // 判断是否要销毁线程
            if (pool->exitNum > 0) {
                pool->exitNum--;
                if (pool->liveNum > pool->minNum) {
                    pool->liveNum--;
                    pthread_mutex_unlock(&pool->mutexPool); // 退出之前解开锁(wait加的锁)，防止死锁
                    // pthread_exit(NULL); // 妙啊
                    threadExit(pool); // 里面多加了一步tid置零（因为创建线程据此判断）
                }
            }
        }
        // 判断线程池是否被关闭了 关闭了就不能干活了
        if (pool->shutdown) {
            pthread_mutex_unlock(&pool->mutexPool); // 避免死锁
            threadExit(pool);
            // pthread_exit(NULL);
        }
        // 从任务队列取出一个任务
        Task task;
        task.function = pool->taskQue[pool->queueFront].function; // taskQue
        task.arg = pool->taskQue[pool->queueFront].arg;
        // 为什么此处不free呢？因为我们想循环使用 把这个数组维护成环形队列
        // 移动头节点(循环队列)
        pool->queueFront = (pool->queueFront + 1) % pool->queueCapacity;
        pool->queueSize--;
        pthread_cond_signal(&pool->notFull); // 消费完 激活一下(取完既可以唤醒生产者了 不用等到执行完毕)
        pthread_mutex_unlock(&pool->mutexPool); // 线程池使用结束
        
        // 执行该任务
        printf("thread %ld start working...\n", pthread_self());
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum++; // 工作线程增加，
        pthread_mutex_unlock(&pool->mutexBusy);
        task.function(task.arg);
        free(task.arg); // 释放掉这一块堆内存
        task.arg = NULL;

        // 任务执行完毕
        printf("thread %ld end working...\n", pthread_self());
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum--;
        pthread_mutex_unlock(&pool->mutexBusy);
    }
}

void* manager(void* arg) {
    /* 一直监测任务队列中任务个数及处理存活线程个数
    任务过多->创建新线程；任务过少->销毁一些线程 
    */
    ThreadPool* pool = (ThreadPool*)arg;
    while (!pool->shutdown) {
        // 不同于工作线程要不断检测任务队列中有无任务，管理者是周期性检测（每隔3s吧）
        sleep(3);
        // 取出线程池中任务个数和存活线程的数量（共享资源）
        pthread_mutex_lock(&pool->mutexPool);
        int queueSize = pool->queueSize;
        int liveNum = pool->liveNum;
        pthread_mutex_unlock(&pool->mutexPool);
        // 取出忙线程数量
        pthread_mutex_lock(&pool->mutexBusy);
        int busyNum = pool->busyNum; // 之所以单独上锁 是因为它被访问的频率比较高 只锁一个变量比较高效
        pthread_mutex_unlock(&pool->mutexBusy);

        // 添加线程
        // 自定义规则(干不过来了)：规定当前任务个数 > 存活线程个数 && 存活线程个数 < 最大工作线程数 -> 一次添加两个
        if (queueSize > liveNum && liveNum < pool->maxNum) {
            // 还得找一下threadIDs中哪个位置可以存储线程id 可能比较散哦 suoyixuyaobianli （有点东西）
            int counter = 0;
            pthread_mutex_lock(&pool->mutexPool); // 这个锁主要针对pool->liveNum
            for (int i = 0; i < pool->maxNum && counter < NUMBER 
                && pool->liveNum < pool->maxNum; ++i) {
                if (pool->threadIDs[i] == 0) { // 没被用过
                    pthread_create(&pool->threadIDs[i], NULL, worker, pool);
                    counter++;
                    pool->liveNum++;
                }
            }
            pthread_mutex_unlock(&pool->mutexPool);
        }

        // 销毁线程
        // 规则：忙线程*2 < 线程存活数 && 存活线程 > 最小线程数
        if (busyNum * 2 < liveNum && liveNum > pool->minNum) {
            int counter = 0;
            pthread_mutex_lock(&pool->mutexPool);
            pool->exitNum = NUMBER;
            pthread_mutex_unlock(&pool->mutexPool);
            for (int i = 0; i > pool->minNum && counter < NUMBER 
                && pool->liveNum > pool->minNum; ++i) {
                // 重点：作为管理者能销毁掉工作线程吗？并不能。所以我们要让工作线程自杀（类似于老板逼人走）。
                for (int i=0; i<NUMBER; ++i)
                    // 唤醒阻塞在wait(notEmpty)的worker
                    pthread_cond_signal(&pool->notEmpty);
            }
        }
    }
}

void threadExit(ThreadPool* pool) {
    pthread_t tid = pthread_self(); // 妙的
    for (int i=0; i<pool->maxNum; ++i) {
        if (pool->threadIDs[i] == tid) {
            pool->threadIDs[i] = 0;
            printf("threadExit() called, %ld exiting...\n", tid);
            break;
        }
    }
    pthread_exit(NULL);
}

// 生产者：将任务放到任务队列
void threadPoolAdd(ThreadPool* pool, void(*func)(void*), void* arg) { // 似乎传Task*也没毛病, 似乎需要多创建一个对象 不过确实应该没毛病
    pthread_mutex_lock(&pool->mutexPool);
    while (pool->queueSize == pool->queueCapacity && !pool->shutdown) { // 阻塞条件
        pthread_cond_wait(&pool->notFull, &pool->mutexPool);
    }
    if (pool->shutdown) {
        pthread_mutex_unlock(&pool->mutexPool);
        return;
    }

    // 将任务添加到任务队列队尾
    pool->taskQue[pool->queueRear].function = func;
    pool->taskQue[pool->queueRear].arg = arg;
    pool->queueRear = (pool->queueRear + 1) % pool->queueCapacity;
    pool->queueSize++;
    pthread_cond_signal(&pool->notEmpty); // 生产完 激活一下
    pthread_mutex_unlock(&pool->mutexPool);
}

int threadPoolBusyNum(ThreadPool* pool) {
    pthread_mutex_lock(&pool->mutexBusy);
    int busyNum = pool->busyNum;
    pthread_mutex_unlock(&pool->mutexBusy);
    return busyNum;
}

int threadPoolLiveNum(ThreadPool* pool) {
    pthread_mutex_lock(&pool->mutexPool);
    int liveNum = pool->liveNum;
    pthread_mutex_unlock(&pool->mutexPool);
    return liveNum;
}

int threadPoolDestory(ThreadPool* pool) {
    if (!pool) return -1;
    // 关闭线程池
    pool->shutdown = 1;
    // 此处阻塞回收管理者线程(manager在shutdown=1时退出)：join和exit的区别 TODO
    pthread_join(pool->managerID, NULL);
    // 唤醒阻塞的消费者线程（有多少活着的就唤醒多少次）
    for (int i=0; i<pool->liveNum; ++i)
        pthread_cond_signal(&pool->notEmpty);
    // 此时已经销毁了所有的manager和worker
    // 释放pool中申请的堆内存
    if (pool->taskQue) free(pool->taskQue);
    if (pool->threadIDs) free(pool->threadIDs);
    // 销毁互斥锁和条件变量
    pthread_mutex_destroy(&pool->mutexBusy);
    pthread_mutex_destroy(&pool->mutexPool);
    pthread_cond_destroy(&pool->notEmpty);
    pthread_cond_destroy(&pool->notFull);
    free(pool);
    pool = NULL; // 防止悬空指针：已经释放了内存(告诉os内存块可以从新被分配) 但变量的值仍然是那一块地址 再次访问就有问题了
    return 0;
}