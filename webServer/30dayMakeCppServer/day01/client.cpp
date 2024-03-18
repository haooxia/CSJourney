#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"

int main(int argc, char* argv[]) {
    char message[30];
    if (argc != 3) {
        printf("usage: %s <IP> <Port>\n", argv[0]);
        exit(1);
    }
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // 客户端套接字文件描述符
    errif(sockfd==-1, "socket create error!");
    printf("client sockfd %d\n", sockfd);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    // 设置地址族: 注意设置的是server的地址
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2])); // host to network short
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); // inet_addr将点分十进制字符串转为用*网络字节序*表示的ipv4地址
    
    errif(connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr))==-1, "socket connect error!");
    printf("socket connect successful\n");

    errif(read(sockfd, message, sizeof(message)-1)==-1, "read() error");
    printf("message from server: %s \n", message);
    close(sockfd);
    return 0;
}