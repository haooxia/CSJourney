package producer_consumer;

// note: 利用缓冲区解决生产者消费者问题：管程法
public class TestPC {
    public static void main(String[] args) {
        SynContainer container = new SynContainer();
        new Productor(container).start();
        new Consumer(container).start();
    }
}

class Productor extends Thread {
    SynContainer container;
    public Productor(SynContainer container) {
        this.container = container;
    }
    // 生产
    @Override
    public void run() {
        for (int i = 0; i < 100; i++) {
            System.out.println("生产了第" + i + "只鸡");
            container.push(new Chicken(i));
        }
    }
}

class Consumer extends  Thread {
    SynContainer container;
    public Consumer(SynContainer container) {
        this.container = container;
    }
    // 消费
    @Override
    public void run() {
        for (int i = 0; i < 100; i++) {
            System.out.println("消费了第" + container.pop().id + "只鸡");
        }
    }
}

// 产品
class Chicken {
    int id;

    public Chicken(int id) {
        this.id = id;
    }
}

class SynContainer {
    Chicken[] chickens = new Chicken[10]; // 容器大小
    int count = 0; // 计数器
    // 生产者放入产品
    public synchronized void push(Chicken chicken) {
        // 如果容器满了，就等待消费者消费
        // 如果没满，就生产产品
        if (count == chickens.length) { // note: 如果缓冲区满了  就等待消费者消费
            try {
                this.wait(); // 等待消费者消费
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        chickens[count] = chicken;
        count++;
    }

    public synchronized Chicken pop() {
        // 如果容器空了，就等待生产者生产
        // 如果没空，就消费产品
        if (count == 0) {
            try {
                this.wait(); // note: 等待生产者生产
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        count--;
        Chicken chicken = chickens[count];
        // 通知生产者生产
        return chicken;
    }
}