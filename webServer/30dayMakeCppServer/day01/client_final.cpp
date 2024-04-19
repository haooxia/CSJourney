#include <stdio.h>
#include <string.h>
#include <unistd.h> // read and write (TCP); sendto and recvfrom (UDP)
#include <arpa/inet.h> // 包含#include <sys/socket.h>

int main(int argc, char* argv[]) {
    // 1. 创建通信fd
    int fd = socket(PF_INET, SOCK_STREAM, 0); // AF_*和PF_*值完全相同，通常混用
    if (fd == -1) {
        perror("socket");
        return -1;
    }
    // 2. 连接服务器
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr)); 
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr.s_addr);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 或者直接指定ip: 172.31.78.11
    int ret = connect(fd, (struct sockaddr*)&saddr, sizeof(saddr));
    if (ret == -1) {
        perror("connect");
        return -1;
    }
    printf("socket connect successful!\n");
    // 3. 通信
    int number = 0;
    while (1) {
        char buf[1024];
        sprintf(buf, "hello, message number #%d...\n", number++); // sprintf将数据写入字符串 而非输出到标准输出流
        send(fd, buf, strlen(buf)+1, 0); // 注意这里不要发送sizeof(buf)，发送实际字符数+'\0'
        memset(buf, 0, sizeof(buf)); // 有必要清空buf的
        int len = recv(fd, buf, sizeof(buf), 0);
        if (len > 0) {
            printf("server say: %s\n", buf);
        } else if (len == 0) {
            printf("服务器已经断开连接...");
            break;
        } else if (len == -1) {
            perror("recv");
            break;
        }
        sleep(1); // 让客户端1秒发一条
    } // 跳出后说明通信结束
    close(fd);
    return 0;
}