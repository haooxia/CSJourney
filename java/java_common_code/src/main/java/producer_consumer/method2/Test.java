package producer_consumer.method2;

public class Test {
    public static void main(String[] args) {
        // note: 此时一共有21个线程同时存在；注意我虽然我们先创建并启动了10个Productor，
        //  但并不意味着这10个线程会立即执行，而是会进入就绪状态，等待CPU调度
        BufferResources bufferResources = new BufferResources();
        // 循环创建10个生产者线程 共享同一个缓冲区
        for (int i = 0; i < 10; i++) {
            new Producer(bufferResources).start();
        }
        for (int i = 0; i < 10; i++) {
            new Consumer(bufferResources).start();
        }
    }
}
