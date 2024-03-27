# KeyNotes for C++面向对象高级编程(下) 兼谈对象模型 侯捷

[toc]

## conversion function, (explicit)-one-argument ctor

![picture 0](../images/91fe2aeceb4c2446dff5284bd81de19516208050f21421ffb0a1bf2fd803a00e.png)  
conversion function可以把这种东西转换为别的东西

下面方向刚好相反
non-explicit-one-argument ctor可以把别的东西转换为这种东西
![picture 2](../images/cf81b54450f3462c50456c0bdb8424e5649affffebde2b60a019039467afab6f.png)  

explicit-one-argument ctor
关键字explicit用来声明单参数构造函数，禁止编译器使用该构造函数进行隐式转换(这里即int转换为fraction)，使用该构造函数创建对象时必须显示地调用该ctor。
![picture 3](../images/0717b642776a92d855f7b7e00b41c165586866e22ff344c6f4c235b9cbf2fd5b.png)  
> Fraction frac = 4; // 这也是隐式类型转换，被禁止；

## pointer-like classes: 智能指针, 迭代器

一个class设计出来之后像一个指针，因为你想要比指针再多做一些事情，所以也被叫做智能指针(is a class)。

![picture 4](../images/3428989dd7b9bb08186e316f6be90a9489a6f6e8ad89b63f48deb9b4f118e63c.png)  

迭代器像是一个指针，你也可以说它是一个智能指针。
除了要处理*, ->(e.g., share pointer)，还要处理++, --等操作。
![picture 6](../images/478432713176a993845b2c3d3fda16be63e8e27673aa6300704683301ae61084.png)  

## function-like classes: 仿函数, 函数对象

能接受一个operator()
标准库中有很多仿函数，都是一个小小的class，里面有重载operator()

![picture 7](../images/dd0e8f363d9408d45a64347ef3b94ed8671b25338d52ae3df482f1c5e0f74667.png)  

## template

### class template

![picture 9](../images/c5fb4287fb5de307a4a3fd4a0338c71305cb8434274bdd1407baaac34ae86463.png)  

### function template

![picture 10](../images/e6ab3e9ac5fd3bbbe6c869b80cdf5d505aa2c3e6f87bb26db04e45c3873513b7.png)  

### member template

类的构造函数常常出现member template
目的是使类的弹性更大

![picture 11](../images/4c0d6f2e81d59b5622f703882e8756ccb336af141cc1695064b5f873d15d9e82.png)  

![picture 12](../images/420d10822549b131336686d7648c88efb5ebd2e78ad53faaac675a18fc2a39a7.png)  
> 为何不能反过来？大概是因为父类中有的成员，子类p里面也有? (up-cast)

## specialization: 特化

泛化就是模板，随便指定类型
特化是泛化的反面；你作为设计者，对于某些独特类型需要做特殊处理。

### full specialization, 全特化

![picture 13](../images/56eda2a962a1903235055aad7556d86338bf232eacb748eecc027ede38278369.png)  

### partial specialization, 偏特化

个数上的偏特化 vs. 范围上的偏特化

![picture 14](../images/674f6ff8cc50204ba8968feb6aae63047f8a882460dc9ba21123e5bc81746628.png)  
> 下面的红字理解不对，正确理解是：当你以后使用一个bool的vector对象的时候，调用的是下面这个类；其他类型的vector的时候是上面那个类。并不是说你不能把第一个参数指定为其他类型

![picture 15](../images/8a0ce2cf1feec37e432b5c48005858887f1cda891dad144d2eaffca237dbc05f.png)  

## c++标准库

略，详见STL笔记部分

## c++11: variadic templates, auto, range-based for loop

### variadic templates

可变参数模板用来实现函数接受可变数量的参数。这种方法通常使用递归调用函数来处理可变数量的参数。
![picture 16](../images/46144751c2b7909034db807fd5ae426961a061b6c2cc7a7ee93b90c49f505a0c.png)  
> 最后args只剩一个的时候，被递归分解为一个和0个，那个0个就走上面的print了（有趣）
> 这里切分为一个和一包，然后想去做什么事情可以自定义，并非一定要像这里自己调用自己

cpp中：

```cpp
// 递归终止条件，当没有参数时返回0
int sum() {
    return 0;
}

// 递归调用，将第一个参数加到剩余参数的总和上
template<typename T, typename... Args>
T sum(T first, Args... args) {
    return first + sum(args...);
}

int main() {
    std::cout << sum(1, 2, 3, 4, 5) << std::endl;  // 输出15
    return 0;
}

```

python中：如此优雅...但看起来没啥分割

```python
def sum(*args):
    return sum(args)

print(sum(1, 2, 3, 4, 5))  # 输出15
```

### auto

![picture 19](../images/a9300bdae97e662b541549166a19d0401c9d4c04dd621eeb2a07197a6b224232.png)  
> 一般来说，你得清楚他是什么类型，才去用auto。

那所有的变量都使用auto咯？
1.除非你真的清晰所有的类型。
2.声明变量没有赋值的时候用不了咯
这是个值得讨论的问题

### range-based for

![picture 20](../images/82a8bffbc34f7e4c1537b982cf79a6a031473cf1bd3935a55b188ea85d2870ad.png)  

