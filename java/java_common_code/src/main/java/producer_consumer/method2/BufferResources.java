package producer_consumer.method2;

import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class BufferResources {
    private final int maxSize = 10;
    private List<Integer> list = new LinkedList<>();
    // 创建锁
    private Lock lock = new ReentrantLock();
    // note 生产者对应Condition 当缓冲区满 生产者会在这个条件上等待 （设置condition可实现更细粒度的控制
    private Condition producerCondition = lock.newCondition(); // empty
    // note 消费者对应Condition 当缓冲区空 消费者会在这个条件上等待
    private Condition consumerCondition = lock.newCondition(); // full

    public void consume() {
        lock.lock();
        try {
            while (list.isEmpty()) {
                System.out.println(Thread.currentThread().getName() + " 当前缓冲区为空，等待生产者生产");
                //wait();
                // 消费者进入等待状态
                consumerCondition.await();
            }
            // 消费数据
            Integer value = list.remove(0);
            System.out.println(Thread.currentThread().getName() + " 消费成功：" + value.toString() +
                    "当前缓冲区： " + list.toString());
            // note: 消费完毕 仅唤醒生产者
            producerCondition.signalAll(); // full.signalAll(); // 给full加一个
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        } finally {
            lock.unlock(); // note: 切记使用finally释放锁
        }
    }

    public void produce(Integer value) {
        lock.lock();
        try {
            while (list.size() == maxSize) {
                System.out.println(Thread.currentThread().getName() + " 当前缓冲区已满，等待消费者消费");
                producerCondition.await();
            }
            // 生产数据
            list.add(value);
            System.out.println(Thread.currentThread().getName() + " 生产成功：" + value.toString() +
                    " 当前缓冲区： " + list.toString());
            // note: 生产完毕 仅唤醒消费者
            consumerCondition.signalAll();
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        } finally {
            lock.unlock();
        }
    }

}
