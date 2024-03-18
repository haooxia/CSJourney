#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "util.h"

int main(int argc, char* argv[]) {
    char message[] = "hello world\n"; 

    if (argc != 2) {
        printf("usage: %s <Port>\n", argv[0]);
        exit(1);
    }
    /* domain底层协议族, type数据传输类型, protocol具体协议 */
    // 决定了协议族并不能决定数据传输类型，PF_INET中也有多种传输方式
    int serv_sockfd = socket(PF_INET, SOCK_STREAM, 0); // AF_*和PF_*值完全相同，通常混用
    errif(serv_sockfd==-1, "socket create error!");

    /* 创建对象并绑定 */
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr)); 
    // bzero(&serv_addr, sizeof(serv_addr)); //bzero将制定内存区域的所有字节都设置为0；已废弃，推荐memset

    // 设置地址族
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1])); // host to network short; 注意输入是一个unsigned short
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // inet_addr将点分十进制字符串转为用*网络字节序*表示的ipv4地址

    // 创建的时候使用专用的sockaddr_in: 因为方便设置ip地址和端口号等
    // 绑定的时候使用通用的sockaddr: bind为了鲁棒性，接受通用的sockaddr参数
    errif(bind(serv_sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr))==-1, "socket bind error!");

    /* 监听 */
    // #define SOMAXCONN 128 // 最大监听队列长度 内部定义过来
    errif(listen(serv_sockfd, SOMAXCONN)==-1, "socket listen error!"); 
    printf("Server listening sockfd %d Address: %s:%d\n", serv_sockfd, inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port)); // inet_ntoa将网络字节序表示的ipv4转为点分十进制字符串

    /* 接受：首先需要保存客户端的socket地址信息 */
    // accept 会阻塞当前程序，知道有一个客户端socket被接受才会继续往下运行
    // 构造一个空的clnt_addr，传入accept会返回clnt的地址，如果不需要这个地址的话可以设置为null
    // 当有客户端连接请求时，accept函数会根据参数addrlen的大小往addr所指向的地址里写信息
    struct sockaddr_in clnt_addr;
    memset(&clnt_addr, 0, sizeof(clnt_addr));
    // 注意accept要求传入的是socklen_t*，所以需要定义一个socklen_t类型变量（不太懂 保留疑问）
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    int clnt_sockfd = accept(serv_sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len); // 返回客户端sockfd
    errif(clnt_sockfd==-1, "socket accept error!");
    // accept函数会自动生成一个新的socketfd
    printf("Client sockfd %d Address: %s:%d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
    
    write(clnt_sockfd, message, sizeof(message));
    close(clnt_sockfd);
    close(serv_sockfd);
    return 0;
}