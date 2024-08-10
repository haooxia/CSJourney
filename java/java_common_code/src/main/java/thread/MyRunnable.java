package thread;

public class MyRunnable implements Runnable {
    @Override
    public void run() {
        System.out.println("Running Thread: " + Thread.currentThread().getName());
    }

    public static void main(String[] args) {
        // 仅需创建一个MyRunnable对象
        MyRunnable mr = new MyRunnable();
        Thread t1 = new Thread(mr);
        Thread t2 = new Thread(mr);
        t1.start();
        t2.start();
    }
}
