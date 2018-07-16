# 进程间通信

## 随书代码

### 15.2.c

```shell
$ gcc 15.2.c -lapue
$ ./a.out
hello world
```

`pipe` 创建的管道, 1 为写端, 0 为读端

### 15.2.2.c && 15.3.c

pass

### 15.3.1.c && 15.3.2.c

```shell
$ gcc 15.3.1.c -lapue -o myuclc
$ gcc 15.3.2.c -lapue
$ ./a.out
prompt> a
a
prompt> A
a
prompt> ABC
abc
```

### 15.4.c && 15.4.2.c

```shell
$ gcc 15.4.c -lapue -o add2
$ gcc 15.4.2.c -lapue
$ ./add2
1 2
3
^C
$ ./a.out
1 2
3
3 4
7
^C
```

### 15.4.3.c

```shell
$ gcc 15.4.3.c -lapue -o add2
$ ./a.out
1 2
3 4
(无响应)
^C
```

因为管道是全缓冲的, 程序运行时管道阻塞

### 15.4.4.c

```shell
$ gcc 15.4.4.c -lapue -o add2
$ ./a.out
1 2
3
3 4
7
^C
```

修改流缓冲方式为行缓存之后, 程序运行成功

### 15.9.c

```shell
$ gcc 15.9.c -lapue
$ ./a.out
array[] from 0x55ab62824060 to 0x55ab6282dca0
stack around 0x7ffd1bc6ef74
malloced from 0x55ab637b1670 to 0x55ab637c9d10
shared memory attached from 0x7f9327e7d000 to 0x7f9327e956a0
```

### 15.9.2.c

```shell
$ gcc 15.9.2.c -lapue
$ ./a.out
update 0 from parent
update 1 from child
update 2 from parent
update 3 from child
......
update 998 from parent
update 999 from child
```

### 15.9.3.c

```shell
$ gcc 15.9.3.c -lapue
$ ./a.out
update 0 from parent
update 1 from child
update 2 from parent
update 3 from child
......
update 998 from parent
update 999 from child
```

可以很方便的实现在父子进程之间共享内存