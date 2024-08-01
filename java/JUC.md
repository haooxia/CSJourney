# 并发编程

[toc]

## 线程基础

* 进程：java中启动main函数即启动了一个JVM进程，main函数所在的线程就是这个进程中的一个线程，也即主线程。
* 多个线程共享进程的堆和方法区资源，每个线程都有自己的程序计数器、虚拟机栈和本地方法栈（寄存器和栈区）
* 用户线程：由用户空间程序管理和调度的线程，运行在用户空间（专门给应用程序使用
  * 无需用户态和内核态切换，开销较小
  * **不可以利用多核**：因为os无法感知用户线程的存在，只能将整个进程调度到某个cpu核心上运行
* 内核线程：由os内核管理和调度的线程，运行在内核空间（只有内核程序可以访问
  * os可以感知内核级线程，此时调度不以进程为单位，而是将内核线程独立地调度到不同的cpu核心上

### java线程和os线程的区别

* jdk1.2之前，java线程是一种用户级线程，即jvm（应用程序）自己模拟了多线程的运行，而不依赖于os
  * 不能直接使用操作系统提供的功能如异步 I/O、只能在一个内核线程上运行无法利用多核
* jdk1.2之后，Java线程改为基于os原生的内核线程实现(即通过`pthread_create()`创建线程)，可以利用多核（即**现在的Java线程的本质上就是操作系统的线程**

### process vs. thread (jvm视角)

* 多个线程共享进程的堆和方法区(jdk1.8之后的元空间)，但每个线程有自己的pc, 虚拟机栈和本地方法栈

### 线程创建

* **继承Thread类、实现Runnable接口、实现Callable接口、使用线程池、使用CompletableFuture等**都可以创建线程
* 严格来说，Java 就只有一种方式可以创建线程，那就是通过new Thread().start()创建。不管是哪种方式，最终还是依赖于new Thread().start()。（不懂...

* **implements Runnable接口，重写run方法（底层是静态代理模式）**
  * 创建一个Thread对象，并将Runnable接口实现类对象传递给它，然后调用start方法。
    * 可以把同一个Runnable对象（即用户自定义的被代理类：只用关注业务逻辑的实现）丢给多个线程使用（即Thread代理类：封装了创建和管理逻辑）（一份资源，多个代理） ==太妙了==
      * ![picture 0](../images/47183e58ad94304f65901c4ffa16a944390e2761fce0cc176a729a43c566cb84.png)  
    * ![picture 1](../images/201740b20c191d51ffd42bc1a2972512ad5a64743acd687c47daf28f79f1b94d.png)  
      * 详见onenote: Static Proxy

### 线程生命周期和状态

`enum Thread.Stat`

> java分6类，os分5类：创建 就绪 运行 阻塞 终止
> java将os的就绪和阻塞态合并为RUNNABLE（因为现代线程切换太快了，没必要分开了
> java将os阻塞细分为：BLIOCKED, WAITING, TIMED_WAITING三种状态

* `NEW`: 初始状态，线程被创建出来但**还没有被调用start()** （os创建态）
* `RUNNABLE`: 运行状态，线程被调用了start()等待运行的状态（等于os中的**就绪态+运行态**）
* `BLOCKED`：阻塞状态，需要**等待锁释放**（等待监视器锁
* `WAITING`：等待状态，表示该线程需要**等待其他线程做出一些特定动作**（通知notify或中断）
* `TIMED_WAITING`：超时等待状态，即具有**指定等待时间**的等待状态；在指定的时间后自行返回而不是像WAITING那样一直等待（比如sleep和wait方法
* `TERMINATED`：终止状态，表示该线程已经运行完毕，即run方法执行完毕（os终止态）;（线程终止之后不能再次启动，重新编译运行创建的是一个新的线程实例）

#### blocked vs. waiting

* 触发条件:
  * 线程进入BLOCKED状态通常是因为试图获取一个对象的锁（monitor lock），但该锁已经被另一个线程持有。这通常发生在尝试进入synchronized块或方法时，如果锁已被占用，则线程将被阻塞直到锁可用。
  * 线程进入WAITING状态是因为它正在等待另一个线程执行某些操作，例如调用Object.wait()方法、Thread.join()方法或LockSupport.park()方法。在这种状态下，**线程将不会消耗CPU资源，并且不会参与锁的竞争**。
* 唤醒机制:
  * 当一个线程被阻塞等待锁时，一旦锁被释放，线程将自动竞争锁，**无需显式唤醒**。如获取到锁，线程从BLOCKED状态变为RUNNABLE状态。
  * 线程在WAITING状态中**需要被显式唤醒**。例如，如果线程调用了Object.wait()，那么它必须等待另一个线程调用同一对象上的Object.notify()或Object.notifyAll()方法才能被唤醒
* 恢复执行：
  * blocked一旦获取锁即可恢复执行
  * waiting被唤醒后，**通常需要重新竞争锁才能继续执行**
