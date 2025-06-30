# Notes

- [Notes](#notes)
  - [1. Allocator](#1-allocator)
  - [2. Container](#2-container)
    - [2.1 list](#21-list)


STL: Standard Template Library 标准模板库
C++标准库包括STL，之外还有其它部件

![picture 2](../images/1449b1b9cae25f8d9e03010c5db617bbbefd77b8f9340ed896e10da98d1d8663.png)  

## 1. Allocator

所有的分配动作最终都是到C-level的malloc()，这个函数再去调用操作系统的api；

![picture 0](../images/3ccc41512ea3b7da09537ec79550cc8c153b0c5439c5ff29ccc108a75140a3d1.png)  

> 我们不直接使用allocate接口，一般是使用容器自动分配。

![picture 1](../images/91e12c406989a91e2478c79a7ad12236d974efcc2463ebb20497ec087ef981f6.png)  
> gcc的容器没使用allocator分配器，用的是alloc分配器；gcc的allocator和前面的vc6的allocator分配多个元素的vector时候，每个元素都会有一个cookie(真的吗)，每次malloc都会得到一对cookie，开销比较大。alloc用于减少这个开销，不去每一个元素都分配cookie。cookie数量大大减少。(版本G2.9)
> G4.9又没用alloc了，换成new_allocator了（换回去了，老师也不懂为什么）；alloc名字改成__pool_alloc

## 2. Container

![picture 3](../images/334ff8cf5f4db5a2c04861b47b686f43b319fd1b454d75df491731b355ccd776.png)  

> 这个sizeof是控制容器所需要的大小，不是实际元素的大小，不论实际有多少元素。

**Sequence Container 序列式容器**
array: 无法扩充；
vector: 尾部可扩充；
deque(double-ended queue): 头尾均可扩充；
list: 双向(环状)链表；
forward-list: 单向链表；
> stack 和 queue 技术上叫做container adapter；叫做容器也没啥毛病了；注意二者没有iterator；

---

**Associative Container 关联式容器**
set/multiset, map/multimap: 底层是红黑树；multi允许元素重复；
unordered set/multiset, unordered map/multimap: 底层HashTable(链地址法separate chaining)
> multimap不可以使用[]赋值，map可以；
> unordered: 不定序

### 2.1 list

![picture 4](../images/3be8b7da8e41cbdf4e19bbb9e9272f6ed5d47b11a86ea4382f75611f37fa4ee8.png)  
> “不是直接++”意思是不是直接物理地址++，实际上也是++，只不过是重载过的智能的++；
> 最后一个元素的下一个元素不属于容器本身（前闭后开）：所以搞了个空节点

![picture 5](../images/3a076ccc3de9616cfb6083ac61d8569302d907235135109bcb238115f1fcfd58.png)  

![picture 6](../images/d1d5ad3707abd681128d0d0ee229da9260468827ecf8dc1c18f149c1d4ea79b9.png)  
