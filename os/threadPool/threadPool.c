#include "threadPool.h"
#include <pthread.h>

// 创建结构 -> 创建操作函数

// 任务结构体
typedef struct Task {
    void (*function) (void* arg); // void*类型的参数可以传递任意类型
    void* arg;
}Task;


// 线程池结构体
typedef struct ThreadPool {
    // 任务队列相关
    Task* taskQue;
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
        pthread_create(&pool->managerID, NULL, manager, NULL);
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