#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "util.h"
#define BUF_SIZE 1024

int main(int argc, char* argv[]) {
    char message[BUF_SIZE]; 

    if (argc != 2) {
        printf("usage: %s <Port>\n", argv[0]);
        exit(1);
    }
    
    int serv_sockfd = socket(PF_INET, SOCK_STREAM, 0);
    errif(serv_sockfd==-1, "socket create error!");

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr)); 


    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    errif(bind(serv_sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr))==-1, "socket bind error!");
    int str_len;
    for (int i=0; i<5; ++i) { // 允许最多5个clnt
        errif(listen(serv_sockfd, SOMAXCONN)==-1, "socket listen error!"); 
        printf("Server listening sockfd %d Address: %s:%d\n", serv_sockfd, inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
        struct sockaddr_in clnt_addr;
        memset(&clnt_addr, 0, sizeof(clnt_addr));
        socklen_t clnt_addr_len = sizeof(clnt_addr);
        int clnt_sockfd = accept(serv_sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len); // 返回客户端sockfd
        errif(clnt_sockfd==-1, "socket accept error!");
        printf("Client sockfd %d Address: %s:%d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
        while ((str_len = read(clnt_sockfd, message, BUF_SIZE)) != 0) // 遇到文件结尾返回0
            errif(write(clnt_sockfd, message, str_len))==-1, "write() error");
        close(clnt_sockfd);
    }
    close(serv_sockfd);
    return 0;
}