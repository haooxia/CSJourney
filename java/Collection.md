# collection

- [collection](#collection)
  - [概括](#概括)
    - [java的集合类有哪些 ☆](#java的集合类有哪些-)
    - [哪些是线程安全/不安全的 ☆](#哪些是线程安全不安全的-)
    - [如何选择](#如何选择)
  - [前置知识](#前置知识)
    - [enhanced for](#enhanced-for)
  - [接口](#接口)
    - [Iterable Interface](#iterable-interface)
    - [Collection Interface](#collection-interface)
      - [Collection vs. Collections](#collection-vs-collections)
  - [1.List Interface](#1list-interface)
    - [ArrayList ☆](#arraylist-)
      - [扩容机制](#扩容机制)
      - [ArrayList线程安全吗 怎么变为线程安全](#arraylist线程安全吗-怎么变为线程安全)
    - [Vector](#vector)
    - [ArrayList vs. Vector](#arraylist-vs-vector)
    - [ArrayList vs. Array (built-in) ☆](#arraylist-vs-array-built-in-)
    - [CopyOnWriteArrayList ☆](#copyonwritearraylist-)
      - [如何保证线程安全？](#如何保证线程安全)
      - [为什么写要上锁，读不上锁？](#为什么写要上锁读不上锁)
      - [为什么写时复制？](#为什么写时复制)
    - [LinkedList](#linkedlist)
    - [ArrayList vs. LinkedList ☆☆](#arraylist-vs-linkedlist-)
  - [2.Set Interface](#2set-interface)
    - [HashSet vs. LinkedHashSet vs. TreeSet](#hashset-vs-linkedhashset-vs-treeset)
    - [HashSet](#hashset)
    - [LinkedHashSet](#linkedhashset)
    - [TreeSet](#treeset)
  - [3.Queue Interface](#3queue-interface)
    - [Queue vs. Deque](#queue-vs-deque)
    - [PriorityQueue](#priorityqueue)
    - [BlockingQueue](#blockingqueue)
  - [4.Map Interface](#4map-interface)
    - [HashMap ☆](#hashmap-)
      - [HashMap的底层实现](#hashmap的底层实现)
      - [HashMap的put流程](#hashmap的put流程)
        - [详解扩容机制 / 以及rehash过程](#详解扩容机制--以及rehash过程)
        - [详解HashMap中的hashCode() + equals()](#详解hashmap中的hashcode--equals)
      - [get()流程 / 查找流程](#get流程--查找流程)
      - [一般如何解决Hash冲突？HashMap中是如何解决的？](#一般如何解决hash冲突hashmap中是如何解决的)
      - [HashMap多线程下有啥问题](#hashmap多线程下有啥问题)
      - [遍历HashMap](#遍历hashmap)
    - [TreeMap](#treemap)
    - [LinkedHashMap](#linkedhashmap)
    - [HashMap vs. Hashtable](#hashmap-vs-hashtable)
    - [HashMap vs. HashSet](#hashmap-vs-hashset)
    - [ConcurrentHashMap是如何实现的 ☆](#concurrenthashmap是如何实现的-)
      - [jdk1.7 数组 + 链表](#jdk17-数组--链表)
      - [jdk1.8 数组 + 链表/红黑树](#jdk18-数组--链表红黑树)
        - [为什么同时需要CAS和synchronized](#为什么同时需要cas和synchronized)
        - [get是否需要加锁 / volatile起到的作用](#get是否需要加锁--volatile起到的作用)
      - [ConcurrentHashMap 1.7 vs. 1.8](#concurrenthashmap-17-vs-18)
  - [java.utils.Collections](#javautilscollections)

TODO
* 说一下红黑树的特点
* 判断一个集合类是否为线程安全的机制是什么？[fail-fast源码](https://javabetter.cn/sidebar/sanfene/collection.html#_5-%E5%BF%AB%E9%80%9F%E5%A4%B1%E8%B4%A5-fail-fast-%E5%92%8C%E5%AE%89%E5%85%A8%E5%A4%B1%E8%B4%A5-fail-safe-%E4%BA%86%E8%A7%A3%E5%90%97)

## 概括

### java的集合类有哪些 ☆

Java集合类主要由Collection和Map这两个接口派生而出，其中Collection又派生出三个子接口，分别是List, Set, Queue，所有Java集合都是List, Set, Queue, Map这四个接口的实现类。

![picture 1](../images/8c59864034ef6c2020329cd184d752e513b468797f89cba43cd3ed57ce597f0b.png)
> LinkedList实现了List和Queue接口，LinkedList底层是双向链表，可作为栈、单向队列和双向队列来使用（但可以使用idx增删中间元素，有点儿难受）; ArrayDeque没实现List，仅实现了Deque，纯粹

我用过的一些Collection：

* `ArrayList`: 动态数组，实现了List接口，支持动态增长。
* `LinkedList`: 双向链表，也实现了List接口，支持快速的插入和删除操作。
* `ArrayDeque`: 双端队列，实现了Queue接口，可以充当栈，FIFO队列和双端队列结构。
* `HashMap`: 基于哈希表的Map实现，存储键值对，通过键快速查找值。
* `HashSet`: 基于HashMap实现的Set集合，用于存储唯一元素。
* `TreeMap`: 基于红黑树实现的有序Map集合，可以按照键的顺序进行排序。
* `LinkedHashMap`: 基于哈希表和双向链表实现的Map集合，保持插入顺序或访问顺序。
* `PriorityQueue`: 优先级队列，可以按照比较器或元素的自然顺序进行排序。可用来充当堆数据结构。

### 哪些是线程安全/不安全的 ☆

* java.util包中线程安全
  * Vector：古老的List实现类，内部方法基本通过synchronized修饰，现推荐使用ArrayList
  * HashTable：古老的哈希表实现，同样适用synchronized修饰内部方法，现推荐使用HashMap
  * `Collections.synchronizedList`, `Collections.synchronizedSet`, `Collections.synchronizedMap`: 这些**方法**可以将非线程安全的集合包装成线程安全的集合
    * 底层本质是：eg synchronizedList类将List的很多方法加了synchronized，但**务必注意在迭代遍历List时必须手动添加synchronized(因为listIterator方法并没有添加synchronized)**
* java.util.concurrent包中线程安全
  * 并发list: `CopyOnWriteArrayList`
  * 并发set: `CopyOnWriteArraySet`
  * 并发map: `ConcurrentHashMap`
* 线程不安全
  * ArrayList, LinkedList, HashSet, HashMap
  * TreeSet, TreeMap: 虽然它们是有序集合，但也是线程不安全的

### 如何选择

先判断是单列还是键值对：

* 单列集合 -> `Collection`接口实现类
  * 允许重复元素 -> `List`接口实现类
    * 增删多 -> `LinkedList` (双向链表) `错误的！不建议;`
    * 改查多 -> `ArrayList` (可变数组)
  * 不允许重复元素 -> `Set`接口实现类
    * 无序数据 -> `HashSet` (HashMap(数组+链表/红黑树))
    * 有序数据 -> `TreeSet` (红黑树)
    * 维护插入顺序(插入取出顺序一致) -> `LinkedHashSet` (数组+双向链表)
* 键值对集合 -> `Map`接口实现子类
  * 无序key -> `HashMap`
    * 保证线程安全 -> `ConcurrentHashMap`
  * 有序key -> `TreeMap`
  * 维护插入顺序 -> `LinkedHashMap`
  * 读取文件 -> `Properties`

## 前置知识

### enhanced for

```java
for (ElementTyle element : collection) {
}
```

* enhanced for (for-each)提供了一种简洁的方式来遍历**实现了Iterable接口的集合**和**数组**。
  * 实现了iterable接口的集合：**即实现了Collection接口的所有类，包括List接口下和Set接口下，注意没有Map接口**
  * 数组没有实现Iterable接口，因此不能直接获取Iterator对象来遍历;java额外特别支持了数组。
* **底层原理**：编译器在编译时将enhanced for转换成了基于iterator的常规for；

```java
int[] array = {1, 2, 3, 4, 5};
for (int element : array)
  sout(element);
// 等价于：
for (int i=0; i<array.length; ++i) 
  sout(array[i]);

List<String> list = Arrays.asList("A", "B", "C");
for (String element : list) {
    // 使用element
}
// 等价于
for (Iterator<String> iterator = list.iterator(); iterator.hasNext(); ) {
  String element = iterator.next();
} // list.iterator()指向首元素之前，第一次调用.next()才返回列表首元素
```

## 接口

### Iterable Interface

methods

* .iterator()
  * 返回迭代器Iterator类型, 指向容器首元素之前
    * > Iterable和Iterator是两个interface
  * Iterator接口中有hasNext(), next()
    * next(): 移动至下一元素 & 将该元素返回
* forEach()

```java
Iterator iterator = ls.iterator();
while (iterator.hasNext()) 
sout(iterator.next()); // .next()会自动将iterator后移
```

### Collection Interface

* collection接口可以存放多个object元素


#### Collection vs. Collections

* Collection是一个**interface**，是所有集合类的基础接口。定义了一组通用的操作和方法，如add(), remove(), size(), isEmpty(), contains(), iterator(), clear(), equals(); Collection接口下有很多子接口，如List，Set和Queue。
  * 即Collection下的所有实现类都有这些方法，尽管实现不同，但我们无需考虑; 比如ArrayList和HashSet都用add()，而HashMap不属于Collection实现类，是Map的实现类，他们喜欢put()
* Collections是一个**工具类**，提供了一系列**静态方法**，用于对集合进行操作，比如排序、查找、翻转等。

## 1.List Interface

* List的常见实现类有ArrayList, LinkedList和Vector
* List支持(顺序)索引: .get(), 不一定支持随机索引哦

methods (Collection和Iterable的方法应该都有)

* **get**(idx)
* getFirst(), getLast()
* **indexOf**()
* **set**(idx, element)
<!-- * subList(start, end) -->
* **sort**()

> List相比于Collection多了很多下标相关的操作，Set是不支持下标的

List的三种遍历方式（即List的所有实现子类均可用: e.g.,Vector, LinkedList...）：

```java
// 遍历的三种方式
for (int i=0; i<arrayList.size(); ++i)
    System.out.println(arrayList.get(i)); // Object -> int

for (Iterator iter=arrayList.iterator(); iter.hasNext();)
    System.out.println(iter.next());

// for-range: based on the iterator
for (Object obj : arrayList)
    System.out.println(obj);
```

### ArrayList ☆

```java
ArrayList<E> objectName = new ArrayList<>(); // 初始化
ArrayList<String> strList = new ArrayList<>();
```

precautions:

* 如果不指定泛型`E`，默认是Object类型，此时允许放入不同类型的数据
* 如果指定了E的类型，比如String，就只能放String了(**编译时检查类型**)
* 可以存null，但无意义
<!-- * ArrayList是线程不安全的（无synchronized） -->

#### 扩容机制

底层分析jdk8.0：

* ArrayList的本质是**Object[] elementData**，当当前元素个数达到数组容量上限就会触发扩容操作;
* 如果使用无参ctor**实例化ArrayList，elementData容量为0, `ie {}`**。第一次add时，扩容elementData为 **==10==**，再次扩容按照**1.5**翻倍（即添加50%）
  * 扩容步骤：
    * 计算新容量：新容量为1.5倍
    * 创建新的更大的数组
    * 逐个复制元素：通过`Arrays.copyOf(elementData, newCapacity);`
    * **更新引用**：将ArrayList内部指向原数组的引用指向新数组
* 如果使用**指定大小n的ctor实例化，容量开始为n**，然后直接按照**1.5倍**扩容；
  * 频繁扩容可能影响性能，故而在初始化ArrayList时可以预分配足够大的容量，避免频繁触发扩容

```java
private static final Object[] DEFAULTCAPACITY_EMPTY_ELEMENTDATA = {};
public ArrayList() {
    this.elementData = DEFAULTCAPACITY_EMPTY_ELEMENTDATA;
}

// 扩容机制核心 -> 扩容为原来的1.5倍
int newCapacity = oldCapacity + (oldCapacity >> 1); // 右移1位 即 缩小一倍；移位要比普通运算符快很多
```

Q: 为何是1.5倍？
A: 可充分利用移位操作，运算速度快

Q: ArrayList会自动缩容吗？
A: 源码中**没有自动缩容机制(已证实)**，remove()和clear()都不会使得elementData的长度减小，但会**把相应的元素置为null**，便于GC回收。

#### ArrayList线程安全吗 怎么变为线程安全

* 通过方法Collections.synchronizedList
* 直接使用线程安全类`CopyOnWriteArrayList`或者`Vector`(旧)

线程不安全可能导致：下标越界异常、size与我们add的数量不一致（具体没啥好说的吧，慢慢推得）

### Vector

* Vector是List的古老实现类，了解即可
  * 如果需要线程安全，通常使用Collections.synchronizedList()是ArrayList同步，来替代Vector
* Vector的底层也是Object[] elementData;
* Vector是线程安全的 (有Synchronized)，适用于多线程环境
* 如果使用无参ctor实例化，**实例化时就会预分配elementData为10**，之后add不够用了再按照 **==2==** 倍扩容
* 如果使用指定大小n的ctor实例化，容量开始为n，然后直接按照**2**倍扩容
  * 扩容底层也采用`Arrays.copyOf(elementData, newCapacity);`

### ArrayList vs. Vector

* ArrayList的无参构造器中并不会**预分配**10个空间，是在add中才会new；而Vector的无参ctor上来就new 10个空间
* ArrayList的**扩容**频率更为频繁，但内存利用率也更高
* ArrayList更高效，因为不考虑**线程安全**
<!-- * ArrayList是新类(jdk1.2), Vector(jdk1.0), 如无特别需要，一般采用ArrayList -->

### ArrayList vs. Array (built-in) ☆

* ArrayList内部基于**动态数组**，Array是静态数组，前者可动态扩容，后者创建后长度就固定了
* ArrayList**只能存对象**/引用(**可以存数组int[]**)，不可存基本数据类型，Array均可
* 前者创建时无需**指定大小**，后者必须
* 前者可以使用泛型确保类型安全（**编译时类型检查**），后者不行
  * 如果上线后运行时报错，那就很麻烦了，所以编译时报错是好很多的
* 功能方面：Array只有length属性，ArrayList提供了**增删等api**，如add(),remove(),size()等
* 维度：Array可以多维，ArrayList只可以一维

### CopyOnWriteArrayList ☆

> 读写分离思想：将数据库的读操作和写操作分开处理，以减轻数据库的负载，提高并发处理能力和响应速度。通常一个主库处理写，1-n个从库处理读。适合读多写少的场景

#### 如何保证线程安全？

> CopyOnWriteArrayList底层也是用一个数组保存数据

* 在**写操作(增删改)时，首先通过ReentrantLock加锁**，然后将当前**数组拷贝一份(copyOf)**，然后进行元素修改，然后将CopyOnWriteArrayList内部数组的引用指向新数组的地址，然后unlock
* 底层的数组使用**volatile**修饰，保证多线程环境中，任何线程对数组引用的更新都能被其他线程立即看到，而不会读取到旧的数组引用（可见性）
  * `private transient volatile Object[] array;`
* 在读操作时并不需要加锁

#### 为什么写要上锁，读不上锁？

* 如果写不上锁，那么多个线程可以一块改，那数据可能不一致
* 之所以读不上锁，是因为**读操作总是基于一个稳定的快照进行**，首先是允许大家一起读的(也即读读不互斥)；此外，即使数组写时，读操作依然可以继续访问旧的数据，不受影响

#### 为什么写时复制？

* 写时复制的好处是，我们可以对容器进行并发的读，而无需加锁，因为容器是一个稳定的快照。
* 不然如果你写时不复制，而是直接锁住原数组进行修改，那读会发生什么？
  * 如果读不上锁，可能读到不一致的数据，因为读时，数组的内容可能被写操作改变，所以你可能读到部分修改后的数据和部分未修改的数据
  * 解决方案是读也上锁，那读就得阻塞了，那读的效率就大大降低了

### LinkedList

![picture 3](../images/91feffe6cb7766844507641c9272aafe89fe6c02b47d439e869589b911c5bd9d.png)  

```java
private static class Node<E> {
    E item;
    Node<E> next;
    Node<E> prev;

    Node(Node<E> prev, E element, Node<E> next) {
        this.item = element;
        this.next = next;
        this.prev = prev;
    }
}
```

* LinkedList底层维护了一个**双向链表**, 一个LinkedList维护两个属性，first指向首结点，**last指向尾结点**(所以尾部增删很快) (当然还有个size属性)
  * 双向链表不同于双向循环链表链表
* 每个结点是一个Node对象，里面维护了prev, next, item三个属性
* 未实现`RandomAccess`标记接口，因为底层内存地址不连续，不支持随机访问
<!-- * 增删比较快（因为没涉及到数组，扩容等;），改查比较慢 (其实有点问题的，增删头尾确实快，中间的话需要先O(n)遍历到为止，所以增删也并不快) -->
<!-- * LinkedList线程不安全 -->
<!-- * Node是LinkedList的static内部类 -->

---

* implements List: 说明是一个列表，支持增删改查（可通过idx访问: get(idx)，只不过其底层是不支持随机访问的
* **implements Deque**: 说明具有双端队列的特性，方便实现stack和queue等数据结构
  * 但一般使用ArrayDeque实现stack和queue，因为其基于动态数组实现，性能更好，更省内存（二者都实现了Deque），而且最重要的是，LinkedList暴露了通过中间idx修改容器的能力，这并不符合栈和队列的特性


<!-- 源码：

* new LinkedList()啥也没干，就初始化了first, last, size..
* .add()通过尾插法，new一个Node（赋值item, next, prev），添加到双向链表中
* .remove()默认删除首元素 unlinkFirst() -->

### ArrayList vs. LinkedList ☆☆

* **底层数据结构不同**：ArrayList底层是**动态数组**Object[], LinkedList底层是**双向链表**（jdk1.6之前是循环链表）
* **随机访问**：前者支持**随机访问**，即O(1)访问，实现了`RandomAccess`接口，后者不可，O(n)
* **空间占用**：ArrayList占用连续内存空间，但可能需要在结尾预留一定的容量空间，相对占用较大空间，LinkedList无需连续，只需要额外空间存储前后节点的引用，**相对较小**
* **插入和删除的效率不同**：ArrayList在**尾部**的插入和删除操作效率较高(O(1))，但在中间或开头的增删需要移动元素，平均O(n)；LinkedList在任意位置的**纯**插入和删除操作效率都比较高，只需调整节点自身的双向指针即可（纯删除操作是O(1)没问题），但找到节点需要O(n)事件，所以除了删除**头尾**节点，其它节点的增删是O(n)的，故而效率也不高
<!-- * **理论上使用场景**：ArrayList适合**频繁随机访问和==中间==或尾部的增删**操作，LinkedList适合频繁的 **头部的增删操作和无需随机**访问的场景 -->
* **增删实验**
  * 我实际的**实验**：头插、中间插和尾插，分别插入1w,10w,100w条，测试结果：
    * 头插：AL需要移动后续的元素，LL直接插，LL更快毋庸置疑，快了上百倍吧
    * 中间插：二位都是O(n)，结果显示**AL大概比LL快了10倍**，**100w数据的时候大概快了100倍**
      * **==一般大家会说中间节点的增删LL更快，错误想法==**
      * 可能是AL这种连续内存空间更有利于CPU缓存，以及内存分配啥的
    * 尾插：二维都是O(1)，实验结果也差不多；~~LL要new对象，效率比AL低一点点~~
  * 故：除了 **频繁头部增删操作 && 无需随机访问** 可能可以考虑LinkedList，其他请用ArrayList；或者说，请遵从LL作者意愿，使用AL；



![picture 2](../images/cefa318ff6accc63831b50a907310f2d478715e193c940a287ab61147bc6f772.png){width=70%}

## 2.Set Interface

* Set就是不可重复,HashSet是典型,TreeSet内部根据红黑树自动排序,LinkedHashSet会保证quchu顺序和插入顺序一致
  * **不可重复性**：指添加的元素按照equals()判断时，返回false，需要同时重写hashCode()和equals()方法
    * 为什么还需要override hashCode()? [github/haooxia](https://github.com/haooxia/interview/blob/main/java/java.md#hashcode)
* Set不支持索引,所以set接口并不能使用索引遍历，只剩下两种遍历方式：迭代器 和 增强for
<!-- * 可以add(null) -->


### HashSet vs. LinkedHashSet vs. TreeSet

> HashSet是不会自动排序的

* 都线程不安全
* 主要区别于底层数据结构：HashSet底层是数组+单链表/红黑树(基于HashMap实现)；LinkedHashSet底层是**双向链表+哈希表**，元素的插入取出符合FIFO；TreeSet底层是**红黑树**，元素有序（可自然排序or定制排序
* HashSet用于无需保证元素插入和取出顺序的场景，LinkedHashSet用于需要保证FIFO的场景，TreeSet用于支持元素自定义排序的场景
  * TreeSet的add时间复杂度是`O(logn)`, HashSet的add是`O(1)`

### HashSet

HashSet的底层是HashMap，参考即可

### LinkedHashSet

* LinkedHashSet底层维护的是数组+**双向**链表
* 会使用双向链表来维护元素顺序，所以能够确保**遍历顺序和插入顺序一致**
* 源码暂略

### TreeSet

* 相比于HashSet最大特点：可以排序
* TreeSet默认构造器的元素按照**自然顺序**（元素实现的Comparable接口中的compareTo默认方法规则，比如String就是字母排序，Integer是数值大小）排序
* TreeSet构造器**可以传入一个Comparator匿名对象来自定义排序规则**，实际上是将其赋给了底层的TreeMap的comparator属性。
* 那为什么TreeSet会自动排序呢?
  * TreeSet基于TreeMap使用**红黑树**数据结构来存储键值对，这种树结构保证了插入、删除、查找操作的时间复杂度为O(logn)，并且自动维护元素的排序。

## 3.Queue Interface

### Queue vs. Deque

* 二者都是interface奥
* Queue是单端队列，Deque是双端队列

### PriorityQueue

* 和Queue区别于：总是优先级最高的元素先出队，不是直接FIFO了
* 底层数据结构是**堆**，底层是可变长数组，默认是小顶堆
  * 创建大根堆: `Queue<Integer> heap = new PriorityQueue<>(Collections.reverseOrder());`
* 通过堆元素的上浮和下沉，插入和删除堆顶元素为O(logn)
* 非线程安全

### BlockingQueue

* BlockingQueue（阻塞队列）是一个interface，继承自Queue
* 支持当队列没有元素时一直阻塞，直到有元素；还支持如果队列已满，一直等到队列可以放入新元素时再放入。常用于生产者-消费者模型
![picture 2](../images/69fa290502aef18c786779b48643a7cbfdcfd53ea05766043f121d5828f12d99.png)  

## 4.Map Interface

* Map保存key-value映射关系的数据；Set中底层也是Map，但只用了K，V使用的是常量`PRESENT`
* K不可重复; 新KV会替换旧KV (K相同时); V可重复
* 可以通过key找到value: `.get(key)`

### HashMap ☆

<!-- ![picture 4](../images/95c10dad5f0e4e95866e3424bb9c6faa261ba9706d4cfca307ed391c100a9eff.png)   -->

![picture 11](../images/ffa035437a9b673218ec23ed503041b584f37136e35fe2853e26aeef789217b6.png)  


* HashMap将数据以键值对的形式存储(存在内部的`HashMap$Node`对象中)

#### HashMap的底层实现

* jdk7中HashMap是`数组+链表`，即拉链法; **HashMap通过哈希算法将元素的键key映射到数组的槽位Bucket中**
  * 哈希冲突时(**多个不同key映射到同一bucket**)，会在冲突位置bucket形成链表，将新增元素加入到链表**头部**，问题是：冲突过多时链表会特别长，导致查找复杂度逐渐退化为O(n)
    * 补充：redis的Dict也是拉链法，也是头插，然后会使用渐进式rehash避免扩容时进程长时间阻塞在rehash过程
* jdk8首先头插改为**尾插**，jdk8引入**红黑树**，链表长度 **>=8**时，会将链表转换为红黑树(Treeify)，以提高查找性能 (**从O(n)降为O(logn)**)。长度 **<=6**时重新从红黑树转换回链表(**untreeify**)

**Q: 为什么>=8树化，<=6时重新变回链表？**
  * 源码注释中所述：根据泊松分布，在负载因子是0.75时，一个hash桶中元素个数为8的概率小于百万分之一，所以7作为分水岭，=7时不转换，>=8时树化，<=6时反树化。

**Q: 为什么引入红黑树，而非其他树？**
  * 二叉搜索树BST(左 < root < 右)，极端情况下会退化为链表(O(n))：比如插入有序数据，BST一边可能无限长 -> 平衡二叉树
  * 平衡二叉树AVL(左右子树高差不超过1，要求严格，追求**绝对平衡**): 旋转操作频繁：在添加元素时需要进行**左旋、右旋**操作维持根节点左右两端的平衡，复杂度和开销很高。
  * 红黑树：**不追求绝对的平衡**，是一种**弱平衡**，整个树的**最长路径不会超过最短路径的2倍**，不需要像平衡树那样频繁调整
    * 即红黑树相比于平衡二叉树：牺牲了一部分查找性能，换取了维持树平衡状态的成本；
  * BTree和B+Tree是m叉的，别乱想了孩子...

**Q: 为什么1.7的头插在1.8要改为尾插？** / **HashMap多线程下有什么问题？**
  * 此即**HashMap多线程操作导致死循环问题**：1.7在多线程扩容时可能死循环，是因为当哈希表进行扩容时，多个线程同时对链表进行操作，**头插法**可能导致**链表中的节点指向错误的位置**，从而形成一个**环形链表**，进而使得查询元素的操作陷入死循环无法结束; 尾插即可解决该问题。（有点抽象，但细节至此，够用了）
  * 至于尾插是否比头插效率低？其实头插和尾插都需要遍历链表然后判等，所以感觉是一样的

#### HashMap的put流程

![picture 7](../images/ff2a59570488e94fa9175ce524dc73385e8f570a58d8c1a61ae6084a58cf3e18.png){width=70%}

**计算hashCode -> 判断table数组是否为空 -> 计算数组索引 -> 判断该位置是否为空 -> 判断是否相等 -> 遍历链表/红黑树 -> 更新/插入 -> 是否需要树化 -> 是否需要扩容**

调用`put()`添加键值对时，会按照以下流程执行(jdk1.8)：

1. 根据key计算hashCode：`(h = key.hashCode()) ^ (h >>> 16)` (^异或)
2. 如果table为空，数组首次扩容：`resize()` (初始大小为 **==16==** 的`Node<K,V>[] table`数组，加载因子为0.75)
3. 根据hashCode计算在数组中的索引：`hash & (n-1)`
4. 检查索引处bucket是否存放键值对数据(是否为空)，**如果为空**则插入一个新的Entry对象来存储键值对
5. **如果非空**，检查该位置第一个键值对的hashCode和key是否与要添加的相同；**如果相同**，则表示找到相同的key，直接替换进行更新
6. **如果hashcode和key不同**，则需要判断集合的结构是链表还是红黑树，然后进行遍历找到是否有相同的key
   1. 如果是链表结构，==**从头开始遍历链表逐个比较key的hashCode和equals()**== 直到找到相同key或到达**链表末尾**，如找到相同key，更新，如没找到相同key就**尾插**
   2. 如果是红黑树结构，**在红黑树中使用hashCode和equals()** 方法进行查找，直到找到相同key或到达末尾，如找到相同key，更新value，没找到就插入
7. 对于链表结构，元素put后需要**检查链表长度是否超过阈值**，ji如果 ==**链表长度>=8且HashMap的数组长度>=64**==，**树化**
8. 最后，**检查负载因子是否超过阈值(0.75)**，如果**键值对entry的数量 / ==数组bucket长度== > 0.75**，扩容
9.  **扩容**：搞一个**2倍**长的新数组，将旧数组中所有键值对~~重新计算hashCode并按~~put到新数组对应位置，最后**更新HashMap的数组引用**
    1. 由于2倍扩容，无需重新计算hashcode

---

**Q: HashMap的hash()是如何设计的 / 扰动函数**

```java
static final int hash(Object key) {
    int h;
    // key的hashCode和key的hashCode右移16位做异或运算
    return (key == null) ? 0 : (h = key.hashCode()) ^ (h >>> 16);
}
```

* 先拿到key的hashcode(32位int)，然后**让hashcode的高16位和低16位进行异或操作**
* 原因：虽然hashcode是32位，但HashMap的长度n一般较小，我们通过`hash % n`来获得索引位置，**与&操作就只能捕获到32位hashcode的低位的特征**，故而将hashcode右移16位，然后亦或高低位，这就**同时考虑到了低位和高维的特征**，**特征越多，哈希碰撞的概率就越低**

**Q: 详解一下树化和反树化**

* **树化机制**：`if (len(this linked list) = TREEIFY_THRESHOLD(8) && len(table) >= MIN_TREEIFY_CAPACITY(64)`才会将该链表(this linked list)树化为红黑树；
* 如果链表长度=8但table长度不够64，会先插入到链表，然后直到达到0.75扩容条件，resize()将数组扩容两倍
  * 反树化的条件：红黑树节点<=6时


**Q: HashMap为什么按2倍扩容**
A: HashMap的容量设置为2的幂次方，是为了快速定位元素所在的数组下标
如果len是2的n次幂，则可利用&加速运算：
`hash % len == hash & (len - 1)`

##### 详解扩容机制 / 以及rehash过程
A: 如果 **所有entry元素个数 / bucket数组长度 > 0.75**，则触发扩容：

1. 搞一个2倍长度的bucket数组
2. 将旧哈希表中的数据逐个put新哈希表中

**由于我们按照2次幂扩容，库容后元素要么在原位置，要么在原位置再移动2次幂的位置**

![picture 12](../images/3a30ab901622e6569637ff111328b24b2ab35a4d5d0d8986ba42848df6ad3fc2.png)  

* 因此，在扩充HashMap时，**无需重新计算hash**，只需看看原来的hash值新增的那个bit是1还是0，0则索引不变，1则新索引=旧索引+旧数组容量；
* 省去了计算hashCode的时间，而且新增的1bit是0还是1可以认为是随机的(因为原本的hashcode每一位都是随机的)，因此resize时，将之前冲突的点均匀地分散到了新的bucket中。

![picture 13](../images/ff1eb04ffe4cc94bafa17ff25aefe2dc42396304b5c100a0a649f3160ca96869.png){width=70%}

##### 详解HashMap中的hashCode() + equals()

**Q: 为何不能只用equals判等？**
A: 只用equals()判等是没问题的，但低效。

* 一方面HashMap通过hashcode确定bucket存储位置，另一方面HashMap在equals()之前利用hashcode来提高查找效率 **（虽然hashcode相同不能直接推出两个key相同，但hashcode不同两个key一定不同**
  * 即先比较hashcode可以快速排除大多数不匹配的情况，很搞笑
* java规范要求：
  * 如果两个对象的equals()相同，则其hashCode()必须相同；
  * 如果hachCode()相同，equals()可以不同，因为可能碰撞

**Q: 只重写equals()没重写hashCode()，put的时候会怎么样？**

A: HashMap会使用Key的hashCode和equals去进行判等，我们get或put都会自动用到，如果你只重写了equals没重写hashcode，**导致equals()相同的两个对象hashCode不同**，**这两个对象本应该放入一个bucket，被放到了两个bucket**，get的时候就找不到了


#### get()流程 / 查找流程

easy

1. 计算hashcode
2. 获取数组下标
3. 判断该节点是否和key匹配，比较hashcode和equals()，是则返回
4. 去查找红黑树/链表，比较hashcode和equals()

#### 一般如何解决Hash冲突？HashMap中是如何解决的？

**Q: 什么是哈希冲突？**
A: 狭义上/一般认为 哈希冲突是指32位hashcode相同(我觉得概率挺低的，有些苛刻)，我认为广义上/实际上 哈希冲突大部分情况是不同的hashcode被映射到同一bucket(原因是一般输入空间是大于输出空间的)

解决Hash冲突的算法：

> 前两种每个槽只放一个元素，拉链法放多个

* **开放地址法**：发生冲突时，在哈希表中寻找下一个空槽位来存储元素，具体实现包括：
  * 线性探测：依次检查下一个槽位，直至找到空槽
  * 二次探测：使用平方函数的增量来探测下一个槽位，`1^2, -1^2, 2^2, -2^2...`
  * 伪随机探测：加个随机数
* **再散列法/再哈希法** (rehashing): 用多个哈希函数。当发生冲突时，依次尝试不同的哈希函数来计算新的地址，直到找到一个空槽位。可以减少聚集现象，但计算复杂
* **拉链法**：每个数组元素bucket上都有一个链表结构,bucket中存头指针，元素存入链表；HashMap

#### HashMap多线程下有啥问题

* jdk1.7HashMap采用头插法插入元素，在多线程的环境下，扩容的时候可能导致出现**环形链表**，出现死循环
  * jdk1.8HashMap改为了尾插，解决了死循环问题
* 多线程下put可能**丢失元素**，如果俩线程一块计算得到同一个索引位置可以放入元素，那么先放入的数据会被后放入的数据覆盖，导致元素丢失(jdk1.8依然存在)

**Q: 那HashMap如何保证线程安全？**

* 可以使用Collections.synchronizedMap同步加锁的方式，还可以使用HashTable，但是同步的方式显然性能不达标，而ConurrentHashMap更适合高并发场景使用
* ConcurrentHashmap在JDK1.7和1.8的版本改动比较大，1.7使用Segment+HashEntry分段锁的方式实现，1.8则抛弃了Segment，改为使用CAS+synchronized+Node实现，同样也加入了红黑树，避免链表过长导致性能的问题。

#### 遍历HashMap

遍历（6种）

* entrySet(): 获取所有k-v
* keySet(): 获取所有key
* values(): 获取所有value
* 基于enhanced-for或者iterator(basic-for) 各有三种
* 基于**entrySet最高效**，因为使用keySet会多一次哈希查找操作

<!-- ### Properties

* Properties继承自HashTable
* 常用于从**配置文件.properties**加载数据到Properties类对象，进行读取和修改。 -->

### TreeMap

* 底层参考TreeSet，因为TreeSet是基于TreeMap的，只不过Value填充的是恒定的PRESENT
* 可以排序，默认ctor按照自然顺序，可以传入一个Comparator匿名对象
* TreeMap底层是Entry

### LinkedHashMap

继承自HashMap，内部维护了一个双向链表，保留了键值对的插入顺序（即我们使用**双向链表 + HashMap** [手撕LRU的实现](https://leetcode.cn/problems/lru-cache/?envType=study-plan-v2&envId=top-interview-150)

### HashMap vs. Hashtable

* 前者非**线程安全**，后者安全
* 底层**数据结构**: HashMap通过数组+链表/红黑树；Hashtable 数组+链表
* **哈希函数不同**：HashMap对哈希值进行了高位和低位的混合扰动处理以减少冲突，而Hashtable直接使用键的hashCode()值
* HashMap默认初始化大小为**16**，然后每次扩充为**2倍**；Hashtable默认初始化大小为**11**，然后每次扩充为**2n+1**；
* 如果指定初始化大小k，HashMap会将其扩充为2的幂次方大小（向上取整，即HashMap的大小始终是2的幂次方；Hashtable会直接使用k
* 前者可以使用null作为key和value，后者不可

### HashMap vs. HashSet

![picture 3](../images/43fd1797d3e44128e71d720e685815aab8eb75a8ce38ee204a129c2705045c75.png)

### ConcurrentHashMap是如何实现的 ☆

#### jdk1.7 数组 + 链表

> Segment数组 + HashEntry数组 + 链表

![picture 10](../images/059978a656395d4bc1a5310f20814d7f0c6a37a6723f63daf81bc633fb3c4a4d.png){width=80%}

* ConcurrentHashMap将整个哈希表分为多个小的哈希表，称作Segment[]数组，每个Segment包含一个HashEntry[]数组，每个HashEntry是一个链表，来村键值对数据。（即把原来的数组分成了多个segment
  * 每个**segment是一种可重入锁**，当一个thread访问其中一个数据段上锁时，其他segment的数据可以并行访问
    * `Segment<K,V> extends ReentrantLock`，segment继承自ReentrantLock
  * segment数组可称作所数组，无他，就是扮演锁的角色
  * Segment数组默认大小为16(不能扩容)，即默认可以同时支持16个线程并发写。每个Segment守护一个HashEntry数组

> HashTable会用一把锁锁住整个哈希表，效率很低 -> 1.7分段锁好点了 -> 1.8每个Node一把锁并发度更好了

#### jdk1.8 数组 + 链表/红黑树

> **Node数组** + 链表/红黑树 (和jdk1.8 HashMap类似)

![picture 6](../images/e1b7477f3031601bdb1ad31f774e5f1faa86bd1bbee8284d19ea9c391490d7da.png)

jdk1.8几乎完全重写了ConcurrentHashMap，取消了Segment分段锁，采用 ==**CAS+synchronized+volatile**== 来保证并发。锁粒度更细，synchronized只锁定当前链表或红黑树的首节点，即锁定到单个bucket级别，每个Node都可以并行操作，大幅度提升效率（1.7是锁到一个segment，即多个bucket

##### 为什么同时需要CAS和synchronized

ConcurrentHashMap使用这俩手段来保证线程安全，使用哪个主要是**根据锁竞争程度**来判断（我们知道竞争激烈的场景适合悲观锁，因为乐观锁在激烈场景下会爆炸）

* 在put中，如果计算出的hash槽没有存放元素，就直接通过CAS设置值
  * 这是因为我们的hash值经过各种扰动之后，造成**hash碰撞的概率是比较低的**，所以比较适合乐观锁
* 而发生hash碰撞时说明哈希表的容量不太够用，或者有大量的线程访问，所以**此时线程竞争是很激烈的**，故而此时采用悲观锁synchronized来处理hash碰撞；上锁后，遍历bucket中的数据，并替换或新增节点到该bucket，再判断是否需要转为红黑树

> 即就能用乐观锁的话，效率还是蛮高的，竞争激烈了就悲观锁

##### get是否需要加锁 / volatile起到的作用

注意：ConcurrentHashMap的get操作是无需加锁的，因为其该容器对Node的value字段使用了==volatile==修饰，这意味着一个线程修改了某个Node的value时，其他线程可以立马看见这个变化（volatile保证可见性）；此外volatile可以禁止指令重排序，确保了对Node的读取和写入操作不会被重排序

#### ConcurrentHashMap 1.7 vs. 1.8

* 结构不同/hash碰撞解决方法不同：1.7采用拉链法，1.8采用拉链法+红黑树
* 线程安全实现方式：1.7采用Segment分段锁，1.8采用`Node+CAS+synchronized`，锁粒度更细，synchronized只锁定当前链表或红黑二叉树的首节点
* **并发度**：1.7的最大并发度是Segment的个数（默认16），1.8最大并发度是Node数组的大小，会大的多

## java.utils.Collections

* 用来操作List, Set, Map等集合的工具类，提供一系列static方法对**集合(List, Set, Map)**进行排序、查找和修改等操作

static method: (默认都按**自然顺序**)

* `Collections.sort(List<T> list)`
  * 底层都是调用`Arrays.sort()`，可能需要先`.toArray()`
* `Collections.sort(List<T> list, Comparator<? super T> c)`
  * `<?>`: 支持**任意泛型**类型：即可以传入T或E任意泛型
  * `<? extents T>`: 支持T类及T类的子类孙子类，规定了泛型的上限
  * `<? super T>`: 支持T类及T类的父类爷类祖宗类，规定了父类的下限
* `Collections.reverse(List<?> list)`
* `Collections.binarySearch(list, key)`
* `Collections.swap(List<?> list, int i, int j)`
* `Collections.max(? extends T)`
* `Collections.max(? extends T, Comparator<? super T>)`
* `Collections.frequency(Collection<?> c, Object o)`
* `Collections.copy(List<? super T> dest, List<? super T> src)`

区分Colletions class和Arrays class

java.util.Arrays类能方便地操作**数组**，它提供的所有方法都是static的。

* Arrays.toString(int[])
* Arrays.sort()
* Arrays.binarySearch()
* Arrays.copyOf(): deep copy
  * =: shallow copy: 仅拷贝数组的引用，共用一份内存
* Arrays.equals()
