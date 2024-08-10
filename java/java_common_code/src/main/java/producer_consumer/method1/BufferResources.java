package producer_consumer.method1;

import java.util.LinkedList;
import java.util.List;

public class BufferResources {
    private final int maxSize = 10;
    private List<Integer> list = new LinkedList<Integer>();
    public synchronized void comsume() {
        while (list.isEmpty()) { // note: 这里一定是while而非if：避免虚假唤醒问题
            // note: 虚假唤醒: 可能并不是因为条件变量的状态改变导致的唤醒；如果使用if检查，现成被唤醒后不会重新检查条件，可能在条件为满足的情况下执行
            System.out.println(Thread.currentThread().getName() + " 当前缓冲区为空，等待生产者生产");
            try {
                wait();
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
        }
        // 消费数据
        //Integer value = (Integer) list.removeFirst();
        Integer value = (Integer) list.remove(0);
        System.out.println(Thread.currentThread().getName() + " 消费成功：" + value.toString() +
                "当前缓冲区： " + list.toString());
        // note: 唤醒所有处于wait状态的线程（包括生产者和*消费者*） 消费者也可以来抢锁消费了
        // note: 如果我只想要唤醒生产者，不像再唤醒消费者 -> lock + condition实现
        notifyAll();
    }

    public synchronized void produce(Integer value) {
        while (list.size() == maxSize) {
            System.out.println(Thread.currentThread().getName() + " 当前缓冲区已满，等待消费者消费");
            try {
                wait();
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
        }
        // 生产数据
        list.add(value);
        System.out.println(Thread.currentThread().getName() + " 生产成功：" + value.toString() +
                " 当前缓冲区： " + list.toString());
        // note: 唤醒所有处于wait状态的线程（包括*生产者*和消费者）
        notifyAll();
    }
}
