# note

## 基于epoll的多线程服务器

### http

**http请求**
![picture 1](../images/256e00b1b9f1e29fbb22bf1c7345d1aaab07570fb5d2c7d7be0bd4aa8f42b9b0.png)  
![picture 2](../images/f29d5aa3795a34a7463fff745e05698da989d57a66d455222c14d29ccc14a166.png)  

http封装好数据之后形成一个数据块，得到若干行数据，每一行的结尾是\r\n换行符

get请求也可以携带数据（请求行中）：
![picture 3](../images/6fb1419897e9a3a1f2df91a47f2e46feaf164556929adedad5f21cf4145cb13b.png)  
浏览器地址栏缓存大概是8k，所以携带数据有限

get请求的请求数据一定是空的，它的数据会在请求行中，所以不是保密的；

post向服务器提交数据：
![picture 4](../images/55d79817bd9e9ccb0a02566fd3557d12e941c27a39c30c29d7f026836700da65.png)  

**http响应**
![picture 5](../images/d421f9f6b4ecf60e11d0bd663b52d9a18dd490b4e7946c0796073b50dd610cd0.png)  
![picture 6](../images/636edf7836cdbdc54a89ff8886c911ad55ec4f67bbf2f0da6b061a7bc99ba896.png)  

## 多反应堆线程池

channel模块：封装一个文件描述符fd，可能用于连接或者通信
![picture 7](../images/98c76d319643c64d49390631629ade786b854c7b31ede6d8e78eb4601435599e.png)  

dispatcher模块（核心）

