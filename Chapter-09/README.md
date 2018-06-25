# 进程控制

## 随书代码

### 9.10.c

```shell
$ gcc 9.10.c -lapue
$ ./a.out
parent: pid = 3830, ppid = 3793, pgrp = 3830, tpgrp = 3830
child: pid = 3831, ppid = 3830, pgrp = 3830, tpgrp = 3830
SIGHUP received, pid = 3831
child: pid = 3831, ppid = 1, pgrp = 3830, tpgrp = 3793
read error 5 on controlling TTY
```

## 课后习题

### 9.1

**考虑 6.8 节中说明的 utmp 和 wtmp 文件，为什么 logout 记录是由 init 进程写的？对于网络登录的处理与此相同吗？**  
因为 init 是登录 shell 的父进程，当登录 shell 终止时它收到 SIGCHLD 信号量，所以 init 进程知道什么时候终端用户注销。

网络登录没有包含 init ，在 utmp 和wtmp 文件中的登录项和相应的注销项是由一个处理登录并检测注销的进程写的（本例中为 telnetd）。

### 9.2

**编写一段程序调用 fork 并使子进程建立一个新的会话。验证子进程变成了进程组组长且不再有控制终端。**  
见 [p2.c](p2.c)

```shell
$ gcc p2.c -lapue
$ ./a.out
child: pid = 4030, ppid = 4029, pgrp = 4030, tpgrp = -1
```