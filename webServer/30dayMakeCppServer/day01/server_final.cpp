#include <stdio.h>
#include <string.h>
#include <unistd.h> // read and write (TCP); sendto and recvfrom (UDP)
#include <arpa/inet.h> // 包含#include <sys/socket.h>

int main(int argc, char* argv[]) {
    // 1. 创建监听fd
    int fd = socket(PF_INET, SOCK_STREAM, 0); // AF_*和PF_*值完全相同，通常混用
    if (fd == -1) {
        perror("socket");
        return -1;
    }
    // 2. 绑定监听fd
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr)); 
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    saddr.sin_addr.s_addr = INADDR_ANY; // 宏INADDR_ANY(可以绑定本地)实际值是0=0.0.0.0；由于大小端没区别，因此无需htonl
    int ret = bind(fd, (struct sockaddr*)&saddr, sizeof(saddr));
    if (ret == -1) {
        perror("bind");
        return -1;
    }
    // 3. 设置监听
    ret = listen(fd, SOMAXCONN); // #define SOMAXCONN 128 // 最大监听队列长度 内部定义过来
    if (ret == -1) {
        perror("listen");
        return -1;
    }
    // 4. 阻塞并等待客户端连接
    struct sockaddr_in caddr;
    memset(&caddr, 0, sizeof(caddr));
    socklen_t caddr_len = sizeof(caddr);
    int cfd = accept(fd, (struct sockaddr*)&caddr, &caddr_len); // 返回通信fd
    if (cfd == -1) {
        perror("accept");
        return -1;
    }
    // 连接成功，打印客户端信息
    // char* ip; inet_pton(AF_INET, &caddr.sin_addr.s_addr, ip, sizeof(ip)); 亦可
    printf("client socket %d, Address: %s:%d\n", cfd, inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));
    // 5. 通信
    while (1) {
        char buf[1024];
        int len = recv(cfd, buf, sizeof(buf), 0);
        if (len > 0) {
            printf("client say: %s\n", buf);
            send(cfd, buf, len, 0); // 长度指定为len 不要传多了
        } else if (len == 0) {
            printf("客户端已经断开连接...");
            break;
        } else if (len == -1) {
            perror("recv");
            break;
        }
    } // 跳出后说明通信结束
    close(cfd);
    close(fd);
    return 0;
}