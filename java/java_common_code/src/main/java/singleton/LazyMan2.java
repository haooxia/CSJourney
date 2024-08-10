package singleton;

// note: 懒汉式单例模式(多线程有效)
//public class LazyMan2 {
//    // 将构造器私有化 防止直接new
//    private LazyMan2(){};
//    // 创建一个私有的静态实例(不初始化)
//    private static LazyMan2 lazyMan;
//    // 提供一个公共的静态方法返回实例
//    public static synchronized LazyMan2 getInstance() { // note: 确保每次只有一个线程进入该方法（缺点是锁粒度有点大
//        if (lazyMan == null) {
//            lazyMan = new LazyMan2();
//        }
//        return lazyMan;
//    }
//}

import java.util.concurrent.atomic.AtomicInteger;

// note: 懒汉式单例模式(多线程有效)
// note: 双重检测锁模式的懒汉式单例 / Double-Checked Blocking (DCL懒汉式单例)
public class LazyMan2 {
    // 将构造器私有化 防止直接new
    private LazyMan2(){};
    // 创建一个私有的静态实例(不初始化)
    private static volatile LazyMan2 lazyMan; // note 请注意添加volatile关键字，禁止指令重排
    // 提供一个公共的静态方法返回实例
    public static LazyMan2 getInstance() {
        // 在第一次检查是否为null时不加锁，只有在需要创建实例时才加锁，减少开销
        if (lazyMan == null) {
            synchronized (LazyMan2.class) {
                if (lazyMan == null) {
                    lazyMan = new LazyMan2();
                    // note: 不是原子性操作，三条指令：为lazyMan分配内存->调用构造方法初始化对象->将LazyMan对象的引用赋值给lazyMan，可能会发生指令重排
                }
            }
        }
        return lazyMan;
    }
}