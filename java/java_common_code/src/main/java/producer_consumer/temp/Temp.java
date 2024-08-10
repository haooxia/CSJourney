package producer_consumer.temp;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Temp {
    public static void main(String[] args) {
        // note: 创建单个线程; 任务都是由同一个线程来执行
        ExecutorService threadPoll1 = Executors.newSingleThreadExecutor();
        // note： 创建固定数量线程池
        ExecutorService threadPoll2 = Executors.newFixedThreadPool(10);
        // note: 创建缓存线程池，可伸缩，遇强则强
        ExecutorService threadPoll3 = Executors.newCachedThreadPool();

        for (int i = 0; i < 1000000; i++) {
            threadPoll3.execute(() -> {
                System.out.println(Thread.currentThread().getName() + " is running");
            }); // 函数式接口Runnable的对象
        }
        // 关闭线程池
        threadPoll3.shutdown();
    }
}
