package producer_consumer.method2;

import java.util.Random;

public class Producer extends Thread{
    private producer_consumer.method2.BufferResources bufferResources;
    Random random = new Random();

    // 构造器需要指定缓冲区
    public Producer(BufferResources bufferResources) {
        this.bufferResources = bufferResources;
    }

    @Override
    public void run() {
        // 生产一个随机数丢进缓冲区
        bufferResources.produce(random.nextInt(100));
    }
}
