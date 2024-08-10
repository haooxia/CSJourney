package producer_consumer.method3;

import java.util.concurrent.BlockingDeque;
import java.util.concurrent.LinkedBlockingDeque;

public class BufferResources {
    private final int maxSize = 10;
    // note: 使用阻塞队列作为缓冲区
    private BlockingDeque buffer = new LinkedBlockingDeque(maxSize);
    public void consume() {
        try {
            // note 这里为何不用判空wait生产者生产 / 同步
            //  因为take()内部已实现：当队列为空时，消费者线程会被阻塞，直到队列非空 (ie BlockingDeque已实现
            Integer value = (Integer) buffer.take();
            System.out.println(Thread.currentThread().getName() + " 消费成功：" + value.toString() +
                    "当前缓冲区： " + buffer.toString());
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }

    public void produce(Integer value) {
        try {
            buffer.put(value);
            System.out.println(Thread.currentThread().getName() + " 生产成功：" + value.toString() +
                    " 当前缓冲区： " + buffer.toString());
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }
}
