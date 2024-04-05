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