## reference

所有的编译器中reference都是pointer实现的
![picture 21](../images/95cabb44215d311d987e85037d19433f78e8740ce82c831c94387dd048664f17.png)  

从内存的角度来看，变量有三种：

- 值本身value：这里x是int，32bit
- 值的指针pointer：32位电脑上，32bit(地址为32bit)；这里的引用r代表x，逻辑上x是int，r就是int；底部实现上是指针。
- 值的引用reference：32bit, **代表谁它就多大**，因为它就代表x。
  - 编译器必须刻意制造一种**假象**：
    - x多大，r就多大：sizeof(r) == sizeof(x);
    - x地址在哪儿，r地址就在哪儿：&x == &r;

> int* p 还是int *p?
> 侯捷更喜欢int* p, p是一个变量，类型是pointer to integer，听起来不错。
> int& r, r是一个变量，类型是reference to intege。

```cpp
  0 #include <iostream>       
  1 using namespace std;
  2
  3 int main() {
  4     int x=8;
  5     cout << "x: \t" << x << endl;
  6     cout << "&x:\t" << &x << endl;
  7     int* p = &x;
  8     cout << "&p:\t" << &p << endl;
  9     cout << "p: \t" << p << endl;
 10     cout << "*p:\t" << *p << endl;
 11     int& r = x;
 12     cout << "r: \t" << r << endl;
 13     cout << "&r:\t" << &r << endl;
 14     cout << "sizeof(r), sizeof(x): " << sizeof(r) << "," << sizeof(x) << endl;
 15     return 0;
 16 }
// print
x:      8
&x:     0x7fff3fda0054
&p:     0x7fff3fda0058 // 指针本身是占用内存，有地址的
p:      0x7fff3fda0054 // 指针的值是x的地址
*p:     8
r:      8
&r:     0x7fff3fda0054 // 引用的地址就是x的地址
sizeof(r), sizeof(x): 4,4 // sizeof(r) == sizeof(x)
```

![picture 22](../images/1af3e76fd8861a9b30edad1dfa85ac9812ec36caf85d8a7c98232bfdf0c63993.png)  

一句有意思话：**reference是一个漂亮的pointer**
我们很少声明一个reference变量，常常出现在函数传参，和返回类型的描述

问题：为什么要造这样一个假象，reference底层是指针，具体怎么实现的？

![picture 23](../images/689422ba7b2b019d107e97b805ead89aa6d3c8d585df904963d9de8c87ec7448.png)  

> 签名signature不含return type，只有返回类型不同是无法构成函数重载的
> const是函数签名的一部分，只有const不同可构成函数重载

## 虚函数 虚指针 虚表 动态绑定

虚函数是oop最重要的部分

![picture 7](../images/cea74f673a75debb5d96c6f87b9ce8a1400c7fbe698b5cc8184725c4cd4696ab.png)

- 继承的时候子类会继承父类的成员变量以及成员函数的调用权（不是继承函数的内存大小）。所以父类有虚函数，子类也一定有。
- 当类内有1或多个虚函数，对象中会有且仅有一个虚指针

![picture 25](../images/f0347f1f213e227175b17e3da0811a8d49563da52af07216a742a8b42d4c030a.png)  

> 问题：B中的vfunc1为什么和A中的vfunc1没关系

在C++中，**函数调用可以通过静态绑定（static binding）或动态绑定**（dynamic binding）来实现。

静态绑定发生在编译时，编译器在编译阶段就能确定要调用的函数地址。这种绑定方式通常用于非虚函数，因为编译器可以根据函数的静态类型（即在编译期间可知的类型）来决定调用哪个函数。静态绑定的形式通常是直接调用函数的地址，比如call some_address。

动态绑定则发生在运行时，主要用于虚函数。
当我们使用基类的指针或引用调用一个虚函数时会发生动态绑定。
动态绑定的形式是virtual mechanism，调用哪个地址不一定，需要看虚指针，虚表指向哪个函数。

符合以下条件会使用**动态绑定**：

1. 必须通过指针调用函数；
2. 指针必须是向上转型，upcast；（派生类到基类的类型转换：将基类的指针或引用绑定到派生类对象中的基类部分上）
3. 调用的是虚函数；

![picture 0](../../images/2fa3314f7d5a03dc5604d5b53fd13803ed42990afb6d821d748ca08663269b86.png)  
这里`(*(p->vptr)[n])(p)`意思是指：通过对象的指针p找到它的虚指针，然后找到它的虚表，取出其中的第n个，把它当成函数指针去调用。
> p->vptr是虚指针，指向虚函数表，虚函数表是存储了虚函数地址的数组
> p->vptr[n]：访问虚函数表的第n个元素，即第n个虚函数的地址/指针
> 然后*是解引用，然后使用()调用该函数，其中参数就是这个对象p（由于通过p来调用，所以p就对应着这个函数的thispointer）

函数指针定义：int (*func_pointer) (int x);
函数指针调用：(*func_pointer) (args); 一般通过解引用的方式调用

虚函数表：
1.同一个类的所有实例共用一个虚函数表
2.派生类和父类不共用一个虚函数表