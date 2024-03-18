#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"
#define BUF_SIZE 1024
#define RST_SIZE 4
#define OPSZ 4

int main(int argc, char* argv[]) {
    char opmsg[BUF_SIZE];
    if (argc != 3) {
        printf("usage: %s <IP> <Port>\n", argv[0]);
        exit(1);
    }
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // 客户端套接字文件描述符
    errif(sockfd==-1, "socket create error!");
    printf("client sockfd %d\n", sockfd);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2])); // host to network short
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); // inet_addr将点分十进制字符串转为用*网络字节序*表示的ipv4地址

    errif(connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr))==-1, "socket connect error!");
    printf("Connexted......\n");
    printf("Operand count: ");
    int operand_num;
    scanf("%d", &operand_num);
    opmsg[0] = char(operand_num);
    for (int i=0; i<operand_num; ++i) {
        printf("Operand %d: ", i+1);
        scanf("%d", (int*)&opmsg[i*OPSZ+1]); // scanf需要一个int*参数，来将读取的int存到所指向的位置 占用4字节
    }
    fgetc(stdin); // 删除缓冲中的字符\n
    printf("Operator: ");
    scanf("%c", &opmsg[1+operand_num*OPSZ]);
    errif(write(sockfd, opmsg, 2+operand_num*OPSZ)==-1, "write() error");
    int result;
    errif(read(sockfd, &result, RST_SIZE)==-1, "read() error");
    printf("Operation result: %d\n", result);
    close(sockfd);
    return 0;
}