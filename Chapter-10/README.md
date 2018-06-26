# 信号

## 随书代码

### 10.3.c

```shell
$ gcc 10.3.c -lapue
$ ./a.out &
[1] 18096
$ kill -USR1 18096
received SIGUSR1
$ kill -USR2 18096
received SIGUSR2
$ kill 18096
```

### 10.6.c

直接 GG

### 10.10.c

```shell
$ gcc 10.10.c
$ ./a.out
before sleep
after sleep
```

通过 alarm 和 pause 实现了一个简陋的 sleep

### 10.10.2.c

```shell
$ gcc 10.10.2.c
$ ./a.out
before sleep
after sleep
```

通过 setjmp 和 longjmp 避免了竞争条件的 sleep

### 10.10.3.c

使用 volatile 组织编译程序优化循环语句，使程序长时间计算。

```shell
$ gcc 10.10.3.c
$ ./a.out
^C
sig_int starting
sleep2 returned: 0
```

数值计算在完成之前就结束了。

### 10.13.c

```shell
$ gcc 10.13.c -lapue
$ ./a.out
^\
SIGQUIT pending
caught SIGQUIT
SIGQUIT unblocked
$ ./a.out
^\
SIGQUIT pending
caught SIGQUIT
SIGQUIT unblocked
^\退出 (核心已转储)
$ ./a.out
^\^\^\
SIGQUIT pending
caught SIGQUIT
SIGQUIT unblocked
```

### 10.15.c

```shell
$ gcc 10.15.c -lapue
$ ./a.out &
[1] 11859
$ starting main:

$ kill -USR1 11859
starting sig_usr1:
$ in sig_alrm:
finishing sig_usr1:
ending main:

[1]+  已完成               ./a.out
```

## 课后习题

### 10.1

**删除图 10-2 程序中的 `for(;;)` 语句，结果会怎样？为什么？**  
删除后

```shell
$ gcc p1.c -lapue
$ ./a.out &
[2] 15231
$ kill -USR1 15231
received SIGUSR1
$ kill -USR2 15231
bash: kill: (15231) - 没有那个进程
[2]-  已完成               ./a.out
```

`pause` 在捕捉到信号的时候就返回了，没有循环调用 `pause` ，因此程序在第一次传递信号后结束了。

### 10.2

**实现 10.22 节中说明的 `sig2str` 函数**  
`sig2str` 接受一个 `int` 型变量和一个 `char *` 变量，将 `signo` 对应的信号的字符串存入 `str` 中

```shell
$ gcc p2.c
$ ./a.out
1
SIGHUP
2
SIGINT
3
SIGQUIT
4
SIGILL
5
SIGTRAP
6
SIGIOT
7
SIGBUS
8
SIGFPE
9
SIGKILL
10
SIGUSR1
11
SIGSEGV
12
SIGUSR2
13
SIGPIPE
14
SIGALRM
15
SIGTERM
16
SIGSTKFLT
17
SIGCHLD
18
SIGCONT
19
SIGSTOP
20
SIGTSTP
0
UNKNOWN
```

### 10.3

**画出运行图 10-9 程序时的栈帧情况**  
见 p745 ，图 C-11

### 10.4

**图 10-11 程序中利用 `setjmp` 和 `longjmp` 设置 I/O 操作的超时，下面的代码也常见用于侧重目的：**  

```C
signal(SIGALRM, sig_alrm);
alarm(60);
if (setjmp(env_alrm) != 0) {
    /* handle timeout */
}
```

**这段代码有什么错误？**  
在第一次调用 `alarm` 和 `setjmp` 之间有一次竞争条件。如果进程在调用 `alarm` 和 `setjmp` 之间被内核阻塞了，闹钟时间超过后就调用信号处理程序，然后调用 `longjmp` 。但是由于没有调用过 `setjmp` ，所以没有设置 `env_alrm` 缓冲区。如果 `longjmp` 的跳转缓冲区没有被 `setjmp` 初始化，则说明 `longjmp` 的操作是未定义的。

### 10.5

**仅适用一个定时器（alarm 或较高精度的 setitimer），构造一组函数，使得进程在该单一定时器基础上可以设置任意数量的定时器。**