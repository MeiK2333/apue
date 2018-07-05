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

### 14.5.3.c && 14.5.3.2.c

```shell
$ ll in.txt
-rw-r--r-- 1 meik meik 2600000 7月   5 19:47 in.txt

$ gcc 14.5.3.c -lapue
$ time ./a.out in.txt out.txt

real 0m0.128s
user 0m0.032s
sys  0m0.004s

$ gcc 14.5.3.2.c -lapue -lrt
$ time ./a.out in.txt out.txt

real 0m0.126s
user 0m0.036s
sys  0m0.016s
```

尝试了 26000 / 260000 / 2600000 数据量, 两个程序均没有明显差距