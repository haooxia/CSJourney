# TinyWebserver

## configuration

```bash
sudo apt update
sudo apt install mysql-server
# sudo apt install libmysqlclient-dev # 安装链接库 解决fatal error: mysql/mysql.h: No such file..
sudo systemctl status mysql # 验证mysql是否在运行


# in WSL
sudo service mysql status # 状态查询
sudo service mysql start # 打开

sudo mysql # run
create database webserver # create
create user 'xiahao'@'localhost' identified by 'xiahao'; # 创建用户xiahao 密码xiahao

create database yourdb;

USE yourdb;
CREATE TABLE user(
    username char(50) NULL,
    passwd char(50) NULL
)ENGINE=InnoDB;

INSERT INTO user(username, passwd) VALUES('name', 'passwd');

bash ./build.sh # compile
sudo ./server # run: notice this `sudo`
input 127.0.0.1:9006 to chrome # or host_ip

```

## basics

1.什么是webserver？
Webserver/网站服务器时一种软件程序，可以处理客户端(通常是浏览器)的web请求并返回相应响应。
可以放置网站文件，让全世界浏览；可以放置数据文件，让全世界下载。
目前最主流的三个Web服务器是Apache, Nginx, Microsoft IIS.
> 本项目中web请求主要指HTTP协议

2.什么是socket？
Socket是**应用程序和操作系统内部的协议栈**(TCP,UDP)之间的桥梁；
Socket是**应用层和传输层**之间的接口，即在传输层之上。
网络编程的大部分内容就是设计并实现**应用层**协议，根据程序特点决定服务器端和客户端之间的**数据传输规则**；而链路层、网络层和传输层已经由**套接字**自动处理了，使得程序员从这些细节中解放出来；