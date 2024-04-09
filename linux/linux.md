# Linux

## commands

ps: 显示当前运行进程的快照

ps -ef: 显示进程详细信息，连同命令行

- UID: user ID of the process owner
- PID: process ID, unique
- PPID: parent process ID
- C: CPU占用率
- STIME: start time
- TTY: 登入者终端机位置
- TIME: cumulative CPU time used by the process
- CMD: command that started the process

ps -ef | grep python: 查看python进程
ps -aux: 显示所有进程更详细的信息，包括cpu和内存占用

- %CPU: CPU占用
- %MEM
- VSZ: 虚拟内存占用量
- STAT: 状态
  - R: 运行
  - D: 不可中断，一般是IO
  - S: 中断
  - T: 停止
  - Z: 僵尸

kill: 向进程发送信号
kil -l: 查询所有信号
kill -9 [PID]: 使用SIGKILL强制杀死进程
Ctrl-C: SIGINT: 终止该进程
Ctrl-Z: SIGTSTP: 将前台的进程置于后台，并暂停；可以使用fg调到前台。

netstat：显示网络连接
可以查看对应的端口有没有被监听

### 如何查看网络的性能指标

网络性能指标：

1. 带宽：链路最大传输速率(bit/s)
2. 延时：请求数据包发送后，收到对端响应所需要的时间
3. 吞吐率：单位时间内成功传输的数据量
4. PPS：packet per second，以网络包为单位的传输速率
5. 并发连接数：TCP连接数量
6. 丢包率：丢失数据包数量 / 发送数据包数量
7. 重传率：重传网络包比例

查询网络配置
`ifconfig` 查询所有网口的配置
`ifconfig eno2` 查询eno网口的配置
返回值包括：ipv4, netmask, ipv6, MAC, mtu_size, broadcast接口广播地址
RX packets: 接收数据包数量；TX packets: 发送数据包数量；
errors: 错误数据包，eg校验错误，帧同步错误
dropped: 丢包数量
overruns: 超限数据包，速度过快，ring buffer来不及处理
