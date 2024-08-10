package producer_consumer.method3;

public class Consumer extends Thread{
    private BufferResources bufferResources;

    public Consumer(BufferResources bufferResources) {
        this.bufferResources = bufferResources;
    }
    @Override
    public void run() {
        this.bufferResources.consume();
    }
}
