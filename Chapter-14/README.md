# 高级 I/O

## 随书代码

### 14.2.c

```shell
$ gcc 14.2.c -lapue
$ ./a.out < /etc/services > temp.file
......(剩下的其他操作)
```

在我的电脑上, /etc/services 文件并没有 50000 大小, 我减小了 buf 的大小, 却发现程序没有按照预期的那样输出大量错误.

### 14.3.c

```shell
$ gcc 14.3.c -lapue
$ ./a.out
parent: got the lock, byte 1
child: got the lock, byte 0
parent: writew_lock error: Resource deadlock avoided
child: got the lock, byte 1
```

### 14.3.2.c

```shell
$ gcc 14.3.2.c -lapue
$ ./a.out temp.lock
read_lock of already-locked region erturns 11
read OK (no mandatory locking), buf = ab
```

我的电脑是 Ubuntu 18.04, 看起来并不支持强制性锁.