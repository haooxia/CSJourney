#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "util.h"
#define BUF_SIZE 1024
#define OPSZ 4

int cal(int operand_num, int opnds[], char op) {
    int result = 0;
    if (op == '+')
        for (int i=0; i<operand_num; ++i) 
            result += opnds[i*OPSZ];
    else if (op == '-')
        for (int i=0; i<operand_num; ++i) 
            result -= opnds[i*OPSZ];
    else if (op == '*') {
        result = 1;
        for (int i=0; i<operand_num; ++i) 
            result *= opnds[i*OPSZ];
    } else {
        result = opnds[0];
        for (int i=1; i<operand_num; ++i)
            result /= opnds[i*OPSZ];
    }
    return result;
}

int main(int argc, char* argv[]) {
    char opinfo[BUF_SIZE];

    if (argc != 2) {
        printf("usage: %s <Port>\n", argv[0]);
        exit(1);
    }

    int serv_sockfd = socket(PF_INET, SOCK_STREAM, 0); // AF_*和PF_*值完全相同，通常混用
    errif(serv_sockfd==-1, "socket create error!");

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1])); // host to network short; 注意输入是一个unsigned short
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // inet_addr将点分十进制字符串转为用*网络字节序*表示的ipv4地址

    errif(bind(serv_sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr))==-1, "socket bind error!");
    errif(listen(serv_sockfd, SOMAXCONN)==-1, "socket listen error!"); 
    printf("Server listening sockfd %d Address: %s:%d\n", serv_sockfd, inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port)); // inet_ntoa将网络字节序表示的ipv4转为点分十进制字符串
    struct sockaddr_in clnt_addr;
    memset(&clnt_addr, 0, sizeof(clnt_addr));

    socklen_t clnt_addr_len = sizeof(clnt_addr);
    int clnt_sockfd = accept(serv_sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len); // 返回客户端sockfd
    errif(clnt_sockfd==-1, "socket accept error!");
    printf("Client sockfd %d Address: %s:%d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
    int recv_len = 0, operand_num, recv_cnt;
    // 我们需要先计算出需要接收多少数据: 通过operand_num确定
    read(clnt_sockfd, &operand_num, 1);
    while (operand_num*OPSZ + 1 > recv_len) { // 还要读取operand_num*OPSZ+1
        recv_cnt = read(clnt_sockfd, opinfo, BUF_SIZE-1);
        recv_len += recv_cnt;
    }
    int result = cal(operand_num, (int*)opinfo, opinfo[operand_num*OPSZ]);
    
    write(clnt_sockfd, (char*)&result, sizeof(result)); // why char*?
    close(clnt_sockfd);
    close(serv_sockfd);
    return 0;
}