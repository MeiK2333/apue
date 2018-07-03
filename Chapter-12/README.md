# 线程控制

## 随书代码

### 12.8.c

```shell
$ gcc 12.8.c -lapue -lpthread
$ ./a.out
^C
interrupt
^C
interrupt
^C
interrupt
^C
interrupt
^\
```

### 12.9.c

```shell
$ gcc 12.9.c -lapue -lpthread
$ ./a.out
thread started...
parent about to fork...
preparing locks...
parent unlocking locks...
parent returned from fork
child unlocking locks...
child returned from fork
```

## 课后习题

### 12.1

**在 Linux 系统中运行图 12-17 中的程序,但把输出结果重定向到一个文件中,并解释结果.**

在 IO 章节中提到，在输出到控制台时，标准输出为行缓存的，也就是说每一行输出都会冲洗缓冲区；而输出到文件时，标准输出为全缓冲，因此，在 fork 子进程的时候，缓冲区还没有被冲洗，因此子进程也带有父进程的缓冲区中尚未输出的部分。

```shell
$ gcc 12.9.c -lapue -lpthread
$ ./a.out
thread started...
parent about to fork...
preparing locks...
parent unlocking locks...
parent returned from fork
child unlocking locks...
child returned from fork
$ ./a.out > a.out.txt
$ tail a.out.txt
thread started...
parent about to fork...
preparing locks...
parent unlocking locks...
parent returned from fork
thread started...
parent about to fork...
preparing locks...
child unlocking locks...
child returned from fork
```

### 12.2

**实现 putenv_r ，即 putenv 的可冲入版本，确保你的实现既是线程安全的，也是异步信号安全的。**  
待续

### 12.3

**是否可以通过在 getenv 函数开始的时候阻塞信号,并在 getenv 函数返回之前回复原来的信号屏蔽字这种方法,让图 12-13 中的 getenv 函数变成异步安全的? 解释其原因.**  
理论上来说,如果在信号处理程序运行时阻塞所有的信号, 那么就能使函数成为异步信号安全的. 问题是我们并不能知道调用的某个函数可能并没有屏蔽已经被阻塞的信号, 这样通过另一个信号处理程序可能会使该函数变成可重入的.

### 12.4

**写一个程序练习图 12-13 中的 getenv 版本, 在 FreeBSD 上编译并运行程序, 会出现什么结果? 解释其原因.**  
待续

### 12.5

**假设可以在一个程序中创建多个线程执行不同的任务, 为什么还是有可能会需要用 fork? 解释其原因.**  
如果希望在一个程序中运行另一个程序, 还需要 fork (即在调用 exec 之前)

### 12.6

**重新实现图 12-29 中的程序, 在不使用 nanosleep 或 clock_nanosleep 的情况下使它成为线程安全的.**  
待续

### 12.7

**调用 fork 以后, 是否可以通过首先用 pthread_cond_destroy 销毁条件变量,然后用 pthread_cond_init 初始化条件变量这种方法安全地在子进程中对条件变量进行重新初始化?**  
待续

### 12.8

**图 12-8 中的 timeout  函数可以大大简化, 解释其原因.**  
待续