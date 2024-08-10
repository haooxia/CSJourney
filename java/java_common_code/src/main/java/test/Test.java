package test;

import java.util.Collections;

public class Test {
    private static final ThreadLocal<Integer> threadLocalValue = ThreadLocal.withInitial(() -> 0);
    public static void main(String[] args) {
        // 创建并启动多个线程
        for (int i = 0; i < 3; i++) {
            Thread thread = new Thread(new Task(), "Thread-" + i);
            thread.start();
            System.out.println("main...");
        }
    }
    static class Task implements Runnable {
        @Override
        public void run() {
            // 获取当前线程的名字
            String threadName = Thread.currentThread().getName();

            // 设置线程本地变量
            threadLocalValue.set((int) (Math.random() * 100));

            // 打印线程本地变量的值
            System.out.println(threadName + " initial value: " + threadLocalValue.get());

            // 模拟一些工作
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }

            // 修改线程本地变量的值
            threadLocalValue.set(threadLocalValue.get() + 1);

            // 打印线程本地变量的值
            System.out.println(threadName + " updated value: " + threadLocalValue.get());
        }
    }
}

