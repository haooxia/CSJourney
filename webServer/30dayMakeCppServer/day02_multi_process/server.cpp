#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h> // read and write (TCP); sendto and recvfrom (UDP)
#include <signal.h>
#include <sys/wait.h>

void handle_client(int clnt_sockfd, struct sockaddr_in clnt_addr) {
    char buf[1024];
    bzero(&buf, sizeof(buf));
    printf("new client fd %d! Address: %s:%d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
    while (read(clnt_sockfd, buf, sizeof(buf)) > 0) {
        printf("message from client fd %d: %s\n", clnt_sockfd, buf);
        write(clnt_sockfd, buf, sizeof(buf)); // 将相同的数据写回clnt
    }
    close(clnt_sockfd);
    printf("client disconnected...");
}

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket create error");
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8888); // host to network short
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // inet_addr将点分十进制字符串转为用*网络字节序*表示的ipv4地址
    printf("Server address: %s:%d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port)); // inet_ntoa将网络字节序表示的ipv4转为点分十进制字符串

    errif(bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket bind error");
    // #define SOMAXCONN 128 // 最大监听队列长度 内部定义过来
    errif(listen(sockfd, SOMAXCONN) == -1, "socket listen error"); 

    struct sockaddr_in clnt_addr;
    memset(&clnt_addr, 0, sizeof(clnt_addr));
    socklen_t clnt_addr_len = sizeof(clnt_addr);

    while (true) {
        // ** 服务器接受连接会返回一个新fd，该fd用于通信
        // 通信部分交给子进程，自己继续监听其他客户端的链接
        int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
        errif(clnt_sockfd == -1, "socket accpet error");

        pid_t pid = fork();
        // fork之后子进程会复制父进程的fd 两个fd指向同一套套接字
        errif(pid==-1, "fork error");
        if (pid == 0) { // 子进程
            close(sockfd); // 子进程无需监听套接字，关掉。
            handle_client(clnt_sockfd, clnt_addr);
            exit(0);
        } else {
            close(clnt_sockfd); // 父进程无需通信，关掉。
            signal(SIGCHLD, SIG_IGN);
        }
    }
    return 0;
}

