#include "util.h"
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h> // read and write (TCP); sendto and recvfrom (UDP)

int main() {
    /* domain底层协议族, type数据传输类型, protocol具体协议 */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // AF_*和PF_*值完全相同，通常混用
    errif(sockfd == -1, "socket create error");
    /* 创建对象并绑定 */
    struct sockaddr_in serv_addr;
    // bzero(&serv_addr, sizeof(serv_addr)); //bzero将制定内存区域的所有字节都设置为0；已废弃，推荐memset
    memset(&serv_addr, 0, sizeof(serv_addr));

    // 设置地址族
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8888); // host to network short
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // inet_addr将点分十进制字符串转为用*网络字节序*表示的ipv4地址
    printf("Server address: %s:%d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port)); // inet_ntoa将网络字节序表示的ipv4转为点分十进制字符串

    // 创建的时候使用专用的sockaddr_in: 因为方便设置ip地址和端口号等
    // 绑定的时候使用通用的sockaddr: bind为了鲁棒性，接受通用的sockaddr参数
    errif(bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket bind error");

    /* 监听 */
    // #define SOMAXCONN 128 // 最大监听队列长度 内部定义过来
    errif(listen(sockfd, SOMAXCONN) == -1, "socket listen error"); 

    /* 接受：首先需要保存客户端的socket地址信息 */
    // accept 会阻塞当前程序，知道有一个客户端socket被接受才会继续往下运行
    // 构造一个空的clnt_addr，传入accept会返回clnt的地址，如果不需要这个地址的话可以设置为null
    // 当有客户端连接请求时，accept函数会根据参数addrlen的大小往addr所指向的地址里写信息
    struct sockaddr_in clnt_addr;
    // bzero(&clnt_addr, sizeof(clnt_addr));
    memset(&clnt_addr, 0, sizeof(clnt_addr));
    // 注意accept要求传入的是socklen_t*，所以需要定义一个socklen_t类型变量（不太懂 保留疑问）
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    
    char buf[1024]; // 定义缓冲区
    bzero(&buf, sizeof(buf));


    // ******
    // 定义迭代式服务器 挂着可以串行执行5个客户端（虽然串行没啥鸟用） 依然无法并发执行任务
    for (int i=0; i<5; ++i) {
        int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len); // 返回客户端sockfd
        errif(clnt_sockfd == -1, "socket accpet error");
        printf("new client fd %d! Address: %s:%d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

        while (read(clnt_sockfd, buf, sizeof(buf)) > 0) {
            printf("message from client fd %d: %s\n", clnt_sockfd, buf);
            write(clnt_sockfd, buf, sizeof(buf)); // 将相同的数据写回clnt
        }
        close(clnt_sockfd);
    }
    close(sockfd);
    return 0;
}