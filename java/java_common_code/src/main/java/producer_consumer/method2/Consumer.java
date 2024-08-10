package producer_consumer.method2;

public class Consumer extends Thread{
    private producer_consumer.method2.BufferResources bufferResources;

    public Consumer(BufferResources bufferResources) {
        this.bufferResources = bufferResources;
    }
    @Override
    public void run() {
        this.bufferResources.consume();
    }
}
