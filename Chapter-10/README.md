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
通过 `alarm` 实现了一个类似于 `JavaScript` 中 `settimgout` 的函数，见 [p5.c](p5.c)

### 10.6

**编写一段程序测试图 10-24 中父进程和子进程的同步函数，要求进程创建一个文件并向文件写一个整数 0, ，然后，进程调用 fork ，接着，父进程和子进程交替增加文件中的计数器值，每次计数器值增加 1 时，打印是哪一个进程（子进程或父进程）进行了该增加 1 操作。**  
见 [p6.c](p6.c)

### 10.7

**在图 10-25 中，若调用者捕捉了 SIGABRT 并从该信号处理程序返回，为什么不是仅仅调用 _exit ，而要恢复其默认设置并再次调用 kill？**  
如果仅仅调用 _exit ，则进程终止状态不能表示该进程是由于 SIGABRT 信号而终止的。

### 10.8

**为什么在 siginfo 结构（见 10.14 节）的 si_uid 字段中包括实际用户 ID 而非有效用户 ID ？**  
如果信号是由其他用户的进程发出的，进程必须设置用户 ID 为根或者是接受进程的所有者，否则 kill 不能执行。所以实际用户 ID 为信号的接受者提供了更多的信息。

### 10.9

**重写图 10-14 中的函数，要求它处理图 10-1 中的所有信号，每次循环处理当前信号屏蔽字中的一个信号（并不是对每一个可能的信号都循环一遍）。**  
差不多得了……

### 10.10

**编写一段程序，要求在一个无限循环中调用 `sleep(60)` 函数，每 5 分子（即 5 次循环）取当前的日期和时间，并打印 tm_sec 字段。将程序执行一晚上，请解释其结果。有些程序，如 cron 守护进程，每分钟运行一次，它是如何处理这类工作的？**  
对于本书作者所用的一个系统，每 60~90 分钟增加一秒，这个误差是由于每次调用 `sleep` 都要调度一次将来的时间事件，但是由于 CPU 调度，有事并没有在事件发生时立即被唤醒。另外一个原因是进程开始运行和再次调用 `sleep` 都需要一定的时间。

cron 守护进程这样的程序每分钟都要获取当前时间，它首先设置一个休眠周期，然后在下一分钟开始时唤醒。（将当前时间转换为本地时间并查看 tm_sec 值）。每一分钟，设置下一个休眠周期，使得在下一分钟开始时可以唤醒。大多数调用是 `sleep(60)` ，偶尔有一个 `sleep(59)` 用于在下一分钟同步。但是，若在进程中花费了很多时间执行命令或者系统的负载重、调度慢，这时休眠值可能远小于 60 。

### 10.11

**修改图 3-5 的程序，要求：（a）将 `BUFFSIZE` 改为 100 ；（b）用 `signal_intr` 函数捕捉 `SIGXFSZ` 信号量并打印消息，然后从信号处理程序中返回；（c）如果没有写满请求的字节数，则打印 `write` 的返回值。将软资源限制 `RLIMIT_FSIZE` （见 7.11 节）更改为 1024 字节（在 shell 中设置软资源限制，如果不行就直接在程序中调用 `setrlimit` ），然后复制一个大于 1024 字节的文件，在各种不同的系统上运行新程序，其结果如何？为什么？**  
在 Linux 3.2.0 、 Mac OS X 10.6.8 和 Solaris 10 中，从来没有调用过 SIGXFSZ 的信号处理程序，一旦文件的大小打到 1024 时， write 就返回 24.

在 FreeBSD 8.0 中，当文件大小已达到 1000 字节，在下一次准备写入 100 字节时调用该信号处理程序， write 返回 -1 ，并且将 errno 设置为 EFBIG （文件太大）。

在所有的 4 中平台上，如果在当前文件偏移量处（文件尾端）尝试再一次 write ，将收到 SIGXFSZ 信号， write 将失败，返回 -1 ，并将 errno 设置为 EFBIG 。

### 10.12

**编写一段调用 `fwrite` 的程序，它使用一个较大的缓冲区（约 1GB ），调用 `fwrite` 钱调用 `alarm` 使得 1s 以后产生信号。在信号处理程序中打印捕捉到的信号，然后返回。 `fwrite` 可以完成吗？结果如何？**  
结果依赖于标准 I/O 库的实现： fwrite 函数如何处理一个被中断的 write 。

例如，在 Linux 3.2.0 上，当使用 fwrite 函数写一个大的缓冲区时， fwrite 以相同的字节数直接调用 write 。在 write 系统调用中，闹钟时间到，但我们直到写结束才看到信号。看上去就好像在 write 系统调用进行当中内核阻塞了信号。

与此不同的是，在 Solaris 10 中， fwrite 函数调用以 8 KB 的增量调用 write ，直到写完整个要求的字节数。当闹钟时间到，会被捕捉到，中断 write 回到 fwrite 。当从信号处理程序返回时，返回到 fwrite 函数内部的循环，并继续以 8KB 的增量写。