* 使用场景：
  * blocked常用于synchronized关键字的锁竞争
  * waiting常用于线程间的协作和通信

#### notify() vs notifyall()

同样是唤醒waiting的线程，同样最多只有一个线程能获得锁，同样不能控制哪个线程获得锁。

* notify：**随机**唤醒等待池中的一个线程（不能确保唤醒的是最合适的线程），其他线程依然处于wait的等待唤醒状态；适用于等待条件只会让一个线程满足的情况
  * 但其实如何唤醒线程是取决于jvm的，一般我们说随机。但其实hotspot是按照**FIFO**的顺序唤醒的（是这样的，synchronized是飞公平锁，遂FIFO
* notifyAll：唤醒等待池中的所有线程（唤醒所有，让他们自己竞争，更可能得到我们想要的结果，但也可能造成不必要的竞争），**所有线程退出wait的状态，开始竞争锁，但只有一个线程能抢到**，这个线程执行完后，其他线程继续竞争；适用于等待条件可能会让多个线程同时满足的情况
* notifyall通常更安全（notify可能死锁），notify通常更高效（notifyall要竞争锁）

#### Thread#sleep() vs. Object#wait()

* 二者都可以暂停线程执行
* sleep执行完毕不会释放锁，wait会释放
* sleep常用于暂停，wait常用于**线程间通信**
* 执行sleep()后线程会自动苏醒；wait()调用后，线程不会自动苏醒，需要其他线程调用同一对象的**notify()或notifyAll()**（此时是**WAITING状态**）；而wait(long timeout)这个有参版本可以设定超时参数，**会自动苏醒**（此时是**TIMED_WAITING状态**）
* 为什么wait()在Object中，而非定义在Thread中？
  * 将wait()定义在Object类中允许在任何对象上调用这个方法，而不仅限于Thread对象；每个对象（Object）都拥有对象锁，既然要释放当前线程占有的对象锁并让其进入 WAITING 状态，自然是要操作对应的对象（Object）而非当前的线程（Thread）

### 线程调用的本质 start() vs. run()

* 当运行该java程序时，即开启了一个进程，首先进入main方法开启主线程(即main线程)，然后t1.start()启动MyThread子线程，start()会调用线程的run(), 进而调用start0()，该方法时一个native本地方法，由jvm调用，底层是c/cpp（linux中jvm应该会调用pthread_create函数创建进程(unix-like os)）;
  * **所以一般可以说java本身是没有权限开启线程的**，java无法直接操作os，需要依赖底层的cpp
* 更进一步，start()调用start0()之后，**线程不会立马执行，只是将线程变成了就绪状态**，交给cpu调度。
* 执行MyThread子线程时，main主线程不会阻塞。此时，主线程和子线程并发或并行(如果有多核cpu)执行。主线程结束之后，子线程未必结束，所以当然进程也未必结束。

> JVM本身不负责线程的调度，而是将线程的调度委托给os；os通过抢占式的调度策略

**为何不直接使用主线程调用run()?** 
因为run()就是一个main线程下的一个**普通方法**，直接使用主线程调用run()并没有真的启动线程。执行该方法时，主线程被阻塞(很正常，没有启动子线程)。

![picture 1](../images/b14244e42b245698334fbd9c84d0d4065f1f82ad745710418ab2f2950308e2f9.png){width=70%}

### 线程相关method

* `setPriority(int)`: 更改线程的**优先级**；优先级低意味着调用概率低
  * `MIN_PRIORITY=1; NORM_PRIORITY=5; MAX_PRIORITY=10`
* `sleep(long millis)`: 休眠指定毫秒，线程被阻塞后会自动苏醒
* `join()`: 等待线程终止
* `yield()`: **线程礼让**，暂停当前正在执行的线程对象，并执行其他线程；注意暂停了并非阻塞，处于就绪状态，等待cpu调度
* `setDaemon()`: **守护线程**，用户线程结束之后，jvm不用管守护线程是否执行完毕...比如GC线程
* `interrupt()`: 中断线程，少用；推荐使用标志位停止线程
* `isAlive()`: 测试是否活动

## 多线程

并发 vs. 并行

同步 vs. 异步

* 同步：发出一个调用之后没有得到结果之前，该调用就不可以返回，一直等待
* 异步：调用在发出之后，不用等待返回结果，该调用直接返回

线程安全：在多线程环境下，对于同一份数据，不管有多少个线程同时访问，都能保证这份数据的正确性和一致性
线程不安全：...可能会导致数据混乱、错误或者丢失

### java如何保证多线程安全/概括

