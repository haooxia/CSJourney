#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"
#define BUF_SIZE 1024

int main(int argc, char* argv[]) {
    char message[BUF_SIZE];
    if (argc != 3) {
        printf("usage: %s <IP> <Port>\n", argv[0]);
        exit(1);
    }
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    errif(sockfd==-1, "socket create error!");
    printf("client sockfd %d\n", sockfd);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2])); 
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); 
    
    errif(connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr))==-1, "socket connect error!");
    printf("socket connect successful\n");
    int str_len=0, recv_len=0, recv_cnt=0;
    while (true) {
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) break;
        errif((str_len = write(sockfd, message, sizeof(message)))==-1, "write() error");
        while (recv_len < str_len) {
            errif(recv_cnt = read(sockfd, message, BUF_SIZE-1)==-1, "read() error"); // 为什么BUF_SIZE-1
            recv_len += recv_cnt;
        }
        message[recv_len] = 0; // 末尾加一个'\0' 确保是一个正确的字符串
        printf("message from server: %s\n", message);
    }
    close(sockfd);
    return 0;
}