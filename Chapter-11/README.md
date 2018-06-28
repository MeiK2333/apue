# 线程

## 随书代码

### 11.4.c

```shell
$ gcc 11.4.c -lapue -lpthread
$ ./a.out
main thread: pid 21744 tid 140060521072448 (0x7f62619b0740)
new thread:  pid 21744 tid 140060512610048 (0x7f626119e700)
```

注意： Linux 下，使用 pthread 库需要链接静态库 libpthread.a ，因此编译选项需要添加 `-lpthread`

### 11.5.c

```shell
$ gcc 11.5.c -lapue -lpthread
$ ./a.out
thread 1 returning
thread 2 exiting
thread 1 exit code 1
thread 2 exit code 2
```

### 11.5.2.c

```shell
$ gcc 11.5.2.c -lapue -lpthread
$ ./a.out
thread 1:
  structure at 0x7f1daacbaed0
  foo.a = 1
  foo.b = 2
  foo.c = 3
  foo.d = 4
parent starting second thread
thread 2: ID is 139765396256512
partne:
  structure at 0x7f1daacbaed0
  foo.a = -1425393824
  foo.b = 32541
  foo.c = -1427798446
  foo.d = 32541
```

### 11.5.3.c

```shell
$ gcc 11.5.3.c -lapue -lpthread
$ ./a.out
thread 2 start
thread 2 push complete
thread 1 start
thread 1 push complete
thread 1 exit code 1
cleanup: thread 2 second handler
cleanup: thread 2 first handler
thread 2 exit code 2
```

和书上的结果有所不同，但只有线程 2 的清理函数执行了是正确的

### 11.6.3.c

```shell
$ gcc 11.6.3.c -lapue -lpthread
$ ./a.out
mutex is locked
current time is 03:17:49 PM
the time is now 03:17:59 PM
can't lock mvtex again: Connection timed out
```

### 11.6.8.c

```shell
$ gcc 11.6.8.c -lapue -lpthread -lbsd
$ ./a.out
sort took 3.1410 seconds
```

这个程序用到了 bsd 库，需要添加 `-lbsd` 参数。

手动将线程数量改为 1 ，发现多线程确实有明显的性能提升，但在我的电脑上还达不到 6 倍的差距。
