package thread;

import java.util.concurrent.*;

public class MyThreadPoolExecutor{
    // 创建了一个线程池，最大线程数为5，核心线程数为2，空闲线程存活时间为10秒
    // 任务队列容量为3。然后向线程池提交了5个任务，每个任务打印当前线程的名称和"ok"。最后关闭线程池。
    public static void main(String[] args) {
        ThreadPoolExecutor threadPool = new ThreadPoolExecutor(
                2,                           // 核心线程数 corePoolSize
                5,                                      // 最大线程数 maximumPoolSize
                10,                                      // 空闲线程存活时间 keepAliveTime
                TimeUnit.SECONDS,                       // 时间单位 unit
                new LinkedBlockingQueue<>(3),   // 阻塞队列的容量 workQueue
                Executors.defaultThreadFactory(),       // 默认线程工厂 (可忽略) threadFactory
                new ThreadPoolExecutor.AbortPolicy()    // 默认拒绝策略 (可忽略) handler
                );
        for (int i=0; i<8; ++i) {
            threadPool.execute(() -> {
                // 这里采用lambda表达式来传入一个Runnable实现类的对象
                System.out.println("Running Thread: " + Thread.currentThread().getName());
            });
        }
        threadPool.shutdown();
    }
}
