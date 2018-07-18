# 网络 IPC: 套接字

## 随书代码

### 16.3.1.c

```shell
$ gcc 16.3.1.c
$ ./a.out
67305985
ch0 = 1
ch1 = 2
ch2 = 3
ch3 = 4
```

我的系统为 macOS, 为小端字节序.

### 16-9

```shell
$ gcc 16.3.3.c -lapue
$ ./a.out baidu.com 80
flags 0 family inet type datagram protocol UDP
        host baidu.com address 123.125.115.110 port 80
flags 0 family inet type stream protocol TCP
        host baidu.com address 123.125.115.110 port 80
flags 0 family inet type datagram protocol UDP
        host baidu.com address 220.181.57.216 port 80
flags 0 family inet type stream protocol TCP
        host baidu.com address 220.181.57.216 port 80
$ ./a.out meik2333.com 80
flags 0 family inet type datagram protocol UDP
        host meik2333.com address 45.116.13.102 port 80
flags 0 family inet type stream protocol TCP
        host meik2333.com address 45.116.13.102 port 80
```

### 16-16 && 16-17

#### 配置

首先在 `/etc/services` 中追加 `ruptime 6666/tcp` (端口号可以自己指定)

#### 16-17

```shell
$ gcc 16-17.c 16-12.c -lapue -o ruptimed
$ ./ruptimed
```

#### 16-16

```shell
$ lsof -i:6666
COMMAND    PID USER   FD   TYPE             DEVICE SIZE/OFF NODE NAME
ruptimed 12829 meik    3u  IPv4 0xdbb00798863434b9      0t0  TCP 10.42.0.232:ruptime (LISTEN)
$ gcc 16-16.c -lapue
$ ./a.out 10.42.0.232
10:57  up 14:37, 1 user, load averages: 2.37 2.71 2.62
```

或者修改 16-17 代码, 使其监听所有 IP

```shell
$ ./a.out 127.0.0.1
11:05  up 14:45, 1 user, load averages: 2.48 2.40 2.48
$ ./a.out 0.0.0.0
11:05  up 14:45, 1 user, load averages: 2.36 2.38 2.47
$ ./a.out 10.42.0.232
11:05  up 14:45, 1 user, load averages: 2.33 2.37 2.47
```

### 16-18

```shell
$ gcc 16-18.c 16-12.c -lapue -o ruptime
$ ./ruptime
$
$ lsof -i:6666
COMMAND   PID USER   FD   TYPE             DEVICE SIZE/OFF NODE NAME
ruptime 15300 meik    5u  IPv4 0xdbb007988381ce19      0t0  TCP 10.42.0.232:ruptime (LISTEN)
```

### 16-19 && 16-20

#### 配置

首先在 `/etc/services` 中追加 `ruptime 6666/udp` (端口号可以自己指定)

#### 16-17

```shell
$ gcc 16-20.c 16-12.c -lapue -o ruptime
$ ./ruptimed
```

#### 16-16

```shell
$ lsof -i:6666
COMMAND   PID USER   FD   TYPE             DEVICE SIZE/OFF NODE NAME
ruptime 24681 meik    5u  IPv4 0xdbb007987647cef1      0t0  UDP 10.42.0.232:ruptime
$ gcc 16-19.c -lapue
$ ./a.out 10.42.0.232
17:15  up 20:55, 1 user, load averages: 2.28 2.53 2.55
```

## 课后习题

### 16.1

**写一个程序判断所使用系统的字节序.**  
MacOS 小端字节序

### 16.2

**写一个程序, 在至少两种不同的平台上打印出所支持的套接字的 stat 结构成员, 并且描述这些结果的不同之处.**  
pass

### 16.3

**图 16-17 的程序只在一个端点上提供了服务. 修改这个程序, 同时支持多个端点(每个端点具有一个不同的地址)上的服务.**  
对于我们将要监听的每个端点, 需要绑定到一个合适的地址, 并对应每个描述符在 fd_set 结构中写一条记录. 然后使用 select 等待从多个端点来的连接请求. 回忆 16.4 节, 当一个连接请求到达时, 一个被动的端点将会变得可读. 当一个连接请求真的到达时, 我们接受该请求, 并如以前一样处理.

### 16.4

**写一个客户端程序和服务端程序, 返回指定主机上当前运行的进程数量.**  
wc 是一个 UNIX 上的计算行数的工具, ps 可以获得当前运行的进程

修改之前的服务器程序, 将执行的 shell 改为 `ps -x | wc -l` 即可获得当前运行的进程数量, 也可以连接 `grep` 来进行更多的筛选

### 16.5

**在图 16-18 的程序中, 服务器等待子进程执行 uptime , 子进程完成后退出, 服务器才接受下一个连接请求. 重新设计服务器, 使得处理一个请求时并不拖延处理到来的连接请求.**  
在 main 过程中, 通过调用我们的 signal 函数(见图 10-18)来捕捉 SIGCHLD, 该函数将使用 `sigaction` 来安装处理程序指定可重启的系统调用选项. 下一步, 从 `serve` 函数中删除 `waitpid` 调用. 当 `fork` 完子进程来处理请求后, 父进程关闭新的文件描述符并继续监听新的连接请求. 最后, 需要一个针对于 SIGCHLD 的信号处理程序, 如下:

```C
void sigchld(int signo) {
        while (waitpid((pid_t) -1 , NULL, WHOHANG) > 0);
}
```

### 16.6

**写两个库例程: 一个在套接字上允许异步 I/O, 一个在套接字上不允许异步 I/O. 使用图 16-23 来保证函数能够在所有平台上运行, 并且支持尽可能多的套接字类型.**  
为了运行异步套接字 I/O, 需要使用 F_SETOWN fcntl 命令建立套接字所有权, 然后使用 FIOASYNC ioctl 命令允许异步信号. 为了不允许异步套接字 I/O, 只要简单的禁用异步信号即可. 我们混合使用 fcntl 和 ioctl 命令的理由是, 想找到最可移植的方法. 代码如下:

```C
#include "apue.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#if defined(BSD) || defined(MACOS) || defined(SOLARIS)
#include <sys/filio.h>
#endif

int setasync(int sockfd) {
    int n;

    if (fcntl(sockfd, F_SETOWN, getpid()) < 0) {
        return (-1);
    }
    n = 1;
    if (ioctl(sockfd, FIOASYNC, &n) < 0) {
        return (-1);
    }
    return (0);
}

int clrasync(int sockfd) {
    int n;

    n = 0;
    if (ioctl(sockfd, FIOASYNC, &n) < 0) {
        return (-1);
    }
    return (0);
}
```