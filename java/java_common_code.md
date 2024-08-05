# some common/important code in Java

[toc]

## 创建线程

* **继承Thread类、实现Runnable接口、实现Callable接口、使用线程池、使用CompletableFuture等**都可以创建线程

### 继承Thread类

最直接的一种方式，用户自定义类继承java.lang.Thread类，**重写run**()方法，run()方法中定义了线程执行的具体任务。创建该类的实例后，通过**调用start**()方法启动线程。

缺点:因为线程类已经继承了Thread类，所以不能再继承其他的父类

```java
public class MyThread extends Thread {
    @Override
    public void run() {
        // 线程执行的逻辑
        // sout(...)
    }
    public static void main(String[] args) {
        new MyThread().start();
    }
}
```

### 实现Runnable接口

实现Runnable接口需要**重写run**()方法，然后**将此Runnable对象作为参数传递给Thread类的构造器**，**创建Thread对象**后调用其start()方法启动线程。

```java
public class MyRunnable implements Runnable {
    @Override
    public void run() {
        // ...
    }
    public static void main(String[] args) {
        Thread t = new Thread(new MyRunnable()); // 将Runnable接口实现类的对象 塞给Thread
        t.start();
    }
}
```

* **implements Runnable接口，重写run方法（底层是静态代理模式）**
  * 创建一个Thread对象，并将Runnable接口实现类对象传递给它，然后调用start方法。
    * 可以把同一个Runnable对象（即用户自定义的被代理类：只用关注业务逻辑的实现）丢给多个线程使用（即Thread代理类：封装了创建和管理逻辑）（一份资源，多个代理） ==太妙了==
      * ![picture 0](../images/47183e58ad94304f65901c4ffa16a944390e2761fce0cc176a729a43c566cb84.png)  
    * ![picture 1](../images/201740b20c191d51ffd42bc1a2972512ad5a64743acd687c47daf28f79f1b94d.png)  
      * 详见onenote: Static Proxy

## 生产者消费者问题

### 实现

> * bg: 经典的多线程并发协作模式，一个生产者线程用于生产数据，一个消费者线程用于消费数据，**为了解耦二者的关系**，通常采用一个共享的数据区域；ie 生产者生产数据后直接放到共享数据区，无需关心消费者的行为；消费者只需从共享数据区拿数据，无需关注生产者行为。（好一个解耦
> * 其中同步互斥关系：生产者之间互斥，消费者之间也互斥；生产者和消费者之间**互斥且同步**

**逻辑等待->业务->通知**

* 基于`synchronized + wait() + notify()`实现
  * wait()和notify()调用之前，**要求线程必须获得该对象的监视器锁，即只能在同步方法或方法块中调用二者**（wait执行完毕 当前线程会释放锁
  * notify()会从WAITTING状态的线程中挑选一个进行通知，使其从等待队列切换到同步队列，**然后还要等待获取对象的监视器锁**之后才可以执行；notifyall()会唤醒所有等待线程（包括生产者类和消费者类）
  * 缺陷：==无法实现精确唤醒，只能同时唤醒两类==
* 基于`Lock + Condition (await() & signal())`实现
  * **可以精确地唤醒一类线程**（可实现仅唤醒消费者类线程或生产者类线程）
    * 就是依赖多个condition
* 基于阻塞队列`BlockingQueue`实现
  * 无脑调用take()和put()即可，底层已经实现了互斥同步机制，它的底层思路就是`Lock + Condition`实现的，和我写的差不多...

### 应用场景

![picture 0](../images/f2c42349e2c03fcadf313cf93cad388b6a635df7cdfd30a2753401b6a197043b.png)  
![picture 1](../images/73d2f1223cfe1efe57084219ed1cf3b5b8b4abea3df2d0af1f8db4c8d85317da.png)  

## 单例模式

### 饿汉式

```java
// note: 饿汉式单例模式
public class Hungry {
    // 1.私有化构造器 防止直接new
    private Hungry(){};
    // 2.私有化静态实例
    private final static Hungry hungry = new Hungry();
    // 3.暴露公共的静态方法返回实例
    public static Hungry getInstance() {
        return hungry;
    }
}
```

### 懒汉式

#### 线程不安全

```java
// note: 懒汉式单例模式(单线程有效 多线程出错)
public class LazyMan {
    // 将构造器私有化 防止直接new
    private LazyMan(){};
    // 创建一个私有的静态实例(不初始化)
    private static LazyMan lazyMan;
    // 提供一个公共的静态方法返回实例
    public static LazyMan getInstance() {
        if (lazyMan == null) { // note: 多个线程同时访问这个方法时，可能会创建多个实例
            lazyMan = new LazyMan();
        }
        return lazyMan;
    }
}
```

#### 双重校验锁模式 (线程安全) DCL

```java

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
                if (lazyMan == null) { // 两个if我还没搞懂
                    lazyMan = new LazyMan2();
                }
            }
        }
        return lazyMan;
    }
}
```

为什么要用volatile修饰变量？

`lazyMan = new LazyMan2();`这一行代码其实底层有三步：

1. 为lazyMan分配内存空间
2. 调用构造方法初始化对象lazyMan
3. 将lazyMan指向分配的内存空间

故而可能发生指令重排，如果排序为132就废废了