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