# notes of 30dayMakeCppServer

## day01

TCP服务器端函数调用:

- socket(): 创建套接字
- bind(): 命名套接字：分配地址；客户端不需要，采用匿名socket(os自动分配)即可。
- listen(): 等待连接请求状态
- accept()
- read()/listen()
- close()

TCP客户端函数调用:

- socket()
- connect()
- read()/write()
- close()

为什么sockaddr_in可以强制类型转换为sockaddr？
由下面的结构体可见二者结构体大小一致(都是16字节)，成员变量相同，故而可以相互转化；

那么sockaddr_in6可以转换为sockaddr吗（游双似乎说可以pg 174）：保留疑问；应该会有信息丢失吧，sockaddr_storage没问题

```c
// 16字节
struct sockaddr {
        u_short sa_family;              /* address family */
        char    sa_data[14];            /* up to 14 bytes of direct address 包含套接字中的目标地址和端口地址 */
};

// 16字节
struct sockaddr_in {
        short   sin_family;         //address family
        u_short sin_port;           //16 bit TCP/UDP port number
        struct  in_addr sin_addr;   //32 bit IP address
        char    sin_zero[8];        //not use, for align
};

// 28字节
struct sockaddr_in6 {
    sa_family_t sin6_family;   // 地址族
    uint16_t sin6_port;      // 端口号
    uint32_t sin6_flowinfo;   // 流信息
    struct in6_addr sin6_addr; // IPv6地址
    uint32_t sin6_scope_id;   // 范围标识符
};

```

服务器种类：
无并发服务器：
1.单一服务器和单一客户端
2.单一服务器和多个客户端 但是客户端需要串行执行 后面的等待前面的客户端执行结束（即迭代服务器） 本质上没改变
并发服务器
1.多进程服务器
![picture 0](../../images/edecf3c5d62d42b5210f85f028cb035c169babb3cbedec006d8239a56bc391c1.png)
2.多路复用服务器
![picture 1](../../images/ddb441dcadcd688c1608305ff73584f713aa1c48bc53e4102e6112aa9bd23043.png)  


## day03 高并发epoll

所有的服务器都是高并发的，可以同时为成千上万个客户端提供服务，这一技术又被称为**IO复用**。
> IO复用和多线程有相似之处，但绝不是一个概念。IO复用是针对IO接口，而多线程是针对CPU。
在Linux系统中，IO复用使用select, poll和epoll来实现。epoll改进了前两者，更加高效、性能更好，是目前几乎所有高并发服务器的基石。