* `synchronized关键字`：同步代码块或方法，确保同一时刻只有一个线程可以访问这些代码。
* `volatile关键字`：用于变量，确保所有线程看到的是该变量的最新值，而不是可能存储在本地寄存器中的副本。
* `Lock接口/ReentrantLock类`：比synchronized更强大灵活高级
* `原子类juc.atomic`：提供原子操作，可用于**更新基本类型变量而无需额外的同步**
* `线程局部变量ThreadLocal`：为每个线程提供独立的变量副本，消除了线程之间的**竞争条件**（多个线程在访问和操作共享数据时，最终结果依赖于执行顺序，可能会导致不可预测的结果/错误）
* `并发集合juc`：集合内部已经实现了线程安全的逻辑，用久完了, e.g., ConcurrentHashMap, ConcurrentLinkedQueue
* `JUC工具类`：一些工具类用于控制线程间的同步和协作, e.g., Semaphore, CyclicBarrier 

### 单核 CPU 上运行多个线程效率一定会高吗

未必，取决于线程的任务类型：

* CPU密集型：主要进行计算和逻辑处理，需要占用大量的CPU资源
  * 多个线程同时运行会导致频繁的线程切换，增加了系统的开销，降低了效率
* IO密集型：主要进行输入输出操作，如读写文件、网络通信等，需要等待IO设备的响应，而不占用太多的CPU资源
  * 多个线程同时运行可以利用 CPU 在等待 IO 时的空闲时间，提高了效率

### 死锁

死锁：多个线程同时被阻塞，它们中的一个或者全部都在等待某个资源被释放。由于线程被无限期地阻塞，因此程序不可能正常终止

### 死锁检测 (java)

* 使用jmap、jstack等命令查看 JVM 线程栈和堆内存的情况。如果有死锁，jstack 的输出中通常会有 Found one Java-level deadlock:的字样，后面会跟着死锁相关的线程信息。另外，实际项目中还可以搭配使用top、df、free等命令查看操作系统的基本情况，出现死锁**可能**会导致 CPU、内存等资源消耗过高
* 采用VisualVM、JConsole等工具进行排查

> 死锁预防与避免见os section

<!-- ### 虚拟线程

暂略 -->

### Java内存模型 JMM

> JMM是一种概念/约定；和JVM内存区域不是一个玩意儿

JMM：**定义了对于一个共享变量，当另一个线程对这个共享变量执行写操作后，这个线程对这个共享变量的可见性**

> **CPU缓存**：我们知道cache是cpu和主存的缓冲，会出现内存-缓存不一致的问题，者通过缓存一致协议解决(MESI协议)
> **指令重排**：为了提升执行速度，计算机在执行java源码时会经历：编译器优化重排 -> 指令并排重排 -> 内存系统重排 的过程，得到最终os可执行的指令序列
> > 指令重排**可以保证串行语义一致，但是没有义务保证多线程间的语义也一致**，所以在多线程下，指令重排序可能会导致一些问题
> > 可以通过内存屏障(memory barrier)这个cpu指令来禁止处理器发生重排，保障指令执行的有序性

==JMM vs. Java内存区域==

* Java内存区域定义了JVM在运行时如何将数据分区存储，强调对内存空间的划分，比如堆主要用于存放对象实例
* JMM和java并发编程相关，**抽象了线程和主内存之间的关系**（比如说线程之间的共享变量必须存储在主内存中），规定了从Java源代码到CPU可执行指令的这个**转化过程**要遵守哪些和并发相关的原则和规范，其主要目的是为了**简化多线程编程**，增强程序可移植性的。

#### JMM抽象了线程和主内存之间的关系

* **主内存** main memory：线程之间的共享变量必须存在主内存（ie 多线程共享的内存区域
* **本地内存** local memory (eg **寄存器**)：每个线程都有一个私有的本地内存，存储共享变量的副本
  * 老版本java总会从主存/共享内存读取变量，所以没啥问题。jdk1.2之后的jmm，线程可以把变量存到本地内存，而非直接在主存读写。这就有问题：一个线程在主存中修改了一个变量的值，而另外一个线程还继续使用它在寄存器中的变量值的拷贝，造成数据的不一致（很像cpu主存-cache不一致问题了

> note: java的主内存和本地内存是逻辑概念；主内存和本地内存可能都在物理内存RAM中的..有时可能在register和cache中
> 主内存与工作内存直接的具体交互协议：jmm搞了8个同步操作来实现...

![picture 4](../images/2b4ecf4aed554055d3de55654d0ba4821261fc15aa70a3af1a7868201ad6b67a.png)  

![picture 2](../images/63418576ee3b83ed7d8aedbe86a4fc3c284ba6cc27a9a79516171f3282efd6bb.png)  

#### happens-before

##### 背景

happens-before用于平衡程序员、编译器和处理器，程序员追求易于理解和编程（强内存模型），编译器追求较少约束，去尽可能地优化性能（弱内存模型）；设计思想：

* 为了减少对编译器和处理器的约束，只要不改变程序的执行结果（单线程及多线程程序），不管你编译器和处理器怎么进行重排序优化都行
* 对于会改变程序执行结果的重排序，JMM要求编译器和处理器必须禁止这种重排序。

##### 定义

