#include "util.h"
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h> // read and write (TCP); sendto and recvfrom (UDP)

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // AF_*和PF_*值完全相同，通常混用
    errif(sockfd == -1, "socket create error");
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8888); // host to network short
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // inet_addr将点分十进制字符串转为用*网络字节序*表示的ipv4地址
    printf("Server address: %s:%d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port)); // inet_ntoa将网络字节序表示的ipv4转为点分十进制字符串

    errif(bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket bind error");
    errif(listen(sockfd, SOMAXCONN) == -1, "socket listen error"); 

    struct sockaddr_in clnt_addr;
    memset(&clnt_addr, 0, sizeof(clnt_addr));
    // 注意accept要求传入的是socklen_t*，所以需要定义一个socklen_t类型变量（不太懂 保留疑问）
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len); // 返回客户端sockfd
    errif(clnt_sockfd == -1, "socket accpet error");
    printf("new client fd %d! Address: %s:%d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

    while (true) {
        char buf[1024]; // 定义缓冲区
        bzero(&buf, sizeof(buf)); // &buf相当于指向整个数组，buf指向数组首元素，虽然数值相同，但++的结果是完全不同的
        ssize_t read_bytes = read(clnt_sockfd, buf, sizeof(buf)); // 返回已读数据大小
        if (read_bytes > 0) {
            printf("message from client fd %d: %s\n", clnt_sockfd, buf);
            write(clnt_sockfd, buf, sizeof(buf)); // 将相同的数据写回clnt
        } else if (read_bytes == 0) {
            printf("client fd %d disconnected\n", clnt_sockfd);
            close(clnt_sockfd);
            break;
        } else if (read_bytes == -1) {
            close(clnt_sockfd);
            errif(true, "socket read error");
        }
    }
    close(sockfd);
    return 0;
}