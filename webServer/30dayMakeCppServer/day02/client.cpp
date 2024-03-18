#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include <unistd.h> // read and write (TCP); sendto and recvfrom (UDP)

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    // bzero(&serv_addr, sizeof(serv_addr)); //bzero将制定内存区域的所有字节都设置为0；已废弃，推荐memset
    memset(&serv_addr, 0, sizeof(serv_addr));

    // 设置地址族: 注意设置的是server的地址
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8888); // host to network short
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // inet_addr将点分十进制字符串转为用*网络字节序*表示的ipv4地址

    errif (connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket connect error");

    while (true) {
        char buf[1024];
        bzero(&buf, sizeof(buf));
        scanf("%s", buf); // 从键盘输入到服务器的数据
        ssize_t write_bytes = write(sockfd, buf, sizeof(buf)); // 发给服务器socket
        if (write_bytes == -1) {
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        // bzero(&buf, sizeof(buf)); // 再次清空
        memset(&buf, 0, sizeof(buf));

        ssize_t read_bytes = read(sockfd, buf, sizeof(buf)); // 从服务器读到buf，返回读取的大小
        if (read_bytes > 0) {
            printf("message from server: %s\n", buf);
        } else if (read_bytes == 0) {
            printf("server socket disconnected!\n");
            break;
        } else if (read_bytes == -1) {
            close(sockfd); // Linux系统的文件描述符理论上是有限的，在使用完后关闭
            errif(true, "socket read error");
        }
    }
return 0;
}