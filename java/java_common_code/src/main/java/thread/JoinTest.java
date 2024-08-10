package thread;

// TODO: 新建T1、T2、T3三个线程，并保证它们按顺序执行
public class JoinTest {
    public static void main(String[] args) {
        Thread t1 = new Thread(() -> {
            System.out.println("Thread-1");
        });
        Thread t2 = new Thread(() -> {
            try {
                t1.join(); // 当一个线程调用另一个线程的join()方法时，调用线程将被挂起，直到目标线程结束
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
            System.out.println("Thread-2");
        });
        Thread t3 = new Thread(() -> {
            try {
                t2.join();
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
            System.out.println("Thread-3");
        });

        t1.start();
        t2.start();
        t3.start();
    }
}