* 如果操作A happens-before 操作B,那么A的执行结果对B可见,且A的执行顺序排在B之前
* 两个操作之间存在happens-before关系，并不意味着Java 平台的具体实现必须要按照happens-before关系指定的顺序来执行。如重排序之后的执行结果，与按happens-before关系来执行的结果一致，那么JMM也允许这样的重排序

> 核心思想：前一个操作的结果对于后一个操作是可见的，无论这两个操作是否在同一个线程里

##### happens-before常见规则

* 程序顺序规则：一个线程内，按照代码顺序执行
* 解锁规则：解锁 happens-before 于加锁
* volatile变量规则：对一个volatile变量的写操作happens-before于后面对这个volatile变量的读操作。说白了就是对volatile变量的写操作的结果对于发生于其后的任何操作都是可见的
* 传递规则：如果A happens-before B，且B happens-before C，那么A happens-before C；

#### 并发编程的三特性

* **原子性**：一次操作或者多次操作，要么所有的操作全部都得到执行要么都不执行；可借助synchronized, Lock及原子类实现...
* **可见性**：当一个线程修改了**共享变量**，那么另外的线程都是立即可以看到更新后的值；可借助synchronized, **volatile**, Lock实现...
* **有序性**：由于指令重排序问题，代码的执行顺序未必就是编写代码时候的顺序。**volatile**可以禁止指令进行重排序优化（底层是利用了内存屏障cpu指令保证原顺序

### volatile

pronounce: `[ˈvɒlətaɪl]`
==volatile可以保证变量的可见性、禁止指令重排序，但**不可保证原子性**==
> synchronized可以保证数据的可见性与原子性

**保证变量可见性**：

在 Java 中，volatile关键字可以**保证变量的可见性**，将变量声明为volatile，这就指示JVM，这个变量是**共享且不稳定的**，**每次使用它都到主存中进行读取**。

> 在C语言中，它最原始的意义就是禁用CPU缓存。如果我们将一个变量使用volatile修饰，这就指示编译器，这个变量是共享且不稳定的，每次使用它都到主存中进行读取

![picture 9](../images/a1ea5e17ad0ee00ae2e55bf4c25993f18f60e72d00f9a2b752b829ce16fc7c93.png)  

**禁止指令重排序**：

将变量声明为 volatile ，在对这个变量进行读写操作的时候，会通过插入特定的 内存屏障 的方式来禁止指令重排序

经典案例：DCL双重校验锁实现单例模式

