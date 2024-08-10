package thread;

import java.util.concurrent.*;

public class MyExecutors implements Runnable{
    @Override
    public void run() {
        System.out.println("Running Thread: " + Thread.currentThread().getName());
    }
    public static void main(String[] args) {
        // 创建线程池，获取ExecutorService实例
        ExecutorService threadPool = Executors.newFixedThreadPool(3);
        // 向线程池提交任务
        for (int i=0; i<10; ++i) { // 整十个吧
            threadPool.execute(new MyExecutors());
        }
        // 等待线程都执行那个完毕后关闭线程池
        threadPool.shutdown();
    }
}
