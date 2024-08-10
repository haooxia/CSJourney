package thread;

//import java.util.concurrent.Callable;
//import java.util.concurrent.FutureTask;

import java.util.concurrent.*;

// Callable是一个泛型接口，指定call()返回类型为String
public class MyCallable implements Callable<String> {
    @Override
    public String call() throws Exception{
        System.out.println("Running Thread: " + Thread.currentThread().getName());
        return "OK";
    }
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        MyCallable mc = new MyCallable();
        // 将Callable对象封装在FutureTask中，可以在多线程环境下执行任务，并获取任务执行结果
        FutureTask<String> ft = new FutureTask<>(mc);
        Thread t1 = new Thread(ft);
//        Thread t2 = new Thread(ft);
        t1.start();
        String result = ft.get(); // 阻塞获取结果(即同步等待call()执行完毕
        System.out.println(result);
    }
}