请务必移步笔记：[link](https://github.com/haooxia/interview/blob/main/java/java_common_code.md)

**volatile不能保证原子性**：

![picture 10](../images/d41cd6cc8b99b2e14e1b0055d1ceee4eda86012c6a6efe757ae6b9beb101a1bd.png)  

### 乐观锁与悲观锁

**悲观锁**：**总是假设最坏的情况**，认为共享资源每次被访问的时候**一定**会出现问题(比如共享数据被修改)，所以每次在获取资源操作的时候都会上锁；这样其他线程想拿到这个资源就会阻塞直到锁被上一个持有者释放。确保各位线程**串行执行**

* e.g., synchronized, ReentrantLock等独占锁就是悲观锁的思想;
* 缺陷
  * 阻塞问题：只有一个线程可以冲，其他线程被阻塞等待，系统并发性能不行
  * **性能较差**：在高并发环境下，多个线程同时竞争同一个锁会导致**频繁的上下文切换**和等待时间增加
    * 获得锁的线程会从"就绪"状态变为"运行"状态。
    * 未获得锁的线程会从"就绪"状态变为"阻塞"状态。
  * 悲观锁还可能会存在死锁问题

**乐观锁**：**总是假设最好的情况**，认为共享资源每次被访问的时候不会出现问题或概率很低，故而乐观锁并不加锁，只有当更新数据时才去判断有没有其他线程对数据做了修改，如无则安全，如有说明发生了线程安全问题，重试或抛异常；

* e.g., 版本号机制, CAS 算法, `juc.atomic`包下的原子变量类(eg, AtomicInteger)就是使用CAS实现的
* 优点：高并发的场景下，不存在锁竞争造成线程阻塞，也不会有死锁的问题，在性能上往往会更胜一筹
* 问题：如果冲突频繁发生（写占比非常多的情况），会频繁失败和重试，这样同样会非常影响性能，导致CPU飙升
* 乐观锁通常多用于写比较少的情况（多读场景，竞争较少），这样可以避免频繁加锁影响性能。不过，乐观锁主要针对的对象是单个共享变量

#### 版本号机制 vs. CAS

**版本号机制**：在数据表加上一个数据版本号version字段，表示数据被修改的次数。如数据被修改，version++；当线程A要更新数据值时，在读取数据的同时也会读取version值，在提交更新时，若刚才读取到的version值为当前数据库中的version值相等时才更新，否则重试更新操作，直到更新成功

**CAS法**（Compare And Swap）：是乐观锁的一种实现方式，是硬件层面支持的原子操作(一条cpu指令)，包含3个操作基数，内存位置（Var），预期原值（Expected）和新值（New），**只有当内存位置V的值等于预期原值E时**（即数据没被修改），**才将该位置的值更新为新值N**，否则不做任何操作。

##### java中是如何实现CAS的

java中实现CAS的关键是Unsafe类，一个提供低级别、不安全操作的类，通常用于JVM内部或一些需要极高性能和底层访问的库中，不推荐普通开发者在应用程序中使用。

* `sun.misc.Unsafe`类提供了`compareAndSwapObject`、`compareAndSwapInt`、`compareAndSwapLong`方法来实现的对Object、int、long类型的CAS操作
* 可见这些函数是native function，即通过c/c++实现，直接调用底层的硬件指令来实现原子操作；即java并没有直接用java实现CAS，通过C++内联汇编实现
* `java.util.concurrent.atomic;`包提供了一些用于**原子操作**的类，利用底层的**原子指令**，确保多线程环境下的操作是线程安全的，通常比锁高效数倍

```java
// 比如用于int的CAS操作：
public final native boolean compareAndSwapInt(Object var1, long var2, int var4, int var5);

// 比如AtomicInteger
// 利用CAS操作实现无锁的线程安全性
// 原子地获取并增加整数值
public final int getAndAddInt(Object o, long offset, int delta) {
    int v;
    do {
        // 以 volatile 方式获取对象 o 在内存偏移量 offset 处的整数值
        v = getIntVolatile(o, offset);
    } while (!compareAndSwapInt(o, offset, v, v + delta));
    // 返回旧值
    return v;
}
```

> 可见getAndAddInt使用了do-while循环，即**自旋锁**

##### CAS的问题

* ABA问题：一个变量V初次读取的时候是A值，并且在准备赋值的时候检查到它仍然是A值，这并不能说明它的值没有被其他线程修改过；因为可能先改为B再改为A，CAS操作就会误认为它从来没有被修改过
  * 解决方案：在变量前面追加上版本号或者时间戳；eg `AtomicStampedReference`类
    * 其中的compareAndSet()方法首先检查**当前引用是否等于预期引用**，并且**当前标志是否等于预期标志**，如果全相等，则以原子方式将该引用和该标志的值设置为给定的更新值（这是结合了CAS和版本号机制？
* 自旋操作使得循环时间长开销大
* 只能保证一个共享变量的原子操作，不过jdk1.5之后AtomicReference可以保证**对象之间**的原子性，那你可以把多个变量放在一个对象中

### synchronized

background:
> 线程同步包括线程互斥和线程同步
> 线程同步属于线程通信的大范畴

**同步synchronism**：多个线程/进程在一些关键点上可能需要相互等待与互通信息（即时间上有先后关系）
> 同步是直接制约关系，因为它明确地规定了进程间的依赖和顺序。

**互斥mutual exclusion / mutex**: 多线程/进程间存在共享数据资源，当一个线程进入临界区，其他线程必须被组织进入（等待） -> 间接制约关系
> 互斥是间接制约关系，因为进程间并不直接相互依赖，而是通过对共享资源的访问间接地相互影响。

---

synchronized is a keyword: 用于**线程同步**
> jdk1.5新版本的synchronized已经不太算重量级锁了，效率还行，很多框架也都用了，所以你也可以用...

#### 如何使用synchronized？

* 修饰实例方法（**锁当前对象实例, ie this**）：进入同步代码前需要获得当前对象实例的锁
* 修饰静态方法（**锁当前类**）：进入同步代码前要获得当前class的锁
  * 当前对象锁和类锁不是同一把，所以线程A调用synchronized修饰的实例方法时，线程B可以去调用synchronized修饰的静态方法
* 修饰代码块
  * 锁指定对象：`synchronized(objectName)`
  * 锁指定类：`synchronized(ClassName.class)`

#### synchronized的底层本质

* javap反编译.class可知：synchronized是通过`monitorenter`和`monitorexit`指令来指定**同步代码块**的开始和结束；
* 执行`monitorenter`时，**线程试图获取锁/对象监视器`monitor`的持有权**
  * 每个Java对象都有一个与之关联的监视器锁 monitor lock
* synchronized**修饰方法**的情况：通过`ACC_SYNCHRONIZED`标识指名是一个同步方法；没有上述两个指令

#### synchronized vs. volatile

二者并非对立，而是互补

* volatile关键字主要用于解决变量在**多个线程之间的可见性**，而 synchronized 关键字解决的是**多个线程之间访问资源的同步性**
* volatile关键字是线程同步的**轻量级实现**，所以volatile性能肯定比synchronized关键字要好。但volatile只能用于变量而synchronized关键字可以修饰方法以及代码块
* volatile能保证数据的可见性，但不能保证数据的原子性。synchronized关键字两者都能保证

### ReentrantLock / Lock

可重入锁：也叫递归锁，允许**同一个线程多次获取同一把锁**的锁机制。它避免了同一线程在递归调用或者在调用链上需要多次获取同一锁时发生死锁（即不可重入锁容易发生死锁
> 比如你可以调用同一个类的两个**synchronized成员方法**（一个线程，一把**对象锁**，获取了两次

* `ReentrantLock`是`java.util.concurrent.locks.Lock`接口的常用实现类
* `ReentrantLock`和`synchronized`类似，但更灵活强大，增加了轮询、超时、中断、公平非公平锁等高级功能
  * Lock和synchronized功能类似，就是可以显示加锁 释放锁，ie `lock.lock(); lock.unlock();`还有一个尝试获取锁`lock.tryLock()`(就是非忙等待锁咯)
* 默认使用非公平锁，可以通过构造函数写true来指定为公平锁
* 底层基于`AQS`，后文再说

#### 公平锁 vs. 非公平锁

* 公平锁: 锁被释放之后，**先申请的线程先得到锁**。吞吐量降低: 严格的先来先得顺序可能导致整体吞吐量下降，因为可能有些线程能够更快地处理任务，但必须等待其前面的线程（FIFO
* 非公平锁：锁被释放之后，后申请的线程可能会先获取到锁，是随机或者按照其他优先级排序的。性能更好，但可能会导致某些线程永远无法获取到锁；（**优先级与饥饿问题**

#### ReentrantLock vs. synchronized

* 二者都是可重入锁（所有的Lock实现类及synchronized都是可重入的
* synchronized**基于JVM**，并没有向外暴露什么实现；ReentrantLock**基于JDK**（API层面，需要lock(),unlock()配和try-finally完成），我们可以看到源码实现
  * Lock必须手动开关，synchronized出了作用域会自动释放
* Lock适合锁大量同步代码，synchronized适合少量代码同步
* ReentrantLock有一些synchronized没的高级功能：
  * synchronized只能是公平锁，ReentrantLock均可
  * 可实现==选择性通知==: synchronized与wait()和notify()/notifyAll()方法相结合可以实现等待/通知机制，但这俩**不可以根据条件指定唤醒特定线程**。ReentrantLock类可以借助于Condition接口，更加灵活，可指定等待线程
  * 等待可中断：正在等待的线程可以选择放弃等待，改为处理其他事情

### ReentrantReadWriteLock / ReadWriteLock

`ReentrantReadWriteLock`是`java.util.concurrent.locks.ReadWriteLock`接口的常用实现类

```java
public interface ReadWriteLock {
    Lock readLock();
    Lock writeLock();
}
```

* 它是**两把锁/Lock**，一把读锁，一把写锁；读锁是**共享锁**（即一把锁可以被多个线程同时获得），写锁是**独占锁**（即同时最多只能被一个线程持有）
  * 一般锁进行并发控制的规则：读读互斥、读写互斥、写写互斥
  * 读写锁进行并发控制的规则：**读读不互斥**、读写互斥、写写互斥；更细粒度
* 使用场景：**读多写少**的情况下，能够明显提升系统性能
* 底层也是基于AQS，后面再说

---

**线程持有读锁还能获取写锁吗？**

不能，获取写锁时，系统会检查是否有任何读锁被持有。如果有，即使这些读锁是由当前线程持有，写锁的获取依然会失败。避免死锁：如果允许一个线程在持有读锁时获取写锁，可能会导致死锁。例如，多个线程都持有读锁并尝试获取写锁时，所有线程都会被阻塞，无法继续执行。

**线程持有写锁还能获取读锁吗？**

能，获取读锁时，系统会检查是否有写锁被持有。如果写锁是由当前线程持有，读锁的获取会成功。**因为写锁已经保证了独占访问，当前线程再获取读锁不会破坏这种独占性。**

**读锁能否升级为写锁？**

不能，写锁可降级为读锁，但读锁不能升级为写锁。可能会有死锁问题发生。举个例子：假设两个线程的读锁都想升级写锁，则需要对方都释放自己锁，而双方都不释放，就会产生死锁。

### ThreadLocal

我们知道：
non-staitc成员变量（堆中创建）是所有线程共享的
static成员变量（在方法区中）是所有线程共享的
局部变量（栈中创建）是**线程私有**的
> 所有线程共享堆和方法区

我们如何实现：每个线程都有自己的专属本地变量？即我们不想让多个线程共享同一变量。

* ThreadLocal是Java中用于解决**线程安全**问题的一种机制，它允许创建线程局部变量，即**每个线程都有自己独立的变量副本**，从而**避免了线程间的资源共享和同步问题**
* 作用
  * **线程隔离**：每个线程都有独立的变量副本，不互相影响，多线程时不用担心数据同步问题
  * **降低耦合度**：在同一个线程内的多个函数或组件之间，**使用ThreadLocal可以减少参数的传递**，降低代码之间的耦合度，使代码更加清晰和模块化
  * **性能优势**：由于ThreadLocal避免了线程间的同步开销，所以在大量线程并发执行时，相比传统的锁机制，它可以提供更好的性能

```java
// 创建方法
private static final ThreadLocal<Integer> threadLocalValue = ThreadLocal.withInitial(() -> 0);
threadLocalValue.set(val)
threadLocalValue.get()
threadLocalValue.remove()
```

#### ThreadLocal原理

* **每个线程都有自己的`ThreadLocalMap`成员变量，用于存储该线程所持有的所有`ThreadLocal`变量的值**。是一个Map键值对结构，维护了一个Entry数组，每个Entry的key是`ThreadLocal`本身，value是value值。
* 如果一个线程中声明了两个`ThreadLocal`对象的话，`Thread`内部使用这一个`ThreadLocalMap`存放多个`ThreadLocal`数据，key是不同的`ThreadLocal`，value是对应的Object（即通过ThreadLocal对象调用set()设置的值）

```java
ThreadLocalMap(ThreadLocal<?> firstKey, Object firstValue) {}
```

![picture 11](../images/f092819fbf9c4122c6a72f288277b60b2be81a1eacfcbebd1fd9ceb5fa249ec8.png)  
> 每个线程都有一个Map成员，独立的空间，get()时就检查当前线程的ThreadLocalMap有没有值，有则返回，无则通过initialValue()创建并放入Map中(前提是重写了该方法)；set()就是将value给到当前线程的key(ThreadLocal)；remove()大概是直接删除对应Entry

#### ThreadLocal内存泄漏

ThreadLocalMap中使用的key是ThreadLocal的弱引用，value是强引用。故而ThreadLocal没有被外部强引用时，垃圾回收的时候key会被清理掉，而value并不会。这下Map就出现了key为null的Entry，这个value不会被GC回收，即为内存泄漏。（当线程结束时，ThreadLocalMap随之销毁，此时ThreadLocal对象还在...直到没有其他引用指向它为止
（不太懂，看完jvm回来看

### BlockingQueue

实现类: `ArrayBlockingQueue`, `LinkedBlockingQueue`, `SynchronousQueue`...
> AbstractQueue: 非阻塞队列
> SynchronousQueue: 同步队列，只能存一个元素，put之后必须先拿出来

* 应用：生产者消费者，线程池

![picture 6](../images/885db25a3731aa5083be2dda7807905691c7ae8bf39bd2f846c320a28140c5a3.png)  

### 线程池

* 池化技术：是一种资源管理策略,旨在提高资源利用效率。其核心思想是**在需要时动态分配和重用资源**,**而不是频繁地创建和销毁**它们
  * 线程池: 预先创建一组线程,用于执行并发任务。适用于Web服务器、多线程下载器等场景；（复用 + 方便管理）
  * 连接池: 管理数据库连接的复用,减少数据库访问延迟，jdbc连接池
  * 对象池: 维护一组已创建的对象实例,适用于需要频繁创建和销毁对象的应用
  * 缓存池(存储频繁访问的数据), 文件池(存fd)，进程池等

#### 线程池三大创建方法

* `SingleThreadExecutor`: 线程池中仅一个线程，若提交给线程池的任务超过一个，存在任务队列中，等到线程空闲时，按照FIFO顺序执行队列中的任务
* `FixedThreadPool`：线程数量固定不动
* `CachedThreadPool`：线程数量不确定，可伸缩
* **三种创建方式底层都是调用`ThreadPoolExecutor`**
![picture 12](../images/5b5a66e423e8314e43aeae799e01280568e35f1a498ec6e99157b485e30175fc.png)  

```java
// note: 创建单个线程; 任务都是由同一个线程来执行
ExecutorService threadPoll1 = Executors.newSingleThreadExecutor();
// note： 创建固定数量线程池
ExecutorService threadPoll2 = Executors.newFixedThreadPool(10);
// note: 创建缓存线程池，可伸缩，遇强则强
ExecutorService threadPoll3 = Executors.newCachedThreadPool();

for (int i = 0; i < 1000000; i++) {
    threadPoll3.execute(() -> {
        System.out.println(Thread.currentThread().getName() + " is running");
    }); // 函数式接口Runnable的对象
}
```

#### ThreadPoolExecutor线程池7大参数

```java
public ThreadPoolExecutor(
  int corePoolSize,
  int maximumPoolSize,
  long keepAliveTime,
  TimeUnit unit,
  BlockingQueue<Runnable> workQueue,
  ThreadFactory threadFactory,
  RejectedExecutionHandler handler
  )
```

* **corePoolSize**: 核心线程数；池中一直保持活动的线程数量；当线程池中线程数量少于corePoolSize时，即使有空闲线程，依然会创建新线程
* **maximumPoolSize**: 最大线程数；当任务队列满 && 活动线程数 < maximumPoolSize -> 创建新新线程
* keepAliveTime: 线程空闲时间；
  * 当线程池中线程数 > corePoolSize时，如果某线程空闲时间 > keepAliveTime -> **终止该线程**（超时不候）
* unit: keepAliveTime参数的时间单位
* **workQueue**: 任务队列，保存**等待**执行的任务；如LinkedBlockingQueue或SynchronousQueue；
  * 如果等待队列满 && 运行的线程数 < maximumPoolSize -> **创建新线程**来处理任务（）
  * 如果等待队列满 && 运行的线程数 = maximumPoolSize -> 根据**拒绝策略**来处理该任务
* threadFactor: 线程工厂；用于创建新线程，可用来设置线程名称、优先级等
* **handler**: 拒绝策略；当任务太多无法处理时，采取的处理方式。RejectedExecutionHandler 接口有几种常见的实现，如 AbortPolicy（抛出异常）、CallerRunsPolicy（由调用线程处理任务）等。

> ==最大承载任务数== = workQueue队列容量 + 最大线程数

#### 线程池处理任务的流程

![picture 8](../images/cbc3ac6317aa65c2c53c0213d2a7b3fdd20638f2b83219cc878bca44e16f9dc3.png){width=80%}
![picture 13](../images/29f694e857041b817c6bc8aaed55149826f3f0699342159518d039772dd3b911.png)  
> 还是挺妙的: 先创建够corePoolSize个 -> 加入队列 -> 创建够maximumPoolSize个 / 终止线程 -> 拒绝策略

#### 拒绝策略

==**4种**拒绝策略==（达到最大线程数 & 任务队列也放满了任务时，即达到最大承载任务数）

* `AbortPolicy()`: default; 不处理，并抛出异常`RejectedExecutionException`
* `CallerRunspolicy`: 交给调用者执行（比如通常返回给main线程执行了），哪儿来的去哪儿（适合**需要保证任意任务都要被执行**的情况, 即不允许丢弃任务；但比较耗时，而且交给主线程的话，主线程可能就阻塞了，影响程序的正常运行
* `DiscardPolicy`: 不管不顾；不处理，不抛出异常，任务**被直接丢弃**
* `DiscardOldestPolicy`: **尝试**和最早的任务进行竞争，如果竞争失败，直接扔掉，不抛出异常；如果成功则执行（丢弃最早的未处理的任务请求

#### 阻塞队列

（暂略）
<!-- **线程池常用阻塞队列**：

* `LinkedBlockingQueue`: 默认容量为Integer.MAX_VALUE的有界阻塞队列，底层是链表
* `SynchronousQueue`: 
* `ArrayBlockingQueue`: -->

#### 参数设置

==ThreadPoolExecutor参数设置经验==

一个简单且适用面比较广的公式：

* corePoolSize
  * CPU密集型任务，设置为cpu核心数 + 1
    * 主要消耗cpu资源，之所以任务比核心数+1：线程可能出现短暂中断而暂停（eg 缺页中断），此时cpu处于空闲状态，就可以用这个核心来跑多一个的任务，保持cpu的利用率
  * I/O密集型任务，可设置较大的corePoolSize，常为cpu核心数的2倍
    * IO操作(eg 读写文件、网络通信)通常比较慢，而一般采用DMA控制器(直接内存访问)来管理IO操作，不用cpu直接控制，所以当需要进行数据传输时，cpu只需要向DMA控制器发送指令，然后就可以去处理其他任务了，DMA完成整个数据传输过程之后再通知cpu。故而中间这段时间就可以去处理其他线程
  * 当然更严谨的计算方法：应该使用VisualVM测试“线程等待时间/线程计算时间”的比例
* maximumPoolSize
  * 建议设置为corePoolSize的2-3倍
  * 避免设置太大，导致资源耗尽
* workQueue
  * 对于短期异步任务,可使用SynchronousQueue（不懂
  * 对于长期任务,可使用LinkedBlockingQueue或ArrayBlockingQueue

#### 线程池关闭

可以通过shutdown()或者shutdownNow()

* shutdown(): 将线程池状态置为shutdown,并不会立即停止
  * 停止接收外部submit的任务
  * 内部正在跑的的任务以及等待队列中的任务会执行完毕
* shotdownNow():  将线程池状态置为 stop。一般会立即停止，事实上不一定
  * 同上，先停止接收外部submit的任务
  * **忽略队列里等待的任务**
  * **尝试将正在跑的任务interrupt中断**（遍历逐个interrupt）
  * 返回未执行的任务列表

in simple terms:
shutdownNow()能立即停止线程池，正在跑的和正在等待的任务都停下了。这样做立即生效，但是风险也比较大。
shutdown()只是关闭了提交通道，用submit()是无效的；而内部的任务该怎么跑还是怎么跑，跑完再彻底停止线程池。

### AQS

* AQS aka AbstractQueueSynchronizer: 抽象队列同步器，属于`juc.locks`包，是一个抽象类
* AQS是一个用于构建锁、同步器、协作工具类的工具类/框架